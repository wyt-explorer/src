# RC Planner — ROS 2 Jazzy

## 1. 编译和启动指令

在 Ubuntu 24.04 / ROS 2 Jazzy 环境中，从工作空间根目录执行：

```bash
cd ~/RC_planner/src
source /opt/ros/jazzy/setup.bash
colcon build --base-paths Primitive-Planner/src --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
ros2 launch primitive_planner swarm.launch.py drone_num:=20
```

这是与上游对应的随机森林、圆周对径交换演示。默认启动 RViz2；无界面时加 `rviz:=false`。等待各机进入 `WAIT_TARGET` 后，在加载相同安装环境的终端触发：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

默认使用上游启用的理想位置跟踪器；加 `simulator:=so3` 选择完整 SO3 控制与动力学。`bash Primitive-Planner/src/scripts/run_swarm.sh 20` 会按上游等待时间自动触发，`AUTO_TRIGGER=false` 可关闭自动触发。也可使用 ROS 2 XML 入口 `swarm.launch.xml`。

保留原来的 YAML 单机演示，不需要修改 `Primitive_env`：

```bash
ros2 launch primitive_planner single_drone.launch.py
# 替换地图：在上面的命令后添加 map_file:=/absolute/path/map.yaml
```

此入口需要已安装的 `primitive_env`。若尚未安装，可先单独构建该包；本次上游功能恢复未修改其源码或配置。地图和起终点仍共用 YAML。不要在同一次运行中混用不同构建目录的安装环境。

本轮隔离验证产物保存在 `.parity-check/`（已忽略），完成构建后可加载：

```bash
source /opt/ros/jazzy/setup.bash
source .parity-check/install/setup.bash
```

原 `.jazzy-check/`、`build/` 和 `install/` 中的旧产物不代表当前源码。上游 ROS 1 的 `catkin_make`、`roslaunch`、`devel/setup.bash` 及旧 XML 配置只作历史参考。

## 2. 系统架构

本地实现以 [ZJU-FAST-Lab/Primitive-Planner 的 cab5aa2 提交](https://github.com/ZJU-FAST-Lab/Primitive-Planner/tree/cab5aa2f82948be58b36e37b6a921b9dba97423c) 为基准，保留原算法和模块分工：

```text
swarm.launch.py
  ├─ 随机森林地图
  ├─ UDP 群体通信桥
  └─ 每架飞机：run_in_sim.launch.py
       ├─ advanced_param.launch.py → FSM → 基元碰撞筛选与评分
       ├─ traj_server → 位置/速度/加速度/偏航指令
       └─ simulator.launch.py → 理想跟踪或 SO3 动力学 → 里程计 → 局部点云感知
```

完整调用映射、适配边界和验证命令见 [上游功能对照](Primitive-Planner/UPSTREAM_PARITY.md)。默认机群入口使用 `Primitive-Planner` 自己的地图生成器，不读取 `Primitive_env`。原 YAML 单机演示仍使用既有环境节点。

## 3. 消息流和统一接口

| 话题 | 类型 | 用途 |
|---|---|---|
| `/map_generator/global_cloud` | `sensor_msgs/msg/PointCloud2` | 世界系静态障碍物 |
| `/traj_start_trigger` | `geometry_msgs/msg/PoseStamped` | 公共规划触发 |
| `/drone_N_visual_slam/odom` | `nav_msgs/msg/Odometry` | 逐机状态 |
| `/drone_N_pcl_render_node/cloud` | `sensor_msgs/msg/PointCloud2` | 逐机局部点云 |
| `/drone_N_planning/selected_path_id` | `traj_utils/msg/SwarmPrimitiveTraj` | 逐机选中基元 |
| `/drone_N_planning/pos_cmd` | `quadrotor_msgs/msg/PositionCommand` | 逐机执行指令 |
| `/drone_N_traj_server/heartbeat` | `std_msgs/msg/Empty` | 逐机规划心跳 |
| `/broadcast_primitive_from_planner` → `/broadcast_primitive_to_planner` | `traj_utils/msg/SwarmPrimitiveTraj` | 经 UDP 交换群体轨迹 |

机群的位置指令、心跳、轨迹反馈和规划 Marker 均按无人机 ID 隔离。原 YAML 单机入口继续使用 `/state/odometry`、`/sensors/rgbd/points`、`/position_cmd` 和 `/planning/*`。

当前位置与点云使用同一 `world` 坐标系，长度为米、速度为米/秒、加速度为米/秒²、角度为弧度；参数与节点时钟采用秒制。默认演示使用系统时间；接入 `/clock` 时统一设置 `use_sim_time:=true`。核心规划器不负责查询 TF 或转换传感器外参。

ROS 2 的消息类型和字段按其命名规范迁移；UDP 使用 ROS 2 CDR，不能与 ROS 1 的网络序列化数据混用。

## 4. 代码功能说明

- `plan_manage`：保持上游 FSM、对应表碰撞筛选、路径评分、轨迹采样和偏航计算，提供完整 ROS 2 多机启动层级。
- `swarm_bridge`：恢复 UDP 的收发、有效包长处理和线程退出，转发里程计、基元、目标及手柄消息。
- `traj_utils`：轨迹消息、群体轨迹容器和规划可视化。
- `local_sensing` / `map_generator`：保留上游 CPU 局部点云查询、随机柱体与圆环地图生成。
- `poscmd_2_odom`：保留上游理想跟踪映射，包括 `twist.angular` 携带加速度的历史约定。
- `so3_control` / `so3_quadrotor_simulator`：恢复完整控制回调、增益/校正量、电机配置、控制分配和动力学积分。
- `random_goals` / `assign_goals` / `manual_take_over`：恢复随机目标占用与重发、编队平移分配、手柄超时与积分接管。
- `rviz_plugins` / `selected_points_publisher`：ROS 2 三维目标与机群框选工具。
- `uav_utils`：ROS 2 消息转换、多个里程计转换器、TF 与姿态诊断脚本。

## 5. 目录结构

```text
Primitive-Planner/src/planner/       规划器、消息和群体通信
Primitive-Planner/src/uav_simulator/ 地图、感知、跟踪及动力学
Primitive-Planner/src/Utils/         目标工具、手动接管、RViz2 与通用工具
Primitive-Planner/tests/             上游算法对照和 ROS 2 集成检查
Primitive_env/                      现有 YAML 地图及标记环境（本轮不修改）
agent.md                            开发约定
memory.md                           工作记录
.parity-check/                      本机验证产物、日志及上游参考（已忽略）
```

## 6. 仿真与实机迁移

真实雷达、RGB-D 等驱动应在适配层输出统一点云与状态接口，再映射到规划器输入；不要把厂商 SDK 加入核心规划算法。接入硬件前仍需完成 TF、外参、时间同步、异常输入与超时处理、rosbag 回放和静态传感器测试。

上游机群演示、完整动力学与真实飞行是不同验证层次。Gazebo/RadaRays、真实雷达适配器、统一烟雾场和真实飞行不属于本次上游功能恢复。

## 7. 当前状态、限制和更新记录

- 2026-11-18：原 YAML 单机闭环曾完成基础构建与运行检查；该历史结果不作为本轮多机验证结果。
- 2026-11-19：完成简洁文件头整理；随后按上游 cab5aa2 恢复功能与调用关系，本次不修改 `Primitive_env`。
- 21 项上游算法对照检查通过；单独保留非负路径 ID 修正，另有初始化、包长和末端索引边界处理，详见功能对照文档。
- 本轮 18 包编译、21 项上游算法对照、6 项组件、两个 RViz2 工厂及 5 种生成 XML 检查通过。4/20 机均到达目标 0.1 米范围，原 YAML 单机回归通过，全部正常停止；日志与汇总位于 `.parity-check/`。
- 随机地图、点云采样和 ROS 调度不保证逐帧一致；尚不能据此宣称与 ROS 1 运行结果逐点相等。
- TCP/MINCO、CUDA/EuRoC 等未接入上游默认演示的历史源码保留为参考；当前演示采用 UDP 与 CPU 局部感知。实机与大规模长时间运行仍需独立验证。
