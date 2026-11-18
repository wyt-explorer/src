/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <quadrotor_msgs/msg/goal_set.hpp>
#include <Eigen/Dense>
#include <vector>
#include <string>
class AssignGoals: public rclcpp::Node{struct D{int id;Eigen::Vector3d p;}; public:AssignGoals():Node("assign_goals"),last_(now()){sel_=create_subscription<geometry_msgs::msg::PoseStamped>("/rviz_selected_drones",100,[this](geometry_msgs::msg::PoseStamped::SharedPtr m){selected(m);}); goal_=create_subscription<geometry_msgs::msg::PoseStamped>("/goal",10,[this](geometry_msgs::msg::PoseStamped::SharedPtr m){goal(m);}); pub_=create_publisher<quadrotor_msgs::msg::GoalSet>("/goal_user2brig",10); mark_=create_publisher<visualization_msgs::msg::MarkerArray>("/new_goals_arrow",10); timer_=create_wall_timer(std::chrono::milliseconds(10),[this]{clear();});}
private:void selected(const geometry_msgs::msg::PoseStamped::SharedPtr m){if((now()-last_).seconds()>2)d_.clear();int id=0;try{id=std::stoi(m->header.frame_id.substr(6));}catch(...){return;}d_.push_back({id,{m->pose.position.x,m->pose.position.y,m->pose.position.z}});last_=now();}
void goal(const geometry_msgs::msg::PoseStamped::SharedPtr m){if(d_.empty())return;Eigen::Vector3d c=Eigen::Vector3d::Zero();for(auto&x:d_)c+=x.p;c/=d_.size();Eigen::Vector3d g(m->pose.position.x,m->pose.position.y,m->pose.position.z),mv=g-c;visualization_msgs::msg::MarkerArray a;int i=0;for(auto&x:d_){Eigen::Vector3d q=x.p+mv;quadrotor_msgs::msg::GoalSet o;o.drone_id=x.id;o.goal[0]=q.x();o.goal[1]=q.y();o.goal[2]=q.z();pub_->publish(o);visualization_msgs::msg::Marker z;z.header.frame_id="world";z.header.stamp=now();z.id=i++;z.type=visualization_msgs::msg::Marker::ARROW;z.action=visualization_msgs::msg::Marker::ADD;z.scale.x=.05;z.scale.y=.2;z.scale.z=.2;z.color.a=1.;z.color.r=1.;geometry_msgs::msg::Point p,e;p.x=x.p.x();p.y=x.p.y();p.z=x.p.z();e.x=q.x();e.y=q.y();e.z=q.z();z.points={p,e};a.markers.push_back(z);}mark_->publish(a);last_pub_=now();need_=true;}
void clear(){if(need_&&(now()-last_pub_).seconds()>2){visualization_msgs::msg::MarkerArray a;visualization_msgs::msg::Marker z;z.action=visualization_msgs::msg::Marker::DELETEALL;a.markers.push_back(z);mark_->publish(a);need_=false;}}
rclcpp::Time now(){return get_clock()->now();}std::vector<D>d_;rclcpp::Time last_,last_pub_;bool need_=false;rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sel_,goal_;rclcpp::Publisher<quadrotor_msgs::msg::GoalSet>::SharedPtr pub_;rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr mark_;rclcpp::TimerBase::SharedPtr timer_;};int main(int c,char**v){rclcpp::init(c,v);rclcpp::spin(std::make_shared<AssignGoals>());rclcpp::shutdown();}
