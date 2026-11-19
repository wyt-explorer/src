// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from quadrotor_msgs:msg/GoalSet.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "quadrotor_msgs/msg/goal_set.hpp"


#ifndef QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_HPP_
#define QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__quadrotor_msgs__msg__GoalSet __attribute__((deprecated))
#else
# define DEPRECATED__quadrotor_msgs__msg__GoalSet __declspec(deprecated)
#endif

namespace quadrotor_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GoalSet_
{
  using Type = GoalSet_<ContainerAllocator>;

  explicit GoalSet_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->drone_id = 0;
      std::fill<typename std::array<float, 3>::iterator, float>(this->goal.begin(), this->goal.end(), 0.0f);
    }
  }

  explicit GoalSet_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : goal(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->drone_id = 0;
      std::fill<typename std::array<float, 3>::iterator, float>(this->goal.begin(), this->goal.end(), 0.0f);
    }
  }

  // field types and members
  using _drone_id_type =
    int16_t;
  _drone_id_type drone_id;
  using _goal_type =
    std::array<float, 3>;
  _goal_type goal;

  // setters for named parameter idiom
  Type & set__drone_id(
    const int16_t & _arg)
  {
    this->drone_id = _arg;
    return *this;
  }
  Type & set__goal(
    const std::array<float, 3> & _arg)
  {
    this->goal = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    quadrotor_msgs::msg::GoalSet_<ContainerAllocator> *;
  using ConstRawPtr =
    const quadrotor_msgs::msg::GoalSet_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      quadrotor_msgs::msg::GoalSet_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      quadrotor_msgs::msg::GoalSet_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__quadrotor_msgs__msg__GoalSet
    std::shared_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__quadrotor_msgs__msg__GoalSet
    std::shared_ptr<quadrotor_msgs::msg::GoalSet_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GoalSet_ & other) const
  {
    if (this->drone_id != other.drone_id) {
      return false;
    }
    if (this->goal != other.goal) {
      return false;
    }
    return true;
  }
  bool operator!=(const GoalSet_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GoalSet_

// alias to use template instance with default allocator
using GoalSet =
  quadrotor_msgs::msg::GoalSet_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace quadrotor_msgs

#endif  // QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__STRUCT_HPP_
