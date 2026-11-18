// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/polynomial.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__TRAITS_HPP_
#define TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "traj_utils/msg/detail/polynomial__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'start_time'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace traj_utils
{

namespace msg
{

inline void to_flow_style_yaml(
  const Polynomial & msg,
  std::ostream & out)
{
  out << "{";
  // member: start_time
  {
    out << "start_time: ";
    to_flow_style_yaml(msg.start_time, out);
    out << ", ";
  }

  // member: coeff_x
  {
    if (msg.coeff_x.size() == 0) {
      out << "coeff_x: []";
    } else {
      out << "coeff_x: [";
      size_t pending_items = msg.coeff_x.size();
      for (auto item : msg.coeff_x) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: coeff_y
  {
    if (msg.coeff_y.size() == 0) {
      out << "coeff_y: []";
    } else {
      out << "coeff_y: [";
      size_t pending_items = msg.coeff_y.size();
      for (auto item : msg.coeff_y) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: coeff_z
  {
    if (msg.coeff_z.size() == 0) {
      out << "coeff_z: []";
    } else {
      out << "coeff_z: [";
      size_t pending_items = msg.coeff_z.size();
      for (auto item : msg.coeff_z) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: duration
  {
    out << "duration: ";
    rosidl_generator_traits::value_to_yaml(msg.duration, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Polynomial & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: start_time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "start_time:\n";
    to_block_style_yaml(msg.start_time, out, indentation + 2);
  }

  // member: coeff_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.coeff_x.size() == 0) {
      out << "coeff_x: []\n";
    } else {
      out << "coeff_x:\n";
      for (auto item : msg.coeff_x) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: coeff_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.coeff_y.size() == 0) {
      out << "coeff_y: []\n";
    } else {
      out << "coeff_y:\n";
      for (auto item : msg.coeff_y) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: coeff_z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.coeff_z.size() == 0) {
      out << "coeff_z: []\n";
    } else {
      out << "coeff_z:\n";
      for (auto item : msg.coeff_z) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: duration
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "duration: ";
    rosidl_generator_traits::value_to_yaml(msg.duration, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Polynomial & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace traj_utils

namespace rosidl_generator_traits
{

[[deprecated("use traj_utils::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const traj_utils::msg::Polynomial & msg,
  std::ostream & out, size_t indentation = 0)
{
  traj_utils::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use traj_utils::msg::to_yaml() instead")]]
inline std::string to_yaml(const traj_utils::msg::Polynomial & msg)
{
  return traj_utils::msg::to_yaml(msg);
}

template<>
inline const char * data_type<traj_utils::msg::Polynomial>()
{
  return "traj_utils::msg::Polynomial";
}

template<>
inline const char * name<traj_utils::msg::Polynomial>()
{
  return "traj_utils/msg/Polynomial";
}

template<>
struct has_fixed_size<traj_utils::msg::Polynomial>
  : std::integral_constant<bool, has_fixed_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct has_bounded_size<traj_utils::msg::Polynomial>
  : std::integral_constant<bool, has_bounded_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct is_message<traj_utils::msg::Polynomial>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // TRAJ_UTILS__MSG__DETAIL__POLYNOMIAL__TRAITS_HPP_
