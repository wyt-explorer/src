/**
 * @file selected_points_publisher.hpp
 * 文件作用：声明 ROS 2 机群框选、选择发布和目标点击工具。
 * 调用关系：RViz2 加载插件，onInitialize() 创建发布器，鼠标/键盘回调读取选择属性。
 * 输入：RViz 选择结果、鼠标/键盘事件；位置为固定坐标系米制。
 * 输出：/rviz_selected_drones 与 /goal（PoseStamped），连接 assign_goals。
 * 运行模式：ROS 2 RViz2 插件，保留上游 C 清除、P 发布和右键目标交互。
 * 主要依赖：rviz_common、rviz_default_plugins、Qt、OGRE、rclcpp、geometry_msgs。
 */
/*
 * Copyright 2019-2020 Autoware Foundation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ********************
 *  v0.1.0: drwnz (david.wong@tier4.jp) *
 *
 * selected_points_publisher.hpp
 *
 *  Created on: December 5th 2019
 */

#pragma once
#include <rviz_default_plugins/tools/select/selection_tool.hpp>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <vector>
namespace rviz_plugin_selected_points_publisher
{
class SelectedPointsPublisher : public rviz_default_plugins::tools::SelectionTool
{
  Q_OBJECT
public:
  SelectedPointsPublisher();
  void onInitialize() override;
  int processMouseEvent(rviz_common::ViewportMouseEvent& event) override;
  int processKeyEvent(QKeyEvent* event, rviz_common::RenderPanel* panel) override;
public Q_SLOTS:
  void updateTopic();
protected:
  void processSelectedArea();
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr selected_pub_, goal_pub_;
  std::vector<geometry_msgs::msg::PoseStamped> selected_drones_;
};
}
