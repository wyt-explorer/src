#!/usr/bin/env python3
# 读取地图起终点配置，周期性发布供 RViz 显示的球形标记。
import os
import yaml
import rospy
from visualization_msgs.msg import Marker
from geometry_msgs.msg import Point

# 创建球形 Marker；同一命名空间下通过固定 id 更新对应标记，label 当前未使用。
def marker(frame, idx, label, xyz, rgb):
    m=Marker(); m.header.frame_id=frame; m.header.stamp=rospy.Time.now()
    m.ns='demo_waypoints'; m.id=idx; m.type=Marker.SPHERE; m.action=Marker.ADD
    m.pose.position=Point(*map(float,xyz)); m.pose.orientation.w=1.0
    # 设置球体直径、颜色和透明度；零 lifetime 表示标记持续存在。
    m.scale.x=m.scale.y=m.scale.z=0.4
    m.color.r,m.color.g,m.color.b=rgb; m.color.a=1.0
    m.lifetime=rospy.Duration(0)
    return m

# 加载地图坐标系与起终点，初始化标记发布节点。
def main():
    rospy.init_node('demo_waypoint_markers')
    with open(os.path.expanduser(rospy.get_param('~map_file')),encoding='utf-8') as f: cfg=yaml.safe_load(f)
    frame=cfg['map'].get('frame_id','world')
    # 锁存最新消息，并通过周期发布持续刷新两个标记。
    pub=rospy.Publisher('/my_primitive_demo/waypoint_markers',Marker,queue_size=2,latch=True)
    rate=rospy.Rate(1)
    while not rospy.is_shutdown():
        # 起点用绿色、终点用红色，分别使用稳定的 id。
        for idx,key,color in [(0,'start',(0.1,1.,0.1)),(1,'goal',(1.,0.2,0.1))]:
            p=cfg[key]; pub.publish(marker(frame,idx,key,[p['x'],p['y'],p['z']],color))
        rate.sleep()

# 脚本入口：正常处理 ROS 关闭引发的中断。
if __name__=='__main__':
    try: main()
    except rospy.ROSInterruptException: pass