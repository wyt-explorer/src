#!/usr/bin/env python3
"""文件作用：验证上游圆周交换场景在 ROS 2 下的逐机接口、通信和目标到达。
调用关系：启动 swarm.launch.py，订阅每架飞机的里程计、点云、指令、心跳和基元广播。
输入：--drones、--radius、--timeout；独立 ROS_DOMAIN_ID 下运行，位置为米。
输出：JSON 结果与进程日志，检查结束后只停止本脚本创建的进程组。
运行模式：ROS 2 集成测试，关闭 RViz，不改动 Primitive_env。
主要依赖：rclpy、ROS 消息包、launch、Python subprocess。
"""
import argparse
import json
import math
import os
from pathlib import Path
import signal
import re
import subprocess
import time
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import PointCloud2
from std_msgs.msg import Empty
from quadrotor_msgs.msg import PositionCommand
from traj_utils.msg import SwarmPrimitiveTraj


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--drones', type=int, default=4)
    parser.add_argument('--radius', type=float, default=3.0)
    parser.add_argument('--timeout', type=float, default=80.0)
    parser.add_argument('--output', type=Path, default=Path('/tmp/primitive-swarm-smoke.json'))
    args = parser.parse_args()
    log = args.output.with_suffix('.log').open('w')
    process = subprocess.Popen(['ros2', 'launch', 'primitive_planner', 'swarm.launch.py',
                                f'drone_num:={args.drones}', f'radius:={args.radius}', f'udp_port:={23800 + args.drones}', 'rviz:=false'],
                               stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
    rclpy.init()
    node = Node('swarm_parity_probe')
    counts = [{k: 0 for k in ('odom', 'cloud', 'command', 'heartbeat', 'primitive')} for _ in range(args.drones)]
    positions, initial, goals, subscriptions = {}, {}, {}, []
    observed_ids = set()
    minimum_distance = float('inf')
    for i in range(args.drones):
        angle = i * 2 * math.pi / args.drones
        initial[i] = (math.cos(angle) * args.radius, math.sin(angle) * args.radius, .5)
        goals[i] = (-initial[i][0], -initial[i][1], .5)
        prefix = f'/drone_{i}_'
        def odom(message, i=i):
            counts[i]['odom'] += 1
            p = message.pose.pose.position
            positions[i] = (p.x, p.y, p.z)
        def primitive(message, i=i):
            assert message.drone_id == i, 'cross-drone primitive delivery'
            assert message.select_path_id >= 0, 'negative primitive ID'
            counts[i]['primitive'] += 1
        subscriptions.append(node.create_subscription(Odometry, prefix+'visual_slam/odom', odom, 10))
        subscriptions.append(node.create_subscription(SwarmPrimitiveTraj, prefix+'planning/selected_path_id', primitive, 100))
        for key, topic, typ in [('cloud', 'pcl_render_node/cloud', PointCloud2),
                                ('command', 'planning/pos_cmd', PositionCommand),
                                ('heartbeat', 'traj_server/heartbeat', Empty)]:
            def callback(message, i=i, key=key):
                counts[i][key] += 1
            subscriptions.append(node.create_subscription(typ, prefix+topic, callback, 10))
    subscriptions.append(node.create_subscription(SwarmPrimitiveTraj, '/broadcast_primitive_to_planner',
                                                  lambda msg: observed_ids.add(msg.drone_id), 100))
    trigger = node.create_publisher(PoseStamped, '/traj_start_trigger', 10)
    start, triggered, errors = time.monotonic(), None, None
    try:
        while time.monotonic() - start < args.timeout:
            rclpy.spin_once(node, timeout_sec=.05)
            if triggered is None and all(c['heartbeat'] > 5 and c['odom'] > 5 and c['cloud'] > 0 for c in counts):
                message = PoseStamped()
                message.header.frame_id = 'world'
                message.header.stamp = node.get_clock().now().to_msg()
                message.pose.orientation.w = 1.0
                trigger.publish(message)
                triggered = time.monotonic()
            if len(positions) == args.drones:
                errors = [math.dist(positions[i], goals[i]) for i in range(args.drones)]
                if triggered:
                    for i in range(args.drones):
                        for j in range(i):
                            minimum_distance = min(minimum_distance, math.dist(positions[i], positions[j]))
                if triggered and all(e < .1 for e in errors) and all(c['primitive'] for c in counts):
                    break
            if process.poll() is not None:
                break
        passed = (triggered is not None and errors is not None and all(e < .1 for e in errors)
                  and all(all(c.values()) for c in counts) and observed_ids == set(range(args.drones)))
        result = {'passed': passed, 'drones': args.drones, 'counts': counts, 'goal_errors_m': errors,
                  'broadcast_ids': sorted(observed_ids), 'minimum_observed_separation_m': minimum_distance if math.isfinite(minimum_distance) else None,
                  'elapsed_s': time.monotonic()-start, 'trigger_sent': triggered is not None}
        args.output.write_text(json.dumps(result, indent=2))
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
        log.close()
    shutdown_log = args.output.with_suffix('.log').read_text()
    result['shutdown_clean'] = not bool(re.search(r'terminate called|exit code -(?:6|11)\b|Failed to delete datawriter|failed to create guard condition', shutdown_log))
    result['passed'] = bool(result['passed'] and result['shutdown_clean'])
    args.output.write_text(json.dumps(result, indent=2))
    print(json.dumps(result), flush=True)
    if not result['passed']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
