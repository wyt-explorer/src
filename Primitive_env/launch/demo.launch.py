"""文件作用：启动地图、起终点标记和 RViz 点击触发转换节点。
调用关系：ros2 launch primitive_env demo.launch.py 加载本文件并创建三个节点。
输入：map_file 参数，默认 map_01.yaml。
输出：LaunchDescription；启动全局地图、标记与 /traj_start_trigger 发布链路。
运行模式：ROS 2 地图演示，不启动规划器和轨迹执行闭环。
主要依赖：launch、launch_ros、ament_index_python、primitive_env。
"""
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
