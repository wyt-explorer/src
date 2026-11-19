/**
 * @file goal_tool.cpp
 * 文件作用：把三维交互目标发布为普通位姿和带 ID 的目标消息。
 * 调用关系：RViz 插件加载 Goal3DTool，Pose3DTool 完成交互后调用 onPoseSet()。
 * 输入：RViz 固定坐标系下的位置（米）和偏航（弧度）。
 * 输出：可配置 goal 话题及 /goal_with_id；当前目标无人机 ID 固定为 0。
 * 运行模式：ROS 2 RViz2 插件，保留上游三维交互状态机。
 * 主要依赖：rviz_common、rviz_rendering、Qt、rclcpp、geometry_msgs、quadrotor_msgs。
 */
/*
 * Copyright (c) 2008, Willow Garage, Inc.
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

#include <cmath>
#include <rviz_common/ros_integration/ros_node_abstraction_iface.hpp>

#include <geometry_msgs/msg/pose_stamped.hpp>

#include "rviz_common/display_context.hpp"
#include "rviz_common/properties/string_property.hpp"

#include <quadrotor_msgs/msg/goal_set.hpp>

#include "goal_tool.h"

namespace primitive_rviz
{

Goal3DTool::Goal3DTool()
{
  shortcut_key_ = 'g';

  topic_property_ = new rviz_common::properties::StringProperty("Topic", "goal",
                                       "The topic on which to publish navigation goals.",
                                       getPropertyContainer(), SLOT(updateTopic()), this);
  // topic_property_droneID_ = new rviz_common::properties::StringProperty("Topic", "goal_with_id",
  //                                              "The topic on which to publish navigation goals.",
  //                                              getPropertyContainer(), SLOT(updateTopic()), this);
}

void Goal3DTool::onInitialize()
{
  Pose3DTool::onInitialize();
  setName("3D Nav Goal");
  updateTopic();
}

void Goal3DTool::updateTopic()
{
  nh_ = context_->getRosNodeAbstraction().lock()->get_raw_node();
  pub_goal_ = nh_->create_publisher<geometry_msgs::msg::PoseStamped>(topic_property_->getStdString(), 1);
  pub_droneID_goal_ = nh_->create_publisher<quadrotor_msgs::msg::GoalSet>("/goal_with_id", 1);
}

void Goal3DTool::onPoseSet(double x, double y, double z, double theta)
{
  geometry_msgs::msg::PoseStamped goal;
  goal.header.frame_id = context_->getFixedFrame().toStdString();
  goal.header.stamp = nh_->now();
  goal.pose.position.x = x;
  goal.pose.position.y = y;
  goal.pose.position.z = z;
  goal.pose.orientation.z = std::sin(theta / 2.0);
  goal.pose.orientation.w = std::cos(theta / 2.0);
  pub_goal_->publish(goal);
  quadrotor_msgs::msg::GoalSet with_id;
  with_id.drone_id = 0;
  with_id.goal = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
  pub_droneID_goal_->publish(with_id);
}

} // end namespace primitive_rviz

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(primitive_rviz::Goal3DTool, rviz_common::Tool)
