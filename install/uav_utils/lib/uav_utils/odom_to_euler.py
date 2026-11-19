#!/usr/bin/env python3
"""文件作用：将里程计/IMU 四元数转换为欧拉角，并提取手柄控制轴。
调用关系：订阅回调分别转换姿态或轴值，沿用输入消息头发布。
输入：odom、imu、ctrlin；四元数和 Joy 轴。
输出：euler、imueuler 的 x/y/z 为 roll/pitch/yaw（度）；ctrlout 为选取的轴值。
运行模式：ROS 2 诊断脚本。
主要依赖：rclpy、nav_msgs、sensor_msgs、geometry_msgs、math。
"""
import math
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu, Joy
from geometry_msgs.msg import Vector3Stamped

def euler(q):
    x,y,z,w=q
    roll=math.atan2(2*(w*x+y*z),1-2*(x*x+y*y))
    pitch=math.asin(max(-1,min(1,2*(w*y-z*x))))
    yaw=math.atan2(2*(w*z+x*y),1-2*(y*y+z*z))
    return roll,pitch,yaw
class NodeE(Node):
    def __init__(self):
        super().__init__('odom_to_euler')
        self.p=self.create_publisher(Vector3Stamped,'euler',10); self.pi=self.create_publisher(Vector3Stamped,'imueuler',10); self.pc=self.create_publisher(Vector3Stamped,'ctrlout',10)
        self.create_subscription(Odometry,'odom',self.odom,10); self.create_subscription(Imu,'imu',self.imu,10); self.create_subscription(Joy,'ctrlin',self.joy,10)
    def odom(self,m): self.pub(m.header,euler((m.pose.pose.orientation.x,m.pose.pose.orientation.y,m.pose.pose.orientation.z,m.pose.pose.orientation.w)),self.p)
    def imu(self,m): self.pub(m.header,euler((m.orientation.x,m.orientation.y,m.orientation.z,m.orientation.w)),self.pi)
    def joy(self,m):
        o=Vector3Stamped(); o.header=m.header; o.vector.x=m.axes[0]; o.vector.y=m.axes[1]; o.vector.z=-m.axes[3]; self.pc.publish(o)
    def pub(self,h,e,p):
        o=Vector3Stamped(); o.header=h; o.vector.x=e[0]*180.0/3.14159; o.vector.y=e[1]*180.0/3.14159; o.vector.z=e[2]*180.0/3.14159; p.publish(o)
def main():
    rclpy.init(); n=NodeE(); rclpy.spin(n); n.destroy_node(); rclpy.shutdown()
if __name__=='__main__': main()
