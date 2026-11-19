cmake_minimum_required(VERSION 3.8)
project(uav_utils)
find_package(ament_cmake REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclpy REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION include/${PROJECT_NAME})
install(PROGRAMS
  scripts/odom_to_euler.py scripts/tf_assist.py scripts/send_odom.py scripts/topic_statistics.py
  DESTINATION lib/${PROJECT_NAME})
ament_export_include_directories(include)
ament_package()
