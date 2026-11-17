cmake_minimum_required(VERSION 3.8)
project(quadrotor_msgs)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_msgs REQUIRED)
set(msg_files
  msg/AuxCommand.msg msg/Corrections.msg msg/Gains.msg msg/OutputData.msg
  msg/PositionCommand.msg msg/PPROutputData.msg msg/Serial.msg msg/SO3Command.msg
  msg/StatusData.msg msg/TRPYCommand.msg msg/Odometry.msg
  msg/PolynomialTrajectory.msg msg/LQRTrajectory.msg msg/GoalSet.msg)
rosidl_generate_interfaces(${PROJECT_NAME} ${msg_files}
  DEPENDENCIES geometry_msgs nav_msgs std_msgs)
ament_export_dependencies(rosidl_default_runtime geometry_msgs nav_msgs std_msgs)
ament_package()
