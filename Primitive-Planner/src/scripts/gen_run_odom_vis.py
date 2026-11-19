"""文件作用：生成地图与多机里程计显示的启动文件。
调用关系：运行脚本直接调用 generate_launch_file()。
输入：num_drones 及脚本内地图和显示参数。
输出：run_odom_vis.launch，将每机里程计映射到独立显示节点。
运行模式：离线 Python 工具，输出 ROS 2 launch XML。
主要依赖：Python 文件接口、旧 odom_visualization/map_generator 配置。
"""
from ros2_launch_xml import convert_file

def generate_launch_file(num_drones, output_file='run_odom_vis.launch'):
    with open(output_file, 'w') as file:
        file.write('<launch>\n\n')
        
        # 写入map_generator的配置
        file.write('    <!-- map -->\n')
        file.write('    <node pkg="map_generator" name="random_forest" type="random_forest" output="screen">\n')
        file.write('        <param name="map/x_size" value="5" />\n')
        file.write('        <param name="map/y_size" value="5" />\n')
        file.write('        <param name="map/z_size" value="3" />\n')
        file.write('        <param name="map/resolution" value="0.1"/>\n')
        file.write('        <param name="seed" value="4"/>\n')
        file.write('        <param name="map/obs_num" value="16"/>\n')
        file.write('        <param name="ObstacleShape/lower_rad" value="0.2"/>\n')
        file.write('        <param name="ObstacleShape/upper_rad" value="0.6"/>\n')
        file.write('        <param name="ObstacleShape/lower_hei" value="0.0"/>\n')
        file.write('        <param name="ObstacleShape/upper_hei" value="3.0"/>\n')
        file.write('        <param name="map/circle_num" value="16"/>\n')
        file.write('        <param name="ObstacleShape/radius_l" value="0.7"/>\n')
        file.write('        <param name="ObstacleShape/radius_h" value="0.5"/>\n')
        file.write('        <param name="ObstacleShape/z_l" value="0.7"/>\n')
        file.write('        <param name="ObstacleShape/z_h" value="0.8"/>\n')
        file.write('        <param name="ObstacleShape/theta" value="0.5"/>\n')
        file.write('        <param name="pub_rate" value="1.0"/>\n')
        file.write('        <param name="min_distance" value="1.3"/>\n')
        file.write('    </node>\n\n')
        
        file.write('    <include file="$(find odom_visualization)/launch/run_vis_rotate.launch"/>\n\n')
    
        
        # 写入每架飞机的启动配置
        for i in range(num_drones):
            file.write('    <!-- Drone {} -->\n'.format(i))
            file.write('    <node pkg="odom_visualization" name="drone_{}_odom_visualization" type="odom_visualization" output="screen">\n'.format(i))
            file.write('        <remap from="~odom" to="drone_{}_visual_slam/odom"/>\n'.format(i))
            file.write('        <param name="color/a" value="1.0"/>\n')
            file.write('        <param name="color/r" value="0.0"/>\n')
            file.write('        <param name="color/g" value="0.0"/>\n')
            file.write('        <param name="color/b" value="0.0"/>\n')
            file.write('        <param name="covariance_scale" value="100.0"/>\n')
            file.write('        <param name="robot_scale" value="0.24"/>\n')
            file.write('        <param name="tf45" value="false"/>\n')
            file.write('        <param name="drone_id" value="{}"/>\n'.format(i))
            file.write('    </node>\n')
        
        # 写入XML尾部
        file.write('</launch>')
    convert_file(output_file)

# 使用示例
num_drones = 20  # 假设我们需要启动5架飞机

generate_launch_file(num_drones)
