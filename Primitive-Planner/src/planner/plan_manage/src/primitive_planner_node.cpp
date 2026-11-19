/**
 * @file primitive_planner_node.cpp
 * 文件作用：规划节点入口，初始化并运行重规划状态机。
 * 调用关系：main() 创建 rclcpp 节点，调用 PPReplanFSM::init() 后 spin。
 * 输入：ROS 2 参数及状态机订阅的话题。
 * 输出：由状态机发布基元选择、心跳和停车等消息。
 * 运行模式：ROS 2；规划行为由 PPReplanFSM 管理。
 * 主要依赖：rclcpp、PPReplanFSM。
 */
#include <rclcpp/rclcpp.hpp>
#include <plan_manage/pp_replan_fsm.h>
using namespace primitive_planner;

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("primitive_planner_node");
  PPReplanFSM planner;
  planner.init(node);
  if (rclcpp::ok()) rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
