// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

#include "traj_utils/msg/detail/swarm_primitive_traj__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_traj_utils
const rosidl_type_hash_t *
traj_utils__msg__SwarmPrimitiveTraj__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x94, 0xbc, 0x35, 0x92, 0xcb, 0xe5, 0xa2, 0xd4,
      0x2b, 0x0b, 0x78, 0x93, 0x20, 0xd0, 0xfa, 0xa5,
      0x60, 0x8f, 0x6d, 0xa0, 0xed, 0xc6, 0x75, 0xa0,
      0x5f, 0xb1, 0x51, 0x90, 0x48, 0x13, 0xbd, 0xe9,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
#endif

static char traj_utils__msg__SwarmPrimitiveTraj__TYPE_NAME[] = "traj_utils/msg/SwarmPrimitiveTraj";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";

// Define type names, field names, and default values
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__drone_id[] = "drone_id";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_time[] = "start_time";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_p[] = "start_p";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_v[] = "start_v";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__end_p[] = "end_p";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__rot_mat[] = "rot_mat";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__vel_id[] = "vel_id";
static char traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__select_path_id[] = "select_path_id";

static rosidl_runtime_c__type_description__Field traj_utils__msg__SwarmPrimitiveTraj__FIELDS[] = {
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__drone_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_time, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_p, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      3,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__start_v, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      3,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__end_p, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      3,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__rot_mat, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      9,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__vel_id, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__SwarmPrimitiveTraj__FIELD_NAME__select_path_id, 14, 14},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription traj_utils__msg__SwarmPrimitiveTraj__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
traj_utils__msg__SwarmPrimitiveTraj__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {traj_utils__msg__SwarmPrimitiveTraj__TYPE_NAME, 33, 33},
      {traj_utils__msg__SwarmPrimitiveTraj__FIELDS, 8, 8},
    },
    {traj_utils__msg__SwarmPrimitiveTraj__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int16 drone_id\n"
  "builtin_interfaces/Time start_time\n"
  "float32[3] start_p\n"
  "float32[3] start_v\n"
  "float32[3] end_p\n"
  "float32[9] rot_mat\n"
  "int32 vel_id\n"
  "int32 select_path_id";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
traj_utils__msg__SwarmPrimitiveTraj__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {traj_utils__msg__SwarmPrimitiveTraj__TYPE_NAME, 33, 33},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 157, 157},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
traj_utils__msg__SwarmPrimitiveTraj__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *traj_utils__msg__SwarmPrimitiveTraj__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
