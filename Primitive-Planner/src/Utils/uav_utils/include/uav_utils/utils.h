/**
 * @file utils.h
 * 文件作用：汇总几何与消息转换工具，并提供范围判断和限幅模板。
 * 调用关系：调用方包含此头文件；uav_utils_test.cpp 验证工具函数。
 * 输入：待判断或限幅的数值及区间上下界。
 * 输出：范围判断结果，或原地限幅后的数值；单位沿用输入。
 * 运行模式：头文件工具；汇总接口仍包含 ROS 1 消息依赖。
 * 主要依赖：cassert、converters.h、geometry_utils.h。
 */
#ifndef __UAV_UTILS_H
#define __UAV_UTILS_H

#include <cassert>

#include <uav_utils/converters.h>
#include <uav_utils/geometry_utils.h>

namespace uav_utils
{

/* judge if value belongs to [low,high] */
template <typename T, typename T2>
bool
in_range(T value, const T2& low, const T2& high)
{
  assert(low < high);
  return (low <= value) && (value <= high);
}

/* judge if value belongs to [-limit, limit] */
template <typename T, typename T2>
bool
in_range(T value, const T2& limit)
{
  assert(limit > 0);
  return in_range(value, -limit, limit);
}

template <typename T, typename T2>
void
limit_range(T& value, const T2& low, const T2& high)
{
  assert(low < high);
  if (value < low)
  {
    value = low;
  }

  if (value > high)
  {
    value = high;
  }

  return;
}

template <typename T, typename T2>
void
limit_range(T& value, const T2& limit)
{
  assert(limit > 0);
  limit_range(value, -limit, limit);
}

typedef std::stringstream DebugSS_t;
} // end of namespace uav_utils

#endif
