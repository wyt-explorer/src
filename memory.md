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
