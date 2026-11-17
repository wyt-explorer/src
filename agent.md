# 项目开发规则

## 1. 每次编码前先阅读

开始修改或生成代码前，必须先阅读：

1. `agent.md`
2. `memory.md`
3. `README.md`
4. 与本次修改直接相关的源文件、配置文件和启动文件

阅读后再决定修改位置，避免重复实现、破坏已有接口或遗漏未完成工作。

## 2. README 更新顺序

项目根目录必须有 `README.md`，内容固定按以下顺序组织：

1. 编译和启动指令
2. 系统架构
3. 消息流和统一接口
4. 代码功能说明
5. 目录结构
6. 仿真与实机迁移
7. 当前状态、限制和更新记录

每次整体架构、节点、话题、依赖、启动方式或目录结构发生变化，都必须同步更新 `README.md`。局部算法修改如果改变了代码功能，也要更新对应的代码功能说明。

## 3. 仿真与实机统一接口

规划器不直接依赖 Gazebo、RadaRays、ARS548 或具体厂商 SDK。仿真和实机通过适配节点输出统一 ROS 2 接口。

```text
仿真：Gazebo/RadaRays → 仿真适配节点 → 统一话题 → Primitive-Planner
实机：真实传感器驱动 → 实机适配节点 → 统一话题 → Primitive-Planner
```

建议统一话题：

| 话题 | 类型 | 作用 |
|---|---|---|
| `/sensors/radar/points` | `sensor_msgs/msg/PointCloud2` | 雷达点云 |
| `/sensors/radar/detections` | `RadarDetectionArray` | 距离、角度、速度、RCS 等检测结果 |
| `/sensors/rgbd/depth` | `sensor_msgs/msg/Image` | 深度图 |
| `/sensors/rgbd/points` | `sensor_msgs/msg/PointCloud2` | RGB-D 点云 |
| `/state/odometry` | `nav_msgs/msg/Odometry` | 位姿和速度 |
| `/state/imu` | `sensor_msgs/msg/Imu` | IMU 数据 |

消息必须明确单位、坐标系、时间戳和字段含义。规划器输入接口应保持稳定，数据源差异放在适配层处理。

## 4. 坐标系和时间

统一使用 TF2，至少维护：

```text
map → odom → base_link
                    ├── radar_link
                    ├── camera_link
                    └── imu_link
```

必须检查：

- 坐标轴方向和右手坐标系。
- 雷达、相机和机体之间的外参。
- 消息 `header.stamp` 和 `frame_id`。
- 仿真使用 `/clock` 和 `use_sim_time=true`。
- 实机使用设备时间或系统时间和 `use_sim_time=false`。

不能在规划器中散落硬编码坐标变换。

## 5. 代码注释要求

每次新建或修改源文件，文件开头必须说明：

- 文件作用。
- 调用关系。
- 输入。
- 输出。
- 运行模式。
- 主要依赖。

每个关键类、函数、回调、定时器、服务、数据转换和算法步骤必须有注释，说明任务、参数、单位、坐标系、返回值和失败条件。

C++ 使用文件级注释和 Doxygen 注释。Python 使用模块级 docstring 和函数 docstring。

注释应解释数据流和设计原因，不能只重复代码表面含义。简单赋值不需要逐行注释。

推荐文件头：

```cpp
/*
 * 文件作用：将雷达检测转换为规划器使用的统一点云。
 * 调用关系：由雷达适配节点启动，输出 /sensors/radar/points。
 * 输入：雷达检测、TF、参数文件。
 * 输出：sensor_msgs/msg/PointCloud2。
 * 运行模式：simulation / real。
 * 依赖：ROS 2、tf2、sensor_msgs。
 */
```

## 6. 修改代码时的工程要求

- 优先复用现有类、函数、消息和参数。
- 不把厂商 SDK 或 Gazebo 专用消息写进核心规划器。
- 不用大量 `if simulation` 分支污染算法代码。
- 所有传感器参数通过 YAML 或 ROS 2 参数配置。
- 安全处理空数据、NaN、无穷值、超时、断连和 TF 查询失败。
- 不改变已有接口语义，除非任务明确要求。
- 新增依赖必须同步修改 `package.xml`、`CMakeLists.txt` 和 README。
- 不提交 `build/`、`install/`、`log/` 等生成目录。
- 修改后至少编译受影响的 ROS 2 包并进行基础运行测试。

## 7. 烟雾与传感器模型

Gazebo 粒子效果主要用于可视化，不能直接当作定量浓度。需要维护统一烟雾场 `C(x,y,z,t)`，由 RGB-D 和毫米波雷达分别使用各自的退化模型。

不能预先把 RGB-D 固定为完全失效、毫米波雷达固定为理想状态。噪声、漏检、衰减和虚警参数必须有实验依据、公开数据或明确记录的假设。

## 8. 实机迁移

实机迁移是替换传感器数据源，不是把 Gazebo 安装到无人机上：

```text
RadaRays → radar_sim_adapter → /sensors/radar/points
ARS548/TI 驱动 → radar_real_adapter → /sensors/radar/points
```

迁移顺序：仿真 → 真实传感器静态测试 → 手动移动测试 → 系留/低风险飞行 → 自主飞行。

必须先完成消息接口、TF、时间同步、外参标定、rosbag 回放和失效处理测试。

## 9. 交付前检查

- [ ] 已阅读 `agent.md`、`memory.md`、`README.md` 和相关源码。
- [ ] 文件头和关键函数注释完整。
- [ ] 输入、输出、单位、坐标系和时间戳明确。
- [ ] 仿真和实机接口一致。
- [ ] README 已按规定顺序同步。
- [ ] memory.md 已记录完成项和待部署项。
- [ ] 受影响的包已编译并完成基础测试。
