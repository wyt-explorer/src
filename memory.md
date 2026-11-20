# 项目记忆

> 每次修改或生成代码前先阅读本文件、`agent.md` 和 `README.md`。

## 已完成

- 已确定仿真与实机使用统一 ROS 2 传感器接口。
- 已确定规划器通过适配节点接收雷达、RGB-D、里程计和 IMU 数据。
- 已规定源文件头注释、关键函数注释和架构变更后的 README 同步规则。
- 已建立 `agent.md` 开发约定。

## 当前架构

```text
仿真：Gazebo/RadaRays → 仿真适配节点 → 统一 ROS 2 话题 → Primitive-Planner
实机：真实传感器驱动 → 实机适配节点 → 统一 ROS 2 话题 → Primitive-Planner
```

## 待部署或待实现

- [x] 创建根目录 `README.md`，记录 ROS 2 编译方式、单机启动和迁移限制。
- [x] 确认输入为世界坐标系 PointCloud2 和 Odometry，单机启动映射到统一话题。
- [ ] 创建雷达仿真适配节点。
- [ ] 创建真实雷达适配节点。
- [ ] 完成 `radar_link`、`camera_link` 和 `base_link` 的 TF 配置。
- [ ] 建立统一烟雾浓度场和传感器退化模型。
- [ ] 完成 Gazebo 仿真启动文件。
- [ ] 完成 rosbag 回放测试。
- [ ] 完成真实传感器静态测试和实机迁移测试。

## 更新记录

- 2026-11-17：新增项目开发规则、统一接口要求、代码注释要求和 README 同步要求。

## ROS 2 Jazzy 移植排查（2026-11-18）

- 已修改各包构建类型、ROS 2 消息字段及时间类型、规划发布器与回调签名。
- 已增加 `single_drone.launch.py`，统一地图、起终点及核心话题，修复递归 spin 和心跳连接。
- 已修复地图脚本 ROS 2 点云模块导入、起终点字典解析和局部点云时间戳。
- 初轮构建有 12 个包通过，但不能据此认定核心闭环通过。
- 上轮 `/tmp` 构建日志已不可用；在 `.jazzy-check/` 重新构建。
- 已重新编译通过：primitive_env、quadrotor_msgs、traj_utils、local_sensing_node、poscmd_2_odom。
- primitive_planner 已修正 traj_utils 头文件导出、遗留消息别名和 Jazzy 回调签名，Release 编译通过。
- 单机闭环已在允许本机 DDS 的环境通过基础运行测试：收到 2426 条里程计、88 帧局部点云、1666 条位置指令和 1688 条心跳。
- 从 (-8, 0, 1.5) 触发后移动至 (7.5212, 0.1044, 1.4947)，距离目标 (8, 0, 1.5) 约 0.49 米后结束测试。
- 运行日志和结果：`.jazzy-check/single-drone-smoke.log`、`.jazzy-check/smoke-result.json`；停止时的 SIGINT/KeyboardInterrupt 是测试主动关闭进程。
- 待完成：旧 RViz 插件移植、辅助包最终复验、多机 UDP、完整 SO3 动力学及实机验证。
- 当前 SO3 ROS 2 模拟器仅发布固定状态；单机 demo 使用理想位置跟踪器。

- 2026-11-18 文档同步：更新根目录、Primitive_env、Primitive-Planner 三份 README，统一 ROS 2 启动与触发命令、构建路径、地图复制行为和验证边界；原 ROS 1 用法明确标为历史记录。

- 2026-11-18：按用户要求为 single_drone.launch.py 增加默认 RViz2 窗口及 rviz:=false 开关；迁移 demo.rviz 到 ROS 2 插件、话题配置，新增 rviz2 运行依赖，同步三份 README。相关两包重新构建通过；启动验证确认 RViz 加载地图并成功初始化 OpenGL。清理 RViz 子进程中继承的 Snap 图形插件路径，解决 libpthread 符号冲突。

- 2026-11-19：核对 planner_manager.cpp、PPReplanFSM 和 traj_server 的实现及单机启动映射，重写 planner_manager.cpp 文件头，明确碰撞筛选、评分、速度坐标系、候选 ID 输出与下游调用关系；未改动算法。
- 本次注释修改验证：primitive_planner 在 /tmp 独立 Release 构建通过，目标文件差异检查通过；6 秒无 RViz 启动检查在初始化期间被 timeout 中断，关闭时出现 context invalid 异常，未据此认定运行测试通过。
- 2026-11-19：按用户反馈精简 planner_manager.cpp 文件头，仅保留职责、调用链、输入输出、坐标单位、运行模式和依赖。

- 2026-11-19：按已确认的简洁格式核对 90 个 C++/CUDA/Python/Shell 文件，本次重写其中 89 个文件头，保留 planner_manager.cpp 已确认说明；清理重复模板，保留许可证，跳过生成目录、自动生成消息及独立第三方库。Python AST、C++/CUDA 正文一致性和 Shell 语法检查通过，未修改执行逻辑。
- 本次基础运行验证：无 RViz 单机闭环收到 636 条里程计、17 帧点云、33 条心跳、32 条位置指令，触发后位移约 0.208 米；检查通过后主动 SIGINT 停止。批量构建已发现 swarm_bridge 原有 Duration::toSec() 编译错误，已记录在文件头和 README，未扩大注释任务范围修改实现。
- 本次构建结果：15 个包批量 Release 构建通过，primitive_planner 随后单独构建通过，共 16 个 ROS 2 包；swarm_bridge 因上述原有 API 错误失败，两个旧 RViz 插件未参与构建。产物和验证日志均位于 /tmp，未改动源码执行逻辑。

## 上游功能恢复（2026-11-19，已完成）

- 用户要求以 ZJU-FAST-Lab/Primitive-Planner 为基准恢复功能和调用，不修改 Primitive_env。
- 基准提交 cab5aa2f82948be58b36e37b6a921b9dba97423c；新增 ROS 2 swarm → run_in_sim → advanced_param/simulator 启动链，逐机隔离话题，恢复 UDP、随机目标、跟踪加速度字段、SO3 控制与完整动力学、RViz2 工具。
- 21 项上游算法体对照通过；保留非负路径 ID 修正并明确记录例外。说明见 Primitive-Planner/UPSTREAM_PARITY.md。
- 上一轮 /tmp 构建与日志在继续时已清理，验证改用 .parity-check/ 忽略目录；现已完成全量构建、组件和 4/20 机及原单机回归测试。

- 最终结果：18 包编译、21 项算法对照、6 项组件行为与正常退出、两个 RViz2 工厂实例化、5 种生成 XML 解析通过。
- 四机最大目标误差约 0.0961 米；20 机最大误差约 0.0964 米，全部 ID 基元收发齐全；原 YAML 单机误差约 0.0981 米，均正常停止。
- 退出异常已修复：循环复用执行器，进程结束前释放全局发布器/订阅器，防止晚于 DDS 上下文析构。复编和回归通过。
- `Primitive_env` 文件 SHA-256 校验与本轮开始时完全一致；本轮未修改该目录。详情见 Primitive-Planner/UPSTREAM_PARITY.md；本机结果见 .parity-check/verification-summary.json。
- 验证边界：未做 ROS 1/ROS 2 逐帧对跑、真实飞行或大规模长期稳定性测试；历史 TCP/MINCO 和 CUDA/EuRoC 不在上游默认演示链内。

- 2026-11-19：补齐根目录和 Primitive-Planner 两份 README 的 ROS 2 离线基元生成步骤，说明依赖、参数位置、清空输出目录、绘图、构建安装及 primitive_folder 加载方式；已有匹配完整库可复用。本次仅修改文档，未执行生成器或修改 Primitive_env。
- 2026-11-19：修复 swarm.launch.py 的 RViz2 配置：Marker Topic 改为 Topic，增加 Displays/Views/Tool Properties 面板、窗口布局及覆盖圆周的 Orbit 视角。primitive_planner 隔离构建通过；实际四机 RViz2 启动确认订阅地图及全部 12 个模型/目标/轨迹话题，探针收到非空地图及四机模型。检查在规划器仍加载基元时主动停止，退出出现 context invalid，未宣称本次完整规划或正常退出验证通过。Primitive_env 未修改。

## 结构对齐与清理（2026-11-20，已完成）

- 用户要求：以 ZJU-FAST-Lab/Primitive-Planner 为基准严格对齐目录结构、类/函数与调用关系，仅做 ROS 1 → ROS 2 Jazzy 转换；先不动 Primitive_env；同步 md；保持文件头注释。
- 确认的 4 项决定：①在现有代码基础上严格对齐 + 修复；②消息命名维持 ROS 2 CamelCase（SwarmPrimitiveTraj）；③沿用上游脚本名、内部换 colcon/ros2；④删除非上游文件。
- 删除：tests/、UPSTREAM_PARITY.md、single_drone.launch.py、swarm.launch.xml、quadrotor_simulator_ros2.cpp、build.catkin.bak/、devel.catkin.bak/、.catkin_workspace；gen_position_swap.py 去掉 swarm.launch.xml 的生成。
- 保留 ros2_launch_xml.py：它是 gen_primitive_planner_launch.py / gen_random_goal_flight.py / gen_run_odom_vis.py 共用的 ROS 1→ROS 2 launch XML 转换助手，属必要转换基础设施（曾误删，已从 git 恢复）。
- 编译：18 包 colcon build 全绿。swarm_bridge 仅编译 bridge_node_udp，与上游 ENABLE_TCP=false 一致，原 toSec 错误已不存在（bridge_node_tcp.cpp / traj2odom_node.cpp 仍为上游禁用分支，未编译）。
- 运行：8 机 swarm 冒烟测试（rviz:=false）验证 FSM 全链 INIT→WAIT_TARGET→GEN_NEW_TRAJ→EXEC_TRAJ→REPLAN→APPROACH_GOAL→WAIT_TARGET，6/8 机到达目标。drone_num≤3 时地图为空导致 pcl_render_node 段错误，属上游边界情况，非本次改动引入。
- 文档：同步根 README、Primitive-Planner/README 与 memory.md。

## 运行排障（2026-11-20）

- CycloneDDS 单机大机群需配 `MaxAutoParticipantIndex`（默认 9），否则 100+ 进程报 `Failed to find a free participant index`。已写 `~/.ros/cyclonedds.xml`（lo + 单播 + localhost + MaxAutoParticipantIndex=120，覆盖约 20 机）。启动前 `export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp CYCLONEDDS_URI=file://$HOME/.ros/cyclonedds.xml`。跨机运行时需删掉 NetworkInterfaceAddress/AllowMulticast/Peers 三行。
- **基元库重新生成有坑（已修复）**：toppra 0.6.x 对一维直线路径 `path(s,1)` 返回 1 元素数组而非标量，导致 swarm_path.py 里 `s_start=v_start/path(...,1)` 得到数组，`compute_trajectory` 内部 `K[self._N]=[sdmin**2,sdmax**2]` 抛 `setting an array element with a sequence`，直线基元（pathID=0）被误判不可行、pathID 整体 +1，规划器报 `Illegal pathID=180` 退出（exit 0 无 WAIT_TARGET）。已在 `solveHorizonToppAndSample` 直线分支加 `np.asarray(...).item()` 转标量修复，重新生成恢复 181 条、pathID 从 0 开始。正确库可随时从 `.parity-check/install/.../primitive_library` 恢复。
- FSM 的 `/traj_start_trigger` 订阅在 `init()` 读完基元库之后才创建，故 pub 触发前必须等启动终端出现 `WAIT_TARGET`。
