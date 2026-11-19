/**
 * @file pose_tool.h
 * 文件作用：声明处理三维位置、高度及朝向的鼠标交互。
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

#ifndef RVIZ_POSE_TOOL_H
#define RVIZ_POSE_TOOL_H

#include <OgreVector3.h>

#include <QCursor>

#include <rclcpp/rclcpp.hpp>

#include "rviz_common/tool.hpp"
#include "rviz_rendering/objects/arrow.hpp"
#include <vector>

namespace primitive_rviz
{



class Pose3DTool : public rviz_common::Tool
{
public:
  Pose3DTool();
  virtual ~Pose3DTool();

  virtual void onInitialize();

  virtual void activate();
  virtual void deactivate();

  virtual int processMouseEvent(rviz_common::ViewportMouseEvent& event);

protected:
  virtual void onPoseSet(double x, double y, double z, double theta) = 0;

  rviz_rendering::Arrow*              arrow_;
  std::vector<rviz_rendering::Arrow*> arrow_array;

  enum State
  {
    Position,
    Orientation,
    Height
  };
  State state_;

  Ogre::Vector3 pos_;
};
}

#endif
