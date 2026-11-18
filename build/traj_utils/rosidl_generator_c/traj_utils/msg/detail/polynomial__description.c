// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

#include "traj_utils/msg/detail/polynomial__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_traj_utils
const rosidl_type_hash_t *
traj_utils__msg__Polynomial__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x69, 0xf8, 0x39, 0xe5, 0x86, 0x82, 0xf5, 0xbf,
      0x4f, 0x1b, 0x4c, 0x3a, 0x19, 0xf1, 0x6b, 0xb9,
      0x86, 0x4d, 0x48, 0x68, 0xbb, 0xfc, 0xc9, 0x9d,
      0x5a, 0x5c, 0x5d, 0x8a, 0x68, 0xbc, 0x48, 0x78,
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

static char traj_utils__msg__Polynomial__TYPE_NAME[] = "traj_utils/msg/Polynomial";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";

// Define type names, field names, and default values
static char traj_utils__msg__Polynomial__FIELD_NAME__start_time[] = "start_time";
static char traj_utils__msg__Polynomial__FIELD_NAME__coeff_x[] = "coeff_x";
static char traj_utils__msg__Polynomial__FIELD_NAME__coeff_y[] = "coeff_y";
static char traj_utils__msg__Polynomial__FIELD_NAME__coeff_z[] = "coeff_z";
static char traj_utils__msg__Polynomial__FIELD_NAME__duration[] = "duration";

static rosidl_runtime_c__type_description__Field traj_utils__msg__Polynomial__FIELDS[] = {
  {
    {traj_utils__msg__Polynomial__FIELD_NAME__start_time, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__Polynomial__FIELD_NAME__coeff_x, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_ARRAY,
      6,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__Polynomial__FIELD_NAME__coeff_y, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_ARRAY,
      6,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__Polynomial__FIELD_NAME__coeff_z, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_ARRAY,
      6,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {traj_utils__msg__Polynomial__FIELD_NAME__duration, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription traj_utils__msg__Polynomial__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
traj_utils__msg__Polynomial__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {traj_utils__msg__Polynomial__TYPE_NAME, 25, 25},
      {traj_utils__msg__Polynomial__FIELDS, 5, 5},
    },
    {traj_utils__msg__Polynomial__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "builtin_interfaces/Time start_time\n"
  "float64[6] coeff_x\n"
  "float64[6] coeff_y\n"
  "float64[6] coeff_z\n"
  "float64 duration";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
traj_utils__msg__Polynomial__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {traj_utils__msg__Polynomial__TYPE_NAME, 25, 25},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 108, 108},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
traj_utils__msg__Polynomial__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *traj_utils__msg__Polynomial__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
