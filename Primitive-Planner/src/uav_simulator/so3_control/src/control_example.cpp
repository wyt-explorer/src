#include <rclcpp/rclcpp.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <limits>
#include <chrono>
#include <thread>
#include <iostream>
int main(int argc,char**argv){rclcpp::init(argc,argv);auto n=std::make_shared<rclcpp::Node>("quad_sim_example");auto p=n->create_publisher<quadrotor_msgs::msg::PositionCommand>("/position_cmd",10);std::this_thread::sleep_for(std::chrono::seconds(2));while(rclcpp::ok()){for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;c.position.x=2;c.position.z=1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;float n=std::numeric_limits<float>::quiet_NaN();c.position.x=c.position.y=c.position.z=n;c.velocity.x=-1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}for(int i=0;i<500&&rclcpp::ok();++i){quadrotor_msgs::msg::PositionCommand c;float n=std::numeric_limits<float>::quiet_NaN();c.position.x=c.position.y=c.position.z=c.velocity.x=c.velocity.y=c.velocity.z=n;c.acceleration.x=1;p->publish(c);std::this_thread::sleep_for(std::chrono::milliseconds(10));}}rclcpp::shutdown();}
