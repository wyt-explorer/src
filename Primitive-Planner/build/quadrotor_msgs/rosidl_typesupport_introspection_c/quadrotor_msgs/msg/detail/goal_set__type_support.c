// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from quadrotor_msgs:msg/GoalSet.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "quadrotor_msgs/msg/detail/goal_set__rosidl_typesupport_introspection_c.h"
#include "quadrotor_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "quadrotor_msgs/msg/detail/goal_set__functions.h"
#include "quadrotor_msgs/msg/detail/goal_set__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  quadrotor_msgs__msg__GoalSet__init(message_memory);
}

void quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_fini_function(void * message_memory)
{
  quadrotor_msgs__msg__GoalSet__fini(message_memory);
}

size_t quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__size_function__GoalSet__goal(
  const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_const_function__GoalSet__goal(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_function__GoalSet__goal(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__fetch_function__GoalSet__goal(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_const_function__GoalSet__goal(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__assign_function__GoalSet__goal(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_function__GoalSet__goal(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_member_array[2] = {
  {
    "drone_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(quadrotor_msgs__msg__GoalSet, drone_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "goal",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(quadrotor_msgs__msg__GoalSet, goal),  // bytes offset in struct
    NULL,  // default value
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__size_function__GoalSet__goal,  // size() function pointer
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_const_function__GoalSet__goal,  // get_const(index) function pointer
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__get_function__GoalSet__goal,  // get(index) function pointer
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__fetch_function__GoalSet__goal,  // fetch(index, &value) function pointer
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__assign_function__GoalSet__goal,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_members = {
  "quadrotor_msgs__msg",  // message namespace
  "GoalSet",  // message name
  2,  // number of fields
  sizeof(quadrotor_msgs__msg__GoalSet),
  false,  // has_any_key_member_
  quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_member_array,  // message members
  quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_init_function,  // function to initialize message memory (memory has to be allocated)
  quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_type_support_handle = {
  0,
  &quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_members,
  get_message_typesupport_handle_function,
  &quadrotor_msgs__msg__GoalSet__get_type_hash,
  &quadrotor_msgs__msg__GoalSet__get_type_description,
  &quadrotor_msgs__msg__GoalSet__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_quadrotor_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, quadrotor_msgs, msg, GoalSet)() {
  if (!quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_type_support_handle.typesupport_identifier) {
    quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &quadrotor_msgs__msg__GoalSet__rosidl_typesupport_introspection_c__GoalSet_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
