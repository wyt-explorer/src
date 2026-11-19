#!/usr/bin/env python3
"""文件作用：验证已恢复的理想跟踪、群体目标分配、UDP 通信和 SO3 动力学链路。
调用关系：启动测试所需节点，发送确定输入并核对输出，随后关闭自身创建的进程。
输入：已加载的 ROS 2 安装环境；建议使用独立 ROS_DOMAIN_ID。
输出：逐项通过结果和 JSON 记录，日志写入 --output 指定目录。
运行模式：ROS 2 本机集成测试，需要本机 DDS 和 UDP socket 权限。
主要依赖：rclpy、ROS 消息包、subprocess、socket、Python 标准库。
"""
import argparse
import json
import math
import os
from pathlib import Path
import signal
import socket
import struct
import subprocess
import time
import rclpy
from rclpy.node import Node
from ament_index_python.packages import get_package_prefix
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Joy
from std_msgs.msg import Empty
from quadrotor_msgs.msg import PositionCommand, GoalSet
from traj_utils.msg import SwarmPrimitiveTraj
from visualization_msgs.msg import MarkerArray


class Harness:
    def __init__(self, output):
        self.output = output
        output.mkdir(parents=True, exist_ok=True)
        rclpy.init()
        self.node = Node('parity_components_probe')
        self.processes, self.streams = [], []

    def start(self, package, executable, remaps, parameters=None):
        command = [str(Path(get_package_prefix(package)) / 'lib' / package / executable), '--ros-args']
        for name, value in (parameters or {}).items():
            command += ['-p', name + ':=' + str(value).lower() if isinstance(value, bool) else name + ':=' + str(value)]
        for source, target in remaps.items():
            command += ['-r', source + ':=' + target]
        stream = (self.output / (executable + '.log')).open('w')
        process = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
        self.processes.append(process)
        self.streams.append(stream)
        return process

    def spin(self, seconds):
        end = time.monotonic() + seconds
        while time.monotonic() < end:
            rclpy.spin_once(self.node, timeout_sec=.05)

    def until(self, predicate, timeout=8, tick=None):
        end = time.monotonic() + timeout
        while time.monotonic() < end:
            if predicate():
                return
            if tick:
                tick()
            rclpy.spin_once(self.node, timeout_sec=.03)
        raise AssertionError('Timed out waiting for expected output')

    def close(self):
        for process in self.processes:
            if process.poll() is None:
                process.send_signal(signal.SIGINT)
        for process in self.processes:
            try:
                process.wait(timeout=8)
            except subprocess.TimeoutExpired:
                os.killpg(process.pid, signal.SIGTERM)
                process.wait(timeout=4)
        for stream in self.streams:
            stream.close()
        self.node.destroy_node()
        rclpy.shutdown()
        return [process.returncode for process in self.processes]


def ideal(h):
    received = []
    sub = h.node.create_subscription(Odometry, '/verify/ideal/odom', received.append, 10)
    pub = h.node.create_publisher(PositionCommand, '/verify/ideal/cmd', 10)
    h.start('poscmd_2_odom', 'poscmd_2_odom', {'command': '/verify/ideal/cmd', 'odometry': '/verify/ideal/odom'})
    h.until(lambda: pub.get_subscription_count() > 0)
    cmd = PositionCommand()
    cmd.position.x, cmd.position.y, cmd.position.z = 1., 2., 3.
    cmd.velocity.x, cmd.velocity.y, cmd.velocity.z = .1, .2, .3
    cmd.acceleration.x, cmd.acceleration.y, cmd.acceleration.z = .4, .5, .6
    cmd.yaw = .7
    h.until(lambda: received and received[-1].pose.pose.position.z == 3., tick=lambda: pub.publish(cmd))
    odom = received[-1]
    assert (odom.twist.twist.angular.x, odom.twist.twist.angular.y, odom.twist.twist.angular.z) == (.4, .5, .6)
    assert odom.twist.twist.linear.y == .2
    q = odom.pose.pose.orientation
    assert abs(sum(v*v for v in (q.x, q.y, q.z, q.w)) - 1.) < 1e-9
    assert odom.header.frame_id == 'world'
    h.node.destroy_subscription(sub)
    h.node.destroy_publisher(pub)


def assignment(h):
    received, markers = [], []
    sub = h.node.create_subscription(GoalSet, '/verify/assigned', received.append, 10)
    marker_sub = h.node.create_subscription(MarkerArray, '/verify/arrows', markers.append, 10)
    select = h.node.create_publisher(PoseStamped, '/verify/selection', 10)
    goal = h.node.create_publisher(PoseStamped, '/verify/goal', 10)
    h.start('assign_goals', 'assign_goals_node', {'/rviz_selected_drones': '/verify/selection', '/goal': '/verify/goal',
            '/goal_user2brig': '/verify/assigned', '/new_goals_arrow': '/verify/arrows'})
    h.until(lambda: select.get_subscription_count() and goal.get_subscription_count())
    for i, x in [(2, -1.), (7, 1.)]:
        message = PoseStamped()
        message.header.frame_id = f'drone_{i}'
        message.pose.position.x, message.pose.position.z = x, 1.
        select.publish(message)
        h.spin(.15)
    message = PoseStamped()
    message.pose.position.x, message.pose.position.y, message.pose.position.z = 3., 4., 2.
    goal.publish(message)
    h.until(lambda: len(received) >= 2 and markers)
    points = {m.drone_id: tuple(m.goal) for m in received}
    assert points[2] == (2., 4., 2.) and points[7] == (4., 4., 2.)
    assert markers[0].markers[0].color.r == 0.
    h.until(lambda: any(m.markers and m.markers[0].action == 3 for m in markers), timeout=4)
    for s in (sub, marker_sub):
        h.node.destroy_subscription(s)
    for p in (select, goal):
        h.node.destroy_publisher(p)


def random_goals(h):
    received = []
    sub = h.node.create_subscription(GoalSet, '/verify/random/goals', received.append, 10)
    pubs = [h.node.create_publisher(Odometry, f'/verify/random/odom{i}', 10) for i in range(2)]
    h.start('random_goals', 'random_goals_node',
            {'/drone_0_visual_slam/odom': '/verify/random/odom0', '/drone_1_visual_slam/odom': '/verify/random/odom1',
             '/others_odom': '/verify/random/others', '/goal_with_id': '/verify/random/goals'},
            {'drone_num': 2, 'goal_num': 2, 'goal0': [10., 0., 1.], 'goal1': [-10., 0., 1.]})
    h.until(lambda: all(p.get_subscription_count() for p in pubs))
    def send_states():
        for i, pub in enumerate(pubs):
            message = Odometry()
            message.pose.pose.position.y, message.pose.pose.position.z = float(2*i-1), 1.
            message.pose.pose.orientation.w = 1.
            pub.publish(message)
    h.until(lambda: {m.drone_id for m in received if abs(m.goal[0]) > 5} == {0, 1}, timeout=9, tick=send_states)
    assigned = {m.drone_id: tuple(m.goal) for m in received if abs(m.goal[0]) > 5}
    assert assigned[0] != assigned[1], 'random targets must not be occupied by both drones'
    h.node.destroy_subscription(sub)
    for pub in pubs:
        h.node.destroy_publisher(pub)


def manual(h):
    commands, stops = [], []
    sub = h.node.create_subscription(PositionCommand, '/verify/manual/cmd', commands.append, 10)
    stop_sub = h.node.create_subscription(Empty, '/verify/manual/stop', stops.append, 10)
    initial = h.node.create_publisher(PositionCommand, '/verify/manual/cmd', 10)
    joy = h.node.create_publisher(Joy, '/verify/manual/joy', 10)
    h.start('manual_take_over', 'manual_take_over', {'/position_cmd': '/verify/manual/cmd',
            '/joystick_from_bridge': '/verify/manual/joy', '/mandatory_stop_to_planner': '/verify/manual/stop'})
    h.until(lambda: initial.get_subscription_count() >= 2 and joy.get_subscription_count())
    command = PositionCommand()
    command.header.frame_id = 'world'
    command.position.x, command.position.y, command.position.z = 2., 3., 1.
    initial.publish(command)
    h.spin(.6)
    message = Joy()
    message.axes, message.buttons = [.2, 0., 0., 0., .5], [1, 0, 0, 0]
    h.until(lambda: stops and any(c.header.frame_id == 'manual_take_over' and c.position.x > 2.02 for c in commands),
            timeout=6, tick=lambda: joy.publish(message))
    last = [c for c in commands if c.header.frame_id == 'manual_take_over'][-1]
    assert abs(last.velocity.x - .1) < 1e-7 and last.position.y == 3. and last.position.z == 1.
    for item in (sub, stop_sub):
        h.node.destroy_subscription(item)
    for item in (initial, joy):
        h.node.destroy_publisher(item)


def udp(h):
    output = {key: [] for key in ('odom', 'primitive', 'goal', 'joy', 'stop')}
    specs = [('odom', Odometry, 'my_odom', '/others_odom'),
             ('primitive', SwarmPrimitiveTraj, '/broadcast_primitive_from_planner', '/broadcast_primitive_to_planner'),
             ('goal', GoalSet, '/goal_user2brig', '/goal_brig2plner'),
             ('joy', Joy, '/joystick_from_users', '/joystick_from_bridge'),
             ('stop', Empty, '/mandatory_stop_from_users', '/mandatory_stop_to_planner')]
    remaps, pubs, subs = {}, {}, []
    for key, typ, source, target in specs:
        remaps[source], remaps[target] = '/verify/udp/send/'+key, '/verify/udp/recv/'+key
        pubs[key] = h.node.create_publisher(typ, remaps[source], 10)
        subs.append(h.node.create_subscription(typ, remaps[target], output[key].append, 10))
    process = h.start('swarm_bridge', 'bridge_node_udp', remaps,
                      {'drone_id': 7, 'broadcast_ip': '127.0.0.255', 'port': 23819, 'enable_stop_broadcast': True})
    h.until(lambda: all(p.get_subscription_count() for p in pubs.values()))
    # 错误包不能使接收线程退出或从缓冲区越界读取。
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.sendto(b'bad', ('127.0.0.1', 23819))
        sock.sendto(struct.pack('II', 101, 1000000), ('127.0.0.1', 23819))
    messages = {'odom': Odometry(), 'primitive': SwarmPrimitiveTraj(), 'goal': GoalSet(), 'joy': Joy(), 'stop': Empty()}
    messages['odom'].pose.pose.position.x = 1.25
    messages['primitive'].drone_id, messages['primitive'].select_path_id = 42, 13
    messages['goal'].drone_id, messages['goal'].goal = 42, [1., 2., 3.]
    messages['joy'].axes, messages['joy'].buttons = [.25, -.5], [1, 0]
    h.until(lambda: all(output.values()), tick=lambda: [pubs[k].publish(v) for k, v in messages.items()])
    assert output['odom'][-1].child_frame_id == 'drone_7'
    assert output['odom'][-1].pose.pose.position.x == 1.25
    assert output['primitive'][-1].drone_id == 42 and output['primitive'][-1].select_path_id == 13
    assert list(output['goal'][-1].goal) == [1., 2., 3.]
    assert list(output['joy'][-1].buttons) == [1, 0]
    assert process.poll() is None
    for s in subs:
        h.node.destroy_subscription(s)
    for p in pubs.values():
        h.node.destroy_publisher(p)


def so3(h):
    states = []
    sub = h.node.create_subscription(Odometry, '/verify/so3/odom', states.append, 10)
    pub = h.node.create_publisher(PositionCommand, '/verify/so3/position', 10)
    h.start('so3_control', 'so3_control_ros2', {'odom': '/verify/so3/odom', 'position_cmd': '/verify/so3/position',
            'imu': '/verify/so3/imu', 'so3_cmd': '/verify/so3/cmd'},
            {'mass': .98, 'use_external_yaw': False, 'so3_control/init_state_z': 1.0})
    h.start('so3_quadrotor_simulator', 'quadrotor_simulator_so3',
            {'odom': '/verify/so3/odom', 'imu': '/verify/so3/imu', 'cmd': '/verify/so3/cmd'},
            {'simulator/init_state_z': 1.0})
    h.until(lambda: len(states) > 40 and pub.get_subscription_count())
    cmd = PositionCommand()
    cmd.position.x, cmd.position.z = .5, 1.6
    h.until(lambda: states[-1].pose.pose.position.x > .3 and states[-1].pose.pose.position.z > 1.3,
            timeout=10, tick=lambda: pub.publish(cmd))
    assert all(math.isfinite(v) for m in states for v in (m.pose.pose.position.x, m.pose.pose.position.z))
    h.node.destroy_subscription(sub)
    h.node.destroy_publisher(pub)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, default=Path('/tmp/primitive-components'))
    args = parser.parse_args()
    harness = Harness(args.output)
    result = {}
    try:
        for name, test in [('ideal_tracking', ideal), ('goal_assignment', assignment), ('random_goal_occupancy', random_goals), ('manual_takeover', manual), ('udp_roundtrip', udp), ('so3_dynamics', so3)]:
            test(harness)
            result[name] = 'passed'
            print(name + ': PASS', flush=True)
    finally:
        exits = harness.close()
        result['exit_codes'] = exits
        result['shutdown_clean'] = all(code == 0 for code in exits)
        (args.output / 'result.json').write_text(json.dumps(result, indent=2))
    if not result['shutdown_clean']:
        raise SystemExit('Component shutdown failed: ' + str(exits))


if __name__ == '__main__':
    main()
