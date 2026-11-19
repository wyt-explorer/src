#!/usr/bin/env python3
"""文件作用：周期广播绕 Z 轴旋转的可视化坐标系。
调用关系：RotateTF 节点以 30 Hz 定时调用 tick()。
输入：节点启动后经过的秒数，无订阅输入。
输出：world → rotate 的 TF，旋转角为 0.02 × 经过秒数（弧度）。
运行模式：ROS 2 可视化辅助脚本。
主要依赖：rclpy、tf2_ros、geometry_msgs、math。
"""
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
