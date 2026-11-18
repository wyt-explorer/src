/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <quadrotor_msgs/msg/so3_command.hpp>
class Simulator:public rclcpp::Node{public:Simulator():Node("quadrotor_simulator_so3"){x_=declare_parameter("init_x",0.);y_=declare_parameter("init_y",0.);z_=declare_parameter("init_z",1.);cmd_=create_subscription<quadrotor_msgs::msg::SO3Command>("/so3_command",10,[this](quadrotor_msgs::msg::SO3Command::SharedPtr m){last_=*m;});odom_=create_publisher<nav_msgs::msg::Odometry>("odom",10);imu_=create_publisher<sensor_msgs::msg::Imu>("imu",10);timer_=create_wall_timer(std::chrono::milliseconds(10),[this]{tick();});}private:void tick(){auto t=get_clock()->now();nav_msgs::msg::Odometry o;o.header.stamp=t;o.header.frame_id="world";o.child_frame_id="quadrotor";o.pose.pose.position.x=x_;o.pose.pose.position.y=y_;o.pose.pose.position.z=z_;o.pose.pose.orientation.w=1.;sensor_msgs::msg::Imu i;i.header=o.header;i.orientation.w=1.;odom_->publish(o);imu_->publish(i);}double x_,y_,z_;quadrotor_msgs::msg::SO3Command last_;rclcpp::Subscription<quadrotor_msgs::msg::SO3Command>::SharedPtr cmd_;rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_;rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_;rclcpp::TimerBase::SharedPtr timer_;};int main(int c,char**v){rclcpp::init(c,v);rclcpp::spin(std::make_shared<Simulator>());rclcpp::shutdown();}
