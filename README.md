# RC Planner — ROS 2 Jazzy 移植

## 1. 编译和启动指令

在 Ubuntu 24.04 / ROS 2 Jazzy 环境中，从仓库根目录（当前为 `~/RC_planner/src`）运行：

```bash
cd ~/RC_planner/src
source /opt/ros/jazzy/setup.bash
rosdep install --from-paths Primitive-Planner/src Primitive_env --ignore-src -r -y
colcon build --packages-up-to primitive_planner --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
ros2 launch primitive_planner single_drone.launch.py
```

如果使用本次已经编译的验证产物，可以直接启动，无需再次构建：

```bash
cd ~/RC_planner/src
source /opt/ros/jazzy/setup.bash
source .jazzy-check/install/setup.bash
ros2 launch primitive_planner single_drone.launch.py
```

`install/` 与 `.jazzy-check/install/` 是两套构建产物，同一次运行的所有终端应加载同一套环境。上述验证目录是本机生成文件，不随 Git 分发。

此命令只构建单机闭环所需依赖。两个旧 RViz 工具尚未完成 ROS 2 迁移，其他辅助包也未完成本轮复验。不要使用仓库中的旧 `build/`、`devel/` 或 `catkin_make`。运动基元库随 `primitive_planner` 安装；可通过 `primitive_folder:=/absolute/path` 指定其他库。更换地图：

```bash
ros2 launch primitive_planner single_drone.launch.py map_file:=/absolute/path/map.yaml
```

规划器加载基元库、收到里程计后进入 `WAIT_TARGET`。在另一终端先加载 `/opt/ros/jazzy/setup.bash` 和同一套 `install/setup.bash`（或 `.jazzy-check/install/setup.bash`），再执行：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

启动命令默认自动打开 RViz2，并加载 `world` 坐标系、障碍物点云、起终点标记和模拟里程计显示。等待 `WAIT_TARGET` 后，通过 RViz 的 Publish Point 工具点击地图即可触发规划。无图形界面时可添加 `rviz:=false` 关闭窗口。`ros2 launch primitive_env demo.launch.py` 只启动地图与标记，不启动规划闭环。

默认地图、起点和终点都来自 `Primitive_env/map/map_01.yaml`。普通构建会将配置复制到安装目录；修改源码目录中的地图后，重新构建 `primitive_env`，或直接指定源码文件：

```bash
ros2 launch primitive_planner single_drone.launch.py map_file:=$PWD/Primitive_env/map/map_01.yaml
```

基础数据流检查（每次执行一条命令，按 Ctrl+C 退出）：

```bash
ros2 topic hz /sensors/rgbd/points
ros2 topic hz /planning/heartbeat
ros2 topic hz /position_cmd
ros2 topic echo /state/odometry --once
```

## 2. 系统架构

当前单机算法仿真：

```text
YAML → 地图发布 → 局部点云感知 → 运动基元规划器 → 轨迹服务
                                  ↑                    ↓
                                  └── 模拟里程计 ← 位置指令跟踪
```

`poscmd_2_odom` 是理想位置指令跟踪器，不模拟飞行动力学。ROS 2 的 `quadrotor_simulator_ros2.cpp` 目前只发布固定状态，不能用于验证 SO3 动力学闭环。

## 3. 消息流和统一接口

| 话题 | 消息类型 | 含义 |
|---|---|---|
| `/map_generator/global_cloud` | `sensor_msgs/msg/PointCloud2` | 静态全局障碍物 |
| `/sensors/rgbd/points` | `sensor_msgs/msg/PointCloud2` | 从全局地图提取的局部点云 |
| `/state/odometry` | `nav_msgs/msg/Odometry` | 当前模拟位姿和速度 |
| `/traj_start_trigger` | `geometry_msgs/msg/PoseStamped` | 开始规划 |
| `/planning/selected_path_id` | `traj_utils/msg/SwarmPrimitiveTraj` | 选定的运动基元 |
| `/planning/heartbeat` | `std_msgs/msg/Empty` | 规划心跳 |
| `/position_cmd` | `quadrotor_msgs/msg/PositionCommand` | 位置、速度、加速度和偏航指令 |

当前演示使用 `world` 坐标系，长度米、速度米/秒、角度弧度，使用系统时间（没有 `/clock`）。接入有 `/clock` 的仿真时，各节点必须统一配置 `use_sim_time`。雷达输入预留 `/sensors/radar/points`；适配层尚未实现。

ROS 2 消息兼容性变更：`swarmPrimitiveTraj.msg` 改为 `SwarmPrimitiveTraj.msg`；时间字段改用 `builtin_interfaces/Time`；`Gains` 的 `Kp/Kd/Kp_yaw/Kd_yaw` 改为 `kp/kd/kp_yaw/kd_yaw`，`SO3Command` 的 `kR/kOm` 改为 `k_r/k_om`，数值含义不变。外部发布者需同步调整字段。

## 4. 代码功能说明

- `plan_manage`：加载基元库、碰撞检测、规划状态机和轨迹指令生成。移植中修正发布器类型、回调签名、消息命名空间和节点时钟；回调不再递归执行节点。
- `traj_utils`：轨迹消息与可视化库。库目标与 rosidl 目标分名，避免 CMake 冲突。
- `local_sensing`：依据模拟里程计提取附近点云，时间戳沿用里程计。
- `Primitive_env`：按 YAML 生成箱体、圆柱和圆环点云，发布起终点标记。起终点与规划器共用同一份配置。
- `single_drone.launch.py`：安装后可启动的单机闭环，连接统一点云和里程计话题。

## 5. 目录结构

```text
Primitive_env/                  地图配置、地图及标记发布脚本
Primitive-Planner/src/planner/  规划器、轨迹消息和 UDP 桥
Primitive-Planner/src/Utils/    自定义消息、可视化及辅助节点
Primitive-Planner/src/uav_simulator/  地图、局部感知与模拟节点
agent.md                       工程开发约定
memory.md                      完成项和待办记录
```

`Primitive-Planner/README.md` 和现有 XML launch 主要记录原 ROS 1 用法，不能直接作为 ROS 2 启动说明。

## 6. 仿真与实机迁移

仿真或实机传感器通过适配节点输出统一点云及里程计，核心规划器不引入厂商 SDK。当前点云回调假定输入已经在世界坐标系，尚未实现 TF 查询和传感器外参转换。

后续需完成 `map → odom → base_link` 和传感器 TF、时间同步、超时与非法输入处理、rosbag 回放，再接入真实硬件。雷达、RGB-D、烟雾退化模型和 Gazebo/RadaRays 不在当前单机仿真中实现。

## 7. 当前状态、限制和更新记录

- 当前分支为 `new`，已发现 ROS 1/ROS 2 混合代码和构建配置遗漏。
- 已补齐各包 `ament_cmake` 构建类型、ROS 2 消息规范和单机启动入口。
- `rviz_plugins`、`selected_points_publisher` 仍引用 ROS 1 RViz API，暂需跳过。
- TCP 桥及旧 XML launch 尚未移植；UDP 桥仍需多机通信验证。
- SO3 控制器/模拟器的完整行为与原 ROS 1 版本不等价；编译通过不能视为动力学验证。
- 多机避碰、真实传感器、仿真时间和实机飞行尚未验证。
- 2026-11-18：单机闭环所需 6 包（primitive_env、quadrotor_msgs、traj_utils、local_sensing_node、poscmd_2_odom、primitive_planner）在 Jazzy 下 Release 编译通过。
- 单机基础运行通过：地图 map_01.yaml，从 (-8, 0, 1.5) 触发后移动至 (7.5212, 0.1044, 1.4947)，距离目标约 0.49 米时结束测试。观察到 88 帧局部点云、1666 条位置指令；这不等于完整动力学、多机避碰或实机验证。
- 本次验证产物位于 `.jazzy-check/`（已忽略），可 `source .jazzy-check/install/setup.bash` 使用；标准构建命令见第 1 节。

- 2026-11-18：单机启动默认打开 RViz2，自动加载 ROS 2 地图、起终点、里程计和点击触发工具；可用 `rviz:=false` 关闭。RViz 子进程会清理继承的 Snap GTK/GIO/Qt 插件路径，避免系统图形库冲突。修改启动文件后需重新构建 `primitive_env` 与 `primitive_planner` 并加载对应安装环境。
