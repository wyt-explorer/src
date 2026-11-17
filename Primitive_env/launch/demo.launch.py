from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    env = get_package_share_directory('primitive_env')
    map_file = LaunchConfiguration('map_file')
    return LaunchDescription([
        DeclareLaunchArgument('map_file', default_value=PathJoinSubstitution([env,'map','map_01.yaml'])),
        Node(package='primitive_env', executable='map_publisher.py', name='custom_map_publisher', output='screen', parameters=[{'map_file': map_file, 'cloud_topic':'/map_generator/global_cloud'}]),
        Node(package='primitive_env', executable='waypoint_publisher.py', name='demo_waypoint_markers', output='screen', parameters=[{'map_file': map_file}]),
        Node(package='primitive_env', executable='rviz_click_trigger.py', name='rviz_click_trigger', output='screen'),
    ])
