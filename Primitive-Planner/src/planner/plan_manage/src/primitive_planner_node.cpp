#include <rclcpp/rclcpp.hpp>
#include <plan_manage/pp_replan_fsm.h>
using namespace primitive_planner;
int main(int argc,char**argv){rclcpp::init(argc,argv);auto node=std::make_shared<rclcpp::Node>("primitive_planner_node");PPReplanFSM planner;planner.init(node);rclcpp::spin(node);rclcpp::shutdown();return 0;}
