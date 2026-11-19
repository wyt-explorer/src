/**
 * @file selected_points_publisher.cpp
 * 文件作用：实现机群框选与群体目标交互，保持上游选择消息的 drone_N 标识。
 * 调用关系：选择属性 → 逐机 PoseStamped → assign_goals；右键地面点击生成高度 1 米目标。
 * 输入：RViz2 选择树及鼠标/键盘事件。
 * 输出：/rviz_selected_drones、/goal；位置为 world 系米制。
 * 运行模式：ROS 2 RViz2 工具，左键框选，P 发布，C 清除，右键发布群体目标。
 * 主要依赖：rviz_common、rviz_default_plugins、rviz_rendering、Qt、rclcpp。
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
 * selected_points_publisher.cpp
 *
 *  Created on: December 5th 2019
 */

#include "selected_points_publisher/selected_points_publisher.hpp"
#include <rviz_common/display_context.hpp>
#include <rviz_common/render_panel.hpp>
#include <rviz_common/viewport_mouse_event.hpp>
#include <rviz_common/interaction/selection_manager_iface.hpp>
#include <rviz_common/properties/property_tree_model.hpp>
#include <rviz_common/properties/vector_property.hpp>
#include <rviz_common/ros_integration/ros_node_abstraction_iface.hpp>
#include <rviz_rendering/viewport_projection_finder.hpp>
#include <pluginlib/class_list_macros.hpp>
#include <QKeyEvent>
#include <regex>

namespace rviz_plugin_selected_points_publisher
{
SelectedPointsPublisher::SelectedPointsPublisher() { shortcut_key_ = 's'; }
void SelectedPointsPublisher::onInitialize()
{
  rviz_default_plugins::tools::SelectionTool::onInitialize();
  node_ = context_->getRosNodeAbstraction().lock()->get_raw_node();
  updateTopic();
}
void SelectedPointsPublisher::updateTopic()
{
  if (!node_) return;
  selected_pub_ = node_->create_publisher<geometry_msgs::msg::PoseStamped>("/rviz_selected_drones", 100);
  goal_pub_ = node_->create_publisher<geometry_msgs::msg::PoseStamped>("/goal", 100);
}
void SelectedPointsPublisher::processSelectedArea()
{
  selected_drones_.clear();
  auto* model = context_->getSelectionManager()->getPropertyModel();
  const std::regex drone_name(R"(.*[Mm]arker drone[_ /]*(\d+).*)");
  for (int i = 0; i < model->rowCount(); ++i) {
    auto* property = model->getProp(model->index(i, 0));
    if (!property) continue;
    std::smatch match;
    const std::string name = property->getNameStd();
    if (!std::regex_match(name, match, drone_name)) continue;
    rviz_common::properties::VectorProperty* position = nullptr;
    for (int k = 0; k < property->numChildren(); ++k) {
      position = dynamic_cast<rviz_common::properties::VectorProperty*>(property->childAt(k));
      if (position) break;
    }
    if (!position) continue;
    const auto point = position->getVector();
    geometry_msgs::msg::PoseStamped message;
    message.header.frame_id = "drone_" + match[1].str();
    message.header.stamp = node_->now();
    message.pose.position.x = point.x;
    message.pose.position.y = point.y;
    message.pose.position.z = point.z;
    selected_drones_.push_back(message);
  }
}
int SelectedPointsPublisher::processKeyEvent(QKeyEvent* event, rviz_common::RenderPanel* panel)
{
  const int flags = rviz_default_plugins::tools::SelectionTool::processKeyEvent(event, panel);
  if (event->type() != QEvent::KeyPress) return flags;
  if (event->key() == Qt::Key_C) {
    selected_drones_.clear();
    context_->getSelectionManager()->select(panel->getRenderWindow(), 0, 0, 0, 0,
        rviz_common::interaction::SelectionManagerIface::Replace);
  } else if (event->key() == Qt::Key_P) {
    processSelectedArea();
    for (const auto& drone : selected_drones_) selected_pub_->publish(drone);
  }
  return flags;
}
int SelectedPointsPublisher::processMouseEvent(rviz_common::ViewportMouseEvent& event)
{
  const int flags = rviz_default_plugins::tools::SelectionTool::processMouseEvent(event);
  if (!event.rightUp() || event.alt()) return flags;
  rviz_rendering::ViewportProjectionFinder finder;
  const auto projected = finder.getViewportPointProjectionOnXYPlane(event.panel->getRenderWindow(), event.x, event.y);
  if (!projected.first) return flags;
  processSelectedArea();
  for (const auto& drone : selected_drones_) selected_pub_->publish(drone);
  geometry_msgs::msg::PoseStamped goal;
  goal.header.frame_id = "world";
  goal.header.stamp = node_->now();
  goal.pose.position.x = projected.second.x;
  goal.pose.position.y = projected.second.y;
  goal.pose.position.z = 1.0;
  goal.pose.orientation.w = 1.0;
  goal_pub_->publish(goal);
  return flags | Render;
}
}
PLUGINLIB_EXPORT_CLASS(rviz_plugin_selected_points_publisher::SelectedPointsPublisher, rviz_common::Tool)
