/**
 * @file bridge_node_udp.cpp
 * 文件作用：按上游消息类型通过 UDP 转发里程计、基元轨迹、目标和手柄数据。
 * 调用关系：订阅回调序列化并广播，接收线程校验包长后发布到规划器输入话题。
 * 输入：my_odom、/broadcast_primitive_from_planner、/goal_user2brig、/joystick_from_users。
 * 输出：/others_odom、/broadcast_primitive_to_planner、/goal_brig2plner、/joystick_from_bridge。
 * 运行模式：ROS 2 网络桥，默认 UDP 8081；ROS 2 CDR 报文不兼容 ROS 1 线格式。
 * 主要依赖：rclcpp 序列化、POSIX socket、C++ thread、相应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/serialization.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <traj_utils/msg/swarm_primitive_traj.hpp>
#include <quadrotor_msgs/msg/goal_set.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/empty.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <array>
#include <atomic>
#include <cstring>
#include <thread>
#include <vector>

class SwarmBridge : public rclcpp::Node
{
  // 保留上游的消息类型编号以及 [类型, CDR长度, CDR数据] 包布局。
  enum MessageType : uint32_t { ODOM = 100, ONE_PRIMITIVE, STOP, GOAL, JOY };
  using Primitive = traj_utils::msg::SwarmPrimitiveTraj;
  using Goal = quadrotor_msgs::msg::GoalSet;
public:
  SwarmBridge() : Node("swarm_bridge"), last_odom_(0, 0, RCL_ROS_TIME)
  {
    const auto ip = declare_parameter<std::string>("broadcast_ip", "127.0.0.255");
    const int port = declare_parameter("port", 8081);
    drone_id_ = declare_parameter("drone_id", -1);
    odom_frequency_ = declare_parameter("odom_max_freq", 1000.0);
    if (drone_id_ < 0 || port < 1 || port > 65535 || odom_frequency_ <= 0)
      throw std::invalid_argument("invalid drone_id, port or odom_max_freq");
    odom_pub_ = create_publisher<nav_msgs::msg::Odometry>("/others_odom", 10);
    primitive_pub_ = create_publisher<Primitive>("/broadcast_primitive_to_planner", 100);
    goal_pub_ = create_publisher<Goal>("/goal_brig2plner", 100);
    joy_pub_ = create_publisher<sensor_msgs::msg::Joy>("/joystick_from_bridge", 100);
    stop_pub_ = create_publisher<std_msgs::msg::Empty>("/mandatory_stop_to_planner", 100);
    send_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    receive_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    try {
      if (send_fd_ < 0 || receive_fd_ < 0) throw std::runtime_error("cannot create UDP sockets");
      int enabled = 1;
      if (setsockopt(send_fd_, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled)) < 0 ||
          setsockopt(receive_fd_, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) < 0)
        throw std::runtime_error("cannot configure UDP sockets");
      // 接收超时使退出时无需 detach 线程或销毁仍被接收线程访问的发布器。
      timeval timeout{0, 100000};
      setsockopt(receive_fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
      destination_.sin_family = AF_INET;
      destination_.sin_port = htons(port);
      if (inet_pton(AF_INET, ip.c_str(), &destination_.sin_addr) != 1)
        throw std::invalid_argument("broadcast_ip must be IPv4");
      sockaddr_in local{};
      local.sin_family = AF_INET;
      local.sin_addr.s_addr = INADDR_ANY;
      local.sin_port = htons(port);
      if (bind(receive_fd_, reinterpret_cast<sockaddr*>(&local), sizeof(local)) < 0)
        throw std::runtime_error("cannot bind UDP port");
    } catch (...) {
      if (send_fd_ >= 0) close(send_fd_);
      if (receive_fd_ >= 0) close(receive_fd_);
      throw;
    }
    subscriptions_.push_back(create_subscription<nav_msgs::msg::Odometry>("my_odom", 10,
        [this](nav_msgs::msg::Odometry::ConstSharedPtr input) {
          const auto stamp = now();
          if ((stamp - last_odom_).seconds() * odom_frequency_ < 1.0) return;
          last_odom_ = stamp;
          auto msg = *input;
          msg.child_frame_id = "drone_" + std::to_string(drone_id_);
          send(ODOM, msg);
        }));
    subscriptions_.push_back(create_subscription<Primitive>("/broadcast_primitive_from_planner", 100,
        [this](Primitive::ConstSharedPtr msg) { send(ONE_PRIMITIVE, *msg); }));
    subscriptions_.push_back(create_subscription<Goal>("/goal_user2brig", 100,
        [this](Goal::ConstSharedPtr msg) { send(GOAL, *msg); }));
    subscriptions_.push_back(create_subscription<sensor_msgs::msg::Joy>("/joystick_from_users", 100,
        [this](sensor_msgs::msg::Joy::ConstSharedPtr msg) { send(JOY, *msg); }));
    // 上游保留但默认关闭的停止广播通道，使用显式参数开启。
    if (declare_parameter("enable_stop_broadcast", false)) {
      subscriptions_.push_back(create_subscription<std_msgs::msg::Empty>("/mandatory_stop_from_users", 100,
          [this](std_msgs::msg::Empty::ConstSharedPtr msg) { send(STOP, *msg); }));
    }
    receive_thread_ = std::thread([this] { receive(); });
  }

  ~SwarmBridge() override
  {
    running_ = false;
    if (receive_thread_.joinable()) receive_thread_.join();
    if (receive_fd_ >= 0) close(receive_fd_);
    if (send_fd_ >= 0) close(send_fd_);
  }
private:
  /** 每条 ROS 消息独立打包；超出 UDP 有效载荷上限时拒绝发送。 */
  template<class Message> void send(MessageType type, const Message& message)
  {
    rclcpp::Serialization<Message> serializer;
    rclcpp::SerializedMessage serialized;
    serializer.serialize_message(&message, &serialized);
    const uint32_t length = static_cast<uint32_t>(serialized.size());
    if (length > 65507 - 8) { RCLCPP_ERROR(get_logger(), "UDP message exceeds payload limit"); return; }
    std::vector<uint8_t> packet(length + 8);
    const uint32_t wire_type = type;
    std::memcpy(packet.data(), &wire_type, 4);
    std::memcpy(packet.data() + 4, &length, 4);
    std::memcpy(packet.data() + 8, serialized.get_rcl_serialized_message().buffer, length);
    if (sendto(send_fd_, packet.data(), packet.size(), 0,
        reinterpret_cast<sockaddr*>(&destination_), sizeof(destination_)) < 0)
      RCLCPP_ERROR(get_logger(), "UDP send failed: %s", strerror(errno));
  }

  /** 在已校验完整包长的前提下反序列化，正确设置 ROS 2 缓冲的有效长度。 */
  template<class Message> void publish(const uint8_t* data, uint32_t length,
      const typename rclcpp::Publisher<Message>::SharedPtr& publisher)
  {
    rclcpp::SerializedMessage serialized(length);
    auto& buffer = serialized.get_rcl_serialized_message();
    std::memcpy(buffer.buffer, data, length);
    buffer.buffer_length = length;
    Message message;
    rclcpp::Serialization<Message> serializer;
    serializer.deserialize_message(&serialized, &message);
    publisher->publish(message);
  }

  void receive()
  {
    std::array<uint8_t, 65536> packet{};
    while (running_) {
      const auto size = recvfrom(receive_fd_, packet.data(), packet.size(), 0, nullptr, nullptr);
      if (size < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
          RCLCPP_WARN(get_logger(), "UDP receive failed: %s", strerror(errno));
        continue;
      }
      if (size < 8) continue;
      uint32_t type, length;
      std::memcpy(&type, packet.data(), 4);
      std::memcpy(&length, packet.data() + 4, 4);
      if (length != static_cast<size_t>(size) - 8 || length < 4) continue;
      try {
        const auto* data = packet.data() + 8;
        switch (type) {
          case ODOM: publish<nav_msgs::msg::Odometry>(data, length, odom_pub_); break;
          case ONE_PRIMITIVE: publish<Primitive>(data, length, primitive_pub_); break;
          case STOP: publish<std_msgs::msg::Empty>(data, length, stop_pub_); break;
          case GOAL: publish<Goal>(data, length, goal_pub_); break;
          case JOY: publish<sensor_msgs::msg::Joy>(data, length, joy_pub_); break;
          default: break;
        }
      } catch (const std::exception& error) {
        RCLCPP_WARN(get_logger(), "Discarding invalid UDP message: %s", error.what());
      }
    }
  }
  int drone_id_, send_fd_{-1}, receive_fd_{-1};
  double odom_frequency_;
  rclcpp::Time last_odom_;
  sockaddr_in destination_{};
  std::atomic<bool> running_{true};
  std::thread receive_thread_;
  std::vector<rclcpp::SubscriptionBase::SharedPtr> subscriptions_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  rclcpp::Publisher<Primitive>::SharedPtr primitive_pub_;
  rclcpp::Publisher<Goal>::SharedPtr goal_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_pub_;
  rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr stop_pub_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SwarmBridge>());
  rclcpp::shutdown();
  return 0;
}
