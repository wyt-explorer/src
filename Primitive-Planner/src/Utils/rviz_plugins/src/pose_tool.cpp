/**
 * @file pose_tool.cpp
 * 文件作用：处理三维位置、高度及朝向的鼠标交互。
 * 调用关系：RViz 调用 Pose3DTool 事件接口，完成操作后调用派生类 onPoseSet()。
 * 输入：RViz 鼠标/键盘事件、相机和固定坐标系。
 * 输出：目标位置与偏航回调，以及交互箭头显示。
 * 运行模式：ROS 2 RViz2 插件，保留上游三维交互状态机。
 * 主要依赖：rviz_common、rviz_rendering、OGRE、Qt、rclcpp。
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

#include <OgrePlane.h>
#include <OgreRay.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>

#include "rviz_rendering/viewport_projection_finder.hpp"
#include <cassert>
#include "rviz_common/load_resource.hpp"
#include "rviz_rendering/objects/arrow.hpp"
#include "rviz_common/render_panel.hpp"
#include "rviz_common/viewport_mouse_event.hpp"

#include "pose_tool.h"

namespace primitive_rviz
{

static bool projectGround(rviz_common::ViewportMouseEvent& event, Ogre::Vector3& point)
{
  rviz_rendering::ViewportProjectionFinder finder;
  const auto projected = finder.getViewportPointProjectionOnXYPlane(event.panel->getRenderWindow(), event.x, event.y);
  point = projected.second;
  return projected.first;
}

Pose3DTool::Pose3DTool()
  : rviz_common::Tool()
  , arrow_(NULL)
{
}

Pose3DTool::~Pose3DTool()
{
  delete arrow_;
  for (auto* arrow : arrow_array) delete arrow;
}

void
Pose3DTool::onInitialize()
{
  arrow_ = new rviz_rendering::Arrow(scene_manager_, NULL, 2.0f, 0.2f, 0.5f, 0.35f);
  arrow_->setColor(0.0f, 1.0f, 0.0f, 1.0f);
  arrow_->getSceneNode()->setVisible(false);
}

void
Pose3DTool::activate()
{
  setStatus("Click and drag mouse to set position/orientation.");
  state_ = Position;
}

void
Pose3DTool::deactivate()
{
  arrow_->getSceneNode()->setVisible(false);
}

int
Pose3DTool::processMouseEvent(rviz_common::ViewportMouseEvent& event)
{
  int                  flags = 0;
  static Ogre::Vector3 ang_pos;
  static double        initz;
  static double        prevz;
  static double        prevangle;
  const double         z_scale    = 50;
  const double         z_interval = 0.5;
  Ogre::Quaternion     orient_x =
    Ogre::Quaternion(Ogre::Radian(Ogre::Math::HALF_PI), Ogre::Vector3::UNIT_Z);

  if (event.leftDown())
  {
    assert(state_ == Position);
    Ogre::Vector3 intersection;
    Ogre::Plane   ground_plane(Ogre::Vector3::UNIT_Z, 0.0f);
    if (projectGround(event, intersection))
    {
      pos_ = intersection;
      arrow_->setPosition(pos_);
      state_ = Orientation;
      flags |= Render;
    }
  }
  else if (event.type == QEvent::MouseMove && event.left())
  {
    if (state_ == Orientation)
    {
      // compute angle in x-y plane
      Ogre::Vector3 cur_pos;
      Ogre::Plane   ground_plane(Ogre::Vector3::UNIT_Z, 0.0f);
      if (projectGround(event, cur_pos))
      {
        double angle = atan2(cur_pos.y - pos_.y, cur_pos.x - pos_.x);
        arrow_->getSceneNode()->setVisible(true);
        arrow_->setOrientation(Ogre::Quaternion(orient_x));
        if (event.right())
          state_  = Height;
        initz     = pos_.z;
        prevz     = event.y;
        prevangle = angle;
        flags |= Render;
      }
    }
    if (state_ == Height)
    {
      double z  = event.y;
      double dz = z - prevz;
      prevz     = z;
      pos_.z -= dz / z_scale;
      arrow_->setPosition(pos_);
      // Create a list of arrows
      for (int k = 0; k < arrow_array.size(); k++)
        delete arrow_array[k];
      arrow_array.clear();
      int cnt = ceil(fabs(initz - pos_.z) / z_interval);
      for (int k = 0; k < cnt; k++)
      {
        rviz_rendering::Arrow* arrow__;
        arrow__ = new rviz_rendering::Arrow(scene_manager_, NULL, 0.5f, 0.1f, 0.0f, 0.1f);
        arrow__->setColor(0.0f, 1.0f, 0.0f, 1.0f);
        arrow__->getSceneNode()->setVisible(true);
        Ogre::Vector3 arr_pos = pos_;
        arr_pos.z = initz - ((initz - pos_.z > 0) ? 1 : -1) * k * z_interval;
        arrow__->setPosition(arr_pos);
        arrow__->setOrientation(
          Ogre::Quaternion(Ogre::Radian(prevangle), Ogre::Vector3::UNIT_Z) *
          orient_x);
        arrow_array.push_back(arrow__);
      }
      flags |= Render;
    }
  }
  else if (event.leftUp())
  {
    if (state_ == Orientation || state_ == Height)
    {
      // Create a list of arrows
      for (int k = 0; k < arrow_array.size(); k++)
        delete arrow_array[k];
      arrow_array.clear();
      onPoseSet(pos_.x, pos_.y, pos_.z, prevangle);
      flags |= (Finished | Render);
    }
  }

  return flags;
}
}
