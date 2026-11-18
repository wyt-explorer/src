/*
 * 文件作用：ROS 2 Jazzy 规划与仿真组件，保留原运动基元算法。
 * 调用关系：由对应包的节点或库调用；输入输出见消息回调与发布接口。
 * 输入：ROS 2 消息、参数及运动基元文件（SI 单位，world 坐标系）。
 * 输出：规划轨迹、控制指令或可视化数据。
 * 运行模式：ROS 2 仿真/实机；依赖 rclcpp 和对应包的消息类型。
 */
#ifndef _PLANNER_MANAGER_H_
#define _PLANNER_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <rclcpp/rclcpp.hpp>
#include <traj_utils/planning_visualization.h>
#include <traj_utils/plan_container.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include<vector>
#include<map>
#include<string>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>

#include <pcl/filters/random_sample.h>

namespace primitive_planner
{
  // Primitive Planner Manager
  // Key algorithms of mapping and planning are called

  class PPPlannerManager
  {
  public:
    PPPlannerManager() {};
    ~PPPlannerManager() {};

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    void initPlanModules(rclcpp::Node::SharedPtr nh, PlanningVisualization::Ptr vis = NULL);
    void odomCallback(const nav_msgs::msg::Odometry::ConstSharedPtr &odom);
    void cloudCallback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &img);
    bool labelObsCollisionPaths(const Eigen::Vector3d &start_pt, const Eigen::Matrix3d &rotVW);
    bool labelAgentCollisionPaths(const Eigen::Vector3d &start_pt, const Eigen::Vector3d &start_v, const double& start_time, const Eigen::Matrix3d &rotVW);
    vector<int> scorePaths(const Eigen::Vector3d &start_pt, const Eigen::Vector3d &global_goal, const Eigen::Matrix3d &rotWV);
    void visAllPaths(const Eigen::Vector3d &start_pt, const Eigen::Matrix3d &rotWV);
    bool trajReplan(const Eigen::Vector3d &start_pt, const Eigen::Vector3d &start_v, const double& start_time, const Eigen::Matrix3d &RWV, const Eigen::Vector3d &global_goal, vector<int> &select_path_id);


    void readCorrespondences();
    void readAgentCorrespondences();
    // int readPlyHeader(FILE *filePtr);
    int readPathList();
    void readPathAll();

    int drone_id;
    double max_vel_;
    double arc_length_;
    double swarm_clearence_;
    std::string primitiveFolder_;
    SwarmTrajData swarm_traj;

    double voxelSize_;

    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> depthCloudStack_;

    int depthCloudStackNum_, depthCloudCount_;

    bool has_odom_, has_cloud_;

  private:
    rclcpp::Node::SharedPtr nh_;
    PlanningVisualization::Ptr visualization_;

    rclcpp::SubscriptionBase::SharedPtr dep_odom_sub_, dep_cloud_sub_;

    Eigen::Vector3d robot_pos_;
    Eigen::Quaterniond robot_q_;

    // pcl::VoxelGrid<pcl::PointXYZ> downSizeFilter_;// VoxelGrid filter
    pcl::RandomSample<pcl::PointXYZ> rs_;//RandomSample

    double boxX_, boxY_, boxZ_;
    double voxelX_, voxelY_, voxelZ_;
    int voxelNumX_, voxelNumY_, voxelNumZ_, voxelNumAll_;
    int agentVoxelNumX_, agentVoxelNumAll_;

    int pathNum_, sampleSize_;

    std::vector<std::vector<int>> correspondences_;
    std::vector<std::vector<std::vector<int>>> allVelCorrespondences_;
    std::vector<int> clearPathList_;
    std::vector<Eigen::Vector3d> pathEndList_; //body frame
    std::vector<std::vector<Eigen::Vector3d>> pathAll_, pathAllWorld_; 
    std::vector<double> pathLengthList_;
    double pathLengthMax_;

    double lamda_c_, lamda_l_, lamda_b_; //TODO:待定，实验效果测试

    double x_size_, y_size_, z_size_;

    // path parameters


    // point cloud



  public:
    typedef std::unique_ptr<PPPlannerManager> Ptr;

  };
} // namespace primitive_planner

#endif
