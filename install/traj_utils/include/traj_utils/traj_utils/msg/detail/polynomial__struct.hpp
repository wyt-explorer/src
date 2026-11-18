// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/polynomial.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_HPP_
#define TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_HPP_

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
# define DEPRECATED__traj_utils__msg__Polynomial __attribute__((deprecated))
#else
# define DEPRECATED__traj_utils__msg__Polynomial __declspec(deprecated)
#endif

namespace traj_utils
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Polynomial_
{
  using Type = Polynomial_<ContainerAllocator>;

  explicit Polynomial_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start_time(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_x.begin(), this->coeff_x.end(), 0.0);
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_y.begin(), this->coeff_y.end(), 0.0);
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_z.begin(), this->coeff_z.end(), 0.0);
      this->duration = 0.0;
    }
  }

  explicit Polynomial_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : start_time(_alloc, _init),
    coeff_x(_alloc),
    coeff_y(_alloc),
    coeff_z(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_x.begin(), this->coeff_x.end(), 0.0);
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_y.begin(), this->coeff_y.end(), 0.0);
      std::fill<typename std::array<double, 6>::iterator, double>(this->coeff_z.begin(), this->coeff_z.end(), 0.0);
      this->duration = 0.0;
    }
  }

  // field types and members
  using _start_time_type =
    builtin_interfaces::msg::Time_<ContainerAllocator>;
  _start_time_type start_time;
  using _coeff_x_type =
    std::array<double, 6>;
  _coeff_x_type coeff_x;
  using _coeff_y_type =
    std::array<double, 6>;
  _coeff_y_type coeff_y;
  using _coeff_z_type =
    std::array<double, 6>;
  _coeff_z_type coeff_z;
  using _duration_type =
    double;
  _duration_type duration;

  // setters for named parameter idiom
  Type & set__start_time(
    const builtin_interfaces::msg::Time_<ContainerAllocator> & _arg)
  {
    this->start_time = _arg;
    return *this;
  }
  Type & set__coeff_x(
    const std::array<double, 6> & _arg)
  {
    this->coeff_x = _arg;
    return *this;
  }
  Type & set__coeff_y(
    const std::array<double, 6> & _arg)
  {
    this->coeff_y = _arg;
    return *this;
  }
  Type & set__coeff_z(
    const std::array<double, 6> & _arg)
  {
    this->coeff_z = _arg;
    return *this;
  }
  Type & set__duration(
    const double & _arg)
  {
    this->duration = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    traj_utils::msg::Polynomial_<ContainerAllocator> *;
  using ConstRawPtr =
    const traj_utils::msg::Polynomial_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<traj_utils::msg::Polynomial_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<traj_utils::msg::Polynomial_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      traj_utils::msg::Polynomial_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<traj_utils::msg::Polynomial_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      traj_utils::msg::Polynomial_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<traj_utils::msg::Polynomial_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<traj_utils::msg::Polynomial_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<traj_utils::msg::Polynomial_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__traj_utils__msg__Polynomial
    std::shared_ptr<traj_utils::msg::Polynomial_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__traj_utils__msg__Polynomial
    std::shared_ptr<traj_utils::msg::Polynomial_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Polynomial_ & other) const
  {
    if (this->start_time != other.start_time) {
      return false;
    }
    if (this->coeff_x != other.coeff_x) {
      return false;
    }
    if (this->coeff_y != other.coeff_y) {
      return false;
    }
    if (this->coeff_z != other.coeff_z) {
      return false;
    }
    if (this->duration != other.duration) {
      return false;
    }
    return true;
  }
  bool operator!=(const Polynomial_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Polynomial_

// alias to use template instance with default allocator
using Polynomial =
  traj_utils::msg::Polynomial_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace traj_utils

#endif  // TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__STRUCT_HPP_
