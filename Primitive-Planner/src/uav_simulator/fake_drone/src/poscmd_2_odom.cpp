/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <Eigen/Dense>
#include <cmath>
class Poscmd2Odom : public rclcpp::Node {
public: Poscmd2Odom():Node("odom_generator") { ix_=declare_parameter("init_x",0.0); iy_=declare_parameter("init_y",0.0); iz_=declare_parameter("init_z",0.0); sub_=create_subscription<quadrotor_msgs::msg::PositionCommand>("command",1,[this](quadrotor_msgs::msg::PositionCommand::SharedPtr m){cmd_=*m; received_=true;}); pub_=create_publisher<nav_msgs::msg::Odometry>("odometry",1); timer_=create_wall_timer(std::chrono::milliseconds(10),[this]{publish();}); }
private: void publish(){ nav_msgs::msg::Odometry o; o.header.stamp=get_clock()->now(); o.header.frame_id="world"; if(received_){o.pose.pose.position.x=cmd_.position.x;o.pose.pose.position.y=cmd_.position.y;o.pose.pose.position.z=cmd_.position.z; Eigen::Vector3d a(cmd_.acceleration.x,cmd_.acceleration.y,cmd_.acceleration.z+9.8),xc(cos(cmd_.yaw),sin(cmd_.yaw),0),yc(-sin(cmd_.yaw),cos(cmd_.yaw),0),xb=(yc.cross(a)).normalized(),yb=(a.cross(xb)).normalized(),zb=xb.cross(yb); Eigen::Matrix3d R;R.col(0)=xb;R.col(1)=yb;R.col(2)=zb; Eigen::Quaterniond q(R);o.pose.pose.orientation.w=q.w();o.pose.pose.orientation.x=q.x();o.pose.pose.orientation.y=q.y();o.pose.pose.orientation.z=q.z();o.twist.twist.linear.x=cmd_.velocity.x;o.twist.twist.linear.y=cmd_.velocity.y;o.twist.twist.linear.z=cmd_.velocity.z;} else {o.pose.pose.position.x=ix_;o.pose.pose.position.y=iy_;o.pose.pose.position.z=iz_;o.pose.pose.orientation.w=1.0;} pub_->publish(o); }
 double ix_,iy_,iz_; bool received_=false; quadrotor_msgs::msg::PositionCommand cmd_; rclcpp::Subscription<quadrotor_msgs::msg::PositionCommand>::SharedPtr sub_; rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_; rclcpp::TimerBase::SharedPtr timer_;
}; int main(int argc,char**argv){rclcpp::init(argc,argv);rclcpp::spin(std::make_shared<Poscmd2Odom>());rclcpp::shutdown();return 0;}
