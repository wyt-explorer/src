/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <quadrotor_msgs/msg/so3_command.hpp>
#include <so3_control/SO3Control.h>
#include <Eigen/Geometry>
class SO3Node:public rclcpp::Node{public:SO3Node():Node("so3_control"){kx_=Eigen::Vector3d(4,4,4);kv_=Eigen::Vector3d(2,2,2);pub_=create_publisher<quadrotor_msgs::msg::SO3Command>("/so3_command",10);cmd_sub_=create_subscription<quadrotor_msgs::msg::PositionCommand>("/position_cmd",10,[this](quadrotor_msgs::msg::PositionCommand::SharedPtr m){cmd_=*m;have_cmd_=true;});odom_sub_=create_subscription<nav_msgs::msg::Odometry>("/odom",10,[this](nav_msgs::msg::Odometry::SharedPtr m){ctrl_.setPosition({m->pose.pose.position.x,m->pose.pose.position.y,m->pose.pose.position.z});ctrl_.setVelocity({m->twist.twist.linear.x,m->twist.twist.linear.y,m->twist.twist.linear.z});publish();});}private:rclcpp::Subscription<quadrotor_msgs::msg::PositionCommand>::SharedPtr cmd_sub_;rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;void publish(){if(!have_cmd_)return;ctrl_.calculateControl({cmd_.position.x,cmd_.position.y,cmd_.position.z},{cmd_.velocity.x,cmd_.velocity.y,cmd_.velocity.z},{cmd_.acceleration.x,cmd_.acceleration.y,cmd_.acceleration.z},cmd_.yaw,cmd_.yaw_dot,kx_,kv_);auto f=ctrl_.getComputedForce();auto q=ctrl_.getComputedOrientation();quadrotor_msgs::msg::SO3Command o;o.header.stamp=get_clock()->now();o.force.x=f.x();o.force.y=f.y();o.force.z=f.z();o.orientation.x=q.x();o.orientation.y=q.y();o.orientation.z=q.z();o.orientation.w=q.w();pub_->publish(o);}SO3Control ctrl_;quadrotor_msgs::msg::PositionCommand cmd_;bool have_cmd_=false;Eigen::Vector3d kx_,kv_;rclcpp::Publisher<quadrotor_msgs::msg::SO3Command>::SharedPtr pub_;};int main(int c,char**v){rclcpp::init(c,v);rclcpp::spin(std::make_shared<SO3Node>());rclcpp::shutdown();}
