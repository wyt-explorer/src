#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
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
        self.msg.header.stamp = self.get_clock().now().to_msg()
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
