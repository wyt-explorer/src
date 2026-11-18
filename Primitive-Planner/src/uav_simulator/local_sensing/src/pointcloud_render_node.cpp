/*
 * 文件作用：提供 ROS 2 节点消息回调；由包内可执行程序启动。
 * 输入/输出：见 create_subscription/create_publisher 的消息及话题声明。
 * 运行模式：ROS 2；位置单位为米，时间为秒；依赖 rclcpp 和对应消息包。
 */
#include <memory>
#include <chrono>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/path.hpp>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/search/kdtree.h>
#include <pcl_conversions/pcl_conversions.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <pcl/search/impl/kdtree.hpp>
#include <vector>

using namespace std;
using namespace Eigen;

rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_cloud;

sensor_msgs::msg::PointCloud2 local_map_pcl;
sensor_msgs::msg::PointCloud2 local_depth_pcl;

rclcpp::SubscriptionBase::SharedPtr odom_sub;
rclcpp::SubscriptionBase::SharedPtr global_map_sub, local_map_sub;

rclcpp::TimerBase::SharedPtr local_sensing_timer;

bool has_global_map(false);
bool has_local_map(false);
bool has_odom(false);

nav_msgs::msg::Odometry _odom;

double sensing_horizon, sensing_rate, estimation_rate;
double _x_size, _y_size, _z_size;
double _gl_xl, _gl_yl, _gl_zl;
double _resolution, _inv_resolution;
int _GLX_SIZE, _GLY_SIZE, _GLZ_SIZE;

rclcpp::Time last_odom_stamp = rclcpp::Time::max();

inline Eigen::Vector3d gridIndex2coord(const Eigen::Vector3i& index) {
  Eigen::Vector3d pt;
  pt(0) = ((double)index(0) + 0.5) * _resolution + _gl_xl;
  pt(1) = ((double)index(1) + 0.5) * _resolution + _gl_yl;
  pt(2) = ((double)index(2) + 0.5) * _resolution + _gl_zl;

  return pt;
};

inline Eigen::Vector3i coord2gridIndex(const Eigen::Vector3d& pt) {
  Eigen::Vector3i idx;
  idx(0) = std::min(std::max(int((pt(0) - _gl_xl) * _inv_resolution), 0),
                    _GLX_SIZE - 1);
  idx(1) = std::min(std::max(int((pt(1) - _gl_yl) * _inv_resolution), 0),
                    _GLY_SIZE - 1);
  idx(2) = std::min(std::max(int((pt(2) - _gl_zl) * _inv_resolution), 0),
                    _GLZ_SIZE - 1);

  return idx;
};

void rcvOdometryCallbck(const nav_msgs::msg::Odometry& odom) {
  /*if(!has_global_map)
    return;*/
  has_odom = true;
  _odom = odom;
}

pcl::PointCloud<pcl::PointXYZ> _cloud_all_map, _local_map;
pcl::VoxelGrid<pcl::PointXYZ> _voxel_sampler;
sensor_msgs::msg::PointCloud2 _local_map_pcd;

pcl::search::KdTree<pcl::PointXYZ> _kdtreeLocalMap;
vector<int> _pointIdxRadiusSearch;
vector<float> _pointRadiusSquaredDistance;

void rcvGlobalPointCloudCallBack(
    const sensor_msgs::msg::PointCloud2& pointcloud_map) {
  if (has_global_map) return;

  // Global pointcloud received;

  pcl::PointCloud<pcl::PointXYZ> cloud_input;
  pcl::fromROSMsg(pointcloud_map, cloud_input);

  _voxel_sampler.setLeafSize(0.1f, 0.1f, 0.1f);
  _voxel_sampler.setInputCloud(cloud_input.makeShared());
  _voxel_sampler.filter(_cloud_all_map);

  _kdtreeLocalMap.setInputCloud(_cloud_all_map.makeShared());

  has_global_map = true;
}

void renderSensedPoints() {
  if (!has_global_map || !has_odom) return;

  Eigen::Quaterniond q;
  q.x() = _odom.pose.pose.orientation.x;
  q.y() = _odom.pose.pose.orientation.y;
  q.z() = _odom.pose.pose.orientation.z;
  q.w() = _odom.pose.pose.orientation.w;

  Eigen::Matrix3d rot;
  rot = q;
  Eigen::Vector3d yaw_vec = rot.col(0);

  _local_map.points.clear();
  pcl::PointXYZ searchPoint(_odom.pose.pose.position.x,
                            _odom.pose.pose.position.y,
                            _odom.pose.pose.position.z);
  _pointIdxRadiusSearch.clear();
  _pointRadiusSquaredDistance.clear();

  pcl::PointXYZ pt;
  if (_kdtreeLocalMap.radiusSearch(searchPoint, sensing_horizon,
                                   _pointIdxRadiusSearch,
                                   _pointRadiusSquaredDistance) > 0) {
    for (size_t i = 0; i < _pointIdxRadiusSearch.size(); ++i) {
      pt = _cloud_all_map.points[_pointIdxRadiusSearch[i]];

      // if ((fabs(pt.z - _odom.pose.pose.position.z) / (pt.x - _odom.pose.pose.position.x)) >
      //     tan(M_PI / 12.0))
      //   continue;
      // if ((fabs(pt.z - _odom.pose.pose.position.z) / sensing_horizon) >
      //     tan(M_PI / 6.0))
      //   continue; 

      Vector3d pt_vec(pt.x - _odom.pose.pose.position.x,
                      pt.y - _odom.pose.pose.position.y,
                      pt.z - _odom.pose.pose.position.z);

      // if (pt_vec.normalized().dot(yaw_vec) < 0) continue; 

      _local_map.points.push_back(pt);
    }
  } else {
    return;
  }

  _local_map.width = _local_map.points.size();
  _local_map.height = 1;
  _local_map.is_dense = true;

  pcl::toROSMsg(_local_map, _local_map_pcd);
  _local_map_pcd.header.frame_id = "world";
  _local_map_pcd.header.stamp = _odom.header.stamp;

  pub_cloud->publish(_local_map_pcd);
}

void rcvLocalPointCloudCallBack(
    const sensor_msgs::msg::PointCloud2& pointcloud_map) {
  // do nothing, fix later
}

int main(int argc, char** argv) {
  rclcpp::init(argc, argv); auto nh=std::make_shared<rclcpp::Node>("pcl_render");
  sensing_horizon=nh->declare_parameter("sensing_horizon",5.0); sensing_rate=nh->declare_parameter("sensing_rate",10.0); estimation_rate=nh->declare_parameter("estimation_rate",10.0);
  _x_size=nh->declare_parameter("map/x_size",50.0); _y_size=nh->declare_parameter("map/y_size",50.0); _z_size=nh->declare_parameter("map/z_size",5.0);
  auto global_sub=nh->create_subscription<sensor_msgs::msg::PointCloud2>("global_map",1,[](sensor_msgs::msg::PointCloud2::SharedPtr m){rcvGlobalPointCloudCallBack(*m);});
  auto local_sub=nh->create_subscription<sensor_msgs::msg::PointCloud2>("local_map",1,[](sensor_msgs::msg::PointCloud2::SharedPtr m){rcvLocalPointCloudCallBack(*m);});
  auto odom=nh->create_subscription<nav_msgs::msg::Odometry>("odometry",50,[](nav_msgs::msg::Odometry::SharedPtr m){rcvOdometryCallbck(*m);});
  pub_cloud=nh->create_publisher<sensor_msgs::msg::PointCloud2>("pcl_render_node/cloud",10);
  _resolution=nh->declare_parameter("map/resolution",0.1);
  if (_resolution <= 0.0 || sensing_rate <= 0.0) throw std::invalid_argument("resolution and sensing_rate must be positive");
  _inv_resolution=1.0/_resolution; _gl_xl=-_x_size/2.; _gl_yl=-_y_size/2.; _gl_zl=0.; _GLX_SIZE=(int)(_x_size*_inv_resolution); _GLY_SIZE=(int)(_y_size*_inv_resolution); _GLZ_SIZE=(int)(_z_size*_inv_resolution);
  auto timer=nh->create_wall_timer(std::chrono::duration<double>(1.0/sensing_rate*2.5),[](){renderSensedPoints();});
  rclcpp::spin(nh); rclcpp::shutdown(); return 0;
}
