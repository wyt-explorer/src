#!/usr/bin/env python3
"""文件作用：生成上游圆周位置交换演示的 ROS 2 启动入口。
调用关系：run_swarm.sh 或命令行调用，入口转交 swarm.launch.py 生成逐机节点。
输入：命令行无人机数量；半径按上游 N/π 米计算。
输出：plan_manage/launch/swarm.launch（ROS 2 XML 入口，转交 swarm.launch.py）。
运行模式：离线 Python 3，不启动 ROS 节点或修改 Primitive_env。
主要依赖：pathlib、argparse、math。
"""
import argparse
import math
from pathlib import Path


def generate_launch_file(start_num, end_num, total_num, radius, obs_density, output_file):
    """兼容上游生成函数；完整机群使用相同 ID、半径和障碍密度。"""
    if start_num != 0 or end_num != total_num - 1:
        raise ValueError('start_num/end_num must cover the complete swarm')
    if not 1 <= total_num <= 1000 or radius <= 0 or obs_density < 0:
        raise ValueError('invalid swarm layout')
    content = f'''<launch>
  <arg name="rviz" default="true"/>
  <include file="$(find-pkg-share primitive_planner)/launch/swarm.launch.py">
    <arg name="drone_num" value="{total_num}"/>
    <arg name="radius" value="{radius}"/>
    <arg name="obs_density" value="{obs_density}"/>
    <arg name="rviz" value="$(var rviz)"/>
  </include>
</launch>
'''
    output = Path(output_file)
    output.write_text(content)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('drone_num', nargs='?', type=int, default=20)
    source_output = Path(__file__).resolve().parents[1] / 'planner/plan_manage/launch/swarm.launch'
    if not source_output.parent.is_dir():
        from ament_index_python.packages import get_package_share_directory
        source_output = Path(get_package_share_directory('primitive_planner')) / 'launch/swarm.launch'
    parser.add_argument('--output', type=Path, default=source_output)
    args = parser.parse_args()
    generate_launch_file(0, args.drone_num - 1, args.drone_num, args.drone_num / math.pi, 1.0, args.output)


if __name__ == '__main__':
    main()
