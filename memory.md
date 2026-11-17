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

- [ ] 创建并完善 `README.md`。
- [ ] 检查并确定 Primitive-Planner 的实际输入话题和消息类型。
- [ ] 创建雷达仿真适配节点。
- [ ] 创建真实雷达适配节点。
- [ ] 完成 `radar_link`、`camera_link` 和 `base_link` 的 TF 配置。
- [ ] 建立统一烟雾浓度场和传感器退化模型。
- [ ] 完成 Gazebo 仿真启动文件。
- [ ] 完成 rosbag 回放测试。
- [ ] 完成真实传感器静态测试和实机迁移测试。

## 更新记录

- 2026-11-17：新增项目开发规则、统一接口要求、代码注释要求和 README 同步要求。
