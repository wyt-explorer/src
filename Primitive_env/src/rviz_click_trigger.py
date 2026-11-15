#!/usr/bin/env python3
import rospy

from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import PointStamped


class RVizClickTrigger:

    def __init__(self):
        rospy.init_node("rviz_click_trigger")

        self.triggered = False

        self.pub = rospy.Publisher(
            "/traj_start_trigger",
            PoseStamped,
            queue_size=1
        )

        self.sub = rospy.Subscriber(
            "/clicked_point",
            PointStamped,
            self.click_callback
        )

        rospy.loginfo("RViz click trigger ready. Click the map to start planning.")

        rospy.spin()

    def click_callback(self, msg):

        if self.triggered:
            return

        trigger = PoseStamped()
        trigger.header.stamp = rospy.Time.now()
        trigger.header.frame_id = "world"

        trigger.pose.orientation.w = 1.0

        self.pub.publish(trigger)

        self.triggered = True

        rospy.loginfo("RViz clicked! Planning triggered.")


if __name__ == "__main__":
    try:
        RVizClickTrigger()
    except rospy.ROSInterruptException:
        pass