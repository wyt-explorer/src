#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "traj_utils::traj_visualization" for configuration "Release"
set_property(TARGET traj_utils::traj_visualization APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(traj_utils::traj_visualization PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtraj_visualization.so"
  IMPORTED_SONAME_RELEASE "libtraj_visualization.so"
  )

list(APPEND _cmake_import_check_targets traj_utils::traj_visualization )
list(APPEND _cmake_import_check_files_for_traj_utils::traj_visualization "${_IMPORT_PREFIX}/lib/libtraj_visualization.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
