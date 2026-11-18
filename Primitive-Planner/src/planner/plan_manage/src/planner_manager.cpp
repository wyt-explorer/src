/*
 * 文件作用：ROS 2 Jazzy 规划与仿真组件，保留原运动基元算法。
 * 调用关系：由对应包的节点或库调用；输入输出见消息回调与发布接口。
 * 输入：ROS 2 消息、参数及运动基元文件（SI 单位，world 坐标系）。
 * 输出：规划轨迹、控制指令或可视化数据。
 * 运行模式：ROS 2 仿真/实机；依赖 rclcpp 和对应包的消息类型。
 */
#include <cstdio>
#include <plan_manage/planner_manager.h>
#include <thread>
#include "visualization_msgs/msg/marker.hpp" 

using namespace std;

namespace primitive_planner
{

void PPPlannerManager::initPlanModules(rclcpp::Node::SharedPtr nh, PlanningVisualization::Ptr vis){
  nh_ = nh;
  primitiveFolder_ = nh->declare_parameter<std::string>("manager/primitiveFolder", "none");
  depthCloudStackNum_ = nh->declare_parameter("manager/depthCloudStackNum", 4);//local map 12Hz - save 1s pointcloud
  boxX_ = nh->declare_parameter("manager/boxX", 8.0);
  boxY_ = nh->declare_parameter("manager/boxY", 9.0);
  boxZ_ = nh->declare_parameter("manager/boxZ", 9.0);
  arc_length_ = nh->declare_parameter("manager/arc_length", 5.0);
  voxelSize_ = nh->declare_parameter("manager/voxelSize", 0.1);
  // pathNum_ = nh->declare_parameter("manager/pathNum", 61);
  sampleSize_ = nh->declare_parameter("manager/sampleSize", 2000);
  // lamda_c_ = nh->declare_parameter("manager/lamda_c", 12.0);
  lamda_l_ = nh->declare_parameter("manager/lamda_l", 12.0);
  lamda_b_ = nh->declare_parameter("manager/lamda_b", 12.0);
  x_size_ = nh->declare_parameter("manager/map_size_x", -1.0);
  y_size_ = nh->declare_parameter("manager/map_size_y", -1.0);
  z_size_ = nh->declare_parameter("manager/map_size_z", -1.0);
  max_vel_ = nh->declare_parameter("manager/max_vel", -1.0);
  drone_id = nh->declare_parameter("manager/drone_id", -1);
  swarm_clearence_ = nh->declare_parameter("manager/swarm_clearence", -1.0);


  voxelNumX_ = int(boxX_ / voxelSize_);
  voxelNumY_ = int(boxY_ / voxelSize_);
  voxelNumZ_ = int(boxZ_ / voxelSize_);
  voxelNumAll_ = voxelNumX_ * voxelNumY_ * voxelNumZ_;
  voxelX_ = boxX_;
  voxelY_ = boxY_ / 2.0;
  voxelZ_ = boxZ_ / 2.0;
  agentVoxelNumX_ = int(2.0 * voxelX_ / voxelSize_);
  agentVoxelNumAll_ = agentVoxelNumX_ * voxelNumY_ * voxelNumZ_;

  correspondences_.resize(voxelNumAll_);
  for(int i = 0; i < voxelNumAll_; i++){
    correspondences_[i].resize(0);
  }

  // ros::Time t1, t2;
  // t1 = nh_->now();

  pathNum_ = readPathList();
  readPathAll();
  readCorrespondences();
  readAgentCorrespondences();
  // t2 = nh_->now();

  // std::cout << "======readCorrespondences time====== " << (t2 - t1).seconds() << std::endl;

  depthCloudCount_ = 0;
  dep_odom_sub_ = nh->create_subscription<nav_msgs::msg::Odometry>("plan_manage/odom", 10, std::bind(&PPPlannerManager::odomCallback, this, std::placeholders::_1));

  //订阅局部点云(world) 转换为局部robot系下 标记path
  dep_cloud_sub_ = nh->create_subscription<sensor_msgs::msg::PointCloud2>("plan_manage/cloud", rclcpp::SensorDataQoS(), std::bind(&PPPlannerManager::cloudCallback, this, std::placeholders::_1));

  // init depthCloudStack_
  depthCloudStack_.resize(depthCloudStackNum_);

  // set leaf size in downsample
  // downSizeFilter_.setLeafSize(voxelSize_, voxelSize_, voxelSize_);

  has_odom_ = false;
  has_cloud_ = false;

  visualization_ = vis;
}

void PPPlannerManager::odomCallback(const nav_msgs::msg::Odometry::ConstSharedPtr &odom)
{

  robot_pos_(0) = odom->pose.pose.position.x;
  robot_pos_(1) = odom->pose.pose.position.y;
  robot_pos_(2) = odom->pose.pose.position.z;

  // robot_q_.x() = odom->pose.pose.orientation.x;
  // robot_q_.y() = odom->pose.pose.orientation.y;
  // robot_q_.z() = odom->pose.pose.orientation.z;
  // robot_q_.w() = odom->pose.pose.orientation.w;

  has_odom_ = true;
}

void PPPlannerManager::cloudCallback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &img)
{
  // std::cout << "cloudCallback 0" << std::endl;
  // *latestCloud in world frame
  pcl::PointCloud<pcl::PointXYZ>::Ptr latestCloud(new pcl::PointCloud<pcl::PointXYZ>());
  pcl::fromROSMsg(*img, *latestCloud);

  if (!has_odom_)
  {
    std::cout << "no odom!" << std::endl;
    return;
  }

  // std::cout << "cloudCallback 1" << std::endl;

  if (latestCloud->points.size() == 0)
    return;

  // if (isnan(robot_pos_(0)) || isnan(robot_pos_(1)) || isnan(robot_pos_(2)))
  //   return;

  depthCloudStack_[depthCloudCount_] = latestCloud;
  depthCloudCount_ = (depthCloudCount_ + 1) % depthCloudStackNum_;

  // std::cout << "cloudCallback 2" << std::endl;

  static int cloud_num = 0;
  cloud_num++;
  if(cloud_num == depthCloudStackNum_){
    cloud_num = 0;
    has_cloud_ = true;
    // std::cout << "cloudCallback 4" << std::endl;
  }

}

// 点云下采样 转换坐标系 按照correspondence voxel大小筛选点云 标记path
bool PPPlannerManager::labelObsCollisionPaths(const Eigen::Vector3d &start_pt, const Eigen::Matrix3d &rotVW)
{
  clearPathList_.clear();
  clearPathList_.resize(pathNum_, 0);

  // static Eigen::Matrix3d rotVW_last;
  // static Eigen::Vector3d start_pt_last;

  // has_cloud_ = true: label collision paths
  if(has_cloud_){
    // std::cout << "labelCollisionPaths 0" << std::endl;
    // std::cout <<  depthCloudStack_[0]->points.size() <<  std::endl;

    // downsample 
    pcl::PointCloud<pcl::PointXYZ>::Ptr plannerCloudStack(new pcl::PointCloud<pcl::PointXYZ>());
    for (int i = 0; i < depthCloudStackNum_; i++) {
      *plannerCloudStack += *(depthCloudStack_[i]);
      // std::cout << "labelCollisionPaths 1:" << i << std::endl;
    }

    // std::cout << "labelCollisionPaths 1" << std::endl;

    // if cloud size is too large, consider setting up random sample(fixed size)
    pcl::PointCloud<pcl::PointXYZ>::Ptr plannerCloud(new pcl::PointCloud<pcl::PointXYZ>());

    // [VoxelGrid --- high fidelity, but low]
    // downSizeFilter_.setInputCloud(plannerCloudStack);
    // downSizeFilter_.filter(*plannerCloud);

    // [RandomSample --- low fidelity, but fast]
    rs_.setInputCloud(plannerCloudStack);
    rs_.setSample(sampleSize_);
    rs_.filter(*plannerCloud);

    // plannerCloud = plannerCloudStack;

    // TODO:[real robot pointcloud noise] use raycast to avoid

    // plannerCloudStack transform to Body Frame
    // Eigen::Matrix3d rotVW = rotWV.inverse();
    Eigen::Vector3d pos_v, pos_w;
    int plannerCloudSize = plannerCloud->points.size();

    // std::cout << "downsample cloud size: " << plannerCloudSize << std::endl;

    int indX, indY, indZ, ind, occPathByVoxelNum;

    // ros::Time t1, t2;
    // t1 = nh_->now();
    // TODO: [???] GPU parallel computation
    for(int i = 0; i < plannerCloudSize; i++){
      //pointcloud: world frame in simulation; body frame in experiment;
      pos_w(0) = plannerCloud->points[i].x - start_pt(0);
      pos_w(1) = plannerCloud->points[i].y - start_pt(1);
      pos_w(2) = plannerCloud->points[i].z - start_pt(2);

      pos_v = rotVW * pos_w;

      // select pointcloud in box 
      if((pos_v(0) >= 1e-4 && pos_v(0) <= voxelX_ - 1e-4) && 
          (pos_v(1) >= - voxelY_ + 1e-4 && pos_v(1) <= voxelY_ - 1e-4) &&
          (pos_v(2) >= - voxelZ_ + 1e-4 && pos_v(2) <= voxelZ_ - 1e-4))
          {
            indX = floor((voxelX_ - pos_v(0)) / voxelSize_);
            indY = floor((voxelY_ - pos_v(1)) / voxelSize_);
            indZ = floor((voxelZ_ - pos_v(2)) / voxelSize_);

            ind = voxelNumY_ * voxelNumZ_ * indX + voxelNumZ_ * indY + indZ;
            occPathByVoxelNum = correspondences_[ind].size();
            // if ( occPathByVoxelNum >= pathNum_ )
            // {

            //   std::vector<double> d_list(plannerCloudSize);
            //   for ( int j=0; j<plannerCloudSize; j++ )
            //   {
            //     d_list[j] = 9999;
            //     for ( int k=0; k<plannerCloudSize; k++ )
            //     {
            //       if ( j != k )
            //       {
            //         Eigen::Vector3d x(plannerCloud->points[j].x, plannerCloud->points[j].y, plannerCloud->points[j].z);
            //         Eigen::Vector3d y(plannerCloud->points[k].x, plannerCloud->points[k].y, plannerCloud->points[k].z);
            //         double d=(x-y).norm();
            //         if (d < d_list[j])
            //           d_list[j] = d;
            //       }
            //     }
            //   }

            //   std::sort(d_list.begin(), d_list.end());
            //   int small_c = 0, big_c = 0;
            //   for ( int j=0; j<plannerCloudSize; j++ )
            //   {
            //     if (d_list[j] <= 0.1)
            //       small_c ++;
            //     else
            //       big_c ++;
            //     cout << " " << d_list[j];
            //   }
            //   cout << endl;
            //   cout << "small_c=" << small_c <<" big_c=" <<big_c << endl;


            //   plannerCloud_debug = *plannerCloud;
            //   rotVW_debug = rotVW;
            //   start_pt_debug = start_pt;
            //   occ_now_debug = plannerCloud->points[i];
            // ROS_ERROR("occPathByVoxelNum=%d, indX=%d, indY=%d, indZ=%d, pos_v=(%f, %f, %f), plannerCloudSize=%d", 
            //   occPathByVoxelNum, indX, indY, indZ, pos_v(0), pos_v(1), pos_v(2), plannerCloudSize);
            // cout << "pos_w=" << pos_w.transpose() << " cp=" << plannerCloud->points[i].x << " " << plannerCloud->points[i].y << " " << plannerCloud->points[i].z << " start_pt=" << start_pt.transpose() << endl;
            

            //   pos_w(0) = plannerCloud->points[i].x - start_pt_last(0);
            //   pos_w(1) = plannerCloud->points[i].y - start_pt_last(1);
            //   pos_w(2) = plannerCloud->points[i].z - start_pt_last(2);
            //   Eigen::Vector3d pc(plannerCloud->points[i].x, plannerCloud->points[i].y, plannerCloud->points[i].z);
            //   pos_v = rotVW_last * pos_w;
            //   for ( int j=0; j<traj_pos_exec_.size(); ++j ) {
            //     cout << "d=" << (traj_pos_exec_[j] - pc).norm() << " traj_pos_exec_[" << j << "]=" << traj_pos_exec_[j].transpose() << endl; 
            //   }
            //   cout << "pos_w=" << pos_w.transpose() << " start_pt_debug=" << start_pt_debug.transpose() << endl;

            //   exit(0);
            // }
            for(int j = 0; j < occPathByVoxelNum; j++){
              clearPathList_[correspondences_[ind][j]]++;
            }
          }
    }
    // t2 = nh_->now();

    // std::cout << "for loop time: " << (t2 - t1).seconds() << std::endl;
  
  }

  // rotVW_last = rotVW;
  // start_pt_last = start_pt;

  return true;
}

vector<int> PPPlannerManager::scorePaths(const Eigen::Vector3d &start_pt, 
                          const Eigen::Vector3d &global_goal, const Eigen::Matrix3d &rotWV)
{
  /* score: 1. collision;
                    2. close to global goal; or delta_pitch/yaw(between endpoint and goal in body frame)
                    || p_0 - p_goal || - || p(t) - p_goal || (p_0 --- odom) 大小始终维持在一定范围内
                    3. low delta_w (smoothness) ;
  TODO: 暂定倾向于靠近中心线的primitive(后续根据实验效果调整)
  */
  double cost, goal_dist;
  // int best_path_id;

  std::map<double, int> mapCost;
  std::vector<int> select_path_id;
  Eigen::Vector3d endPoint;
  // Eigen::Vector4d collision_color(1, 0, 0, 1);
  for(int i = 0; i < pathNum_; i++){

    // collision paths
    if(clearPathList_[i] > 0){
      // TODO:[lable red]
      // visualization_->displayInitPathList(pathAllWorld_[i], 0.05, collision_color, 0);
      continue;
    }

    // cost: lower -> better
    //rotWV * pathEndList_ + start_pt: body -> world;
    endPoint = rotWV * pathEndList_[i] + start_pt;
    if ( (start_pt - global_goal).norm() > pathLengthMax_ || rotWV.col(0).dot(global_goal - start_pt) <= 0 ) {
      goal_dist = (endPoint - global_goal).norm() - (start_pt - global_goal).norm();
    }
    else {
      goal_dist = 99999;
      for ( size_t j = 0; j < pathAll_[i].size(); ++j )
      {
        Eigen::Vector3d traj_pt = rotWV * pathAll_[i][j] + start_pt;
        double dist = (traj_pt - global_goal).norm();;
        goal_dist = goal_dist > dist ? dist : goal_dist;
      }
    }
    

    double bound_cost = 0;
    if(endPoint(0) < -x_size_/2 || endPoint(0) > x_size_/2 || endPoint(1) < -y_size_/2 || endPoint(1) > y_size_/2 || endPoint(2) < 0 || endPoint(2) > z_size_){
      bound_cost = 10;
    }

    // std::cout << "i = " << i << ": " << clearPathList_[i] << " " << goal_dist << " " << bound_cost << std::endl;

    // cost = lamda_c_ * clearPathList_[i] + lamda_l_ * goal_dist + lamda_b_ * bound_cost;
    cost = lamda_l_ * goal_dist + lamda_b_ * bound_cost;

    // std::cout << "cost = " << cost << std::endl;

    // if(cost < best_cost){
    //   best_cost = cost;
    //   best_path_id = i;
    // }
    mapCost.insert({cost, i});
  }

  // TODO: map 数据结构需要修改 我们并不需要key 去查询 value
  if(!mapCost.empty()){
    std::map<double, int>::iterator it;
    for (it = mapCost.begin(); it != mapCost.end(); it++){
      select_path_id.push_back((*it).second);
    }
    // TODO:[label green]
    // Eigen::Vector4d select_color(0, 1, 0, 1);
    // visualization_->displayInitPathList(pathAllWorld_[select_path_id.front()], 0.05, select_color, 0);

  } else{
    std::fprintf(stderr, "====[id:%d] All primitives are infeasible!====\n", drone_id);
  }

  // std::cout << "select_path_id: ";
  // for(int i = 0; i < (int)select_path_id.size(); i++){
  //   std::cout << select_path_id[i] << " ";
  // }

  // revise to vector<int> select_path_id
  return select_path_id;
}

void PPPlannerManager::readCorrespondences()
{
  std::string fileName = primitiveFolder_ +"/obs_correspondence/obs_correspondence.txt";

  // std::cout << "primitiveFolder:" << primitiveFolder_ << std::endl;
  // std::cout << "file:" << fileName << std::endl;
  FILE *filePtr = fopen(fileName.c_str(), "rb");
  if (filePtr == NULL) {
    printf ("\nCannot read input [correspondence files], exit.\n\n");
    exit(1);
  }

  int val1, voxelID, pathID;
  for (int i = 0; i < voxelNumAll_; i++) {
    // std::cout << "voxelNum: " << i << std::endl;

    val1 = fread(&voxelID, 4, 1, filePtr);
    if (val1 != 1) {
      printf ("\nError reading [voxelID] input files, exit.\n\n");
        exit(1);
    }

    while (1) {
      val1 = fread(&pathID, 4, 1, filePtr);
      if (val1 != 1) {
        printf ("\nError reading [pathID] input files, exit.\n\n");
          exit(1);
      }

      // std::cout << "pathID: " << pathID << std::endl;

      if (pathID != -1) {
        if (voxelID >= 0 && voxelID < voxelNumAll_ && pathID >= 0 && pathID < pathNum_) {
          correspondences_[voxelID].push_back(pathID);
        }
      } else {
        break;
      }
    }
  }

  // check
  // for (int i = 0; i < voxelNumAll_; i++) {
  //   if(!correspondences_[i].empty()){
  //     std::cout << "correspondences_[" << i << "]: " << std::endl;
  //     for(int j = 0; j < (int)correspondences_[i].size(); j++){
  //       std::cout << correspondences_[i][j] << " ";
  //     }
  //     std::cout << std::endl;
  //   }
  // }

  fclose(filePtr);
}

void PPPlannerManager::readAgentCorrespondences()
{
  std::string a = primitiveFolder_ + "/agent_correspondence/";
  std::string b = "_correspondence.txt";
  for(int i = 0; i <= int(max_vel_*10); i++)
  {
    std::stringstream ss;
    ss << a << i << b;
    std::string fileName = ss.str();
    // std::cout << "file:" << fileName << std::endl;

    FILE *filePtr = fopen(fileName.c_str(), "rb");
    if (filePtr == NULL) {
      printf ("\nCannot read input [agentcorrespondence files], i=%d, exit.\n\n", i);
      exit(1);
    }

    std::vector<std::vector<int>> velCorrespondences(agentVoxelNumAll_, std::vector<int>(0, 0));

    int val1, val2, val3, voxelID, pathID, tStart, tEnd;
    for (int j = 0; j < agentVoxelNumAll_; j++) {

      val1 = fread(&voxelID, 4, 1, filePtr);
      if (val1 != 1) {
        printf ("\nError reading [voxelID] input files, exit.\n\n");
          exit(1);
      }

      while (1) {
        val1 = fread(&pathID, 4, 1, filePtr);
        if (val1 != 1) {
          printf ("\nError reading [pathID] input files, exit.\n\n");
            exit(1);
        }

        // std::cout << "pathID: " << pathID << std::endl;

        if (pathID != -1) {
          val2 = fread(&tStart, 4, 1, filePtr);
          val3 = fread(&tEnd, 4, 1, filePtr);

          if (voxelID >= 0 && voxelID < agentVoxelNumAll_ && pathID >= 0 && pathID < pathNum_) {
            velCorrespondences[voxelID].push_back(pathID);
            velCorrespondences[voxelID].push_back(tStart);
            velCorrespondences[voxelID].push_back(tEnd);
          }
        } else {
          break;
        }
      }
    }
    fclose(filePtr);

    allVelCorrespondences_.push_back(velCorrespondences);
  }

  // check
  // for (int i = 0; i <= int(max_vel_*10); i++) {
  //   std::cout << "=====Output vel = " << i <<" correspondence=====" << std::endl;
  //   for (int j = 0; j < voxelNumAll_; j++) {
  //     if(!allVelCorrespondences_[i][j].empty()){
  //       std::cout << "allVelCorrespondences_[" << i << "][" << j << "]: " << std::endl;
  //       for(int k = 0; k < (int)allVelCorrespondences_[i][j].size(); k++){
  //         std::cout << allVelCorrespondences_[i][j][k] << " ";
  //       }
  //       std::cout << std::endl;
  //     }
  //   }
  // }
}

// void PPPlannerManager::readPathList()
// {
//   std::string fileName = primitiveFolder_ + "/obs_correspondence/path_end.ply";

//   FILE *filePtr = fopen(fileName.c_str(), "r");
//   if (filePtr == NULL) {
//     printf ("\nCannot read input [path_end files], exit.\n\n");
//     exit(1);
//   }

//   // if (pathNum != readPlyHeader(filePtr)) {
//   //   printf ("\nIncorrect path number, exit.\n\n");
//   //   exit(1);
//   // }

//   int val1, val2, val3, val4, pathID;
//   double endX, endY, endZ;
//   for (int i = 0; i < pathNum_; i++) {
//     val1 = fscanf(filePtr, "%lf", &endX);
//     val2 = fscanf(filePtr, "%lf", &endY);
//     val3 = fscanf(filePtr, "%lf", &endZ);
//     val4 = fscanf(filePtr, "%d", &pathID);

//     if (val1 != 1 || val2 != 1 || val3 != 1 || val4 != 1) {
//       printf ("\nError reading [pathList] input files, exit.\n\n");
//         exit(1);
//     }

//     if (pathID >= 0 && pathID < pathNum_) {
//       pathEndList_.push_back(Eigen::Vector3d(endX, endY, endZ));
//     }
//   }

//   // check
//   // for(int i = 0; i < (int)pathEndList_.size(); i++){
//   //   std::cout << "pathEndList_[" << i << "]: " << pathEndList_[i] << std::endl;
//   // }

//   fclose(filePtr);

// }

int PPPlannerManager::readPathList()
{
  std::string fileName = primitiveFolder_ + "/obs_correspondence/path_end.ply";

  FILE *filePtr = fopen(fileName.c_str(), "r");
  if (filePtr == NULL) {
    std::cerr << "\nCannot read input [path_end files], exit.\n\n";
    exit(1);
  }

  int val1, val2, val3, val4;
  double endX, endY, endZ;
  int pathID;
  int totalLines = 0; 

  while (fscanf(filePtr, "%lf %lf %lf %d", &endX, &endY, &endZ, &pathID) == 4) 
  {
    if (pathID >= 0) 
    {
      pathEndList_.push_back(Eigen::Vector3d(endX, endY, endZ));
    }
    totalLines++; 
  }

  fclose(filePtr);
  return totalLines;
}

void PPPlannerManager::readPathAll()
{
  pathAll_.resize(pathNum_);
  pathLengthList_.resize(pathNum_);
  pathLengthMax_ = 0;
  
  std::string fileName = primitiveFolder_ + "/obs_correspondence/path_all.ply";

  FILE *filePtr = fopen(fileName.c_str(), "r");
  if (filePtr == NULL) {
    printf ("\nCannot read [path_all] input files, exit.\n\n");
    exit(1);
  }

  int pointNum, val_num, val1, val2, val3, val4, pathID, pathID_last = -666;
  Eigen::Vector3d pos, pos_last;
  val_num = fscanf(filePtr, "%d", &pointNum);
  double length = 0;

  for (int i = 0; i < pointNum; i++) {
    val1 = fscanf(filePtr, "%lf", &pos(0));
    val2 = fscanf(filePtr, "%lf", &pos(1));
    val3 = fscanf(filePtr, "%lf", &pos(2));
    val4 = fscanf(filePtr, "%d", &pathID);

    if (val1 != 1 || val2 != 1 || val3 != 1 || val4 != 1) {
      printf ("\nError reading [path_all] input files, exit.\n\n");
      exit(1);
    }

    if ( pathID == pathID_last ){
      length += (pos - pos_last).norm();
      if ( pathLengthMax_ < length )
        pathLengthMax_ = length;
    }

    // TODO: path上间隔0.1m保存一次；
    if (pathID >= 0 && pathID < pathNum_) {
      if ( pathAll_[pathID].empty() || (pos - pathAll_[pathID].back()).norm() > 0.1) {
        pathAll_[pathID].push_back(pos);
      }
    }
    else {
      std::fprintf(stderr, "Illegal pathID=%d\n", pathID);
      exit(0);
    }
    

    if ( i == pointNum - 1 ){
      pathAll_[pathID].push_back(pos_last);
      pathLengthList_[pathID] = length;
    }

    if ( pathID_last != -666 && pathID != pathID_last ){
      pathAll_[pathID_last].push_back(pos_last);
      pathLengthList_[pathID_last] = length;
      length = 0;
    }

    pos_last = pos;
    pathID_last = pathID;
  }

  fclose(filePtr);
}

void PPPlannerManager::visAllPaths(const Eigen::Vector3d &start_pt, const Eigen::Matrix3d &rotWV)
{
  pathAllWorld_.clear();
  pathAllWorld_.resize(pathNum_);
  Eigen::Vector3d pos_world;
  for(int i = 0; i < (int)pathAll_.size(); i++){
    for(int j = 0; j < (int)pathAll_[i].size(); j++){
      pos_world = start_pt + rotWV * pathAll_[i][j];
      pathAllWorld_[i].push_back(pos_world);
    }
  }

  visualization_->displayMultiOptimalPathList(pathAllWorld_, 0.05);

}

bool PPPlannerManager::labelAgentCollisionPaths(const Eigen::Vector3d &start_pt, const Eigen::Vector3d &start_v, const double& start_time, const Eigen::Matrix3d &rotVW)
{
  // Eigen::Matrix3d rotVW = rotWV.inverse();
  int vel_id = int(round(start_v.norm() * 10));
  if(vel_id > int(max_vel_ * 10)){
    vel_id = max_vel_ * 10;
  }

  Eigen::Vector3d pos_v;
  double other_start_time, other_cur_time;

  // std::cout << "swarm_traj.size(): " << swarm_traj.size() <<  std::endl;
  for(int i = 0; i < (int)swarm_traj.size(); i++)
  {
    if(swarm_traj[i].drone_id < 0 || swarm_traj[i].drone_id == drone_id)
    {
      // ROS_WARN("Drone %d trajectory is infeasible. status = %d", i, swarm_traj[i].drone_id);
      continue;
    }

    other_start_time = swarm_traj[i].start_time;

    // std::cout << "swarm_traj[" << i << "].traj_pos.size(): " << swarm_traj[i].traj_pos.size() << std::endl;

    int indX, indY, indZ, ind, occPathNumByVoxel;

    // TODO:[??? param set] check time resolution = voxel_resolution/max_vel / 5
    for(int j = 0; j < (int)swarm_traj[i].traj_pos.size(); j += std::max(1, (int)(floor(voxelSize_ / max_vel_ * 100 / 5))))
    {
      pos_v = rotVW * (swarm_traj[i].traj_pos[j] - start_pt);

      // std::cout << "pos_v: " << pos_v << "j: " << j << std::endl;

      // TODO: whether in the range of box
      if((pos_v(0) >= -voxelX_ + 1e-4 && pos_v(0) <= voxelX_ - 1e-4) &&
          (pos_v(1) >= - voxelY_ + 1e-4 && pos_v(1) <= voxelY_ - 1e-4) &&
          (pos_v(2) >= - voxelZ_ + 1e-4 && pos_v(2) <= voxelZ_ - 1e-4))
          {
            indX = floor((voxelX_ - pos_v(0)) / voxelSize_);
            indY = floor((voxelY_ - pos_v(1)) / voxelSize_);
            indZ = floor((voxelZ_ - pos_v(2)) / voxelSize_);

            ind = voxelNumY_ * voxelNumZ_ * indX + voxelNumZ_ * indY + indZ;

            occPathNumByVoxel = allVelCorrespondences_[vel_id][ind].size() / 3;

            // std::cout << "occPathNumByVoxel: " << occPathNumByVoxel << "j: " << j << std::endl;

            // time unit must be same sec.
            other_cur_time = other_start_time + j * 0.01;
            for(int k = 0; k < occPathNumByVoxel; k++)
            {
              if(other_cur_time > start_time + allVelCorrespondences_[vel_id][ind][3*k + 1] / 1000.0 && other_cur_time < start_time + allVelCorrespondences_[vel_id][ind][3*k + 2] / 1000.0){
                // std::cout << "=== labelAgentCollisionPaths ID === " << allVelCorrespondences_[vel_id][ind][3*k] << "  " << 
                // allVelCorrespondences_[vel_id][ind][3*k+1] << "  " <<
                // allVelCorrespondences_[vel_id][ind][3*k+2] << std::endl;
                clearPathList_[allVelCorrespondences_[vel_id][ind][3*k]]++;
              }
            }
          }
    }
  }

  // std::cout << "=== labelAgentCollisionPaths 3 ===" << std::endl;

  return true;

}

bool PPPlannerManager::trajReplan(const Eigen::Vector3d &start_pt, const Eigen::Vector3d &start_v, const double& start_time, const Eigen::Matrix3d &RWV, const Eigen::Vector3d &global_goal, vector<int> &select_path_id)
{
  // std::cout << "trajReplan 0" << std::endl;
  // Eigen::Vector3d rotWV = start_q;
  
  Eigen::Matrix3d RVW = RWV.inverse();

  bool success_obs, success_agents;
  rclcpp::Time t0, t1, t2, t3;
  t0 = nh_->now();
  success_obs = labelObsCollisionPaths(start_pt, RVW);
  t1 = nh_->now();

  // bool safe = false;
  // for (auto it : clearPathList_)
  //   if (it == 0)
  //   {
  //     safe = true;
  //     break;
  //   }
  // if ( !safe )
  //   cout << "A clearPathList_ all > 0" << endl;

  // std::cout << "trajReplan 1" << std::endl;

  success_agents = labelAgentCollisionPaths(start_pt, start_v, start_time, RVW);
  t2 = nh_->now();

  // safe = false;
  // for (auto it : clearPathList_)
  //   if (it == 0)
  //   {
  //     safe = true;
  //     break;
  //   }
  // if ( !safe )
  //   cout << "B clearPathList_ all > 0" << endl;

  if(!success_obs || !success_agents)
    return false;

  // std::cout << "trajReplan 2" << std::endl;

  // visulize all path
  // visAllPaths(start_pt, RWV);

  select_path_id = scorePaths(start_pt, global_goal, RWV);
  t3 = nh_->now();

  // printf("\033[44;97m In id=%d, t1=%.1f, t2=%.1f, t3=%.1f \033[0m\n", drone_id, (t1-t0).seconds() * 1000, (t2-t0).seconds() * 1000, (t3-t0).seconds() * 1000);

  if(select_path_id.empty())
    return false;

  return true;
}


} // namespace primitive_planner
