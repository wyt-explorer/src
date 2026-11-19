#!/usr/bin/env python3
"""文件作用：将地图 YAML 中的起点和终点显示为不同颜色的球形标记。
调用关系：demo.launch.py 或 single_drone.launch.py 启动，每秒发布两个 Marker。
输入：map_file 中的 start、goal 与 map.frame_id；位置为米。
输出：/my_primitive_demo/waypoint_markers，起点绿色、终点红色。
运行模式：ROS 2 仿真可视化节点。
主要依赖：rclpy、visualization_msgs、PyYAML。
"""
import os,yaml
import rclpy
from rclpy.node import Node
from visualization_msgs.msg import Marker
class Waypoints(Node):
 def __init__(self):
  super().__init__('demo_waypoint_markers'); self.declare_parameter('map_file',''); p=self.get_parameter('map_file').value
  with open(os.path.expanduser(p)) as f:self.cfg=yaml.safe_load(f)
  self.frame=self.cfg.get('map',{}).get('frame_id','world');self.pub=self.create_publisher(Marker,'/my_primitive_demo/waypoint_markers',2);self.create_timer(1.,self.publish)
 def publish(self):
  for i,key,color in [(0,'start',(0.1,1.,0.1)),(1,'goal',(1.,.2,.1))]:
   m=Marker();m.header.frame_id=self.frame;m.header.stamp=self.get_clock().now().to_msg();m.ns='demo_waypoints';m.id=i;m.type=Marker.SPHERE;m.action=Marker.ADD;m.pose.position.x,m.pose.position.y,m.pose.position.z=(float(self.cfg[key][axis]) for axis in 'xyz');m.pose.orientation.w=1.;m.scale.x=m.scale.y=m.scale.z=.4;m.color.r,m.color.g,m.color.b=color;m.color.a=1.;self.pub.publish(m)
def main():
 rclpy.init();n=Waypoints();rclpy.spin(n);n.destroy_node();rclpy.shutdown()
if __name__=='__main__':main()
