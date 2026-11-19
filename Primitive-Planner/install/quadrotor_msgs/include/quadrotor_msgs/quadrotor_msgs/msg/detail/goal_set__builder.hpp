// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from quadrotor_msgs:msg/GoalSet.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "quadrotor_msgs/msg/goal_set.hpp"


#ifndef QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__BUILDER_HPP_
#define QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "quadrotor_msgs/msg/detail/goal_set__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace quadrotor_msgs
{

namespace msg
{

namespace builder
{

class Init_GoalSet_goal
{
public:
  explicit Init_GoalSet_goal(::quadrotor_msgs::msg::GoalSet & msg)
  : msg_(msg)
  {}
  ::quadrotor_msgs::msg::GoalSet goal(::quadrotor_msgs::msg::GoalSet::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::quadrotor_msgs::msg::GoalSet msg_;
};

class Init_GoalSet_drone_id
{
public:
  Init_GoalSet_drone_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GoalSet_goal drone_id(::quadrotor_msgs::msg::GoalSet::_drone_id_type arg)
  {
    msg_.drone_id = std::move(arg);
    return Init_GoalSet_goal(msg_);
  }

private:
  ::quadrotor_msgs::msg::GoalSet msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::quadrotor_msgs::msg::GoalSet>()
{
  return quadrotor_msgs::msg::builder::Init_GoalSet_drone_id();
}

}  // namespace quadrotor_msgs

#endif  // QUADROTOR_MSGS__MSG__DETAIL__GOAL_SET__BUILDER_HPP_
