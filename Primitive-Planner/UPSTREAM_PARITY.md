# 上游功能对照（ROS 2 Jazzy）

基准：[ZJU-FAST-Lab/Primitive-Planner](https://github.com/ZJU-FAST-Lab/Primitive-Planner/tree/cab5aa2f82948be58b36e37b6a921b9dba97423c)，提交 `cab5aa2f82948be58b36e37b6a921b9dba97423c`。

本次保留上游的算法和模块分工，替换 ROS 1 消息、节点、定时器、启动语法及 RViz 接口。`Primitive_env` 不在修改范围。

| 上游入口/模块 | ROS 2 对应实现 | 保留的行为 |
|---|---|---|
| `gen_position_swap.py` / `swarm.launch` | `swarm.launch.py`、`swarm.launch.xml` | N/π 半径、圆周起点、对径目标、随机森林 |
| `run_in_sim.xml` | `run_in_sim.launch.py` | 每机规划器、轨迹服务、仿真感知链 |
| `advanced_param.xml` | `advanced_param.launch.py` | 原权重、体素、碰撞间距和重规划阈值 |
| `simulator.xml` | `simulator.launch.py` | 默认理想跟踪；可选 `simulator:=so3` |
| `PPPlannerManager` | 同名类与函数 | 点云随机采样、对应表碰撞标记、目标/边界评分 |
| `PPReplanFSM` | 同名状态机 | 触发、预置/交互目标、重规划、停车和恢复 |
| `traj_server` | 同名节点 | 基元/多项式采样、偏航、心跳超时、端点反馈 |
| `bridge_node_udp` | 同名节点 | 里程计、基元、目标、Joy 广播；消息类型编号保持不变 |
| 理想跟踪器 | `poscmd_2_odom` | 位置/速度/姿态；上游用 `twist.angular` 携带加速度的约定 |
| SO3 nodelet / 完整模拟器 | `so3_control_ros2` / `quadrotor_simulator_so3` | 电机使能、校正、IMU 反馈、控制分配、刚体积分 |
| `random_goals` | 同名包节点 | 随机抽样、目标占用、到达停留、未启动重发 |
| `assign_goals` / 手动接管 | 同名包节点 | 编队平移、箭头显示/清除、Joy 积分接管 |
| 三维目标 / 机群框选插件 | `rviz_plugins` / `selected_points_publisher` | 三维位置/高度/偏航、框选与目标发布 |
| `tf_assist` / 姿态诊断 | 同名脚本 | 多个 converterN、仅 yaw 的中间系、RPY 度制输出 |

## 启动

```bash
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch primitive_planner swarm.launch.py drone_num:=20
```

等待各机 `WAIT_TARGET` 后，在相同环境的另一个终端执行：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

无界面加 `rviz:=false`。默认入口不读取 `Primitive_env`；原 `single_drone.launch.py` 仍可使用已有 YAML 地图。`run_swarm.sh N` 会沿用上游等待时间自动触发；`AUTO_TRIGGER=false` 关闭自动触发。

逐机话题保持 `/drone_N_visual_slam/odom`、`/drone_N_pcl_render_node/cloud`、`/drone_N_planning/pos_cmd` 等上游命名。各机基元经公共发送话题 → UDP 桥 → 公共接收话题共享；位置指令、心跳、端点反馈及规划 Marker 均按 ID 隔离。

## 必要适配与边界

- ROS 2 的 CDR UDP 数据不能与 ROS 1 序列化数据混用；功能与消息含义保留。
- 回调内的两秒触发等待、目标朝向等待改成状态与截止时间，避免递归 spin；规划公式不变。
- 保留已有的非负路径 ID 修正：生成器从 `0` 开始，而上游从 `-1` 开始，与其非负索引读取器不兼容。轨迹几何、时间参数化与对应表公式保持上游实现。
- 初始化无效轨迹记录，校验网络包长和接收 ID，夹住末端采样索引；这些处理只约束未初始化或越界输入。
- 小机群且圆内无有效森林区域时生成空地图，避免上游半径公式得到负地图尺寸；20 机默认场景沿用原几何公式。
- 上游默认演示使用理想跟踪器，完整 SO3 动力学作为可选链路，不能把二者测试结果混为一谈。
- 历史 TCP/MINCO、CUDA/EuRoC 等未接入上游默认演示的源码保留；本轮主入口采用 UDP 和 CPU 局部点云感知。随机地图、点云采样及 ROS 调度不保证逐帧数值相同。

## 验证命令

```bash
python3 Primitive-Planner/tests/check_upstream_parity.py --upstream /path/to/upstream-checkout
ROS_DOMAIN_ID=74 python3 Primitive-Planner/tests/smoke_components.py --output .parity-check/components
ROS_DOMAIN_ID=75 python3 Primitive-Planner/tests/smoke_swarm.py --drones 4 --radius 3 --output .parity-check/swarm-4.json
ROS_DOMAIN_ID=76 python3 Primitive-Planner/tests/smoke_swarm.py --drones 20 --radius 6.366197723675814 --output .parity-check/swarm-20.json
```

算法对照检查覆盖 21 项，包括碰撞/评分、轨迹与偏航采样、控制分配、刚体动力学和基元生成；只归一化中间件、日志以及上述单独列明的路径 ID 修正。

## 本轮验证结果（2026-11-19）

| 检查 | 结果 |
|---|---|
| 编译 | `Primitive-Planner` 下 18 个 ROS 2 包全部通过；最终生命周期修复涉及的 8 包已重新编译 |
| 上游算法对照 | 21 项通过 |
| 组件 | 理想跟踪、编队目标、随机目标占用、手动接管、UDP 往返、SO3 积分共 6 项通过；所有节点退出码为 0 |
| 四机圆周交换 | 4/4 到达目标 0.1 米范围；UDP ID 0–3 全部观察到，正常停止 |
| 二十机默认半径交换 | 20/20 到达目标 0.1 米范围，最大误差约 0.0964 米；UDP ID 0–19 全部观察到，正常停止 |
| 原 YAML 单机入口 | 到达目标 0.1 米范围，误差约 0.0981 米，正常停止 |
| RViz2 | 两个工具通过 pluginlib 工厂发现、加载、实例化；未覆盖鼠标交互逐项操作 |
| 生成器 | 圆形/方形目标、指定起终点、随机目标飞行、里程计显示共 5 种 XML 通过 ROS 2 前端解析 |
| 修改范围 | `Primitive_env` 全文件 SHA-256 校验一致 |

本机汇总：`.parity-check/verification-summary.json`。完整日志与结果保存在同目录；该目录不进入 Git。测试停止信号仅发送给本轮启动的进程，不清理其他 ROS 进程。测试用 UDP 端口按机数隔离，正常启动仍默认使用 8081。

这些是本轮给定场景的功能与回归结果，不是随机场景全覆盖、ROS 1/ROS 2 逐帧数值等同或真实飞行证明。
