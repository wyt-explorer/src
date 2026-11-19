/**
 * @file random_goals_node.cpp
 * 文件作用：恢复上游随机目标分配：占用管理、到达停留判断与未启动重发。
 * 调用关系：里程计回调更新各机状态；主循环随机抽取飞机和可用目标。
 * 输入：逐机里程计、/others_odom、goalN 参数；世界系米制，时间为秒。
 * 输出：/goal_with_id（GoalSet）；目标须距当前飞机大于 5 米。
 * 运行模式：ROS 2 多机目标工具，保留上游 5 秒预热和 0.1 秒检查周期。
 * 主要依赖：rclcpp、Eigen、uav_utils、nav_msgs、quadrotor_msgs。
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
#include <quadrotor_msgs/msg/goal_set.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <uav_utils/geometry_utils.h>

using namespace std;

// ros::Subscriber trajs_sub_;
rclcpp::Node::SharedPtr node;
rclcpp::Publisher<quadrotor_msgs::msg::GoalSet>::SharedPtr goals_pub_;
struct Drone_Info_t
{
  Eigen::Vector3d goal{Eigen::Vector3d::Zero()};
  Eigen::Vector3d last_goal{Eigen::Vector3d::Zero()};
  int goal_id{-1};
  Eigen::Vector3d cur_p{Eigen::Vector3d::Zero()};
  Eigen::Vector3d cur_v{Eigen::Vector3d::Zero()};
  double cur_yaw;
  Eigen::Vector3d last_p{Eigen::Vector3d::Zero()};
  rclcpp::Time arrived_time{0, 0, RCL_ROS_TIME};
  rclcpp::Time set_new_goal_time{0, 0, RCL_ROS_TIME};
  bool arrived_for_a_while{true};
  bool odom_received{false};
};
struct Goal_t
{
  Eigen::Vector3d p;
  bool occupied;
};
std::vector<Drone_Info_t> drones_;

void set_odom_data(const nav_msgs::msg::Odometry::ConstSharedPtr &msg, const int &drone_id)
{
  drones_[drone_id].odom_received = true;
  drones_[drone_id].last_p = drones_[drone_id].cur_p;
  drones_[drone_id].cur_p << msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z;
  drones_[drone_id].cur_v << msg->twist.twist.linear.x, msg->twist.twist.linear.y, msg->twist.twist.linear.z;
  drones_[drone_id].cur_yaw = uav_utils::get_yaw_from_quaternion(Eigen::Quaterniond(
      msg->pose.pose.orientation.w,
      msg->pose.pose.orientation.x,
      msg->pose.pose.orientation.y,
      msg->pose.pose.orientation.z));
}

void odoms_sim_sub_cb(const nav_msgs::msg::Odometry::ConstSharedPtr &msg, const int &drone_id)
{
  set_odom_data(msg, drone_id);
}

void combined_odoms_sim_sub_cb(const nav_msgs::msg::Odometry::ConstSharedPtr &msg)
{
  if (msg->child_frame_id.size() < 7) return;
  int id = atoi(msg->child_frame_id.substr(6, 10).c_str());
  if ( msg->child_frame_id.substr(0, 6) != string("drone_") || id < 0 || id >= (int)drones_.size())
  {
    RCLCPP_ERROR(node->get_logger(), "[random_goals_node] Wrong child_frame_id: %s, or wrong drone_id: %d", msg->child_frame_id.substr(0, 6).c_str(), id);
    return;
  }
  set_odom_data(msg, id);
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  node = std::make_shared<rclcpp::Node>("random_goals");
  // 复用执行器，避免停止后的 spin_some(node) 再创建无效上下文的守卫条件。
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);

  srand(floor(node->now().seconds() * 10));

  int drone_num, goal_num;
  drone_num = node->declare_parameter("drone_num", -1);
  goal_num = node->declare_parameter("goal_num", -1);
  if (drone_num <= 0 || goal_num <= 0) throw std::invalid_argument("drone_num and goal_num must be positive");

  vector<Goal_t> goals(goal_num);
  for (int i = 0; i < goal_num; ++i)
  {
    vector<double> pt;
    pt = node->declare_parameter<std::vector<double>>("goal" + to_string(i), {});
    if (pt.size() != 3) throw std::invalid_argument("goalN requires three coordinates");
    goals[i].p << pt[0], pt[1], pt[2];
    goals[i].occupied = false;
  }

  drones_.resize(drone_num);
  std::vector<rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr> odoms_sim_sub;
  for (int i = 0; i < drone_num; ++i) {
    odoms_sim_sub.push_back(node->create_subscription<nav_msgs::msg::Odometry>(
        "/drone_" + to_string(i) + "_visual_slam/odom", 1000,
        [i](nav_msgs::msg::Odometry::ConstSharedPtr msg) { odoms_sim_sub_cb(msg, i); }));
  }
  auto combined_odoms_sim_sub = node->create_subscription<nav_msgs::msg::Odometry>(
      "/others_odom", 1000, combined_odoms_sim_sub_cb);
  goals_pub_ = node->create_publisher<quadrotor_msgs::msg::GoalSet>("/goal_with_id", 10);

  std::vector<int> count(goal_num);
  fill(count.begin(), count.end(), 0);

  // Wait for 4s
  RCLCPP_INFO(node->get_logger(), "[random goals] Wait for 5 seconds.");
  std::this_thread::sleep_for(std::chrono::duration<double>(5.0));

  while (rclcpp::ok())
  {
    rclcpp::Time t_now = node->now();

    for (int i = 0; i < drone_num; ++i)
    {
      if (!drones_[i].odom_received) continue;
      double d_to_goal = (drones_[i].cur_p - drones_[i].goal).norm();
      double last_d_to_goal = (drones_[i].last_p - drones_[i].goal).norm();
      // cout << "last_p=" << drones_[i].last_p.transpose() << " cur_p=" << drones_[i].cur_p.transpose() << " goal=" << drones_[i].goal.transpose() << endl;
      // cout << "d_to_goal=" << d_to_goal << " last_d_to_goal=" << last_d_to_goal << endl;
      if (d_to_goal > 0.2 || last_d_to_goal > 0.2)
        drones_[i].arrived_time = t_now;
      drones_[i].arrived_for_a_while |= ((t_now - drones_[i].arrived_time).seconds() > 2);

      if ( (drones_[i].cur_p - drones_[i].last_goal).norm() < 0.3 && drones_[i].cur_v.norm() < 0.3 && (t_now - drones_[i].set_new_goal_time).seconds() > 3.0 ) // did not start
      {
        drones_[i].set_new_goal_time = t_now;
        quadrotor_msgs::msg::GoalSet msg;
        msg.drone_id = i;
        msg.goal[0] = drones_[i].goal(0);
        msg.goal[1] = drones_[i].goal(1);
        msg.goal[2] = drones_[i].goal(2);
        goals_pub_->publish(msg);
        RCLCPP_WARN(node->get_logger(), "Drone %d doesn't start, set goal again!", i);
        cout << "drone_id=" << i << " goal=" << drones_[i].goal.transpose() << " t=" << t_now.seconds() << endl;
      }
    }

    int drone_trials = 0;
    while (drone_trials < drone_num)
    {
      int d_id = std::min(drone_num - 1, int(floor(((double)rand() / RAND_MAX) * drone_num)));
      // cout << "d_id=" << d_id << " drones_[d_id].odom_received=" << drones_[d_id].odom_received << " drones_[d_id].arrived_for_a_while=" << drones_[d_id].arrived_for_a_while << endl;
      if (drones_[d_id].odom_received && drones_[d_id].arrived_for_a_while)
      {
        int goal_trials = 0;
        while (goal_trials < goal_num)
        {
          int g_id = std::min(goal_num - 1, int(floor(((double)rand() / RAND_MAX) * goal_num)));
          double ang = acos(((goals[g_id].p - drones_[d_id].cur_p).normalized()).dot((Eigen::Vector3d(cos(drones_[d_id].cur_yaw), sin(drones_[d_id].cur_yaw), 0)).normalized()));
          double dist_to_goal = (goals[g_id].p - drones_[d_id].cur_p).norm();
          // cout << "g_id=" << g_id << " goals[g_id].occupied=" << goals[g_id].occupied << " ang=" << ang << endl;
          if (!goals[g_id].occupied && dist_to_goal > 5.0)
          {
            if (drones_[d_id].goal_id >= 0)
              goals[drones_[d_id].goal_id].occupied = false;
            goals[g_id].occupied = true;
            drones_[d_id].arrived_for_a_while = false;
            drones_[d_id].last_goal = drones_[d_id].goal;
            drones_[d_id].goal = goals[g_id].p;
            drones_[d_id].goal_id = g_id;
            drones_[d_id].set_new_goal_time = t_now;
            quadrotor_msgs::msg::GoalSet msg;
            msg.drone_id = d_id;
            msg.goal[0] = drones_[d_id].goal(0);
            msg.goal[1] = drones_[d_id].goal(1);
            msg.goal[2] = drones_[d_id].goal(2);
            goals_pub_->publish(msg);
            cout << "drone_id=" << d_id << " goal=" << drones_[d_id].goal.transpose() << " t=" << t_now.seconds() << endl;
            goto multi_loop;
          }

          goal_trials++;
        }
      }

      drone_trials++;
    }

  multi_loop:;

    std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
    if (rclcpp::ok()) executor.spin_some();
  }

  // 在进程静态析构前释放 ROS 实体，避免全局发布器晚于 DDS 上下文析构。
  odoms_sim_sub.clear();
  combined_odoms_sim_sub.reset();
  goals_pub_.reset();
  executor.remove_node(node);
  node.reset();
  rclcpp::shutdown();
  return 0;
}
