#!/usr/bin/env python3
"""文件作用：回归现有 YAML 单机入口，确认上游功能恢复未破坏其规划闭环。
调用关系：启动 single_drone.launch.py，观测状态、点云、心跳和指令并发送触发。
输入：已安装 primitive_env 的默认地图；只读取，不修改环境源码或配置。
输出：JSON 结果和启动日志，检查目标误差小于 0.1 米及正常停止。
运行模式：ROS 2 无 RViz 集成测试，建议独立 ROS_DOMAIN_ID。
主要依赖：rclpy、ament_index_python、PyYAML、ROS 消息包、subprocess。
"""
import argparse
import json
import math
import os
from pathlib import Path
import re
import signal
import subprocess
import time
import yaml
import rclpy
from rclpy.node import Node
from ament_index_python.packages import get_package_share_directory
from nav_msgs.msg import Odometry
from sensor_msgs.msg import PointCloud2
from std_msgs.msg import Empty
from geometry_msgs.msg import PoseStamped
from quadrotor_msgs.msg import PositionCommand


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=Path('/tmp/primitive-single.json'))
    args = parser.parse_args()
    config = yaml.safe_load((Path(get_package_share_directory('primitive_env'))/'map/map_01.yaml').read_text())
    goal = tuple(float(config['goal'][a]) for a in 'xyz')
    counts = dict(odom=0, cloud=0, heartbeat=0, command=0)
    position = None
    rclpy.init()
    node = Node('single_parity_probe')
    def odom(message):
        nonlocal position
        counts['odom'] += 1
        p = message.pose.pose.position
        position = (p.x, p.y, p.z)
    subscriptions = [node.create_subscription(Odometry, '/state/odometry', odom, 10)]
    for key, topic, typ in [('cloud', '/sensors/rgbd/points', PointCloud2), ('heartbeat', '/planning/heartbeat', Empty),
                            ('command', '/position_cmd', PositionCommand)]:
        def callback(message, key=key):
            counts[key] += 1
        subscriptions.append(node.create_subscription(typ, topic, callback, 10))
    trigger = node.create_publisher(PoseStamped, '/traj_start_trigger', 10)
    stream = args.output.with_suffix('.log').open('w')
    process = subprocess.Popen(['ros2', 'launch', 'primitive_planner', 'single_drone.launch.py', 'rviz:=false'],
                               stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
    start, triggered = time.monotonic(), False
    try:
        while time.monotonic()-start < 65:
            rclpy.spin_once(node, timeout_sec=.05)
            if not triggered and counts['odom'] and counts['cloud'] and counts['heartbeat'] > 5:
                message = PoseStamped()
                message.header.frame_id = 'world'
                message.header.stamp = node.get_clock().now().to_msg()
                message.pose.orientation.w = 1.
                trigger.publish(message)
                triggered = True
            if triggered and counts['command'] and position and math.dist(position, goal) < .1:
                break
            if process.poll() is not None:
                break
        error = math.dist(position, goal) if position else None
        result = dict(passed=bool(all(counts.values()) and error is not None and error < .1), counts=counts,
                      goal_error_m=error, elapsed_s=time.monotonic()-start, trigger_sent=triggered)
    finally:
        node.destroy_node()
        rclpy.shutdown()
        if process.poll() is None:
            process.send_signal(signal.SIGINT)
            try:
                process.wait(timeout=10)
            except subprocess.TimeoutExpired:
                os.killpg(process.pid, signal.SIGTERM)
                process.wait(timeout=5)
        stream.close()
    result['shutdown_clean'] = not bool(re.search(r'terminate called|exit code -(?:6|11)\b|Failed to delete datawriter|failed to create guard condition', args.output.with_suffix('.log').read_text()))
    result['passed'] = result['passed'] and result['shutdown_clean']
    args.output.write_text(json.dumps(result, indent=2))
    print(json.dumps(result), flush=True)
    if not result['passed']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
