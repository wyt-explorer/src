// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "traj_utils/msg/detail/polynomial__functions.h"
#include "traj_utils/msg/detail/polynomial__struct.hpp"
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

void Polynomial_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) traj_utils::msg::Polynomial(_init);
}

void Polynomial_fini_function(void * message_memory)
{
  auto typed_message = static_cast<traj_utils::msg::Polynomial *>(message_memory);
  typed_message->~Polynomial();
}

size_t size_function__Polynomial__coeff_x(const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * get_const_function__Polynomial__coeff_x(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void * get_function__Polynomial__coeff_x(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void fetch_function__Polynomial__coeff_x(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__Polynomial__coeff_x(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__Polynomial__coeff_x(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__Polynomial__coeff_x(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

size_t size_function__Polynomial__coeff_y(const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * get_const_function__Polynomial__coeff_y(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void * get_function__Polynomial__coeff_y(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void fetch_function__Polynomial__coeff_y(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__Polynomial__coeff_y(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__Polynomial__coeff_y(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__Polynomial__coeff_y(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

size_t size_function__Polynomial__coeff_z(const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * get_const_function__Polynomial__coeff_z(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void * get_function__Polynomial__coeff_z(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<double, 6> *>(untyped_member);
  return &member[index];
}

void fetch_function__Polynomial__coeff_z(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__Polynomial__coeff_z(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__Polynomial__coeff_z(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__Polynomial__coeff_z(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Polynomial_message_member_array[5] = {
  {
    "start_time",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<builtin_interfaces::msg::Time>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::Polynomial, start_time),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "coeff_x",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::Polynomial, coeff_x),  // bytes offset in struct
    nullptr,  // default value
    size_function__Polynomial__coeff_x,  // size() function pointer
    get_const_function__Polynomial__coeff_x,  // get_const(index) function pointer
    get_function__Polynomial__coeff_x,  // get(index) function pointer
    fetch_function__Polynomial__coeff_x,  // fetch(index, &value) function pointer
    assign_function__Polynomial__coeff_x,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "coeff_y",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::Polynomial, coeff_y),  // bytes offset in struct
    nullptr,  // default value
    size_function__Polynomial__coeff_y,  // size() function pointer
    get_const_function__Polynomial__coeff_y,  // get_const(index) function pointer
    get_function__Polynomial__coeff_y,  // get(index) function pointer
    fetch_function__Polynomial__coeff_y,  // fetch(index, &value) function pointer
    assign_function__Polynomial__coeff_y,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "coeff_z",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::Polynomial, coeff_z),  // bytes offset in struct
    nullptr,  // default value
    size_function__Polynomial__coeff_z,  // size() function pointer
    get_const_function__Polynomial__coeff_z,  // get_const(index) function pointer
    get_function__Polynomial__coeff_z,  // get(index) function pointer
    fetch_function__Polynomial__coeff_z,  // fetch(index, &value) function pointer
    assign_function__Polynomial__coeff_z,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "duration",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(traj_utils::msg::Polynomial, duration),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Polynomial_message_members = {
  "traj_utils::msg",  // message namespace
  "Polynomial",  // message name
  5,  // number of fields
  sizeof(traj_utils::msg::Polynomial),
  false,  // has_any_key_member_
  Polynomial_message_member_array,  // message members
  Polynomial_init_function,  // function to initialize message memory (memory has to be allocated)
  Polynomial_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Polynomial_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Polynomial_message_members,
  get_message_typesupport_handle_function,
  &traj_utils__msg__Polynomial__get_type_hash,
  &traj_utils__msg__Polynomial__get_type_description,
  &traj_utils__msg__Polynomial__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace traj_utils


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<traj_utils::msg::Polynomial>()
{
  return &::traj_utils::msg::rosidl_typesupport_introspection_cpp::Polynomial_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, traj_utils, msg, Polynomial)() {
  return &::traj_utils::msg::rosidl_typesupport_introspection_cpp::Polynomial_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
