// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "traj_utils/msg/detail/polynomial__rosidl_typesupport_introspection_c.h"
#include "traj_utils/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "traj_utils/msg/detail/polynomial__functions.h"
#include "traj_utils/msg/detail/polynomial__struct.h"


// Include directives for member types
// Member `start_time`
#include "builtin_interfaces/msg/time.h"
// Member `start_time`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  traj_utils__msg__Polynomial__init(message_memory);
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_fini_function(void * message_memory)
{
  traj_utils__msg__Polynomial__fini(message_memory);
}

size_t traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_x(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_x(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_x(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_x(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_x(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

size_t traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_y(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_y(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_y(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_y(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_y(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

size_t traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_z(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_z(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_z(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_z(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_z(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_z(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_z(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_member_array[5] = {
  {
    "start_time",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils__msg__Polynomial, start_time),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "coeff_x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils__msg__Polynomial, coeff_x),  // bytes offset in struct
    NULL,  // default value
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_x,  // size() function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_x,  // get_const(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_x,  // get(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_x,  // fetch(index, &value) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_x,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "coeff_y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils__msg__Polynomial, coeff_y),  // bytes offset in struct
    NULL,  // default value
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_y,  // size() function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_y,  // get_const(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_y,  // get(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_y,  // fetch(index, &value) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_y,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "coeff_z",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils__msg__Polynomial, coeff_z),  // bytes offset in struct
    NULL,  // default value
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__size_function__Polynomial__coeff_z,  // size() function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_const_function__Polynomial__coeff_z,  // get_const(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__get_function__Polynomial__coeff_z,  // get(index) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__fetch_function__Polynomial__coeff_z,  // fetch(index, &value) function pointer
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__assign_function__Polynomial__coeff_z,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "duration",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils__msg__Polynomial, duration),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_members = {
  "traj_utils__msg",  // message namespace
  "Polynomial",  // message name
  5,  // number of fields
  sizeof(traj_utils__msg__Polynomial),
  false,  // has_any_key_member_
  traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_member_array,  // message members
  traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_init_function,  // function to initialize message memory (memory has to be allocated)
  traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_type_support_handle = {
  0,
  &traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_members,
  get_message_typesupport_handle_function,
  &traj_utils__msg__Polynomial__get_type_hash,
  &traj_utils__msg__Polynomial__get_type_description,
  &traj_utils__msg__Polynomial__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_traj_utils
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, traj_utils, msg, Polynomial)() {
  traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_type_support_handle.typesupport_identifier) {
    traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &traj_utils__msg__Polynomial__rosidl_typesupport_introspection_c__Polynomial_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
