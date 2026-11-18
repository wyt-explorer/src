// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/swarm_primitive_traj.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__BUILDER_HPP_
#define TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "traj_utils/msg/detail/swarm_primitive_traj__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace traj_utils
{

namespace msg
{

namespace builder
{

class Init_SwarmPrimitiveTraj_select_path_id
{
public:
  explicit Init_SwarmPrimitiveTraj_select_path_id(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  ::traj_utils::msg::SwarmPrimitiveTraj select_path_id(::traj_utils::msg::SwarmPrimitiveTraj::_select_path_id_type arg)
  {
    msg_.select_path_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_vel_id
{
public:
  explicit Init_SwarmPrimitiveTraj_vel_id(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_select_path_id vel_id(::traj_utils::msg::SwarmPrimitiveTraj::_vel_id_type arg)
  {
    msg_.vel_id = std::move(arg);
    return Init_SwarmPrimitiveTraj_select_path_id(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_rot_mat
{
public:
  explicit Init_SwarmPrimitiveTraj_rot_mat(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_vel_id rot_mat(::traj_utils::msg::SwarmPrimitiveTraj::_rot_mat_type arg)
  {
    msg_.rot_mat = std::move(arg);
    return Init_SwarmPrimitiveTraj_vel_id(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_end_p
{
public:
  explicit Init_SwarmPrimitiveTraj_end_p(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_rot_mat end_p(::traj_utils::msg::SwarmPrimitiveTraj::_end_p_type arg)
  {
    msg_.end_p = std::move(arg);
    return Init_SwarmPrimitiveTraj_rot_mat(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_start_v
{
public:
  explicit Init_SwarmPrimitiveTraj_start_v(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_end_p start_v(::traj_utils::msg::SwarmPrimitiveTraj::_start_v_type arg)
  {
    msg_.start_v = std::move(arg);
    return Init_SwarmPrimitiveTraj_end_p(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_start_p
{
public:
  explicit Init_SwarmPrimitiveTraj_start_p(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_start_v start_p(::traj_utils::msg::SwarmPrimitiveTraj::_start_p_type arg)
  {
    msg_.start_p = std::move(arg);
    return Init_SwarmPrimitiveTraj_start_v(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_start_time
{
public:
  explicit Init_SwarmPrimitiveTraj_start_time(::traj_utils::msg::SwarmPrimitiveTraj & msg)
  : msg_(msg)
  {}
  Init_SwarmPrimitiveTraj_start_p start_time(::traj_utils::msg::SwarmPrimitiveTraj::_start_time_type arg)
  {
    msg_.start_time = std::move(arg);
    return Init_SwarmPrimitiveTraj_start_p(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

class Init_SwarmPrimitiveTraj_drone_id
{
public:
  Init_SwarmPrimitiveTraj_drone_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SwarmPrimitiveTraj_start_time drone_id(::traj_utils::msg::SwarmPrimitiveTraj::_drone_id_type arg)
  {
    msg_.drone_id = std::move(arg);
    return Init_SwarmPrimitiveTraj_start_time(msg_);
  }

private:
  ::traj_utils::msg::SwarmPrimitiveTraj msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::traj_utils::msg::SwarmPrimitiveTraj>()
{
  return traj_utils::msg::builder::Init_SwarmPrimitiveTraj_drone_id();
}

}  // namespace traj_utils

#endif  // TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__BUILDER_HPP_
