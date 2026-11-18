/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
class Ground:public rclcpp::Node{public:Ground():Node("manual_take_over_ground_station"){pub_=create_publisher<sensor_msgs::msg::Joy>("/joystick_from_users",10);sub_=create_subscription<sensor_msgs::msg::Joy>("/joy",10,[this](sensor_msgs::msg::Joy::SharedPtr m){if(m->buttons.size()>3&&(m->buttons[0]||m->buttons[1]||m->buttons[2]||m->buttons[3]))stop_=true;if(stop_)pub_->publish(*m);});}private:bool stop_=false;rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr pub_;};int main(int c,char**v){rclcpp::init(c,v);rclcpp::spin(std::make_shared<Ground>());rclcpp::shutdown();}
