#!/bin/bash
# 文件作用：启动上游圆周对径交换场景，等待预热后发送统一触发。
# 调用关系：调用 swarm.launch.py，再发布 /traj_start_trigger，最后等待节点退出。
# 输入：首个参数为无人机数量（默认 20），其余参数传给 ros2 launch。
# 输出：地图、通信桥和逐机规划闭环；AUTO_TRIGGER=false 可改为手动触发。
# 运行模式：ROS 2，沿用上游按机群规模计算的预热等待时间。
# 主要依赖：Bash、Python 3、ros2 launch/topic、primitive_planner。
set -e
count=${1:-20}
if [ "$#" -gt 0 ]; then shift; fi
ros2 launch primitive_planner swarm.launch.py drone_num:="$count" "$@" &
launch_pid=$!
trap 'kill -INT "$launch_pid" 2>/dev/null || true; wait "$launch_pid" || true' EXIT
if [ "${AUTO_TRIGGER:-true}" = true ]; then
  delay=$(python3 -c 'import sys; print(int(sys.argv[1]) * 0.2 + 5)' "$count")
  sleep "$delay"
  ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
fi
wait "$launch_pid"
