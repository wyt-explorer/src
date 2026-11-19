// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/swarm_primitive_traj.h"


#ifndef TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_H_
#define TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'start_time'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/SwarmPrimitiveTraj in the package traj_utils.
typedef struct traj_utils__msg__SwarmPrimitiveTraj
{
  int16_t drone_id;
  builtin_interfaces__msg__Time start_time;
  float start_p[3];
  float start_v[3];
  float end_p[3];
  float rot_mat[9];
  int32_t vel_id;
  int32_t select_path_id;
} traj_utils__msg__SwarmPrimitiveTraj;

// Struct for a sequence of traj_utils__msg__SwarmPrimitiveTraj.
typedef struct traj_utils__msg__SwarmPrimitiveTraj__Sequence
{
  traj_utils__msg__SwarmPrimitiveTraj * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} traj_utils__msg__SwarmPrimitiveTraj__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_H_
