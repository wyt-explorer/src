"""文件作用：启动逐机跟踪器、局部感知和里程计显示，可选完整 SO3 动力学。
调用关系：run_in_sim.launch.py 包含本文件，对应上游 simulator.xml。
输入：初始位置、地图尺寸、速度与时钟配置；world 系米、秒制。
输出：逐机里程计、局部点云、可视化与可选 IMU。
运行模式：ROS 2；默认使用上游启用的理想位置跟踪，simulator:=so3 启用完整动力学。
主要依赖：poscmd_2_odom、local_sensing_node、odom_visualization、可选 so3_control。
"""
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def simulator_nodes(context):
    value = lambda name: LaunchConfiguration(name).perform(context)
    drone_id = int(value('drone_id'))
    prefix = f'/drone_{drone_id}_'
    odom = prefix + value('odom_topic')
    clock = {'use_sim_time': value('use_sim_time').lower() == 'true'}
    initial = {f'init_{axis}': float(value(f'init_{axis}')) for axis in 'xyz'}
    mode = value('simulator')
    if mode not in ('ideal', 'so3'):
        raise ValueError('simulator must be ideal or so3')
    if mode == 'ideal':
        nodes = [Node(package='poscmd_2_odom', executable='poscmd_2_odom',
                      name=f'drone_{drone_id}_poscmd_2_odom', parameters=[initial, clock],
                      remappings=[('command', prefix + 'planning/pos_cmd'), ('odometry', odom)])]
    else:
        state = {f'simulator/init_state_{a}': initial[f'init_{a}'] for a in 'xyz'}
        control_state = {f'so3_control/init_state_{a}': initial[f'init_{a}'] for a in 'xyz'}
        nodes = [
            Node(package='so3_quadrotor_simulator', executable='quadrotor_simulator_so3',
                 name=f'drone_{drone_id}_quadrotor_simulator_so3', parameters=[state, clock],
                 remappings=[('odom', odom), ('imu', prefix + 'imu'), ('cmd', prefix + 'so3_cmd'),
                             ('force_disturbance', prefix + 'force_disturbance'),
                             ('moment_disturbance', prefix + 'moment_disturbance')]),
            Node(package='so3_control', executable='so3_control_ros2', name=f'drone_{drone_id}_so3_control',
                 parameters=[control_state, clock, {'mass': 0.98, 'use_external_yaw': False,
                                  'gains/rot/x': 1.0, 'gains/rot/y': 1.0, 'gains/rot/z': 1.0,
                                  'gains/ang/x': 0.07, 'gains/ang/y': 0.07, 'gains/ang/z': 0.1}],
                 remappings=[('odom', odom), ('position_cmd', prefix + 'planning/pos_cmd'),
                             ('imu', prefix + 'imu'), ('motors', prefix + 'motors'),
                             ('corrections', prefix + 'corrections'), ('so3_cmd', prefix + 'so3_cmd')]),
        ]
    nodes += [
        Node(package='odom_visualization', executable='odom_visualization',
             name=f'drone_{drone_id}_odom_visualization', namespace=f'drone_{drone_id}_odom_visualization',
             parameters=[clock, {'color/a': 1.0, 'color/r': 0.0, 'color/g': 0.0, 'color/b': 0.0,
                                 'covariance_scale': 100.0, 'robot_scale': 0.24, 'tf45': False,
                                 'drone_id': drone_id}], remappings=[('odom', odom)]),
        Node(package='local_sensing_node', executable='pcl_render_node', name=f'drone_{drone_id}_pcl_render_node',
             parameters=[clock, {'sensing_horizon': 5.0, 'sensing_rate': 10.0, 'estimation_rate': 30.0,
                                 **{f'map/{a}_size': float(value(f'map_size_{a}')) for a in 'xyz'}}],
             remappings=[('global_map', '/map_generator/global_cloud'), ('odometry', odom),
                         ('pcl_render_node/cloud', prefix + 'pcl_render_node/cloud')]),
    ]
    return nodes


def generate_launch_description():
    defaults = {'drone_id': '0', 'init_x': '0.0', 'init_y': '0.0', 'init_z': '0.5',
                'map_size_x': '50.0', 'map_size_y': '50.0', 'map_size_z': '3.0',
                'odom_topic': 'visual_slam/odom', 'simulator': 'ideal', 'use_sim_time': 'false'}
    return LaunchDescription([DeclareLaunchArgument(k, default_value=v) for k, v in defaults.items()]
                             + [OpaqueFunction(function=simulator_nodes)])
