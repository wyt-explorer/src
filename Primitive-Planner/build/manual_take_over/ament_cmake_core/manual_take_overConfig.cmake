# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_manual_take_over_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED manual_take_over_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(manual_take_over_FOUND FALSE)
  elseif(NOT manual_take_over_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(manual_take_over_FOUND FALSE)
  endif()
  return()
endif()
set(_manual_take_over_CONFIG_INCLUDED TRUE)

# output package information
if(NOT manual_take_over_FIND_QUIETLY)
  message(STATUS "Found manual_take_over: 0.1.0 (${manual_take_over_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'manual_take_over' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT manual_take_over_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(manual_take_over_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${manual_take_over_DIR}/${_extra}")
endforeach()
