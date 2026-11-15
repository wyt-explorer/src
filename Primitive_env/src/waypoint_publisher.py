#!/usr/bin/env python3
import os
import yaml
import rospy
from visualization_msgs.msg import Marker
from geometry_msgs.msg import Point

def marker(frame, idx, label, xyz, rgb):
    m=Marker(); m.header.frame_id=frame; m.header.stamp=rospy.Time.now()
    m.ns='demo_waypoints'; m.id=idx; m.type=Marker.SPHERE; m.action=Marker.ADD
    m.pose.position=Point(*map(float,xyz)); m.pose.orientation.w=1.0
    m.scale.x=m.scale.y=m.scale.z=0.4
    m.color.r,m.color.g,m.color.b=rgb; m.color.a=1.0
    m.lifetime=rospy.Duration(0)
    return m

def main():
    rospy.init_node('demo_waypoint_markers')
    with open(os.path.expanduser(rospy.get_param('~map_file')),encoding='utf-8') as f: cfg=yaml.safe_load(f)
    frame=cfg['map'].get('frame_id','world')
    pub=rospy.Publisher('/my_primitive_demo/waypoint_markers',Marker,queue_size=2,latch=True)
    rate=rospy.Rate(1)
    while not rospy.is_shutdown():
        for idx,key,color in [(0,'start',(0.1,1.,0.1)),(1,'goal',(1.,0.2,0.1))]:
            p=cfg[key]; pub.publish(marker(frame,idx,key,[p['x'],p['y'],p['z']],color))
        rate.sleep()

if __name__=='__main__':
    try: main()
    except rospy.ROSInterruptException: pass