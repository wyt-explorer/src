#!/usr/bin/env python3
"""文件作用：按上游配置创建多个里程计转换器、TF 广播和路径显示。
调用关系：TFAssist 创建 converterN，各订阅回调转换消息头，定时器限频 TF 并发布路径。
输入：converterN/frame_id_in、frame_id_out 等参数及 converterN/in_odom；米、秒制。
输出：各 converterN/out_odom、out_path 及 world/body/intermediate TF。
运行模式：ROS 2；只重命名坐标标签，保持原数值，intermediate 仅保留 yaw。
主要依赖：rclpy、tf2_ros、nav_msgs、geometry_msgs、math、copy。
"""
import copy
import math
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry, Path
from geometry_msgs.msg import PoseStamped, TransformStamped
from tf2_ros import TransformBroadcaster


class OdometryConverter:
    """保留上游 converterN 的独立输入输出、10 Hz TF 与 2 Hz 路径。"""
    def __init__(self, node, prefix, frame_in, frame_out):
        self.node, self.prefix = node, prefix
        self.frame_in, self.frame_out = frame_in, frame_out
        param = lambda key, default: node.declare_parameter(prefix + key, default).value
        self.broadcast = param('broadcast_tf', False)
        self.body, self.intermediate = param('body_frame_id', 'body'), param('intermediate_frame_id', 'intermediate')
        self.world = param('world_frame_id', 'world')
        self.path, self.tf_ready = [], True
        self.pub = node.create_publisher(Odometry, prefix + 'out_odom', 10)
        self.path_pub = node.create_publisher(Path, prefix + 'out_path', 10)
        self.subscription = node.create_subscription(Odometry, prefix + 'in_odom', self.odom, 10)
        self.tf_timer = node.create_timer(0.1, self.allow_tf)
        self.path_timer = node.create_timer(0.5, self.publish_path)

    def allow_tf(self):
        self.tf_ready = True

    def send(self, parent, child, position, quaternion, stamp):
        transform = TransformStamped()
        transform.header.stamp, transform.header.frame_id, transform.child_frame_id = stamp, parent, child
        transform.transform.translation.x, transform.transform.translation.y, transform.transform.translation.z = position
        q = transform.transform.rotation
        q.x, q.y, q.z, q.w = quaternion
        self.node.tf.sendTransform(transform)

    def odom(self, message):
        if message.header.frame_id != self.frame_in:
            self.node.get_logger().error(f'Expected {self.frame_in}, got {message.header.frame_id}')
            return
        output = copy.deepcopy(message)
        output.header.frame_id, output.child_frame_id = self.frame_out, ''
        self.pub.publish(output)
        point, q = output.pose.pose.position, output.pose.pose.orientation
        position, quaternion = (point.x, point.y, point.z), (q.x, q.y, q.z, q.w)
        if self.broadcast and self.tf_ready:
            self.tf_ready = False
            stamp = output.header.stamp
            if self.frame_in != self.frame_out:
                self.send(self.frame_out, self.frame_in, (0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0), stamp)
            if self.world != self.frame_out:
                self.send(self.frame_out, self.world, (0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0), stamp)
            self.send(self.world, self.body, position, quaternion, stamp)
            yaw = math.atan2(2 * (q.w*q.z + q.x*q.y), 1 - 2 * (q.y*q.y + q.z*q.z))
            self.send(self.world, self.intermediate, position, (0.0, 0.0, math.sin(yaw/2), math.cos(yaw/2)), stamp)
        pose = PoseStamped()
        pose.header, pose.pose = output.header, output.pose.pose
        self.path.append(pose)
        self.path = self.path[-30000:]

    def publish_path(self):
        if self.path:
            message = Path()
            message.header, message.poses = self.path[-1].header, list(self.path)
            self.path_pub.publish(message)


class TFAssist(Node):
    def __init__(self):
        super().__init__('tf_assist')
        self.tf, self.converters = TransformBroadcaster(self), []
        index = 0
        while True:
            prefix = f'converter{index}/'
            frame_in = self.declare_parameter(prefix + 'frame_id_in', '').value
            frame_out = self.declare_parameter(prefix + 'frame_id_out', '').value
            if not frame_in or not frame_out:
                break
            self.converters.append(OdometryConverter(self, prefix, frame_in, frame_out))
            index += 1
        # 兼容先前 ROS 2 单转换器参数，同时保留上游的 converterN 入口。
        if not self.converters:
            frame_in = self.declare_parameter('frame_id_in', 'world').value
            frame_out = self.declare_parameter('frame_id_out', 'world').value
            self.converters.append(OdometryConverter(self, '', frame_in, frame_out))


def main():
    rclpy.init()
    node = TFAssist()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
