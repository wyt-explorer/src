// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice
#include "traj_utils/msg/detail/polynomial__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "traj_utils/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "traj_utils/msg/detail/polynomial__struct.h"
#include "traj_utils/msg/detail/polynomial__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "builtin_interfaces/msg/detail/time__functions.h"  // start_time

// forward declare type support functions

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
bool cdr_serialize_builtin_interfaces__msg__Time(
  const builtin_interfaces__msg__Time * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
bool cdr_deserialize_builtin_interfaces__msg__Time(
  eprosima::fastcdr::Cdr & cdr,
  builtin_interfaces__msg__Time * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
size_t get_serialized_size_builtin_interfaces__msg__Time(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
size_t max_serialized_size_builtin_interfaces__msg__Time(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
bool cdr_serialize_key_builtin_interfaces__msg__Time(
  const builtin_interfaces__msg__Time * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
size_t get_serialized_size_key_builtin_interfaces__msg__Time(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
size_t max_serialized_size_key_builtin_interfaces__msg__Time(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_IMPORT_traj_utils
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, builtin_interfaces, msg, Time)();


using _Polynomial__ros_msg_type = traj_utils__msg__Polynomial;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
bool cdr_serialize_traj_utils__msg__Polynomial(
  const traj_utils__msg__Polynomial * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: start_time
  {
    cdr_serialize_builtin_interfaces__msg__Time(
      &ros_message->start_time, cdr);
  }

  // Field name: coeff_x
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_x;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: coeff_y
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_y;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: coeff_z
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_z;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: duration
  {
    cdr << ros_message->duration;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
bool cdr_deserialize_traj_utils__msg__Polynomial(
  eprosima::fastcdr::Cdr & cdr,
  traj_utils__msg__Polynomial * ros_message)
{
  // Field name: start_time
  {
    cdr_deserialize_builtin_interfaces__msg__Time(cdr, &ros_message->start_time);
  }

  // Field name: coeff_x
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_x;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: coeff_y
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_y;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: coeff_z
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_z;
    cdr.deserialize_array(array_ptr, size);
  }

  // Field name: duration
  {
    cdr >> ros_message->duration;
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
size_t get_serialized_size_traj_utils__msg__Polynomial(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _Polynomial__ros_msg_type * ros_message = static_cast<const _Polynomial__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: start_time
  current_alignment += get_serialized_size_builtin_interfaces__msg__Time(
    &(ros_message->start_time), current_alignment);

  // Field name: coeff_x
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_x;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: coeff_y
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_y;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: coeff_z
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_z;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: duration
  {
    size_t item_size = sizeof(ros_message->duration);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
size_t max_serialized_size_traj_utils__msg__Polynomial(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: start_time
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_builtin_interfaces__msg__Time(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: coeff_x
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: coeff_y
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: coeff_z
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: duration
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = traj_utils__msg__Polynomial;
    is_plain =
      (
      offsetof(DataType, duration) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
bool cdr_serialize_key_traj_utils__msg__Polynomial(
  const traj_utils__msg__Polynomial * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: start_time
  {
    cdr_serialize_key_builtin_interfaces__msg__Time(
      &ros_message->start_time, cdr);
  }

  // Field name: coeff_x
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_x;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: coeff_y
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_y;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: coeff_z
  {
    size_t size = 6;
    auto array_ptr = ros_message->coeff_z;
    cdr.serialize_array(array_ptr, size);
  }

  // Field name: duration
  {
    cdr << ros_message->duration;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
size_t get_serialized_size_key_traj_utils__msg__Polynomial(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _Polynomial__ros_msg_type * ros_message = static_cast<const _Polynomial__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: start_time
  current_alignment += get_serialized_size_key_builtin_interfaces__msg__Time(
    &(ros_message->start_time), current_alignment);

  // Field name: coeff_x
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_x;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: coeff_y
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_y;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: coeff_z
  {
    size_t array_size = 6;
    auto array_ptr = ros_message->coeff_z;
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: duration
  {
    size_t item_size = sizeof(ros_message->duration);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_traj_utils
size_t max_serialized_size_key_traj_utils__msg__Polynomial(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: start_time
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_key_builtin_interfaces__msg__Time(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: coeff_x
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: coeff_y
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: coeff_z
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  // Field name: duration
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = traj_utils__msg__Polynomial;
    is_plain =
      (
      offsetof(DataType, duration) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _Polynomial__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const traj_utils__msg__Polynomial * ros_message = static_cast<const traj_utils__msg__Polynomial *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_traj_utils__msg__Polynomial(ros_message, cdr);
}

static bool _Polynomial__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  traj_utils__msg__Polynomial * ros_message = static_cast<traj_utils__msg__Polynomial *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_traj_utils__msg__Polynomial(cdr, ros_message);
}

static uint32_t _Polynomial__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_traj_utils__msg__Polynomial(
      untyped_ros_message, 0));
}

static size_t _Polynomial__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_traj_utils__msg__Polynomial(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_Polynomial = {
  "traj_utils::msg",
  "Polynomial",
  _Polynomial__cdr_serialize,
  _Polynomial__cdr_deserialize,
  _Polynomial__get_serialized_size,
  _Polynomial__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _Polynomial__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_Polynomial,
  get_message_typesupport_handle_function,
  &traj_utils__msg__Polynomial__get_type_hash,
  &traj_utils__msg__Polynomial__get_type_description,
  &traj_utils__msg__Polynomial__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, traj_utils, msg, Polynomial)() {
  return &_Polynomial__type_support;
}

#if defined(__cplusplus)
}
#endif
