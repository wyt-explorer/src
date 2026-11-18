// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from quadrotor_msgs:msg/GoalSet.idl
// generated code does not contain a copyright notice

#include "quadrotor_msgs/msg/detail/goal_set__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_quadrotor_msgs
const rosidl_type_hash_t *
quadrotor_msgs__msg__GoalSet__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xcb, 0x04, 0x56, 0x51, 0x4a, 0x10, 0xf7, 0x94,
      0x40, 0x7c, 0xb4, 0xc7, 0xda, 0xff, 0x03, 0xad,
      0x11, 0xb9, 0xb2, 0x38, 0x7c, 0xd4, 0x4f, 0x2e,
      0x5e, 0xb8, 0x20, 0x77, 0xc2, 0x49, 0x93, 0xdf,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char quadrotor_msgs__msg__GoalSet__TYPE_NAME[] = "quadrotor_msgs/msg/GoalSet";

// Define type names, field names, and default values
static char quadrotor_msgs__msg__GoalSet__FIELD_NAME__drone_id[] = "drone_id";
static char quadrotor_msgs__msg__GoalSet__FIELD_NAME__goal[] = "goal";

static rosidl_runtime_c__type_description__Field quadrotor_msgs__msg__GoalSet__FIELDS[] = {
  {
    {quadrotor_msgs__msg__GoalSet__FIELD_NAME__drone_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {quadrotor_msgs__msg__GoalSet__FIELD_NAME__goal, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      3,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
quadrotor_msgs__msg__GoalSet__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {quadrotor_msgs__msg__GoalSet__TYPE_NAME, 26, 26},
      {quadrotor_msgs__msg__GoalSet__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int16 drone_id\n"
  "float32[3] goal";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
quadrotor_msgs__msg__GoalSet__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {quadrotor_msgs__msg__GoalSet__TYPE_NAME, 26, 26},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 31, 31},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
quadrotor_msgs__msg__GoalSet__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *quadrotor_msgs__msg__GoalSet__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
