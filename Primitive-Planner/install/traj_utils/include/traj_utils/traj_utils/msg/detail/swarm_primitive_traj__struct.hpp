// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/swarm_primitive_traj.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_HPP_
#define TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'start_time'
#include "builtin_interfaces/msg/detail/time__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__traj_utils__msg__SwarmPrimitiveTraj __attribute__((deprecated))
#else
# define DEPRECATED__traj_utils__msg__SwarmPrimitiveTraj __declspec(deprecated)
#endif

namespace traj_utils
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SwarmPrimitiveTraj_
{
  using Type = SwarmPrimitiveTraj_<ContainerAllocator>;

  explicit SwarmPrimitiveTraj_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start_time(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->drone_id = 0;
      std::fill<typename std::array<float, 3>::iterator, float>(this->start_p.begin(), this->start_p.end(), 0.0f);
      std::fill<typename std::array<float, 3>::iterator, float>(this->start_v.begin(), this->start_v.end(), 0.0f);
      std::fill<typename std::array<float, 3>::iterator, float>(this->end_p.begin(), this->end_p.end(), 0.0f);
      std::fill<typename std::array<float, 9>::iterator, float>(this->rot_mat.begin(), this->rot_mat.end(), 0.0f);
      this->vel_id = 0l;
      this->select_path_id = 0l;
    }
  }

  explicit SwarmPrimitiveTraj_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start_time(_alloc, _init),
    start_p(_alloc),
    start_v(_alloc),
    end_p(_alloc),
    rot_mat(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->drone_id = 0;
      std::fill<typename std::array<float, 3>::iterator, float>(this->start_p.begin(), this->start_p.end(), 0.0f);
      std::fill<typename std::array<float, 3>::iterator, float>(this->start_v.begin(), this->start_v.end(), 0.0f);
      std::fill<typename std::array<float, 3>::iterator, float>(this->end_p.begin(), this->end_p.end(), 0.0f);
      std::fill<typename std::array<float, 9>::iterator, float>(this->rot_mat.begin(), this->rot_mat.end(), 0.0f);
      this->vel_id = 0l;
      this->select_path_id = 0l;
    }
  }

  // field types and members
  using _drone_id_type =
    int16_t;
  _drone_id_type drone_id;
  using _start_time_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _start_time_type start_time;
  using _start_p_type =
    std::array<float, 3>;
  _start_p_type start_p;
  using _start_v_type =
    std::array<float, 3>;
  _start_v_type start_v;
  using _end_p_type =
    std::array<float, 3>;
  _end_p_type end_p;
  using _rot_mat_type =
    std::array<float, 9>;
  _rot_mat_type rot_mat;
  using _vel_id_type =
    int32_t;
  _vel_id_type vel_id;
  using _select_path_id_type =
    int32_t;
  _select_path_id_type select_path_id;

  // setters for named parameter idiom
  Type & set__drone_id(
    const int16_t & _arg)
  {
    this->drone_id = _arg;
    return *this;
  }
  Type & set__start_time(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->start_time = _arg;
    return *this;
  }
  Type & set__start_p(
    const std::array<float, 3> & _arg)
  {
    this->start_p = _arg;
    return *this;
  }
  Type & set__start_v(
    const std::array<float, 3> & _arg)
  {
    this->start_v = _arg;
    return *this;
  }
  Type & set__end_p(
    const std::array<float, 3> & _arg)
  {
    this->end_p = _arg;
    return *this;
  }
  Type & set__rot_mat(
    const std::array<float, 9> & _arg)
  {
    this->rot_mat = _arg;
    return *this;
  }
  Type & set__vel_id(
    const int32_t & _arg)
  {
    this->vel_id = _arg;
    return *this;
  }
  Type & set__select_path_id(
    const int32_t & _arg)
  {
    this->select_path_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> *;
  using ConstRawPtr =
    const traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__traj_utils__msg__SwarmPrimitiveTraj
    std::shared_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__traj_utils__msg__SwarmPrimitiveTraj
    std::shared_ptr<traj_utils::msg::SwarmPrimitiveTraj_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SwarmPrimitiveTraj_ & other) const
  {
    if (this->drone_id != other.drone_id) {
      return false;
    }
    if (this->start_time != other.start_time) {
      return false;
    }
    if (this->start_p != other.start_p) {
      return false;
    }
    if (this->start_v != other.start_v) {
      return false;
    }
    if (this->end_p != other.end_p) {
      return false;
    }
    if (this->rot_mat != other.rot_mat) {
      return false;
    }
    if (this->vel_id != other.vel_id) {
      return false;
    }
    if (this->select_path_id != other.select_path_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const SwarmPrimitiveTraj_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SwarmPrimitiveTraj_

// alias to use template instance with default allocator
using SwarmPrimitiveTraj =
  traj_utils::msg::SwarmPrimitiveTraj_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace traj_utils

#endif  // TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__STRUCT_HPP_
