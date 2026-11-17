<!-- 介绍自定义仿真环境的启动方法、目录结构、ROS 话题及地图配置。 -->
# Primitive_env

基于 ROS 2 Jazzy 的自定义三维障碍物环境，复用 **Primitive-Planner** 的运动基元规划算法，在 RViz 中显示地图并通过鼠标点击触发单架无人机的规划与仿真。

## 1. 运行指令

以下命令使用当前工程路径 `/home/wyt/RC_planner`，ROS 功能包名为 **`primitive_env`**（注意全小写）。

### 1.1 编译与配置环境

首次配置功能包，或修改 `CMakeLists.txt`、C++ 源码等需要重新编译的文件时：

```bash
cd ~/RC_planner
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

以后每次打开新终端，运行 ROS 命令前先加载环境：

```bash
source ~/RCplanner/install/setup.bash
```

修改 `map_01.yaml`、普通 Python 脚本、`demo.launch` 或 `demo.rviz`，通常**不需要重新编译**；保存后重新启动相应节点即可。

### 1.2 启动地图、规划器与 RViz

```bash
source ~/RCplanner/install/setup.bash
ros2 launch primitive_env demo.launch.py
```

`demo.launch` 同时启动地图点云发布、起终点标记、Primitive-Planner 单机规划及仿真、RViz 和鼠标点击触发节点。

### 1.3 在 RViz 中触发规划飞行

1. 等待规划器进入 `WAIT_TARGET` 状态。
2. 在 RViz 工具栏选择 **Publish Point**；如果没有该工具，点击工具栏的 `+` 添加。
3. 确认工具的话题为 `/clicked_point`，然后在 RViz 地图中点击一次。
4. `rviz_click_trigger.py` 将点击转换为 `/traj_start_trigger` 消息，触发规划器开始执行当前固定目标任务。

**注意：点击位置仅用于触发，不会被当作新的目标点。** 起点和终点由 `demo.launch` 中的参数指定；`map_01.yaml` 中的起终点用于可视化，两处坐标应保持一致。

### 1.4 常用检查命令

```bash
# 查看功能包是否被 ROS 识别
ros2 pkg prefix primitive_env

# 查看规划器及仿真节点
ros2 node list | grep drone_0

# 检查全局障碍物点云
ros2 topic hz /map_generator/global_cloud

# 查看 RViz 点击事件与规划触发话题
ros2 topic info /clicked_point
ros2 topic info /traj_start_trigger

# 查看可用于轨迹或无人机显示的实际话题
ros2 topic list | grep -E 'odom|visualization|planning|pos_cmd'
```

结束运行：在启动 `ros2 launch` 的终端按 **Ctrl+C**。

## 2. 文件夹结构

```text
~/RC_planner/
├── src/
│   ├── Primitive-Planner/           # 原始运动基元规划项目
│   └── Primitive_env/              # 自定义环境功能包（包名 primitive_env）
│       ├── src/
│       │   ├── map_publisher.py
│       │   ├── waypoint_publisher.py
│       │   └── rviz_click_trigger.py
│       ├── map/
│       │   └── map_01.yaml
│       ├── launch/
│       │   └── demo.launch
│       ├── rviz/
│       │   └── demo.rviz
│       ├── CMakeLists.txt
│       ├── package.xml
│       └── README.md
├── build/
└── devel/
```

## 3. 文件功能说明

| 文件 | 功能 |
| --- | --- |
| `map/map_01.yaml` | 配置地图尺寸、点云分辨率、起终点及障碍物；支持长方体 `box`、圆柱体 `cylinder`、圆环 `ring`，并设置圆环朝向。 |
| `src/map_publisher.py` | 读取 YAML、生成三维障碍物点云，发布 `sensor_msgs/PointCloud2` 到 `/map_generator/global_cloud`。 |
| `src/waypoint_publisher.py` | 读取 YAML 中的起点与终点，发布 RViz Marker 进行标记显示。 |
| `src/rviz_click_trigger.py` | 订阅 RViz 的 `/clicked_point`，发布 `/traj_start_trigger` 启动一次固定目标规划任务。 |
| `launch/demo.launch` | 统一启动地图节点、起终点节点、Primitive-Planner 仿真与规划节点、点击触发节点及 RViz；设置无人机初始位置和目标。 |
| `rviz/demo.rviz` | 保存 RViz 的固定坐标系、网格、点云、起终点 Marker、工具栏等显示配置。 |
| `CMakeLists.txt` | ament_cmake 构建配置及 Python 节点安装配置。 |
| `package.xml` | ROS 功能包名称、版本和依赖声明。 |
| `README.md` | 项目使用指令和目录说明。 |

## 4. 主要 ROS 话题

| 话题 | 作用 |
| --- | --- |
| `/map_generator/global_cloud` | 自定义三维障碍物点云，供局部感知和 RViz 使用。 |
| `/clicked_point` | RViz `Publish Point` 工具发布的点击坐标。 |
| `/traj_start_trigger` | 触发 Primitive-Planner 开始当前规划任务。 |

无人机模型、里程计和轨迹可视化话题的名称以当前启动后的 `ros2 topic list` 为准。

## 5. 修改地图

直接编辑 `map/map_01.yaml` 中的 `obstacles`。例如：

```yaml
- type: ring
  position: [0.0, 0.0, 1.5]
  axis: y
  radius: 1.0
  tube_radius: 0.15
```

`axis: x / y / z` 分别表示圆环平面法向量沿 X / Y / Z 轴。地图脚本在启动时读取 YAML，因此修改后**重新启动 `ros2 launch primitive_env demo.launch.py`** 即可，不需要重新编译，也不需要重新生成运动基元库。
