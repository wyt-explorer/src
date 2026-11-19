#!/bin/bash
# 文件作用：发送一次与上游含义相同的规划启动触发。
# 调用关系：由终端、run_swarm.sh 或批量实验调用。
# 输入：已加载 ROS 2 工作空间的环境。
# 输出：/traj_start_trigger（PoseStamped，world 系单位姿态）。
# 运行模式：ROS 2；静态地图保持发布，当前启动链不依赖关闭地图进程。
# 主要依赖：Bash、ros2 topic、geometry_msgs。
set -e
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
