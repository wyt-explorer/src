# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target quadrotor_msgs::quadrotor_msgs
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${quadrotor_msgs_TARGETS}.
if(quadrotor_msgs_TARGETS AND NOT TARGET quadrotor_msgs::quadrotor_msgs)
  add_library(quadrotor_msgs::quadrotor_msgs INTERFACE IMPORTED)
  set_target_properties(quadrotor_msgs::quadrotor_msgs PROPERTIES
    INTERFACE_LINK_LIBRARIES "${quadrotor_msgs_TARGETS}")
endif()
