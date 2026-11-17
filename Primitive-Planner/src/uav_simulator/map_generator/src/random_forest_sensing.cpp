#include <thread>
#include <chrono>
#include <memory>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
// #include <pcl/search/kdtree.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl_conversions/pcl_conversions.h>
#include <iostream>

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <math.h>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>

#include <sensor_msgs/msg/point_cloud2.hpp>
#include <Eigen/Eigen>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

// pcl::search::KdTree<pcl::PointXYZ> kdtreeLocalMap;
pcl::KdTreeFLANN<pcl::PointXYZ> kdtreeLocalMap;
vector<int> pointIdxRadiusSearch;
vector<float> pointRadiusSquaredDistance;

random_device rd;
// default_random_engine eng(4);
default_random_engine eng;
uniform_real_distribution<double> rand_x;
uniform_real_distribution<double> rand_y;
uniform_real_distribution<double> rand_w;
uniform_real_distribution<double> rand_h;
uniform_real_distribution<double> rand_inf;

rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr _local_map_pub;
rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr _all_map_pub;
rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr click_map_pub_;
rclcpp::SubscriptionBase::SharedPtr _odom_sub;

vector<double> _state;

int _obs_num;
double _x_size, _y_size, _z_size;
double _x_l, _x_h, _y_l, _y_h, _w_l, _w_h, _h_l, _h_h;
double _z_limit, _sensing_range, _resolution, _pub_rate;
double _min_dist;

bool _map_ok = false;
bool _has_odom = false;

int circle_num_;
double radius_l_, radius_h_, z_l_, z_h_;
double theta_;
uniform_real_distribution<double> rand_radius_;
uniform_real_distribution<double> rand_radius2_;
uniform_real_distribution<double> rand_theta_;
uniform_real_distribution<double> rand_theta_tilt_;
uniform_real_distribution<double> rand_z_;

sensor_msgs::msg::PointCloud2 globalMap_pcd;
pcl::PointCloud<pcl::PointXYZ> cloudMap;

sensor_msgs::msg::PointCloud2 localMap_pcd;
pcl::PointCloud<pcl::PointXYZ> clicked_cloud_;

void RandomMapGenerate()
{
  pcl::PointXYZ pt_random;

  rand_x = uniform_real_distribution<double>(_x_l, _x_h);
  rand_y = uniform_real_distribution<double>(_y_l, _y_h);
  rand_w = uniform_real_distribution<double>(_w_l, _w_h);
  rand_h = uniform_real_distribution<double>(_h_l, _h_h);

  rand_radius_ = uniform_real_distribution<double>(radius_l_, radius_h_);
  rand_radius2_ = uniform_real_distribution<double>(radius_l_, 1.2);
  rand_theta_ = uniform_real_distribution<double>(-theta_, theta_);
  rand_z_ = uniform_real_distribution<double>(z_l_, z_h_);

  // generate polar obs
  for (int i = 0; i < _obs_num; i++)
  {
    double x, y, w, h;
    x = rand_x(eng);
    y = rand_y(eng);
    w = rand_w(eng);

    x = floor(x / _resolution) * _resolution + _resolution / 2.0;
    y = floor(y / _resolution) * _resolution + _resolution / 2.0;

    int widNum = ceil(w / _resolution);

    for (int r = -widNum / 2.0; r < widNum / 2.0; r++)
      for (int s = -widNum / 2.0; s < widNum / 2.0; s++)
      {
        h = rand_h(eng);
        int heiNum = ceil(h / _resolution);
        for (int t = -20; t < heiNum; t++)
        {
          pt_random.x = x + (r + 0.5) * _resolution + 1e-2;
          pt_random.y = y + (s + 0.5) * _resolution + 1e-2;
          pt_random.z = (t + 0.5) * _resolution + 1e-2;
          cloudMap.points.push_back(pt_random);
        }
      }
  }

  // generate circle obs
  for (int i = 0; i < circle_num_; ++i)
  {
    double x, y, z;
    x = rand_x(eng);
    y = rand_y(eng);
    z = rand_z_(eng);

    x = floor(x / _resolution) * _resolution + _resolution / 2.0;
    y = floor(y / _resolution) * _resolution + _resolution / 2.0;
    z = floor(z / _resolution) * _resolution + _resolution / 2.0;

    Eigen::Vector3d translate(x, y, z);

    double theta = rand_theta_(eng);
    Eigen::Matrix3d rotate;
    rotate << cos(theta), -sin(theta), 0.0, sin(theta), cos(theta), 0.0, 0, 0,
        1;

    double radius1 = rand_radius_(eng);
    double radius2 = rand_radius2_(eng);

    // draw a circle centered at (x,y,z)
    Eigen::Vector3d cpt;
    for (double angle = 0.0; angle < 6.282; angle += _resolution / 2)
    {
      cpt(0) = 0.0;
      cpt(1) = radius1 * cos(angle);
      cpt(2) = radius2 * sin(angle);

      // inflate
      Eigen::Vector3d cpt_if;
      for (int ifx = -0; ifx <= 0; ++ifx)
        for (int ify = -0; ify <= 0; ++ify)
          for (int ifz = -0; ifz <= 0; ++ifz)
          {
            cpt_if = cpt + Eigen::Vector3d(ifx * _resolution, ify * _resolution,
                                           ifz * _resolution);
            cpt_if = rotate * cpt_if + Eigen::Vector3d(x, y, z);
            pt_random.x = cpt_if(0);
            pt_random.y = cpt_if(1);
            pt_random.z = cpt_if(2);
            cloudMap.push_back(pt_random);
          }
    }
  }

  cloudMap.width = cloudMap.points.size();
  cloudMap.height = 1;
  cloudMap.is_dense = true;

  // Finished generate random map

  kdtreeLocalMap.setInputCloud(cloudMap.makeShared());

  _map_ok = true;
}

void RandomMapGenerateCylinder()
{
  pcl::PointXYZ pt_random;

  vector<Eigen::Vector2d> obs_position;

  rand_x = uniform_real_distribution<double>(_x_l, _x_h);
  rand_y = uniform_real_distribution<double>(_y_l, _y_h);
  rand_w = uniform_real_distribution<double>(_w_l, _w_h);
  rand_h = uniform_real_distribution<double>(_h_l, _h_h);
  rand_inf = uniform_real_distribution<double>(0.5, 1.5);

  rand_radius_ = uniform_real_distribution<double>(radius_l_, radius_h_);
  rand_radius2_ = uniform_real_distribution<double>(radius_l_, 1.2);
  rand_theta_ = uniform_real_distribution<double>(-theta_, theta_);
  rand_z_ = uniform_real_distribution<double>(z_l_, z_h_);

  // generate polar obs
  for (int i = 0; i < _obs_num && rclcpp::ok(); i++)
  {
    double x, y, w, h, inf;
    x = rand_x(eng);
    y = rand_y(eng);
    w = rand_w(eng);
    inf = rand_inf(eng);

    bool flag_continue = false;
    for (auto p : obs_position)
      if ((Eigen::Vector2d(x, y) - p).norm() < _min_dist /*metres*/)
      {
        i--;
        flag_continue = true;
        break;
      }
    if (flag_continue)
      continue;

    obs_position.push_back(Eigen::Vector2d(x, y));

    x = floor(x / _resolution) * _resolution + _resolution / 2.0;
    y = floor(y / _resolution) * _resolution + _resolution / 2.0;

    int widNum = ceil((w * inf) / _resolution);
    double radius = (w * inf) / 2;

    for (int r = -widNum / 2.0; r < widNum / 2.0; r++)
      for (int s = -widNum / 2.0; s < widNum / 2.0; s++)
      {
        h = rand_h(eng);
        int heiNum = ceil(h / _resolution);
        for (int t = -10; t < heiNum; t++)
        {
          double temp_x = x + (r + 0.5) * _resolution + 1e-2;
          double temp_y = y + (s + 0.5) * _resolution + 1e-2;
          double temp_z = (t + 0.5) * _resolution + 1e-2;
          if ((Eigen::Vector2d(temp_x, temp_y) - Eigen::Vector2d(x, y)).norm() <= radius)
          {
            pt_random.x = temp_x;
            pt_random.y = temp_y;
            pt_random.z = temp_z;
            cloudMap.points.push_back(pt_random);
          }
        }
      }
  }

  // generate circle obs
  for (int i = 0; i < circle_num_; ++i)
  {
    double x, y, z;
    x = rand_x(eng);
    y = rand_y(eng);
    z = rand_z_(eng);

    x = floor(x / _resolution) * _resolution + _resolution / 2.0;
    y = floor(y / _resolution) * _resolution + _resolution / 2.0;
    z = floor(z / _resolution) * _resolution + _resolution / 2.0;

    Eigen::Vector3d translate(x, y, z);

    double theta = rand_theta_(eng);
    Eigen::Matrix3d rotate;
    rotate << cos(theta), -sin(theta), 0.0, sin(theta), cos(theta), 0.0, 0, 0,
        1;

    double radius1 = rand_radius_(eng);
    double radius2 = rand_radius2_(eng);

    // draw a circle centered at (x,y,z)
    Eigen::Vector3d cpt;
    for (double angle = 0.0; angle < 6.282; angle += _resolution / 2)
    {
      cpt(0) = 0.0;
      cpt(1) = radius1 * cos(angle);
      cpt(2) = radius2 * sin(angle);

      // inflate
      Eigen::Vector3d cpt_if;
      for (int ifx = -0; ifx <= 0; ++ifx)
        for (int ify = -0; ify <= 0; ++ify)
          for (int ifz = -0; ifz <= 0; ++ifz)
          {
            cpt_if = cpt + Eigen::Vector3d(ifx * _resolution, ify * _resolution,
                                           ifz * _resolution);
            cpt_if = rotate * cpt_if + Eigen::Vector3d(x, y, z);
            pt_random.x = cpt_if(0);
            pt_random.y = cpt_if(1);
            pt_random.z = cpt_if(2);
            cloudMap.push_back(pt_random);
          }
    }
  }

  cloudMap.width = cloudMap.points.size();
  cloudMap.height = 1;
  cloudMap.is_dense = true;

  // Finished generate random map

  kdtreeLocalMap.setInputCloud(cloudMap.makeShared());

  _map_ok = true;
}


bool GenerateMapFromObstacleFile(const std::string file_path) {
  
  std::ifstream file(file_path); // 替换为你的文件路径
  if (!file.is_open()) {
      std::cerr << "Failed to open file" << std::endl;
      return false;
  }

  std::vector<Eigen::Vector3d> vectors;
  std::string line;
  while (std::getline(file, line)) {
      std::string::size_type pos;
      double x, y, z;

      pos = line.find(',');
      if (pos != std::string::npos) {
          x = std::stod(line.substr(0, pos));
          line.erase(0, pos + 1); // 移除已解析的部分

          pos = line.find(',');
          if (pos != std::string::npos) {
              y = std::stod(line.substr(0, pos));
              z = std::stod(line.substr(pos + 1)); // 解析剩余部分

              Eigen::Vector3d vec(x, y, z);
              vectors.push_back(vec);
          } else {
              std::cerr << "Error parsing line: " << line << std::endl;
          }
      } else {
          std::cerr << "Error parsing line: " << line << std::endl;
      }
  }

  file.close();

  // for (const auto& vec : vectors) {
  //   std::cout << vec.transpose() << std::endl;
  // }

  assert(vectors.size() > 0 && vectors.size() % 2 == 0);

  pcl::PointXYZ pt_random;
  for ( int i=0; i<vectors.size()/2; ++i )
  {
      Eigen::Vector3d lb = vectors[i * 2], ub = vectors[i * 2 + 1];
      for (double x = lb(0); x <= ub(0); x += _resolution)
        for (double y = lb(1); y <= ub(1); y += _resolution)
          for (double z = lb(2); z <= ub(2); z += _resolution) {
            pt_random.x = x;
            pt_random.y = y;
            pt_random.z = z;
            cloudMap.push_back(pt_random);
          }
  }

  cloudMap.width = cloudMap.points.size();
  cloudMap.height = 1;
  cloudMap.is_dense = true;

  // Finished reading map

  kdtreeLocalMap.setInputCloud(cloudMap.makeShared());

  _map_ok = true;

  return true;

}

void RandomInclinedColumn()
{
  pcl::PointXYZ pt_random;

  vector<Eigen::Vector2d> obs_position;

  rand_x = uniform_real_distribution<double>(_x_l, _x_h);
  rand_y = uniform_real_distribution<double>(_y_l, _y_h);
  rand_w = uniform_real_distribution<double>(_w_l, _w_h);
  rand_h = uniform_real_distribution<double>(_h_l, _h_h);
  rand_inf = uniform_real_distribution<double>(0.2, 0.5);

  rand_theta_ = uniform_real_distribution<double>(-3.14, 3.14);
  rand_theta_tilt_ = uniform_real_distribution<double>(-theta_, theta_);
  rand_z_ = uniform_real_distribution<double>(z_l_, z_h_);

  // generate inclined column obs
  for (int i = 0; i < _obs_num && rclcpp::ok(); i++)
  {
    double x, y, w, h, inf;
    x = rand_x(eng);
    y = rand_y(eng);
    w = rand_w(eng);
    inf = rand_inf(eng);

    bool flag_continue = false;
    for (auto p : obs_position)
      if ((Eigen::Vector2d(x, y) - p).norm() < _min_dist /*metres*/)
      {
        i--;
        flag_continue = true;
        break;
      }
    if (flag_continue)
      continue;

    obs_position.push_back(Eigen::Vector2d(x, y));

    x = floor(x / _resolution) * _resolution + _resolution / 2.0;
    y = floor(y / _resolution) * _resolution + _resolution / 2.0;
    h = rand_h(eng);
    double rand_theta = rand_theta_(eng);
    double rand_theta_tilt = rand_theta_tilt_(eng);

    int widNum = ceil((w * inf) / _resolution);
    // double radius = (w * inf) / 2;
    Eigen::AngleAxisd R_v(rand_theta_tilt, Eigen::Vector3d(cos(rand_theta),sin(rand_theta),0));
    Eigen::Matrix3d R_m = R_v.matrix();

    for (int r = -widNum / 2.0; r < widNum / 2.0; r++)
      for (int s = -widNum / 2.0; s < widNum / 2.0; s++)
      {
        int heiNum = ceil(h / _resolution);
        for (int t = -10; t < heiNum; t++)
        {
          Eigen::Vector3d temp_p(x + (r + 0.5) * _resolution + 1e-2, y + (s + 0.5) * _resolution + 1e-2, (t + 0.5) * _resolution + 1e-2);
          temp_p = R_m * (temp_p - Eigen::Vector3d(x,y,0)) + Eigen::Vector3d(x,y,0);
          
          pt_random.x = temp_p(0);
          pt_random.y = temp_p(1);
          pt_random.z = temp_p(2);
          cloudMap.points.push_back(pt_random);
        }
      }
  }

  cloudMap.width = cloudMap.points.size();
  cloudMap.height = 1;
  cloudMap.is_dense = true;

  // Finished generate random map

  kdtreeLocalMap.setInputCloud(cloudMap.makeShared());

  _map_ok = true;
}

void clickCallback(const geometry_msgs::msg::PoseStamped &msg)
{
  double x = msg.pose.position.x;
  double y = msg.pose.position.y;
  double w = rand_w(eng);
  double h;
  pcl::PointXYZ pt_random;

  x = floor(x / _resolution) * _resolution + _resolution / 2.0;
  y = floor(y / _resolution) * _resolution + _resolution / 2.0;

  int widNum = ceil(w / _resolution);

  for (int r = -widNum / 2.0; r < widNum / 2.0; r++)
    for (int s = -widNum / 2.0; s < widNum / 2.0; s++)
    {
      h = rand_h(eng);
      int heiNum = ceil(h / _resolution);
      for (int t = -1; t < heiNum; t++)
      {
        pt_random.x = x + (r + 0.5) * _resolution + 1e-2;
        pt_random.y = y + (s + 0.5) * _resolution + 1e-2;
        pt_random.z = (t + 0.5) * _resolution + 1e-2;
        clicked_cloud_.points.push_back(pt_random);
        cloudMap.points.push_back(pt_random);
      }
    }
  clicked_cloud_.width = clicked_cloud_.points.size();
  clicked_cloud_.height = 1;
  clicked_cloud_.is_dense = true;

  pcl::toROSMsg(clicked_cloud_, localMap_pcd);
  localMap_pcd.header.frame_id = "world";
  click_map_pub_->publish(localMap_pcd);

  cloudMap.width = cloudMap.points.size();

  return;
}

int i = 0;
void pubPoints()
{
  while (rclcpp::ok())
  {
    rclcpp::spin_some(n);
    if (_map_ok)
      break;
  }

  pcl::toROSMsg(cloudMap, globalMap_pcd);
  globalMap_pcd.header.frame_id = "world";
  _all_map_pub->publish(globalMap_pcd);
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto n = std::make_shared<rclcpp::Node>("random_map_sensing");

  _local_map_pub = n->create_publisher<sensor_msgs::msg::PointCloud2>("/map_generator/local_cloud", 1);
  _all_map_pub = n->create_publisher<sensor_msgs::msg::PointCloud2>("/map_generator/global_cloud", 1);

  click_map_pub_ =
      n->create_publisher<sensor_msgs::msg::PointCloud2>("/pcl_render_node/local_map", 1);

  _x_size = n->declare_parameter("map/x_size", 50.0);
  _y_size = n->declare_parameter("map/y_size", 50.0);
  _z_size = n->declare_parameter("map/z_size", 5.0);
  _obs_num = n->declare_parameter("map/obs_num", 30);
  _resolution = n->declare_parameter("map/resolution", 0.1);
  circle_num_ = n->declare_parameter("map/circle_num", 30);

  _w_l = n->declare_parameter("ObstacleShape/lower_rad", 0.3);
  _w_h = n->declare_parameter("ObstacleShape/upper_rad", 0.8);
  _h_l = n->declare_parameter("ObstacleShape/lower_hei", 3.0);
  _h_h = n->declare_parameter("ObstacleShape/upper_hei", 7.0);

  radius_l_ = n->declare_parameter("ObstacleShape/radius_l", 7.0);
  radius_h_ = n->declare_parameter("ObstacleShape/radius_h", 7.0);
  z_l_ = n->declare_parameter("ObstacleShape/z_l", 7.0);
  z_h_ = n->declare_parameter("ObstacleShape/z_h", 7.0);
  theta_ = n->declare_parameter("ObstacleShape/theta", 7.0);

  _pub_rate = n->declare_parameter("pub_rate", 10.0);
  _min_dist = n->declare_parameter("min_distance", 1.0);

  _x_l = -_x_size / 2.0;
  _x_h = +_x_size / 2.0;

  _y_l = -_y_size / 2.0;
  _y_h = +_y_size / 2.0;

  _obs_num = min(_obs_num, (int)_x_size * 10);
  _z_limit = _z_size;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  unsigned int seed = rd();
  eng.seed(seed);

  // RandomMapGenerate();
  RandomMapGenerateCylinder();
  // RandomInclinedColumn();
  // GenerateMapFromObstacleFile("/home/hjl/project/A_personal_project/primitive_swarm/src/scripts/obstacles.txt");

  auto period = std::chrono::duration<double>(1.0 / _pub_rate);

  while (rclcpp::ok())
  {
    pubPoints();
    rclcpp::spin_some(n);
    std::this_thread::sleep_for(period);
  }
  rclcpp::shutdown();
}
