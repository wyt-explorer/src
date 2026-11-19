"""文件作用：恢复上游随机森林中圆周起点到对径目标的多机演示。
调用关系：创建地图和 UDP 桥，逐机包含 run_in_sim.launch.py，可启动 RViz2。
输入：drone_num、radius、obs_density、primitive_folder 等参数；米、秒制 world 坐标。
输出：隔离的逐机规划闭环、公共地图、群体基元通信及 RViz2 显示配置。
运行模式：ROS 2；不依赖 Primitive_env，默认沿用上游理想跟踪器。
主要依赖：launch_ros、map_generator、swarm_bridge、primitive_planner、RViz2。
"""
import math
from pathlib import Path
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, OpaqueFunction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def swarm_nodes(context):
    """保留 gen_position_swap.py 的圆周布局、地图尺度、目标及参数。"""
    value = lambda name: LaunchConfiguration(name).perform(context)
    count = int(value('drone_num'))
    if not 1 <= count <= 1000:
        raise ValueError('drone_num must be between 1 and 1000')
    radius = float(value('radius'))
    if radius <= 0:
        radius = count / 2.0 / math.pi * 2.0
    # 上游布局为小机群时可能产生负宽度；只在无有效森林区域时生成空地图。
    width = max(0.0, (radius - 1.0) / 1.414 * 2.0)
    density = float(value('obs_density'))
    obstacles = max(0, int(width * width * density / 2.0))
    extent = (radius + 10.0) * 2.0
    sim_time = value('use_sim_time').lower() == 'true'
    folder = Path(get_package_share_directory('primitive_planner')) / 'launch'
    nodes = [
        Node(package='swarm_bridge', executable='bridge_node_udp', name='swarm_bridge',
             parameters=[{'drone_id': 0, 'broadcast_ip': value('broadcast_ip'),
                          'odom_max_freq': 70.0, 'port': int(value('udp_port')), 'use_sim_time': sim_time}],
             remappings=[('/goal_brig2plner', '/goal_with_id')], output='screen'),
        Node(package='map_generator', executable='random_forest', name='random_forest',
             parameters=[{'map/x_size': max(width, 0.1), 'map/y_size': max(width, 0.1), 'map/z_size': 3.0,
                          'map/resolution': 0.1, 'map/obs_num': obstacles, 'map/circle_num': obstacles,
                          'ObstacleShape/lower_rad': 0.2, 'ObstacleShape/upper_rad': 0.5,
                          'ObstacleShape/lower_hei': 0.0, 'ObstacleShape/upper_hei': 3.0,
                          'ObstacleShape/radius_l': 0.7, 'ObstacleShape/radius_h': 0.5,
                          'ObstacleShape/z_l': 0.7, 'ObstacleShape/z_h': 0.8,
                          'ObstacleShape/theta': 0.5, 'pub_rate': 1.0, 'min_distance': 0.8,
                          'use_sim_time': sim_time}]),
    ]
    for i in range(count):
        angle = i * 2.0 * math.pi / count
        x, y = math.cos(angle) * radius, math.sin(angle) * radius
        args = {'drone_id': str(i), 'max_vel': value('max_vel'), 'map_size_x': str(extent),
                'map_size_y': str(extent), 'map_size_z': '3.0', 'init_x': str(x), 'init_y': str(y),
                'init_z': '0.5', 'target0_x': str(-x), 'target0_y': str(-y), 'target0_z': '0.5',
                'odom_topic': 'visual_slam/odom', 'primitive_folder': value('primitive_folder'),
                'use_sim_time': value('use_sim_time'), 'simulator': value('simulator')}
        nodes.append(IncludeLaunchDescription(PythonLaunchDescriptionSource(str(folder / 'run_in_sim.launch.py')),
                                               launch_arguments=args.items()))
    if value('rviz').lower() == 'true':
        import os
        import tempfile
        import yaml
        displays = [
            {'Class': 'rviz_default_plugins/Grid', 'Name': 'Grid', 'Enabled': True, 'Cell Size': 1.0},
            {'Class': 'rviz_default_plugins/PointCloud2', 'Name': 'Obstacles', 'Enabled': True,
             'Topic': {'Value': '/map_generator/global_cloud', 'Reliability Policy': 'Reliable'},
             'Style': 'Points', 'Size (Pixels)': 3, 'Color Transformer': 'AxisColor'},
        ]
        for i in range(count):
            for title, topic in [('Drone', f'/drone_{i}_odom_visualization/robot'),
                                 ('Goal', f'/drone_{i}_primitive_planner_node/goal_point'),
                                 ('Trajectory', f'/drone_{i}_primitive_planner_node/optimal_list')]:
                displays.append({'Class': 'rviz_default_plugins/Marker', 'Name': f'{title} {i}',
                                 'Enabled': True, 'Topic': {'Value': topic}})
        # 使用 RViz2 属性名，并使初始视角覆盖圆周起点及森林地图。
        config = {'Panels': [{'Class': 'rviz_common/Displays', 'Name': 'Displays'},
                             {'Class': 'rviz_common/Views', 'Name': 'Views'},
                             {'Class': 'rviz_common/Tool Properties', 'Name': 'Tool Properties'}],
                  'Window Geometry': {'Width': 1280, 'Height': 800,
                                      'Hide Left Dock': False, 'Hide Right Dock': False},
                  'Visualization Manager': {'Enabled': True,
                  'Global Options': {'Fixed Frame': 'world', 'Background Color': '48; 48; 48'},
                  'Displays': displays,
                  'Views': {'Current': {'Class': 'rviz_default_plugins/Orbit',
                                        'Distance': max(15.0, radius * 3.0),
                                        'Focal Point': {'X': 0.0, 'Y': 0.0, 'Z': 0.5},
                                        'Pitch': 0.8, 'Yaw': 0.8}},
                  'Tools': [{'Class': 'rviz_default_plugins/Interact'}, {'Class': 'rviz_default_plugins/MoveCamera'},
                            {'Class': 'rviz_default_plugins/Select'}, {'Class': 'rviz_default_plugins/PublishPoint'},
                            {'Class': 'rviz_plugins/Goal3DTool'},
                            {'Class': 'selected_points_publisher/SelectedPointsPublisher'}]}}
        with tempfile.NamedTemporaryFile(mode='w', suffix='.rviz', prefix='primitive-swarm-', delete=False) as stream:
            yaml.safe_dump(config, stream)
            config_path = stream.name
        nodes.append(Node(package='rviz2', executable='rviz2', name='rviz2', arguments=['-d', config_path],
                          additional_env={k: '' for k in ('GTK_PATH', 'GIO_MODULE_DIR', 'QT_PLUGIN_PATH',
                                                        'QT_QPA_PLATFORM_PLUGIN_PATH') if '/snap/' in os.environ.get(k, '')}))
    return nodes


def generate_launch_description():
    defaults = {'drone_num': '20', 'radius': '0.0', 'obs_density': '1.0', 'max_vel': '1.0',
                'broadcast_ip': '127.0.0.255', 'udp_port': '8081', 'simulator': 'ideal', 'rviz': 'true', 'use_sim_time': 'false',
                'primitive_folder': str(Path(get_package_share_directory('primitive_planner')) / 'primitive_library')}
    return LaunchDescription([DeclareLaunchArgument(k, default_value=v) for k, v in defaults.items()]
                             + [OpaqueFunction(function=swarm_nodes)])
