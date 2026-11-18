// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "traj_utils/msg/swarm_primitive_traj.hpp"


#ifndef TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__TRAITS_HPP_
#define TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "traj_utils/msg/detail/swarm_primitive_traj__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'start_time'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace traj_utils
{

namespace msg
{

inline void to_flow_style_yaml(
  const SwarmPrimitiveTraj & msg,
  std::ostream & out)
{
  out << "{";
  // member: drone_id
  {
    out << "drone_id: ";
    rosidl_generator_traits::value_to_yaml(msg.drone_id, out);
    out << ", ";
  }

  // member: start_time
  {
    out << "start_time: ";
    to_flow_style_yaml(msg.start_time, out);
    out << ", ";
  }

  // member: start_p
  {
    if (msg.start_p.size() == 0) {
      out << "start_p: []";
    } else {
      out << "start_p: [";
      size_t pending_items = msg.start_p.size();
      for (auto item : msg.start_p) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: start_v
  {
    if (msg.start_v.size() == 0) {
      out << "start_v: []";
    } else {
      out << "start_v: [";
      size_t pending_items = msg.start_v.size();
      for (auto item : msg.start_v) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: end_p
  {
    if (msg.end_p.size() == 0) {
      out << "end_p: []";
    } else {
      out << "end_p: [";
      size_t pending_items = msg.end_p.size();
      for (auto item : msg.end_p) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: rot_mat
  {
    if (msg.rot_mat.size() == 0) {
      out << "rot_mat: []";
    } else {
      out << "rot_mat: [";
      size_t pending_items = msg.rot_mat.size();
      for (auto item : msg.rot_mat) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: vel_id
  {
    out << "vel_id: ";
    rosidl_generator_traits::value_to_yaml(msg.vel_id, out);
    out << ", ";
  }

  // member: select_path_id
  {
    out << "select_path_id: ";
    rosidl_generator_traits::value_to_yaml(msg.select_path_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SwarmPrimitiveTraj & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: drone_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "drone_id: ";
    rosidl_generator_traits::value_to_yaml(msg.drone_id, out);
    out << "\n";
  }

  // member: start_time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "start_time:\n";
    to_block_style_yaml(msg.start_time, out, indentation + 2);
  }

  // member: start_p
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.start_p.size() == 0) {
      out << "start_p: []\n";
    } else {
      out << "start_p:\n";
      for (auto item : msg.start_p) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: start_v
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.start_v.size() == 0) {
      out << "start_v: []\n";
    } else {
      out << "start_v:\n";
      for (auto item : msg.start_v) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: end_p
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.end_p.size() == 0) {
      out << "end_p: []\n";
    } else {
      out << "end_p:\n";
      for (auto item : msg.end_p) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: rot_mat
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.rot_mat.size() == 0) {
      out << "rot_mat: []\n";
    } else {
      out << "rot_mat:\n";
      for (auto item : msg.rot_mat) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: vel_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "vel_id: ";
    rosidl_generator_traits::value_to_yaml(msg.vel_id, out);
    out << "\n";
  }

  // member: select_path_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "select_path_id: ";
    rosidl_generator_traits::value_to_yaml(msg.select_path_id, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SwarmPrimitiveTraj & msg, bool use_flow_style = false)
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
  const traj_utils::msg::SwarmPrimitiveTraj & msg,
  std::ostream & out, size_t indentation = 0)
{
  traj_utils::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use traj_utils::msg::to_yaml() instead")]]
inline std::string to_yaml(const traj_utils::msg::SwarmPrimitiveTraj & msg)
{
  return traj_utils::msg::to_yaml(msg);
}

template<>
inline const char * data_type<traj_utils::msg::SwarmPrimitiveTraj>()
{
  return "traj_utils::msg::SwarmPrimitiveTraj";
}

template<>
inline const char * name<traj_utils::msg::SwarmPrimitiveTraj>()
{
  return "traj_utils/msg/SwarmPrimitiveTraj";
}

template<>
struct has_fixed_size<traj_utils::msg::SwarmPrimitiveTraj>
  : std::integral_constant<bool, has_fixed_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct has_bounded_size<traj_utils::msg::SwarmPrimitiveTraj>
  : std::integral_constant<bool, has_bounded_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct is_message<traj_utils::msg::SwarmPrimitiveTraj>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // TRAJ_UTILS__MSG__DETAIL__SWARM_PRIMITIVE_TRAJ__TRAITS_HPP_
