// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from quadrotor_msgs:msg/GoalSet.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "quadrotor_msgs/msg/goal_set.h"


#ifndef QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_H_
#define QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/GoalSet in the package quadrotor_msgs.
typedef struct quadrotor_msgs__msg__GoalSet
{
  int16_t drone_id;
  float goal[3];
} quadrotor_msgs__msg__GoalSet;

// Struct for a sequence of quadrotor_msgs__msg__GoalSet.
typedef struct quadrotor_msgs__msg__GoalSet__Sequence
{
  quadrotor_msgs__msg__GoalSet * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} quadrotor_msgs__msg__GoalSet__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_H_
