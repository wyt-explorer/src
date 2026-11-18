#!/usr/bin/env python3
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
