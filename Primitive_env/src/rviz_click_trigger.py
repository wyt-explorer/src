#!/usr/bin/env python3
"""文件作用：将第一次 RViz 点击转换为规划启动消息，忽略后续点击。
调用关系：demo.launch.py 或 single_drone.launch.py 启动，click() 回调执行一次转发。
输入：/clicked_point（PointStamped）；仅使用点击事件，不将点击坐标作为目标。
输出：/traj_start_trigger（PoseStamped），world 系、单位姿态和当前时间戳。
运行模式：ROS 2 单次交互触发节点。
主要依赖：rclpy、geometry_msgs。
"""
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped, PointStamped
class RVizClickTrigger(Node):
    def __init__(self):
        super().__init__('rviz_click_trigger'); self.triggered=False
        self.pub=self.create_publisher(PoseStamped,'/traj_start_trigger',1)
        self.create_subscription(PointStamped,'/clicked_point',self.click,10)
    def click(self,msg):
        if self.triggered:return
        out=PoseStamped();out.header.stamp=self.get_clock().now().to_msg();out.header.frame_id='world';out.pose.orientation.w=1.;self.pub.publish(out);self.triggered=True;self.get_logger().info('RViz clicked; planning triggered.')
def main():
 rclpy.init();n=RVizClickTrigger();rclpy.spin(n);n.destroy_node();rclpy.shutdown()
if __name__=='__main__':main()
