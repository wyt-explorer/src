/**
 * @file encode_msgs.h
 * 文件作用：声明飞控通信消息的编码接口。
 * 调用关系：外部通信调用方通过 encode_msgs.h 调用，字段布局由 comm_types.h 定义。
 * 输入：SO3Command、TRPYCommand、Gains 消息及各字段物理量。
 * 输出：按协议缩放、打包的 uint8_t 字节数组。
 * 运行模式：遗留 ROS 1 消息接口，当前 ROS 2 CMake 未构建此编解码代码。
 * 主要依赖：quadrotor_msgs 旧消息头、comm_types.h、vector。
 */
#ifndef __QUADROTOR_MSGS_QUADROTOR_MSGS_H__
#define __QUADROTOR_MSGS_QUADROTOR_MSGS_H__

#include <stdint.h>
#include <vector>
#include <quadrotor_msgs/SO3Command.h>
#include <quadrotor_msgs/TRPYCommand.h>
#include <quadrotor_msgs/Gains.h>

namespace quadrotor_msgs
{

void encodeSO3Command(const quadrotor_msgs::SO3Command &so3_command,
                      std::vector<uint8_t> &output);
void encodeTRPYCommand(const quadrotor_msgs::TRPYCommand &trpy_command,
                       std::vector<uint8_t> &output);

void encodePPRGains(const quadrotor_msgs::Gains &gains,
                    std::vector<uint8_t> &output);
}

#endif
