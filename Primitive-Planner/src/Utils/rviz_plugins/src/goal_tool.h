/**
 * @file goal_tool.h
 * 文件作用：声明把三维交互目标发布为普通位姿和带 ID 的目标消息。
 * 调用关系：RViz 插件加载 Goal3DTool，Pose3DTool 完成交互后调用 onPoseSet()。
 * 输入：RViz 固定坐标系下的位置（米）和偏航（弧度）。
 * 输出：可配置 goal 话题及 /goal_with_id；当前目标无人机 ID 固定为 0。
 * 运行模式：ROS 2 RViz2 插件，保留上游三维交互状态机。
 * 主要依赖：rviz_common、rviz_rendering、Qt、rclcpp、geometry_msgs、quadrotor_msgs。
 */
/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RVIZ_GOAL_TOOL_H
#define RVIZ_GOAL_TOOL_H

#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
# include <QObject>

# include <rclcpp/rclcpp.hpp>

# include "pose_tool.h"
# include <geometry_msgs/msg/pose_stamped.hpp>
# include <quadrotor_msgs/msg/goal_set.hpp>
# include <rviz_common/properties/string_property.hpp>
#endif

namespace primitive_rviz
{




class Goal3DTool: public Pose3DTool
{
Q_OBJECT
public:
  Goal3DTool();
  virtual ~Goal3DTool() {}
  virtual void onInitialize();

protected:
  virtual void onPoseSet(double x, double y, double z, double theta);

private Q_SLOTS:
  void updateTopic();

private:
  rclcpp::Node::SharedPtr nh_;
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pub_goal_;
  rclcpp::Publisher<quadrotor_msgs::msg::GoalSet>::SharedPtr pub_droneID_goal_;

  rviz_common::properties::StringProperty* topic_property_;
};

}

#endif


