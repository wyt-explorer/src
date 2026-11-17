#!/usr/bin/env python3
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
        o=Vector3Stamped(); o.header=h; o.vector.x=math.degrees(e[2]); o.vector.y=math.degrees(e[1]); o.vector.z=math.degrees(e[0]); p.publish(o)
def main():
    rclpy.init(); n=NodeE(); rclpy.spin(n); n.destroy_node(); rclpy.shutdown()
if __name__=='__main__': main()
