#!/usr/bin/env python3
"""文件作用：每秒发布一次世界原点处、单位姿态的测试里程计。
调用关系：独立运行 OdomSender，由定时器调用 publish()。
输入：节点时钟，无订阅输入；时间戳按上游延迟 0.2 秒。
输出：odom（Odometry），frame_id 为 world，位置和速度均为零。
运行模式：ROS 2 消息连通性测试。
主要依赖：rclpy、nav_msgs。
"""
import rclpy
from rclpy.node import Node
from rclpy.duration import Duration
from nav_msgs.msg import Odometry

class OdomSender(Node):
    def __init__(self):
        super().__init__('odom_sender')
        self.pub = self.create_publisher(Odometry, 'odom', 10)
        self.msg = Odometry()
        self.msg.header.frame_id = 'world'
        self.msg.pose.pose.orientation.w = 1.0
        self.timer = self.create_timer(1.0, self.publish)
        self.counter = 0
    def publish(self):
        self.counter += 1
        self.msg.header.stamp = (self.get_clock().now() - Duration(seconds=0.2)).to_msg()
        self.pub.publish(self.msg)
        self.get_logger().info(f'Send {self.counter:3d} msg(s).')

def main():
    rclpy.init()
    node = OdomSender()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node(); rclpy.shutdown()
if __name__ == '__main__':
    main()
