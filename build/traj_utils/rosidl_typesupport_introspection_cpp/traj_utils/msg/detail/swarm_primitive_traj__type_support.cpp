// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "traj_utils/msg/detail/swarm_primitive_traj__functions.h"
#include "traj_utils/msg/detail/swarm_primitive_traj__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace traj_utils
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void SwarmPrimitiveTraj_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) traj_utils::msg::SwarmPrimitiveTraj(_init);
}

void SwarmPrimitiveTraj_fini_function(void * message_memory)
{
  auto typed_message = static_cast<traj_utils::msg::SwarmPrimitiveTraj *>(message_memory);
  typed_message->~SwarmPrimitiveTraj();
}

size_t size_function__SwarmPrimitiveTraj__start_p(const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * get_const_function__SwarmPrimitiveTraj__start_p(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void * get_function__SwarmPrimitiveTraj__start_p(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void fetch_function__SwarmPrimitiveTraj__start_p(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__SwarmPrimitiveTraj__start_p(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__SwarmPrimitiveTraj__start_p(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__SwarmPrimitiveTraj__start_p(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__SwarmPrimitiveTraj__start_v(const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * get_const_function__SwarmPrimitiveTraj__start_v(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void * get_function__SwarmPrimitiveTraj__start_v(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void fetch_function__SwarmPrimitiveTraj__start_v(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__SwarmPrimitiveTraj__start_v(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__SwarmPrimitiveTraj__start_v(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__SwarmPrimitiveTraj__start_v(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__SwarmPrimitiveTraj__end_p(const void * untyped_member)
{
  (void)untyped_member;
  return 3;
}

const void * get_const_function__SwarmPrimitiveTraj__end_p(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void * get_function__SwarmPrimitiveTraj__end_p(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 3> *>(untyped_member);
  return &member[index];
}

void fetch_function__SwarmPrimitiveTraj__end_p(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__SwarmPrimitiveTraj__end_p(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__SwarmPrimitiveTraj__end_p(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__SwarmPrimitiveTraj__end_p(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__SwarmPrimitiveTraj__rot_mat(const void * untyped_member)
{
  (void)untyped_member;
  return 9;
}

const void * get_const_function__SwarmPrimitiveTraj__rot_mat(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 9> *>(untyped_member);
  return &member[index];
}

void * get_function__SwarmPrimitiveTraj__rot_mat(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 9> *>(untyped_member);
  return &member[index];
}

void fetch_function__SwarmPrimitiveTraj__rot_mat(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__SwarmPrimitiveTraj__rot_mat(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__SwarmPrimitiveTraj__rot_mat(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__SwarmPrimitiveTraj__rot_mat(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SwarmPrimitiveTraj_message_member_array[8] = {
  {
    "drone_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, drone_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "start_time",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<builtin_interfaces::msg::Time>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, start_time),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "start_p",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, start_p),  // bytes offset in struct
    nullptr,  // default value
    size_function__SwarmPrimitiveTraj__start_p,  // size() function pointer
    get_const_function__SwarmPrimitiveTraj__start_p,  // get_const(index) function pointer
    get_function__SwarmPrimitiveTraj__start_p,  // get(index) function pointer
    fetch_function__SwarmPrimitiveTraj__start_p,  // fetch(index, &value) function pointer
    assign_function__SwarmPrimitiveTraj__start_p,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "start_v",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, start_v),  // bytes offset in struct
    nullptr,  // default value
    size_function__SwarmPrimitiveTraj__start_v,  // size() function pointer
    get_const_function__SwarmPrimitiveTraj__start_v,  // get_const(index) function pointer
    get_function__SwarmPrimitiveTraj__start_v,  // get(index) function pointer
    fetch_function__SwarmPrimitiveTraj__start_v,  // fetch(index, &value) function pointer
    assign_function__SwarmPrimitiveTraj__start_v,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "end_p",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    3,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, end_p),  // bytes offset in struct
    nullptr,  // default value
    size_function__SwarmPrimitiveTraj__end_p,  // size() function pointer
    get_const_function__SwarmPrimitiveTraj__end_p,  // get_const(index) function pointer
    get_function__SwarmPrimitiveTraj__end_p,  // get(index) function pointer
    fetch_function__SwarmPrimitiveTraj__end_p,  // fetch(index, &value) function pointer
    assign_function__SwarmPrimitiveTraj__end_p,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "rot_mat",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    9,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, rot_mat),  // bytes offset in struct
    nullptr,  // default value
    size_function__SwarmPrimitiveTraj__rot_mat,  // size() function pointer
    get_const_function__SwarmPrimitiveTraj__rot_mat,  // get_const(index) function pointer
    get_function__SwarmPrimitiveTraj__rot_mat,  // get(index) function pointer
    fetch_function__SwarmPrimitiveTraj__rot_mat,  // fetch(index, &value) function pointer
    assign_function__SwarmPrimitiveTraj__rot_mat,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "vel_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, vel_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "select_path_id",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::SwarmPrimitiveTraj, select_path_id),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SwarmPrimitiveTraj_message_members = {
  "traj_utils::msg",  // message namespace
  "SwarmPrimitiveTraj",  // message name
  8,  // number of fields
  sizeof(traj_utils::msg::SwarmPrimitiveTraj),
  false,  // has_any_key_member_
  SwarmPrimitiveTraj_message_member_array,  // message members
  SwarmPrimitiveTraj_init_function,  // function to initialize message memory (memory has to be allocated)
  SwarmPrimitiveTraj_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SwarmPrimitiveTraj_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SwarmPrimitiveTraj_message_members,
  get_message_typesupport_handle_function,
  &traj_utils__msg__SwarmPrimitiveTraj__get_type_hash,
  &traj_utils__msg__SwarmPrimitiveTraj__get_type_description,
  &traj_utils__msg__SwarmPrimitiveTraj__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace traj_utils


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<traj_utils::msg::SwarmPrimitiveTraj>()
{
  return &::traj_utils::msg::rosidl_typesupport_introspection_cpp::SwarmPrimitiveTraj_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, traj_utils, msg, SwarmPrimitiveTraj)() {
  return &::traj_utils::msg::rosidl_typesupport_introspection_cpp::SwarmPrimitiveTraj_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
