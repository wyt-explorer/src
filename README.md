# RC Planner — ROS 2 Jazzy

## 1. 编译和启动指令

在 Ubuntu 24.04 / ROS 2 Jazzy 环境中，按“离线生成基元 → 编译 → 启动”的顺序执行。规划器启动时只读取基元库，不会自动生成。

基元库已随仓库生成（181 条路径，`path_end.ply` 首行 pathID=0），正常使用**无需重新生成**；仅修改 [swarm_path.py](Primitive-Planner/src/scripts/swarm_path.py) 参数后才需重跑：

```bash
cd ~/RC_planner/src
python3 -m pip install --user --break-system-packages toppra pyrfc3339
python3 Primitive-Planner/src/scripts/swarm_path.py
```

生成后核对 `primitive_library/obs_correspondence/path_end.ply` 首行末列应为 `0`（共 181 行）。`swarm_path.py` 已修复 toppra 0.6.x 对一维直线路径 `path(s,1)` 返回数组、导致直线基元被误判不可行（pathID 偏移）的问题。

生成参数在 [swarm_path.py](Primitive-Planner/src/scripts/swarm_path.py) 的 `run()` 和末尾调用处：速度、加速度、弧长、半径、旋转间隔、机体半径及体素尺寸。默认开启绘图，生成过程中出现图窗时需关闭后继续。脚本会先清空 `Primitive-Planner/src/planner/plan_manage/primitive_library/`，再生成 `trajectory/`、`trajectory_pos/`、`obs_correspondence/`、`agent_correspondence/`。修改参数后应整套重新生成，并核对规划器的速度范围、体素尺寸和覆盖盒尺寸配置。

生成完成后编译并启动，构建时会将基元库复制到安装目录：

```bash
cd ~/RC_planner/src
source /opt/ros/jazzy/setup.bash
colcon build --base-paths Primitive-Planner/src --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
ros2 launch primitive_planner swarm.launch.py drone_num:=20
```

若在编译后重新生成，需重新构建 `primitive_planner` 并加载安装环境；也可启动时直接指定源码中的新库：

```bash
ros2 launch primitive_planner swarm.launch.py drone_num:=20 \
  primitive_folder:="$PWD/Primitive-Planner/src/planner/plan_manage/primitive_library"
```

这是与上游对应的随机森林、圆周对径交换演示。默认启动 RViz2；无界面时加 `rviz:=false`。

RViz2 默认显示森林地图、各机模型、目标和规划轨迹，并提供 Displays、Views 面板；初始视角覆盖整个圆周。地图和无人机应在触发前显示，轨迹在规划后出现。

等待各机进入 `WAIT_TARGET` 后，在加载相同安装环境的终端触发：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

默认使用上游启用的理想位置跟踪器；加 `simulator:=so3` 选择完整 SO3 控制与动力学。`bash Primitive-Planner/src/scripts/run_swarm.sh 20` 会按上游等待时间自动触发，`AUTO_TRIGGER=false` 可关闭自动触发。也可使用 ROS 2 XML 入口 `swarm.launch`。

`Primitive_env` 目录本轮未修改，保留原样（后续单独接入）。上游 ROS 1 的 `catkin_make`、`roslaunch`、`devel/setup.bash` 及旧 XML 配置只作历史参考。

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

默认机群入口使用 `Primitive-Planner` 自己的地图生成器，不读取 `Primitive_env`。

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

机群的位置指令、心跳、轨迹反馈和规划 Marker 均按无人机 ID 隔离。

当前位置与点云使用同一 `world` 坐标系，长度为米、速度为米/秒、加速度为米/秒²、角度为弧度；参数与节点时钟采用秒制。默认演示使用系统时间；接入 `/clock` 时统一设置 `use_sim_time:=true`。核心规划器不负责查询 TF 或转换传感器外参。

ROS 2 的消息类型和字段按其命名规范迁移；UDP 使用 ROS 2 CDR，不能与 ROS 1 的网络序列化数据混用。

## 4. 代码功能说明

- `scripts/swarm_path.py`：离线生成运动基元、轨迹采样及障碍物/机间碰撞对应表，供规划器启动时加载。
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
Primitive_env/                      现有 YAML 地图及标记环境（本轮不修改）
agent.md                            开发约定
memory.md                           工作记录
```

## 6. 仿真与实机迁移

真实雷达、RGB-D 等驱动应在适配层输出统一点云与状态接口，再映射到规划器输入；不要把厂商 SDK 加入核心规划算法。接入硬件前仍需完成 TF、外参、时间同步、异常输入与超时处理、rosbag 回放和静态传感器测试。

上游机群演示、完整动力学与真实飞行是不同验证层次。Gazebo/RadaRays、真实雷达适配器、统一烟雾场和真实飞行不属于本次上游功能恢复。

## 7. 当前状态、限制和更新记录

- 2026-11-19：完成简洁文件头整理；随后按上游 cab5aa2 恢复功能与调用关系，本次不修改 `Primitive_env`。单独保留非负路径 ID 修正，另有初始化、包长和末端索引边界处理。
- 2026-11-20：按上游严格对齐目录结构与函数，删除非上游文件（tests/、UPSTREAM_PARITY.md、single_drone.launch.py、swarm.launch.xml、quadrotor_simulator_ros2.cpp 及 ROS 1 构建残留）；18 包编译全绿；8 机 swarm 冒烟测试验证完整 FSM 状态链（INIT→WAIT_TARGET→GEN_NEW_TRAJ→EXEC_TRAJ→REPLAN→APPROACH_GOAL→WAIT_TARGET）并到达目标。
- 随机地图、点云采样和 ROS 调度不保证逐帧一致；尚不能据此宣称与 ROS 1 运行结果逐点相等。
- TCP/MINCO、CUDA/EuRoC 等未接入上游默认演示的历史源码保留为参考；当前演示采用 UDP 与 CPU 局部感知。实机与大规模长时间运行仍需独立验证。
