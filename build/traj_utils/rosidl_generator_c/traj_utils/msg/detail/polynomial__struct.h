// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/polynomial.h"


#ifndef TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_H_
#define TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_H_

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

/// Struct defined in msg/Polynomial in the package traj_utils.
typedef struct traj_utils__msg__Polynomial
{
  builtin_interfaces__msg__Time start_time;
  double coeff_x[6];
  double coeff_y[6];
  double coeff_z[6];
  double duration;
} traj_utils__msg__Polynomial;

// Struct for a sequence of traj_utils__msg__Polynomial.
typedef struct traj_utils__msg__Polynomial__Sequence
{
  traj_utils__msg__Polynomial * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} traj_utils__msg__Polynomial__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_H_
