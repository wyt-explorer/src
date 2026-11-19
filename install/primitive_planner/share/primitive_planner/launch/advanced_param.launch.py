"""文件作用：按上游 advanced_param.xml 配置规划状态机和基元管理器。
调用关系：run_in_sim.launch.py 包含本文件，创建每架飞机的规划节点。
输入：无人机 ID、地图尺寸、目标、速度上限及基元库路径；world 系米、秒制。
输出：隔离的逐机规划话题，以及公共基元广播和触发接口。
运行模式：ROS 2，保留上游规划权重、体素参数和重规划阈值。
主要依赖：launch_ros、primitive_planner。
"""
from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def planner_node(context):
    """将上游私有话题映射为明确的逐机话题，避免多机共享位置指令。"""
    value = lambda name: LaunchConfiguration(name).perform(context)
    drone_id = int(value('drone_id'))
    prefix = f'/drone_{drone_id}_'
    odom = prefix + value('odom_topic')
    params = {
        'use_sim_time': value('use_sim_time').lower() == 'true',
        'fsm/thresh_replan_time': 0.2, 'fsm/realworld_experiment': True,
        'fsm/fail_safe': True, 'fsm/no_replan_thresh': 0.5,
        'fsm/flight_type': int(value('flight_type')), 'fsm/waypoint_num': int(value('point_num')),
        'manager/primitiveFolder': value('primitive_folder'),
        'manager/depthCloudStackNum': 1, 'manager/boxX': 6.0, 'manager/boxY': 6.0,
        'manager/boxZ': 6.0, 'manager/arc_length': 3.0, 'manager/voxelSize': 0.1,
        'manager/sampleSize': 10000, 'manager/lamda_l': 200.0, 'manager/lamda_b': 50.0,
        'manager/drone_id': drone_id, 'manager/max_vel': float(value('max_vel')),
        'manager/swarm_clearence': 0.24,
    }
    for axis in 'xyz':
        params[f'manager/map_size_{axis}'] = float(value(f'map_size_{axis}'))
        for i in range(3):
            params[f'fsm/waypoint{i}_{axis}'] = float(value(f'target{i}_{axis}'))
    remaps = [
        ('odom_world', odom), ('plan_manage/odom', odom),
        ('plan_manage/cloud', prefix + 'pcl_render_node/cloud'),
        ('mandatory_stop', '/mandatory_stop_to_planner'),
        ('/position_cmd', prefix + 'planning/pos_cmd'),
        ('planning/broadcast_primitive_send', '/broadcast_primitive_from_planner'),
        ('planning/broadcast_primitive_recv', '/broadcast_primitive_to_planner'),
        ('planning/heartbeat', prefix + 'traj_server/heartbeat'),
    ]
    for topic in ('selected_path_id', 'polynomial_traj', 'yaw_cmd', 'stop_command', 'global_goal', 'select_path_end'):
        remaps.append(('planning/' + topic, prefix + 'planning/' + topic))
    for topic in ('goal_point', 'global_list', 'init_list', 'optimal_list', 'failed_list', 'a_star_list'):
        remaps.append((topic, f'/drone_{drone_id}_primitive_planner_node/' + topic))
    return [Node(package='primitive_planner', executable='primitive_planner_node',
                 name=f'drone_{drone_id}_primitive_planner_node', parameters=[params],
                 remappings=remaps, output='screen')]


def generate_launch_description():
    defaults = {
        'drone_id': '0', 'max_vel': '1.0', 'map_size_x': '50.0', 'map_size_y': '50.0',
        'map_size_z': '3.0', 'flight_type': '2', 'point_num': '1',
        'odom_topic': 'visual_slam/odom', 'use_sim_time': 'false',
        'primitive_folder': str(Path(get_package_share_directory('primitive_planner')) / 'primitive_library'),
    }
    defaults.update({f'target{i}_{axis}': '0.0' for i in range(3) for axis in 'xyz'})
    return LaunchDescription([DeclareLaunchArgument(k, default_value=v) for k, v in defaults.items()]
                             + [OpaqueFunction(function=planner_node)])
