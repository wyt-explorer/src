"""文件作用：连接单架飞机的规划器、轨迹服务与仿真/感知节点。
调用关系：swarm.launch.py 逐机包含；再包含 advanced_param 与 simulator 两层配置。
输入：无人机 ID、起终点、地图尺寸、基元库；world 系米、秒制。
输出：相互隔离的逐机轨迹、控制、里程计和点云话题。
运行模式：ROS 2，对应上游 run_in_sim.xml 的节点调用链。
主要依赖：launch_ros、primitive_planner、仿真及感知包。
"""
from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, OpaqueFunction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def drone_nodes(context):
    value = lambda name: LaunchConfiguration(name).perform(context)
    keys = ['drone_id', 'max_vel', 'map_size_x', 'map_size_y', 'map_size_z', 'flight_type',
            'point_num', 'odom_topic', 'primitive_folder', 'use_sim_time']
    keys += [f'target{i}_{a}' for i in range(3) for a in 'xyz']
    args = {name: value(name) for name in keys}
    folder = Path(get_package_share_directory('primitive_planner')) / 'launch'
    drone_id = int(args['drone_id'])
    prefix = f'/drone_{drone_id}_'
    initial = {f'init_{a}': float(value(f'init_{a}')) for a in 'xyz'}
    remaps = [('odom', prefix + value('odom_topic')), ('heartbeat', prefix + 'traj_server/heartbeat'),
              ('stop_command', prefix + 'planning/stop_command'), ('/position_cmd', prefix + 'planning/pos_cmd')]
    remaps += [('planning/' + topic, prefix + 'planning/' + topic)
               for topic in ('selected_path_id', 'polynomial_traj', 'yaw_cmd', 'global_goal', 'select_path_end')]
    return [
        IncludeLaunchDescription(PythonLaunchDescriptionSource(str(folder / 'advanced_param.launch.py')),
                                 launch_arguments=args.items()),
        Node(package='primitive_planner', executable='traj_server', name=f'drone_{drone_id}_traj_server',
             parameters=[initial, {'traj_server/primitiveFolder': args['primitive_folder'],
                                  'traj_server/max_vel': float(args['max_vel']), 'traj_server/time_forward': 1.0,
                                  'traj_server/time_delay_idx': 0,
                                  'use_sim_time': args['use_sim_time'].lower() == 'true'}],
             remappings=remaps, output='screen'),
        IncludeLaunchDescription(PythonLaunchDescriptionSource(str(folder / 'simulator.launch.py')),
                                 launch_arguments={**args, **{k: value(k) for k in ('init_x', 'init_y', 'init_z', 'simulator')}}.items()),
    ]


def generate_launch_description():
    defaults = {'drone_id': '0', 'max_vel': '1.0', 'map_size_x': '50.0', 'map_size_y': '50.0',
                'map_size_z': '3.0', 'init_x': '0.0', 'init_y': '0.0', 'init_z': '0.5',
                'flight_type': '2', 'point_num': '1', 'odom_topic': 'visual_slam/odom',
                'simulator': 'ideal', 'use_sim_time': 'false',
                'primitive_folder': str(Path(get_package_share_directory('primitive_planner')) / 'primitive_library')}
    defaults.update({f'target{i}_{a}': '0.0' for i in range(3) for a in 'xyz'})
    return LaunchDescription([DeclareLaunchArgument(k, default_value=v) for k, v in defaults.items()]
                             + [OpaqueFunction(function=drone_nodes)])
