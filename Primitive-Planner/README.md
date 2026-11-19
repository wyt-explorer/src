# Primitive_Planner — ROS 2 Jazzy

本地实现以 [ZJU-FAST-Lab 上游 cab5aa2](https://github.com/ZJU-FAST-Lab/Primitive-Planner/tree/cab5aa2f82948be58b36e37b6a921b9dba97423c) 为基准，保留规划算法和节点分工，恢复圆周对径交换的多机演示。调用映射、适配说明和验证见 [UPSTREAM_PARITY.md](UPSTREAM_PARITY.md)。本次不修改 `Primitive_env`。

从工作空间根目录编译：

```bash
source /opt/ros/jazzy/setup.bash
colcon build --base-paths Primitive-Planner/src --cmake-args -DCMAKE_BUILD_TYPE=Release
source install/setup.bash
ros2 launch primitive_planner swarm.launch.py drone_num:=20
```

等待各机 `WAIT_TARGET` 后，在加载相同环境的终端触发：

```bash
ros2 topic pub --once /traj_start_trigger geometry_msgs/msg/PoseStamped '{header: {frame_id: world}, pose: {orientation: {w: 1.0}}}'
```

默认使用上游的理想位置跟踪器；`simulator:=so3` 选择完整 SO3 控制/动力学。`rviz:=false` 关闭 RViz2。保留 `single_drone.launch.py` 以使用现有 YAML 地图；详见[根目录 README](../README.md)。

下面保留论文及上游 ROS 1 历史说明。当前 ROS 2 使用 `.launch.py`/`.launch.xml` 入口，不使用 `catkin_make`、`roslaunch` 或 `devel/setup.bash`。

## Paper

Primitive-Swarm: An Ultra-lightweight and Scalable Planner for Large-scale Aerial Swarms, Accepted by IEEE Transactions on Robotics 2025. [[IEEE TRO]](https://ieeexplore.ieee.org/document/11015263) [[Arxiv]](https://arxiv.org/abs/2502.16887) [[Video]](https://www.bilibili.com/video/BV1Q4PNeeE7D?spm_id_from=333.788.videopod.episodes&vd_source=acac17792f24970ff76c5eae9ba38289&p=2)

Author list: Jialiang Hou, Xin Zhou, Neng Pan, Ang Li, Yuxiang Guan, Chao Xu, Zhongxue Gan, Fei Gao

```
@ARTICLE{Hou2025PrimitiveSwarm,
  author={Hou, Jialiang and Zhou, Xin and Pan, Neng and Li, Ang and Guan, Yuxiang and Xu, Chao and Gan, Zhongxue and Gao, Fei},
  journal={IEEE Transactions on Robotics}, 
  title={Primitive-Swarm: An Ultra-Lightweight and Scalable Planner for Large-Scale Aerial Swarms}, 
  year={2025},
  volume={41},
  number={},
  pages={3629-3648},
  keywords={Robots;Trajectory;Collision avoidance;Computational efficiency;Scalability;Planning;Libraries;Robot sensing systems;Drones;Heuristic algorithms;Autonomous aerial vehicles;collision avoidance;motion planning;swarm robotics;trajectory optimization},
  doi={10.1109/TRO.2025.3573667}}

```


## System Overview
<p align = "center">
<img src="misc/system overview.png" />
</p>


## 原项目 ROS 1 环境要求（历史记录）

The code is tested on clean Ubuntu 20.04 with ROS noetic installation.

Install the required package toppra:
```
sudo pip3 install toppra catkin_pkg PyYAML empy matplotlib pyrfc3339
```

## 原项目 ROS 1 使用方式（历史记录）

### 1. Download and compile the code

```
git clone https://github.com/ZJU-FAST-Lab/Primitive-Planner.git
cd Primitive-Planner
catkin_make -DCMAKE_BUILD_TYPE=Release
```

### 2. Generate the motion primitive library

```
cd src/scripts
python3 swarm_path.py
```
The generated motion primitive library is stored in "src/planner/plan_manage/primitive_library/".

### 3. Run the planner

In the "Primitive-Planner/" directory:
```
source devel/setup.bash # or setup.zsh
cd src/scripts
python3 gen_position_swap.py 20 # It will generate the swarm.launch with 20 drones
roslaunch primitive_planner swarm.launch
```

Wait until all the nodes have their launch process finished and keep printing "[FSM]: state: WAIT_TARGET. Waiting for trigger".

Open another terminal, publish the trigger
```
cd src/scripts
bash pub_trigger.sh
```
Then the drones (drone number is 40) will start to fly like this
<p align = "center">
<img src="misc/40_drone.gif" width = "500" height = "347" border="5" />
</p>

Change the drone number when executing "python3 gen_position_swap.py <drone_number>". 

Before starting the "roslaunch" command, please open another terminal and run "htop" to monitor the Memory usage and CPU usage. Each drone requires about 200 MB memory. Keep the htop opened for entire flight.

The computation time is printed on the terminal in blue text, named as "plan_time". 
To get the accurate computation time, please fix the CPU frequency to its maximum by
```
sudo apt install cpufrequtils
sudo cpufreq-set -g performance
```
Otherwise the CPU will run in powersave mode with low frequency.

## Projects Supported by Primitive-Swarm

- **DPNet: Efficient Dead-End Prediction and Avoidance for Vision-Based UAV Navigation**: DPNet combines dead-end prediction with a compact primitive library to proactively prune unsafe trajectories. [[Paper]](https://arxiv.org/abs/2608.16640)

 <p align="center">
    <a href="https://arxiv.org/abs/2608.16640" target="_blank">
      <img src="misc/dpnet_system_overview.png" width="600" alt="DPNet system overview" />
    </a>
</p>

- **Neural-Primitive: An Efficient End-to-end Local Planner with Primitive-based Imitation Learning for Autonomous Flight**: It uses motion primitives as the structured representation for imitation learning, enabling end-to-end local planning for autonomous flight. [[Project]](https://zhitaoliu.github.io/neural-primitive/)

 <p align="center">
    <a href="https://zhitaoliu.github.io/neural-primitive/" target="_blank">
      <img src="misc/neural_primitive_system_overview.svg" width="600" alt="Neural-Primitive system
      overview" />
    </a>
</p>
