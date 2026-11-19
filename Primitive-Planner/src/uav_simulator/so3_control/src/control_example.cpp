/**
 * @file control_example.cpp
 * 文件作用：循环发布位置、速度和加速度三类位置指令示例。
 * 调用关系：独立运行 quad_sim_example，按每段约 5 秒、每 10 ms 一次发送。
 * 输入：脚本内固定目标；用 NaN 标识相应位置或速度字段。
 * 输出：/position_cmd（PositionCommand）；位置米、速度米/秒、加速度米/秒²。
 * 运行模式：ROS 2 控制接口示例，不验证接收方对 NaN 的处理。
 * 主要依赖：rclcpp、quadrotor_msgs、C++ chrono/thread/limits。
 */
#include <rclcpp/rclcpp.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <limits>
#include <chrono>
#include <thread>
#include <iostream>
int main(int argc,char**argv){rclcpp::init(argc,argv);auto n=std::make_shared<rclcpp::Node>("quad_sim_example");auto p=n->create_publisher<quadrotor_msgs::msg::PositionCommand>("/position_cmd",10);std::this_thread::sleep_for(std::chrono::seconds(2));while(rclcpp::ok()){for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;c.position.x=2;c.position.z=1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;float n=std::numeric_limits<float>::quiet_NaN();c.position.x=c.position.y=c.position.z=n;c.velocity.x=-1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;float n=std::numeric_limits<float>::quiet_NaN();c.position.x=c.position.y=c.position.z=c.velocity.x=c.velocity.y=c.velocity.z=n;c.acceleration.x=1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}}rclcpp::shutdown();}
