#!/usr/bin/env python3
import math
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry, Path
from geometry_msgs.msg import PoseStamped, TransformStamped
from tf2_ros import TransformBroadcaster

def euler(q):
    x,y,z,w=q
    roll=math.atan2(2*(w*x+y*z),1-2*(x*x+y*y))
    pitch=math.asin(max(-1.0,min(1.0,2*(w*y-z*x))))
    yaw=math.atan2(2*(w*z+x*y),1-2*(y*y+z*z))
    return roll,pitch,yaw

def quat(roll,pitch,yaw):
    cr,cp,cy=math.cos(roll/2),math.cos(pitch/2),math.cos(yaw/2)
    sr,sp,sy=math.sin(roll/2),math.sin(pitch/2),math.sin(yaw/2)
    return (sr*cp*cy-cr*sp*sy, cr*sp*cy+sr*cp*sy, cr*cp*sy-sr*sp*cy, cr*cp*cy+sr*sp*sy)

class TFAssist(Node):
    def __init__(self):
        super().__init__('tf_assist')
        self.declare_parameter('frame_id_in','world'); self.declare_parameter('frame_id_out','world')
        self.declare_parameter('body_frame_id','body'); self.declare_parameter('intermediate_frame_id','intermediate')
        self.declare_parameter('world_frame_id','world'); self.declare_parameter('broadcast_tf',False)
        self.fi=self.get_parameter('frame_id_in').value; self.fo=self.get_parameter('frame_id_out').value
        self.body=self.get_parameter('body_frame_id').value; self.inter=self.get_parameter('intermediate_frame_id').value; self.world=self.get_parameter('world_frame_id').value
        self.broadcast=self.get_parameter('broadcast_tf').value; self.path=[]; self.tf=TransformBroadcaster(self)
        self.pub=self.create_publisher(Odometry,'out_odom',10); self.path_pub=self.create_publisher(Path,'out_path',10)
        self.create_subscription(Odometry,'in_odom',self.odom,10); self.create_timer(.5,self.publish_path)
    def odom(self,m):
        if self.fi and m.header.frame_id and m.header.frame_id != self.fi: self.get_logger().warn(f'expected {self.fi}, got {m.header.frame_id}')
        q=(m.pose.pose.orientation.x,m.pose.pose.orientation.y,m.pose.pose.orientation.z,m.pose.pose.orientation.w); p=m.pose.pose.position; r,pi,y=euler(q)
        m.header.frame_id=self.fo; m.child_frame_id=''; self.pub.publish(m)
        if self.broadcast:
            self.send(self.world,self.body,p.x,p.y,p.z,quat(r,pi,y),m.header.stamp)
            self.send(self.world,self.inter,p.x,p.y,p.z,quat(r,0,0),m.header.stamp)
        pose=PoseStamped(); pose.header=m.header; pose.pose=m.pose.pose; self.path.append(pose); self.path=self.path[-30000:]
    def send(self,parent,child,x,y,z,q,stamp):
        t=TransformStamped(); t.header.stamp=stamp; t.header.frame_id=parent; t.child_frame_id=child; t.transform.translation.x=x; t.transform.translation.y=y; t.transform.translation.z=z; t.transform.rotation.x=q[0]; t.transform.rotation.y=q[1]; t.transform.rotation.z=q[2]; t.transform.rotation.w=q[3]; self.tf.sendTransform(t)
    def publish_path(self):
        if self.path:
            p=Path(); p.header=self.path[-1].header; p.poses=list(self.path); self.path_pub.publish(p)
def main():
    rclpy.init(); n=TFAssist(); rclpy.spin(n); n.destroy_node(); rclpy.shutdown()
if __name__=='__main__': main()
