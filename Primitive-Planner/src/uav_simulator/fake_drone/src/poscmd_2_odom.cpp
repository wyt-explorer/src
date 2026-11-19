/**
 * @file poscmd_2_odom.cpp
 * 文件作用：将位置指令直接转换为模拟里程计，不积分飞行动力学。
 * 调用关系：single_drone.launch.py 启动；10 ms 定时器根据最近指令或初始位置发布状态。
 * 输入：command（PositionCommand）和 init_x/y/z；世界系，米、秒、弧度制。
 * 输出：odometry（Odometry）；位置/速度跟随指令，姿态由加速度与偏航构造。
 * 运行模式：ROS 2 理想位置跟踪仿真。
 * 主要依赖：rclcpp、Eigen、nav_msgs、quadrotor_msgs。
 */
#include <iostream>
#include <math.h>
#include <random>
#include <eigen3/Eigen/Dense>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <thread>
#include <functional>
#include <nav_msgs/msg/odometry.hpp>
#include "quadrotor_msgs/msg/position_command.hpp"

rclcpp::Node::SharedPtr node;
rclcpp::Subscription<quadrotor_msgs::msg::PositionCommand>::SharedPtr _cmd_sub;
rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr _odom_pub;

quadrotor_msgs::msg::PositionCommand _cmd;
double _init_x, _init_y, _init_z;

bool rcv_cmd = false;
/** 缓存最近位置指令；米、秒、弧度制，供 100 Hz 理想跟踪器读取。 */
void rcvPosCmdCallBack(const quadrotor_msgs::msg::PositionCommand cmd)
{
	rcv_cmd = true;
	_cmd    = cmd;
}

/** 复制上游位姿/速度映射；twist.angular 按上游约定承载加速度。 */
void pubOdom()
{
	nav_msgs::msg::Odometry odom;
	odom.header.stamp    = node->now();
	odom.header.frame_id = "world";

	if(rcv_cmd)
	{
	    odom.pose.pose.position.x = _cmd.position.x;
	    odom.pose.pose.position.y = _cmd.position.y;
	    odom.pose.pose.position.z = _cmd.position.z;

		Eigen::Vector3d alpha = Eigen::Vector3d(_cmd.acceleration.x, _cmd.acceleration.y, _cmd.acceleration.z) + 9.8*Eigen::Vector3d(0,0,1);
		Eigen::Vector3d xC(cos(_cmd.yaw), sin(_cmd.yaw), 0);
		Eigen::Vector3d yC(-sin(_cmd.yaw), cos(_cmd.yaw), 0);
		Eigen::Vector3d xB = (yC.cross(alpha)).normalized();
		Eigen::Vector3d yB = (alpha.cross(xB)).normalized();
		Eigen::Vector3d zB = xB.cross(yB);
		Eigen::Matrix3d R;
		R.col(0) = xB;
		R.col(1) = yB;
		R.col(2) = zB;
		Eigen::Quaterniond q(R);
	    odom.pose.pose.orientation.w = q.w();
	    odom.pose.pose.orientation.x = q.x();
	    odom.pose.pose.orientation.y = q.y();
	    odom.pose.pose.orientation.z = q.z();

	    odom.twist.twist.linear.x = _cmd.velocity.x;
	    odom.twist.twist.linear.y = _cmd.velocity.y;
	    odom.twist.twist.linear.z = _cmd.velocity.z;

	    odom.twist.twist.angular.x = _cmd.acceleration.x;
	    odom.twist.twist.angular.y = _cmd.acceleration.y;
	    odom.twist.twist.angular.z = _cmd.acceleration.z;
	}
	else
	{
		odom.pose.pose.position.x = _init_x;
	    odom.pose.pose.position.y = _init_y;
	    odom.pose.pose.position.z = _init_z;

		double init_yaw = 0;
		Eigen::Quaterniond q;
		q = Eigen::AngleAxisd(init_yaw, Eigen::Vector3d::UnitZ());
	    odom.pose.pose.orientation.w = q.w();
	    odom.pose.pose.orientation.x = q.x();
	    odom.pose.pose.orientation.y = q.y();
	    odom.pose.pose.orientation.z = q.z();

	    odom.twist.twist.linear.x = 0.0;
	    odom.twist.twist.linear.y = 0.0;
	    odom.twist.twist.linear.z = 0.0;

	    odom.twist.twist.angular.x = 0.0;
	    odom.twist.twist.angular.y = 0.0;
	    odom.twist.twist.angular.z = 0.0;
	}

    _odom_pub->publish(odom);
}

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  node = std::make_shared<rclcpp::Node>("odom_generator");
  // 复用执行器，避免停止后的 spin_some(node) 再创建无效上下文的守卫条件。
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  _init_x = node->declare_parameter("init_x", 0.0);
  _init_y = node->declare_parameter("init_y", 0.0);
  _init_z = node->declare_parameter("init_z", 0.0);
  _cmd_sub = node->create_subscription<quadrotor_msgs::msg::PositionCommand>(
      "command", 1, [](quadrotor_msgs::msg::PositionCommand::SharedPtr cmd) { rcvPosCmdCallBack(*cmd); });
  _odom_pub = node->create_publisher<nav_msgs::msg::Odometry>("odometry", 1);
  rclcpp::Rate rate(100, node->get_clock());
  while (rclcpp::ok()) {
    pubOdom();
    if (rclcpp::ok()) executor.spin_some();
    rate.sleep();
  }
  // 在进程静态析构前释放 ROS 实体，避免全局发布器晚于 DDS 上下文析构。
  _cmd_sub.reset();
  _odom_pub.reset();
  executor.remove_node(node);
  node.reset();
  rclcpp::shutdown();
  return 0;
}
