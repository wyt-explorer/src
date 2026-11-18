# Primitive_env — ROS 2 Jazzy 地图环境

本包生成三维障碍物点云、显示起终点标记，并将 RViz 点击转换为规划触发信号。完整单机仿真由 `primitive_planner` 的 `single_drone.launch.py` 启动，详见[根目录 README](../README.md)。

## 编译和启动

以下命令从仓库根目录 `~/RC_planner/src` 执行，构建产物也位于该目录：

```bash
cd ~/RC_planner/src
source /opt/ros/jazzy/setup.bash
colcon build --packages-up-to primitive_planner --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
ros2 launch primitive_planner single_drone.launch.py
```

如使用本机已有验证产物，将上面的 `source install/setup.bash` 换成 `source .jazzy-check/install/setup.bash`。每个终端都应先加载 Jazzy，再加载同一套工作区环境。

仅查看地图和起终点时运行：

```bash
ros2 launch primitive_env demo.launch.py
```

`demo.launch.py` 启动地图、标记和点击触发节点；不会启动规划器、模拟里程计或 RViz。不要与 `single_drone.launch.py` 同时运行，以免重复发布。

## 可视化和触发

在另一个加载同一环境的终端运行 `rviz2`，设置：

- Fixed Frame：`world`。
- PointCloud2：`/map_generator/global_cloud`。
- Marker：`/my_primitive_demo/waypoint_markers`。
- Odometry：`/state/odometry`（仅完整单机仿真启动后有数据）。

等待规划器进入 `WAIT_TARGET`，使用 Publish Point 工具在 `/clicked_point` 发布一次点击。点击只触发固定目标任务，不改变目标位置。也可以在终端触发：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

起点、终点、地图范围均由同一份 YAML 读取，不需要在两个启动文件中重复配置。

## 地图配置

默认文件为 `map/map_01.yaml`，位置、尺寸单位为米，当前单机启动要求 `map.frame_id: world`。支持 `box`、`cylinder`、`ring`：

```yaml
- type: ring
  position: [0.0, 0.0, 1.5]
  axis: y
  radius: 1.0
  tube_radius: 0.15
```

`axis` 指圆环平面法向轴。`box` 和 `cylinder` 的 `position` 为底面中心；`ring` 的 `position` 为圆环中心。`start`、`goal` 使用包含 `x`、`y`、`z` 的字典。

普通构建将地图复制到安装目录。修改源码地图后，可以重新构建 `primitive_env`，也可以直接指定源码路径并重启：

```bash
ros2 launch primitive_planner single_drone.launch.py map_file:=$PWD/Primitive_env/map/map_01.yaml
# 仅显示环境时：
ros2 launch primitive_env demo.launch.py map_file:=$PWD/Primitive_env/map/map_01.yaml
```

修改障碍物和起终点不需要重新生成运动基元库。

## 文件与话题

| 文件 | 作用 |
|---|---|
| `src/map_publisher.py` | 从 YAML 采样障碍物，发布 `/map_generator/global_cloud`，类型 `sensor_msgs/msg/PointCloud2`。 |
| `src/waypoint_publisher.py` | 发布 `/my_primitive_demo/waypoint_markers`，类型 `visualization_msgs/msg/Marker`。 |
| `src/rviz_click_trigger.py` | 将 `/clicked_point` 转成 `/traj_start_trigger` 的 `geometry_msgs/msg/PoseStamped`。 |
| `launch/demo.launch.py` | 启动环境的三个节点。 |
| `map/map_01.yaml` | 地图、障碍物、起点与终点。 |
| `rviz/demo.rviz` | 保留的显示配置；本次验证使用上述手动添加的 ROS 2 显示项。 |

## 验证状态

2026-11-18：单机闭环所需 6 包在 Jazzy 下 Release 编译通过。默认地图的基础运行测试中，模拟里程计从 `(-8, 0, 1.5)` 移动到距离目标约 0.49 米的位置，收到 88 帧局部点云和 1666 条位置指令。

当前为理想位置跟踪仿真，使用系统时间和 `world` 坐标系。完整飞行动力学、真实传感器、多机通信和实机飞行仍待验证。
