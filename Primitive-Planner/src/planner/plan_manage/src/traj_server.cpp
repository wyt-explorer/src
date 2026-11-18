/*
 * 文件作用：ROS 2 Jazzy 规划与仿真组件，保留原运动基元算法。
 * 调用关系：由对应包的节点或库调用；输入输出见消息回调与发布接口。
 * 输入：ROS 2 消息、参数及运动基元文件（SI 单位，world 坐标系）。
 * 输出：规划轨迹、控制指令或可视化数据。
 * 运行模式：ROS 2 仿真/实机；依赖 rclcpp 和对应包的消息类型。
 */
/*
 * 文件作用：ROS 2 Jazzy 规划与仿真组件，保留原运动基元算法。
 * 调用关系：由对应包的节点或库调用；输入输出见消息回调与发布接口。
 * 输入：ROS 2 消息、参数及运动基元文件（SI 单位，world 坐标系）。
 * 输出：规划轨迹、控制指令或可视化数据。
 * 运行模式：ROS 2 仿真/实机；依赖 rclcpp 和对应包的消息类型。
 */
#include <cstdio>
#include <nav_msgs/msg/odometry.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <std_msgs/msg/empty.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <std_msgs/msg/float64.hpp>
#include <traj_utils/msg/swarm_primitive_traj.hpp>
#include <traj_utils/msg/polynomial.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <rclcpp/rclcpp.hpp>
#include <Eigen/Eigen>
#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

using namespace Eigen;
using namespace std;

rclcpp::Node::SharedPtr node;

const double loop_time_ = 0.01;

rclcpp::Publisher<quadrotor_msgs::msg::PositionCommand>::SharedPtr pos_cmd_pub;
rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr select_path_end_pub;

quadrotor_msgs::msg::PositionCommand cmd;

bool receive_traj_ = false, require_yaw_turning_ = false, require_yaw_turning_external_ = false;
double traj_duration_;
rclcpp::Time start_time_(0), last_traj_start_time_(0);
bool have_new_ = false;

std::vector<int> path_id_;

rclcpp::Time heartbeat_time_(0);
Eigen::Vector3d last_pos_, last_vel_, global_goal_, init_odom_pos_, odom_pos_;
double odom_yaw_, init_odom_yaw_;
std::vector<Eigen::Vector3d> traj_pos_, traj_vel_, traj_acc_;

// yaw control
double last_yaw_, last_yawdot_;
double time_forward_;
double yaw_to_goal_;
double yaw_cmd_external_;
int yaw_dir_ = 1; // 1: increase; -1: decrease

bool have_odom_ = false;
Eigen::Quaterniond odom_q_;

double total_length_ = 0.0, integ_dis_ = 0.0, total_time_ = 0.0, integ_time_ = 0.0;
bool have_first_ = true;

std::string primitiveFolder_;
double max_vel_;
int time_delay_idx_;

double _init_x, _init_y, _init_z;

double yaw_diff(double yaw1, double yaw2)
{
  double diff = yaw1 - yaw2;
  diff = std::fmod(diff + M_PI, 2 * M_PI) - M_PI;
  if (diff < -M_PI)
    diff += 2 * M_PI;

  return diff;
}

double limit_yaw(double yaw)
{
  if ( yaw > M_PI ) yaw -= 2 * M_PI;
  if ( yaw < -M_PI ) yaw += 2 * M_PI;

  return yaw;
}

void publish_cmd(Vector3d p, Vector3d v, Vector3d a, double y, double yd)
{

  cmd.header.stamp = node->now();
  cmd.header.frame_id = "world";
  cmd.trajectory_flag = quadrotor_msgs::msg::PositionCommand::TRAJECTORY_STATUS_READY;
  // cmd.trajectory_id = traj_id_;

  cmd.position.x = p(0);
  cmd.position.y = p(1);
  cmd.position.z = p(2);
  cmd.velocity.x = v(0);
  cmd.velocity.y = v(1);
  cmd.velocity.z = v(2);
  cmd.acceleration.x = a(0);
  cmd.acceleration.y = a(1);
  cmd.acceleration.z = a(2);
  // TODO:[real world] close feedforward item
  // cmd.acceleration.x = 0;
  // cmd.acceleration.y = 0;
  // cmd.acceleration.z = 0;
  cmd.yaw = y;
  cmd.yaw_dot = yd;
  pos_cmd_pub->publish(cmd);

  last_pos_ = p;
  last_vel_ = v; 
}

void heartbeatCallback(std_msgs::msg::Empty::SharedPtr msg)
{
  heartbeat_time_ = node->now();
}

void odometryCallback(const nav_msgs::msg::Odometry::ConstSharedPtr msg)
{
    odom_pos_(0) = msg->pose.pose.position.x;
    odom_pos_(1) = msg->pose.pose.position.y;
    odom_pos_(2) = msg->pose.pose.position.z;

    odom_q_.x() = msg->pose.pose.orientation.x;
    odom_q_.y() = msg->pose.pose.orientation.y;
    odom_q_.z() = msg->pose.pose.orientation.z;
    odom_q_.w() = msg->pose.pose.orientation.w;

    Eigen::Vector3d x_dir = odom_q_.toRotationMatrix().col(0);
    odom_yaw_ = atan2(x_dir(1), x_dir(0));
    // std::cout << "odom_yaw_=" << odom_yaw_ <<std::endl;

    have_odom_ = true;
}

void stopCommandCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
  Eigen::Vector3d stop_pos(msg->data[0], msg->data[1], msg->data[2]);
  fprintf(stderr, "%s\n", "[TRAJ_SERVER] No suitable path exists 2!!!");
  publish_cmd(stop_pos, Vector3d::Zero(), Vector3d::Zero(),  last_yaw_, 0);
  //TODO: After emergency stop, not restart
  // receive_traj_ = false;
}

void goalEndCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
  if(!have_odom_)
  {
    fprintf(stderr, "%s\n", "[traj_server] no odom!");
    return;
  }

  global_goal_(0) = msg->data[0];
  global_goal_(1) = msg->data[1];
  global_goal_(2) = msg->data[2];

  if ( !receive_traj_ )
    require_yaw_turning_ = true;

  init_odom_pos_ = odom_pos_;
  init_odom_yaw_ = odom_yaw_;

  Eigen::Vector3d dir_to_goal = global_goal_ - odom_pos_;
  yaw_to_goal_ = atan2(dir_to_goal(1), dir_to_goal(0));

  double diff = yaw_diff(yaw_to_goal_, odom_yaw_);
  yaw_dir_ = diff > 0 ? 1 : -1;

  // std::cout << "global_goal_: " << global_goal_ << std::endl;
}

void yawCmdCallback(const std_msgs::msg::Float64::SharedPtr msg)
{
  if(!have_odom_)
  {
    fprintf(stderr, "%s\n", "[traj_server] no odom!");
    return;
  }

  yaw_cmd_external_ = msg->data;
  init_odom_yaw_ = odom_yaw_;
  init_odom_pos_ = odom_pos_;

  double diff = yaw_diff(yaw_cmd_external_, odom_yaw_);
  yaw_dir_ = diff > 0 ? 1 : -1;

  require_yaw_turning_external_ = true;
  receive_traj_ = false;
  

  // std::cout << "yaw_cmd_external_: " << yaw_cmd_external_ << std::endl;
}

void trajCallback(traj_utils::msg::SwarmPrimitiveTraj::SharedPtr msg)
{
  // if(!have_odom_){
  //   return;
  // }
  // fprintf(stderr, "%s\n", "[-----New Traj-----] Start !!!");

  Eigen::Vector3d start_pos;
  start_pos << msg->start_p[0], msg->start_p[1], msg->start_p[2];
  Eigen::Matrix<double, 3, 3> Rwv;
  Rwv << msg->rot_mat[0], msg->rot_mat[1], msg->rot_mat[2], 
      msg->rot_mat[3], msg->rot_mat[4], msg->rot_mat[5], 
      msg->rot_mat[6], msg->rot_mat[7], msg->rot_mat[8];

  path_id_.clear();
  path_id_.push_back(msg->select_path_id);

  FILE *filePtr;
  int idx = 0;
  std::string a = primitiveFolder_ + "/trajectory/";
  std::string b = "/";
  std::string c = "_trajectory.ply";
  while(true){
    std::stringstream ss;
    ss << a << msg->vel_id << b << path_id_[idx] << c;
    // std::string fileName = "../../primitive_library/trajectory/" + vel_id + "/" + path_id_[idx] + "_trajectory.ply";
    std::string fileName = ss.str();

    // std::cout << "file: " << fileName << std::endl;

    filePtr = fopen(fileName.c_str(), "r");
    if (filePtr == NULL) {
      printf ("\n[TRAJ_SERVER]path %d not exist for vel_id %d.\n\n", path_id_[idx], msg->vel_id);
      idx++;
    }
    else{
      // success
      break;
    }

    if(idx >= (int)path_id_.size()){
      fprintf(stderr, "%s\n", "[TRAJ_SERVER]No suitable path exists 1!!!");
      publish_cmd(last_pos_, Vector3d::Zero(), Vector3d::Zero(), last_yaw_, 0);
    }
  }

  Eigen::Vector3d pos_body, vel_body, acc_body, pos_world, vel_world, acc_world;
  int pointNum, val_num, val_dur, val1, val2, val3, val4, val5, val6, val7, val8, val9;
  double pos_x, pos_y, pos_z, vel_x, vel_y, vel_z, acc_x, acc_y, acc_z;
  val_num = fscanf(filePtr, "%d", &pointNum);
  val_dur = fscanf(filePtr, "%lf", &traj_duration_);

  traj_pos_.clear();
  traj_vel_.clear();
  traj_acc_.clear();

  for(int i = 0; i < pointNum; i++){
    val1 = fscanf(filePtr, "%lf", &pos_x);
    val2 = fscanf(filePtr, "%lf", &pos_y);
    val3 = fscanf(filePtr, "%lf", &pos_z);
    val4 = fscanf(filePtr, "%lf", &vel_x);
    val5 = fscanf(filePtr, "%lf", &vel_y);
    val6 = fscanf(filePtr, "%lf", &vel_z);
    val7 = fscanf(filePtr, "%lf", &acc_x);
    val8 = fscanf(filePtr, "%lf", &acc_y);
    val9 = fscanf(filePtr, "%lf", &acc_z);

    if (val1 != 1 || val2 != 1 || val3 != 1 || val4 != 1 || val5 != 1 || val6 != 1 || val7 != 1 || val8 != 1 || val9 != 1) {
      printf ("\nError reading input files, exit.\n\n");
        exit(1);
    }

    // std::cout << "trajCallback 4" << std::endl;    
    // velocity frame -> world frame start_pos + Rwv * pos
    pos_body = Eigen::Vector3d(pos_x, pos_y, pos_z);
    vel_body = Eigen::Vector3d(vel_x, vel_y, vel_z);
    acc_body = Eigen::Vector3d(acc_x, acc_y, acc_z);
    pos_world = start_pos + Rwv * pos_body;
    vel_world = Rwv * vel_body;
    acc_world = Rwv * acc_body;
    // not push_back
    traj_pos_.push_back(pos_world);
    traj_vel_.push_back(vel_world);
    traj_acc_.push_back(acc_world);
  }

  fclose(filePtr);

  // last end point position
  std_msgs::msg::Float64MultiArray pos_msg;
  for(int i = 0; i < 3; i++){
    pos_msg.data.push_back(pos_world(i));
  }
  select_path_end_pub->publish(pos_msg);
  start_time_ = msg->start_time;

  // obtain traj_pos_ traj_vel_ traj_acc_ [frame transform!!!] traj_duration_ from file
  // start_time_ - really start to execute
  receive_traj_ = true;
  require_yaw_turning_ = false;
  require_yaw_turning_external_ = false;
}

void polytrajCallback(traj_utils::msg::Polynomial::SharedPtr msg)
{
  Eigen::Matrix<double, 3, 6, Eigen::RowMajor> coeff;
  memcpy(coeff.row(0).data(), &msg->coeff_x, 6 * 8);
  memcpy(coeff.row(1).data(), &msg->coeff_y, 6 * 8);
  memcpy(coeff.row(2).data(), &msg->coeff_z, 6 * 8);

  traj_pos_.clear();
  traj_vel_.clear();
  traj_acc_.clear();
  for ( double t=0; t<=msg->duration + 1e-5; t+=0.01 )
  {
    double tt1 = pow(t,1), tt2 = pow(t,2), tt3 = pow(t,3), tt4 = pow(t,4), tt5 = pow(t,5);
    Eigen::Matrix<double, 3, 6> TT;
    TT << tt5, tt4, tt3, tt2, tt1, 1,
      5*tt4, 4*tt3, 3*tt2, 2*tt1, 1, 0,
      20*tt3, 12*tt2, 6*tt1, 2, 0, 0;
    
    Eigen::Matrix3d pva = coeff * TT.transpose();
    traj_pos_.push_back(pva.col(0));
    traj_vel_.push_back(pva.col(1));
    traj_acc_.push_back(pva.col(2));
  }

  start_time_ = msg->start_time;
  traj_duration_ = msg->duration;

  // obtain traj_pos_ traj_vel_ traj_acc_ [frame transform!!!] traj_duration_ from file
  // start_time_ - really start to execute
  receive_traj_ = true;
  require_yaw_turning_ = false;
  require_yaw_turning_external_ = false;
}

std::pair<double, double> calculate_yaw(double t_cur, Eigen::Vector3d &pos, double dt)
{
  constexpr double YAW_DOT_MAX_PER_SEC = 1 * M_PI;
  std::pair<double, double> yaw_yawdot(0, 0);

  // int idx_f = floor((t_cur + time_forward_) * 100);
  // int idx_d = floor((traj_duration_) * 100) - 1;//-1 ensure traj_pos_ isn't Nan.
  // Eigen::Vector3d dir = t_cur + time_forward_ <= traj_duration_
  //                           ? traj_pos_[idx_f] - pos
  //                           : traj_pos_[idx_d] - pos;
  // double yaw_des = dir.norm() > 0.1
  //                       ? atan2(dir(1), dir(0))
  //                       : last_yaw_;

  int idx = floor(t_cur * 100);
  idx = min(idx, (int)(traj_vel_.size()-1));
  Eigen::Vector3d dir = traj_vel_[idx];
  double yaw_des = dir.norm() > 0.1
                        ? atan2(traj_vel_[idx](1), traj_vel_[idx](0))
                        : last_yaw_;

  double yawdot = 0;
  double diff = yaw_diff(yaw_des, last_yaw_);
  int yaw_dir = diff > 0 ? 1 : -1;
  double yaw_temp = limit_yaw(last_yaw_ + yaw_dir * YAW_DOT_MAX_PER_SEC * loop_time_);
  if ( yaw_dir * yaw_diff(yaw_des, yaw_temp) > 0 )
  {
    yaw_yawdot.first = yaw_temp;
    yaw_yawdot.second = yaw_dir * YAW_DOT_MAX_PER_SEC;
  }
  else
  {
    yaw_yawdot.first = yaw_des;
    yaw_yawdot.second = yaw_diff(yaw_des, last_yaw_) / loop_time_;
  }

  last_yaw_ = yaw_yawdot.first;
  return yaw_yawdot;
}

void cmdCallback()
{
  /* no publishing before receive traj_ and have heartbeat */
  if (heartbeat_time_.seconds() <= 1e-5)
  {
    // ROS_ERROR_ONCE("[traj_server] No heartbeat from the planner received");
    return;
  }

  if ( receive_traj_ )
  {
    rclcpp::Time time_now = node->now();

    if ((time_now - heartbeat_time_).seconds() > 0.5)
    {
      fprintf(stderr, "%s\n", "[traj_server] Lost heartbeat from the planner, is it dead?");

      receive_traj_ = false;
      publish_cmd(last_pos_, Vector3d::Zero(), Vector3d::Zero(), last_yaw_, 0);
    }

    double t_cur = (time_now - start_time_).seconds();
    // std::cout << "t_cur: " << t_cur << std::endl; 
    int idx_t = floor(t_cur *100) + time_delay_idx_; // 1000/10 10ms-traj resolution

    // TODO:插值时间 6ms按照0处理 有偏移
    // std::cout << "idx_t: " << idx_t << std::endl;

    Eigen::Vector3d pos(Eigen::Vector3d::Zero()), vel(Eigen::Vector3d::Zero()), acc(Eigen::Vector3d::Zero());
    std::pair<double, double> yaw_yawdot(0, 0);

    static rclcpp::Time time_last = node->now();
    if (t_cur <= traj_duration_ && t_cur >= 0.0 && idx_t < traj_pos_.size() ) // to make sure the "vel = 0" command is published 
    {
      pos = traj_pos_[idx_t];
      vel = traj_vel_[idx_t];
      acc = traj_acc_[idx_t];

      /*** calculate yaw ***/
      yaw_yawdot = calculate_yaw(t_cur, pos, (time_now - time_last).seconds());
      /*** calculate yaw ***/

      // compute total length
      if(have_first_){
        // TODO:[revise] to launch 
        integ_dis_ = (pos - Eigen::Vector3d(_init_x, _init_y, _init_z)).norm();
        have_first_ = false;
      }
      else{
        integ_dis_ = (pos - last_pos_).norm();
        integ_time_ = (time_now - time_last).seconds();
      }
      total_length_ += integ_dis_;
      total_time_ += integ_time_;


      time_last = time_now;
      last_yaw_ = yaw_yawdot.first;
      last_pos_ = pos;
      last_vel_ = vel; 

      // publish
      publish_cmd(pos, vel, acc, yaw_yawdot.first, yaw_yawdot.second);

    }
    else
    {
      receive_traj_ = false;
    }
    
  }
  else if ( (require_yaw_turning_ || require_yaw_turning_external_) && have_odom_ )
  {
    const double YAW_DOT = M_PI / 2;

    // cout << "B init_odom_pos_=" <<init_odom_pos_.transpose() << " C=" << require_yaw_turning_ <<" D=" << require_yaw_turning_external_ <<endl;

    init_odom_yaw_ += yaw_dir_ * loop_time_ * YAW_DOT;
    init_odom_yaw_ = limit_yaw(init_odom_yaw_);

    double yaw_des = yaw_to_goal_;
    if ( require_yaw_turning_external_ )
      yaw_des = yaw_cmd_external_;

    if ( yaw_dir_ * yaw_diff(yaw_des, init_odom_yaw_) < 0 )
    {
      require_yaw_turning_ = false;
      require_yaw_turning_external_ = false;
      yaw_cmd_external_ = false;
    }
    
    last_yaw_ = init_odom_yaw_;
    publish_cmd(init_odom_pos_, Eigen::Vector3d::Zero(), Vector3d::Zero(), init_odom_yaw_, yaw_dir_ * YAW_DOT);

    // cout << "C init_odom_pos_=" <<init_odom_pos_.transpose() << " C=" << require_yaw_turning_ <<" D=" << require_yaw_turning_external_ <<endl;
  }

}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  node = std::make_shared<rclcpp::Node>("traj_server");
  primitiveFolder_ = node->declare_parameter<std::string>("primitiveFolder", "none");
  max_vel_ = node->declare_parameter("max_vel", -1.0);
  time_delay_idx_ = node->declare_parameter("time_delay_idx", 1);
  _init_x = node->declare_parameter("init_x", 0.0); _init_y = node->declare_parameter("init_y", 0.0); _init_z = node->declare_parameter("init_z", 0.0);
  time_forward_ = node->declare_parameter("time_forward", -1.0); last_yaw_=0.; last_yawdot_=0.;
  auto s1=node->create_subscription<std_msgs::msg::Empty>("heartbeat",10,heartbeatCallback);
  auto s2=node->create_subscription<nav_msgs::msg::Odometry>("odom",10,odometryCallback);
  auto s3=node->create_subscription<std_msgs::msg::Float64MultiArray>("stop_command",10,stopCommandCallback);
  auto s4=node->create_subscription<std_msgs::msg::Float64MultiArray>("planning/global_goal",10,goalEndCallback);
  auto s5=node->create_subscription<std_msgs::msg::Float64>("planning/yaw_cmd",10,yawCmdCallback);
  auto s6=node->create_subscription<traj_utils::msg::SwarmPrimitiveTraj>("planning/selected_path_id",10,trajCallback);
  auto s7=node->create_subscription<traj_utils::msg::Polynomial>("planning/polynomial_traj",10,polytrajCallback);
  pos_cmd_pub=node->create_publisher<quadrotor_msgs::msg::PositionCommand>("/position_cmd",50);
  select_path_end_pub=node->create_publisher<std_msgs::msg::Float64MultiArray>("planning/select_path_end",10);
  auto timer=node->create_wall_timer(std::chrono::duration<double>(loop_time_),[](){cmdCallback();});
  RCLCPP_INFO(node->get_logger(), "Traj server ready.");
  rclcpp::spin(node); rclcpp::shutdown(); return 0;
}
