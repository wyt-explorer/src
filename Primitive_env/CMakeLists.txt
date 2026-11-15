cmake_minimum_required(VERSION 3.0.2)
project(primitive_env)
find_package(catkin REQUIRED COMPONENTS rospy sensor_msgs std_msgs geometry_msgs visualization_msgs)
catkin_package()
catkin_install_python(PROGRAMS
  src/map_publisher.py
  src/waypoint_publisher.py
  src/rviz_click_trigger.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
install(DIRECTORY launch map rviz DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
