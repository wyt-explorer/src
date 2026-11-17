#include <chrono>
#include <array>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <quadrotor_msgs/msg/goal_set.hpp>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <random>
struct Drone{Eigen::Vector3d p{0,0,0},v{0,0,0},goal{0,0,0};bool odom=false;};
class RandomGoals:public rclcpp::Node{public:RandomGoals():Node("random_goals"),rng_(std::random_device{}()){n_=declare_parameter("drone_num",1); int gn=declare_parameter("goal_num",0); for(int i=0;i<gn;i++){auto a=declare_parameter<std::vector<double>>("goal"+std::to_string(i),{0.,0.,0.});if(a.size()>=3)goals_.push_back({a[0],a[1],a[2]});} d_.resize(n_); pub_=create_publisher<quadrotor_msgs::msg::GoalSet>("/goal_with_id",10);for(int i=0;i<n_;i++)subs_.push_back(create_subscription<nav_msgs::msg::Odometry>("/drone_"+std::to_string(i)+"_visual_slam/odom",10,[this,i](auto m){d_[i].p={m->pose.pose.position.x,m->pose.pose.position.y,m->pose.pose.position.z};d_[i].v={m->twist.twist.linear.x,m->twist.twist.linear.y,m->twist.twist.linear.z};d_[i].odom=true;}));timer_=create_wall_timer(std::chrono::milliseconds(100),[this]{tick();});}
private:void tick(){if(goals_.empty())return;for(int i=0;i<n_;i++)if(d_[i].odom&&(d_[i].goal-d_[i].p).norm()<.3&&d_[i].v.norm()<.3){auto &g=goals_[next_++%goals_.size()];d_[i].goal=g;quadrotor_msgs::msg::GoalSet m;m.drone_id=i;m.goal[0]=g.x();m.goal[1]=g.y();m.goal[2]=g.z();pub_->publish(m);}}
int n_,next_=0;std::vector<Drone>d_;std::vector<Eigen::Vector3d>goals_;std::vector<rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr>subs_;rclcpp::Publisher<quadrotor_msgs::msg::GoalSet>::SharedPtr pub_;rclcpp::TimerBase::SharedPtr timer_;std::mt19937 rng_;};int main(int c,char**v){rclcpp::init(c,v);rclcpp::spin(std::make_shared<RandomGoals>());rclcpp::shutdown();}
