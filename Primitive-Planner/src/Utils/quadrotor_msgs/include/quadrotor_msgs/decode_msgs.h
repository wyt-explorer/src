/**
 * @file decode_msgs.h
 * 文件作用：声明飞控通信消息的解码接口。
 * 调用关系：外部通信调用方通过 decode_msgs.h 调用，字段布局由 comm_types.h 定义。
 * 输入：飞控协议字节数组，字段缩放见 comm_types.h。
 * 输出：OutputData、StatusData、PPROutputData 消息及解码成功标志。
 * 运行模式：遗留 ROS 1 消息接口，当前 ROS 2 CMake 未构建此编解码代码。
 * 主要依赖：quadrotor_msgs 旧消息头、comm_types.h、vector。
 */
#ifndef __QUADROTOR_MSGS_QUADROTOR_MSGS_H__
#define __QUADROTOR_MSGS_QUADROTOR_MSGS_H__

#include <stdint.h>
#include <vector>
#include <quadrotor_msgs/OutputData.h>
#include <quadrotor_msgs/StatusData.h>
#include <quadrotor_msgs/PPROutputData.h>

namespace quadrotor_msgs
{

bool decodeOutputData(const std::vector<uint8_t> &data,
                      quadrotor_msgs::OutputData &output);

bool decodeStatusData(const std::vector<uint8_t> &data,
                      quadrotor_msgs::StatusData &status);

bool decodePPROutputData(const std::vector<uint8_t> &data,
                         quadrotor_msgs::PPROutputData &output);
}

#endif
