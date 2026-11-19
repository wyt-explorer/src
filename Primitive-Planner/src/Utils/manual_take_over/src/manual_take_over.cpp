/**
 * @file manual_take_over.cpp
 * 文件作用：请求规划器停止，并在规划指令超时后用手柄轴积分生成位置指令。
 * 调用关系：手柄回调发布停止请求；主循环检测规划指令中断，满足条件后进入手动输出。
 * 输入：/joystick_from_bridge、/position_cmd；位置米、速度米/秒、偏航弧度。
 * 输出：/mandatory_stop_to_planner 和 /position_cmd；手动指令 frame_id 为 manual_take_over。
 * 运行模式：ROS 2 手动接管节点。
 * 主要依赖：rclcpp、sensor_msgs、std_msgs、quadrotor_msgs。
 */
#include <Eigen/Eigen>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <thread>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <std_msgs/msg/empty.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <sensor_msgs/msg/joy.hpp>


using namespace std;

rclcpp::Node::SharedPtr node;
rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr mandatory_stop_pub_;
rclcpp::Publisher<quadrotor_msgs::msg::PositionCommand>::SharedPtr cmd_pub_;

rclcpp::Time recv_joy_time_;
bool flag_recv_joy_ = false;

rclcpp::Time recv_cmd_time_;
bool flag_recv_cmd_ = false;
bool flag_planner_stop_cmds_ = false;

bool flag_mandatory_stoped_ = false;

Eigen::Vector4d cur_drone_pos{Eigen::Vector4d::Zero()};

sensor_msgs::msg::Joy joy_;

void joy_sub_cb(const sensor_msgs::msg::Joy::ConstSharedPtr &msg)
{
  rclcpp::Time now = node->now();
  flag_recv_joy_ = true;
  recv_joy_time_ = now;
  joy_ = *msg;

  if (msg->buttons.size() < 4 || msg->axes.size() < 5) return;
  if ( msg->buttons[0] || msg->buttons[1] || msg->buttons[2] || msg->buttons[3] )
  {
    flag_mandatory_stoped_ = true;
    std_msgs::msg::Empty stop_msg;
    mandatory_stop_pub_->publish(stop_msg);
  }

  if ( flag_mandatory_stoped_ && flag_planner_stop_cmds_ )
  {
    constexpr double MAX_VEL = 0.2;
    static bool have_last_cmd = false;
    static rclcpp::Time last_cmd_t;
    static Eigen::Vector4d last_cmd;

    if ( !have_last_cmd )
    {
      have_last_cmd = true;
      last_cmd_t = now;
      last_cmd = cur_drone_pos;
    }

    quadrotor_msgs::msg::PositionCommand cmd_msg;
    cmd_msg.header.stamp = now;
    cmd_msg.header.frame_id = "manual_take_over";
    double delta_t = (now - last_cmd_t).seconds();
    last_cmd(0) += joy_.axes[4] * MAX_VEL * delta_t;
    last_cmd(1) += joy_.axes[3] * MAX_VEL * delta_t;
    last_cmd(2) += joy_.axes[1] * MAX_VEL * delta_t;
    last_cmd(3) += joy_.axes[0] * MAX_VEL * delta_t;
    cmd_msg.position.x = last_cmd(0);
    cmd_msg.position.y = last_cmd(1);
    cmd_msg.position.z = last_cmd(2);
    cmd_msg.yaw = last_cmd(3);
    cmd_msg.velocity.x = joy_.axes[4] * MAX_VEL;
    cmd_msg.velocity.y = joy_.axes[3] * MAX_VEL;
    cmd_msg.velocity.z = joy_.axes[1] * MAX_VEL;
    cmd_pub_->publish(cmd_msg);

    last_cmd_t = now;
  }

}

// #      ^                ^
// #    +1|              +4|
// # <-+0      ->     <-+3      ->
// #      |                |
// #      V                V

void position_cmd_sub_cb(const quadrotor_msgs::msg::PositionCommand::ConstSharedPtr &msg)
{
  if ( msg->header.frame_id != string("manual_take_over") )
  {
    flag_recv_cmd_ = true;
    recv_cmd_time_ = node->now();
  }

  cur_drone_pos(0) = msg->position.x;
  cur_drone_pos(1) = msg->position.y;
  cur_drone_pos(2) = msg->position.z;
  cur_drone_pos(3) = msg->yaw;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  node = std::make_shared<rclcpp::Node>("manual_take_over");
  // 复用执行器，避免停止后的 spin_some(node) 再创建无效上下文的守卫条件。
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);

  mandatory_stop_pub_ = node->create_publisher<std_msgs::msg::Empty>("/mandatory_stop_to_planner", 10);
  cmd_pub_ = node->create_publisher<quadrotor_msgs::msg::PositionCommand>("/position_cmd", 10);
  auto joy_sub = node->create_subscription<sensor_msgs::msg::Joy>("/joystick_from_bridge", 10, joy_sub_cb);
  auto cmd_sub = node->create_subscription<quadrotor_msgs::msg::PositionCommand>("/position_cmd", 100, position_cmd_sub_cb);

  while (rclcpp::ok())
  {
    rclcpp::Time t_now = node->now();

    constexpr double TIME_OUT = 1.0;
    if ( flag_recv_joy_ && (t_now - recv_joy_time_).seconds() > TIME_OUT )
    {
      RCLCPP_ERROR(node->get_logger(), "Lost manual take over joystick messages!");
    }

    if ( flag_recv_cmd_ && (t_now - recv_cmd_time_).seconds() > TIME_OUT )
    {
      flag_planner_stop_cmds_ = true;
    }
    else
    {
      flag_planner_stop_cmds_ = false;
    }


    std::this_thread::sleep_for(std::chrono::duration<double>(0.4));
    if (rclcpp::ok()) executor.spin_some();
  }

  // 在进程静态析构前释放 ROS 实体，避免全局发布器晚于 DDS 上下文析构。
  joy_sub.reset();
  cmd_sub.reset();
  mandatory_stop_pub_.reset();
  cmd_pub_.reset();
  executor.remove_node(node);
  node.reset();
  rclcpp::shutdown();
  return 0;
}
