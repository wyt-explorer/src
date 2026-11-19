/**
 * @file ground_station.cpp
 * 文件作用：手柄按键触发后，将后续 Joy 消息转发给通信桥。
 * 调用关系：地面站节点订阅 /joy，前四个按钮任一按下后保持转发状态。
 * 输入：/joy（sensor_msgs/Joy），保留轴与按钮原值。
 * 输出：/joystick_from_users（Joy）。
 * 运行模式：ROS 2 地面站手动接管辅助节点。
 * 主要依赖：rclcpp、sensor_msgs。
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
#include <sensor_msgs/msg/joy.hpp>


using namespace std;
bool flag_mandatory_stoped_ = false;

rclcpp::Node::SharedPtr node;
rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_pub_;

void joy_sub_cb(const sensor_msgs::msg::Joy::ConstSharedPtr &msg)
{

  if (msg->buttons.size() < 4) return;
  if ( msg->buttons[0] || msg->buttons[1] || msg->buttons[2] || msg->buttons[3] )
  {
    flag_mandatory_stoped_ = true;
  }

  if ( flag_mandatory_stoped_ )
  {
    joy_pub_->publish(*msg);
  }

}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  node = std::make_shared<rclcpp::Node>("manual_take_over_ground_station");

  auto joy_sub = node->create_subscription<sensor_msgs::msg::Joy>("/joy", 10, joy_sub_cb);
  joy_pub_ = node->create_publisher<sensor_msgs::msg::Joy>("/joystick_from_users", 10);

  rclcpp::spin(node);

  // 在进程静态析构前释放 ROS 实体，避免全局发布器晚于 DDS 上下文析构。
  joy_sub.reset();
  joy_pub_.reset();
  node.reset();
  rclcpp::shutdown();
  return 0;
}
