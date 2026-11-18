# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target traj_utils::traj_utils
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${traj_utils_TARGETS}.
if(traj_utils_TARGETS AND NOT TARGET traj_utils::traj_utils)
  add_library(traj_utils::traj_utils INTERFACE IMPORTED)
  set_target_properties(traj_utils::traj_utils PROPERTIES
    INTERFACE_LINK_LIBRARIES "${traj_utils_TARGETS}")
endif()
