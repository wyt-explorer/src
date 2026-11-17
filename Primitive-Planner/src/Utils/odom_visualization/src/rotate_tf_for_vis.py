#!/usr/bin/env python3
import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster
class RotateTF(Node):
    def __init__(self):
        super().__init__('rotate_tf_for_vis'); self.br=TransformBroadcaster(self); self.t0=self.get_clock().now(); self.timer=self.create_timer(1/30,self.tick)
    def tick(self):
        t=self.get_clock().now(); sec=(t-self.t0).nanoseconds*1e-9; m=TransformStamped(); m.header.stamp=t.to_msg(); m.header.frame_id='world'; m.child_frame_id='rotate'; m.transform.rotation.z=math.sin(.01*sec); m.transform.rotation.w=math.cos(.01*sec); self.br.sendTransform(m)
def main():
    rclpy.init(); n=RotateTF(); rclpy.spin(n); n.destroy_node(); rclpy.shutdown()
if __name__=='__main__': main()
