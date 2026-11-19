"""文件作用：按上游 bridge_udp.launch 创建 UDP 群体通信节点。
调用关系：离线生成的多机启动文件包含本入口。
输入：drone_id、broadcast_ip；里程计和网络参数沿用上游含义。
输出：转发的里程计、基元、目标及 Joy 话题。
运行模式：ROS 2；默认 odom_max_freq 为 70 Hz。
主要依赖：launch_ros、swarm_bridge。
"""
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('drone_id', default_value='0'),
        DeclareLaunchArgument('broadcast_ip', default_value='127.0.0.255'),
        Node(package='swarm_bridge', executable='bridge_node_udp', name='swarm_bridge',
             parameters=[{'drone_id': ParameterValue(LaunchConfiguration('drone_id'), value_type=int),
                          'broadcast_ip': LaunchConfiguration('broadcast_ip'), 'odom_max_freq': 70.0}],
             remappings=[('my_odom', '/vins_estimator/imu_propagate'), ('/goal_brig2plner', '/goal_with_id')]),
    ])
