// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/polynomial.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__BUILDER_HPP_
#define TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "traj_utils/msg/detail/polynomial__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace traj_utils
{

namespace msg
{

namespace builder
{

class Init_Polynomial_duration
{
public:
  explicit Init_Polynomial_duration(::traj_utils::msg::Polynomial & msg)
  : msg_(msg)
  {}
  ::traj_utils::msg::Polynomial duration(::traj_utils::msg::Polynomial::_duration_type arg)
  {
    msg_.duration = std::move(arg);
    return std::move(msg_);
  }

private:
  ::traj_utils::msg::Polynomial msg_;
};

class Init_Polynomial_coeff_z
{
public:
  explicit Init_Polynomial_coeff_z(::traj_utils::msg::Polynomial & msg)
  : msg_(msg)
  {}
  Init_Polynomial_duration coeff_z(::traj_utils::msg::Polynomial::_coeff_z_type arg)
  {
    msg_.coeff_z = std::move(arg);
    return Init_Polynomial_duration(msg_);
  }

private:
  ::traj_utils::msg::Polynomial msg_;
};

class Init_Polynomial_coeff_y
{
public:
  explicit Init_Polynomial_coeff_y(::traj_utils::msg::Polynomial & msg)
  : msg_(msg)
  {}
  Init_Polynomial_coeff_z coeff_y(::traj_utils::msg::Polynomial::_coeff_y_type arg)
  {
    msg_.coeff_y = std::move(arg);
    return Init_Polynomial_coeff_z(msg_);
  }

private:
  ::traj_utils::msg::Polynomial msg_;
};

class Init_Polynomial_coeff_x
{
public:
  explicit Init_Polynomial_coeff_x(::traj_utils::msg::Polynomial & msg)
  : msg_(msg)
  {}
  Init_Polynomial_coeff_y coeff_x(::traj_utils::msg::Polynomial::_coeff_x_type arg)
  {
    msg_.coeff_x = std::move(arg);
    return Init_Polynomial_coeff_y(msg_);
  }

private:
  ::traj_utils::msg::Polynomial msg_;
};

class Init_Polynomial_start_time
{
public:
  Init_Polynomial_start_time()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Polynomial_coeff_x start_time(::traj_utils::msg::Polynomial::_start_time_type arg)
  {
    msg_.start_time = std::move(arg);
    return Init_Polynomial_coeff_x(msg_);
  }

private:
  ::traj_utils::msg::Polynomial msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::traj_utils::msg::Polynomial>()
{
  return traj_utils::msg::builder::Init_Polynomial_start_time();
}

}  // namespace traj_utils

#endif  // TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__BUILDER_HPP_
