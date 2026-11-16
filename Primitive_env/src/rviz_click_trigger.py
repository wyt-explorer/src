#!/usr/bin/env python3
# 将 RViz 的首次地图点击转换为固定目标规划任务的启动消息。
import rospy

from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import PointStamped


# 管理点击订阅和触发发布，每次节点运行只触发一次。
class RVizClickTrigger:

    # 初始化 ROS 通信接口，并进入回调事件循环。
    def __init__(self):
        rospy.init_node("rviz_click_trigger")

        self.triggered = False

        # 规划器通过该话题接收任务启动信号。
        self.pub = rospy.Publisher(
            "/traj_start_trigger",
            PoseStamped,
            queue_size=1
        )

        # 接收 RViz Publish Point 工具产生的点击事件。
        self.sub = rospy.Subscriber(
            "/clicked_point",
            PointStamped,
            self.click_callback
        )

        rospy.loginfo("RViz click trigger ready. Click the map to start planning.")

        rospy.spin()

    # 点击仅作为启动事件，msg 中的位置不用于设置目标。
    def click_callback(self, msg):

        # 忽略后续点击，避免重复启动当前任务。
        if self.triggered:
            return

        # 构造 world 坐标系下的触发消息，使用单位四元数。
        trigger = PoseStamped()
        trigger.header.stamp = rospy.Time.now()
        trigger.header.frame_id = "world"

        trigger.pose.orientation.w = 1.0

        self.pub.publish(trigger)

        self.triggered = True

        rospy.loginfo("RViz clicked! Planning triggered.")


# 脚本入口：正常处理 ROS 关闭引发的中断。
if __name__ == "__main__":
    try:
        RVizClickTrigger()
    except rospy.ROSInterruptException:
        pass