"""启动 ROS 2 单机运动基元仿真闭环。

输入：环境 YAML（米、world 坐标系）及运动基元库；输出：轨迹指令、模拟里程计和 RViz2 窗口。
调用关系：地图、局部感知、规划器、轨迹服务、理想位置跟踪模拟器。
模式：无 Gazebo 的算法仿真，使用系统时间；依赖 launch_ros、PyYAML、rviz2。
"""
import os
from pathlib import Path
import yaml
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def launch_nodes(context):
    """从同一地图配置读取起终点，连接 world 坐标系的单机话题。"""
    map_file = LaunchConfiguration('map_file').perform(context)
    primitive_folder = LaunchConfiguration('primitive_folder').perform(context)
    with open(map_file, encoding='utf-8') as stream:
        config = yaml.safe_load(stream)
    if config['map'].get('frame_id', 'world') != 'world':
        raise ValueError('This demo requires map.frame_id=world')
    start, goal = config['start'], config['goal']
    initial = {f'init_{axis}': float(start[axis]) for axis in 'xyz'}
    params = {
        'fsm/flight_type': 2, 'fsm/waypoint_num': 1,
        'fsm/realworld_experiment': True, 'fsm/thresh_replan_time': 0.2,
        'fsm/no_replan_thresh': 0.5, 'manager/primitiveFolder': primitive_folder,
        'manager/depthCloudStackNum': 1, 'manager/boxX': 6.0,
        'manager/boxY': 6.0, 'manager/boxZ': 6.0,
        'manager/arc_length': 3.0, 'manager/voxelSize': 0.1,
        'manager/sampleSize': 10000, 'manager/lamda_l': 200.0,
        'manager/lamda_b': 50.0, 'manager/drone_id': 0,
        'manager/max_vel': 1.0, 'manager/swarm_clearence': 0.24,
    }
    for axis in 'xyz':
        params[f'fsm/waypoint0_{axis}'] = float(goal[axis])
        params[f'manager/map_size_{axis}'] = float(config['map']['size'][axis])
    return [
        # Load a ROS 2 configuration with world-frame topics and the click tool.
        Node(package='rviz2', executable='rviz2', name='rviz2',
             arguments=['-d', str(Path(get_package_share_directory('primitive_env'))
                                  / 'rviz/demo.rviz')],
             # Snap editors can inject GTK/GIO paths incompatible with system RViz.
             additional_env={key: '' for key in ('GTK_PATH', 'GIO_MODULE_DIR',
                                                'QT_PLUGIN_PATH',
                                                'QT_QPA_PLATFORM_PLUGIN_PATH')
                             if '/snap/' in os.environ.get(key, '')},
             condition=IfCondition(LaunchConfiguration('rviz')), output='screen'),
        Node(package='primitive_env', executable='map_publisher.py',
             parameters=[{'map_file': map_file}]),
        Node(package='primitive_env', executable='waypoint_publisher.py',
             parameters=[{'map_file': map_file}]),
        Node(package='primitive_env', executable='rviz_click_trigger.py'),
        Node(package='poscmd_2_odom', executable='poscmd_2_odom',
             parameters=[initial], remappings=[('command', '/position_cmd'),
                                               ('odometry', '/state/odometry')]),
        Node(package='local_sensing_node', executable='pcl_render_node',
             parameters=[{'sensing_horizon': 5.0, 'sensing_rate': 10.0}],
             remappings=[('odometry', '/state/odometry'),
                         ('global_map', '/map_generator/global_cloud'),
                         ('local_map', '/map_generator/local_cloud'),
                         ('pcl_render_node/cloud', '/sensors/rgbd/points')]),
        Node(package='primitive_planner', executable='primitive_planner_node',
             output='screen', parameters=[params],
             remappings=[('odom_world', '/state/odometry'),
                         ('plan_manage/odom', '/state/odometry'),
                         ('plan_manage/cloud', '/sensors/rgbd/points')]),
        Node(package='primitive_planner', executable='traj_server', output='screen',
             parameters=[initial, {'primitiveFolder': primitive_folder, 'max_vel': 1.0,
                                   'time_forward': 1.0, 'time_delay_idx': 0}],
             remappings=[('odom', '/state/odometry'),
                         ('heartbeat', 'planning/heartbeat'),
                         ('stop_command', 'planning/stop_command')]),
    ]


def generate_launch_description():
    """声明可替换地图和基元库路径，允许从安装目录启动。"""
    return LaunchDescription([
        DeclareLaunchArgument('rviz', default_value='true',
                              description='Open RViz2 with the single-drone displays'),
        DeclareLaunchArgument('map_file', default_value=str(Path(
            get_package_share_directory('primitive_env')) / 'map/map_01.yaml')),
        DeclareLaunchArgument('primitive_folder', default_value=str(Path(
            get_package_share_directory('primitive_planner')) / 'primitive_library')),
        OpaqueFunction(function=launch_nodes),
    ])
