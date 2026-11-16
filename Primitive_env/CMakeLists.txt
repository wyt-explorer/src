# 配置 primitive_env 的 Catkin 依赖、Python 节点及资源安装。
cmake_minimum_required(VERSION 3.0.2)
project(primitive_env)
# 声明节点使用的 ROS 组件，并导出 Catkin 功能包。
find_package(catkin REQUIRED COMPONENTS rospy sensor_msgs std_msgs geometry_msgs visualization_msgs)
catkin_package()
# 安装三个可执行 Python 节点，供 rosrun 和 roslaunch 查找。
catkin_install_python(PROGRAMS
  src/map_publisher.py
  src/waypoint_publisher.py
  src/rviz_click_trigger.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
# 安装启动文件、地图参数和 RViz 显示配置。
install(DIRECTORY launch map rviz DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
