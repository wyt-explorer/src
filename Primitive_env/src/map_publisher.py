#!/usr/bin/env python3
# 读取地图配置，将长方体、圆柱和圆环障碍物转换为 ROS 点云并发布。
import os
import yaml
import numpy as np
import rospy
from std_msgs.msg import Header
from sensor_msgs.msg import PointCloud2
from sensor_msgs import point_cloud2

# 在闭区间内均匀采样，包含两端点，采样间距不超过 res（要求 a < b、res > 0）。
def axis(a,b,res):
    return np.linspace(a,b,max(2,int(np.ceil((b-a)/res))+1),dtype=np.float32)

# 按配置生成各障碍物的实体采样点，返回形状为 (N, 3) 的 float32 数组。
def build_points(cfg):
    # 检查点云分辨率，避免采样数量计算无效。
    res=float(cfg['map'].get('resolution',0.1))
    if res<=0: raise ValueError('resolution must be positive')
    parts=[]
    for ob in cfg.get('obstacles',[]):
        x,y,z=map(float,ob['position'])
        # 长方体以底面中心定位，在整个体积内进行三维网格采样。
        if ob['type']=='box':
            lx,ly,h=map(float,ob['size'])
            if min(lx,ly,h)<=0: raise ValueError('box dimensions must be positive')
            xs=axis(x-lx/2,x+lx/2,res); ys=axis(y-ly/2,y+ly/2,res); zs=axis(z,z+h,res)
            xx,yy,zz=np.meshgrid(xs,ys,zs,indexing='ij')
            pts=np.column_stack((xx.ravel(),yy.ravel(),zz.ravel()))
        # 圆柱以底面中心定位，筛选圆盘内的网格点并沿高度复制。
        elif ob['type']=='cylinder':
            radius=float(ob['radius']); h=float(ob['height'])
            if min(radius,h)<=0: raise ValueError('cylinder dimensions must be positive')
            xs=axis(-radius,radius,res); ys=axis(-radius,radius,res); zs=axis(z,z+h,res)
            xx,yy=np.meshgrid(xs,ys,indexing='ij')
            xy=np.column_stack((xx.ravel(),yy.ravel()))
            xy=xy[np.sum(xy**2,axis=1)<=radius**2+1e-6]
            pts=np.column_stack((np.repeat(xy[:,0]+x,len(zs)),np.repeat(xy[:,1]+y,len(zs)),np.tile(zs,len(xy))))
        elif ob['type']=='ring':
            # 实心圆管组成的圆环。position 表示圆环中心，不是底面中心。
            R=float(ob['radius'])
            tube=float(ob['tube_radius'])
            direction=ob.get('axis','x').lower()
            if R<=tube or tube<=0:
                raise ValueError('ring radius must be greater than tube_radius > 0')
            if direction not in ('x','y','z'):
                raise ValueError('ring axis must be x, y or z')
            # 沿圆环周向按分辨率采样，并在管截面内填充点云。
            ntheta=max(32,int(np.ceil(2*np.pi*(R+tube)/res)))
            theta=np.linspace(0,2*np.pi,ntheta,endpoint=False,dtype=np.float32)
            offsets=axis(-tube,tube,res/2)
            uu,vv=np.meshgrid(offsets,offsets,indexing='ij')
            inside=(uu**2+vv**2 <= tube**2+1e-8)
            u=uu[inside]; v=vv[inside]
            angle=theta[:,None]
            ring_rad=(R+v)[None,:]
            a=ring_rad*np.cos(angle)
            b=ring_rad*np.sin(angle)
            d=np.broadcast_to(u[None,:],a.shape)
            # 将圆环局部坐标映射到指定法向轴，并平移至世界坐标。
            if direction=='x':
                pts=np.column_stack(((x+d).ravel(),(y+a).ravel(),(z+b).ravel()))
            elif direction=='y':
                pts=np.column_stack(((x+a).ravel(),(y+d).ravel(),(z+b).ravel()))
            else:
                pts=np.column_stack(((x+a).ravel(),(y+b).ravel(),(z+d).ravel()))
        else:
            rospy.logwarn('Unsupported obstacle type: %s',ob['type']); continue
        parts.append(pts.astype(np.float32))
    # 合并所有障碍物；无障碍物时保留合法的空点云形状。
    return np.concatenate(parts) if parts else np.empty((0,3),dtype=np.float32)

# 初始化节点，加载地图，并以 1 Hz 发布带当前时间戳的全局点云。
def main():
    rospy.init_node('custom_map_publisher')
    filename=rospy.get_param('~map_file')
    with open(os.path.expanduser(filename),encoding='utf-8') as f: cfg=yaml.safe_load(f)
    points=build_points(cfg)
    frame=cfg['map'].get('frame_id','world')
    topic=rospy.get_param('~cloud_topic','/map_generator/global_cloud')
    # 锁存最近一条消息，供后续接入的订阅者立即获取地图。
    pub=rospy.Publisher(topic,PointCloud2,queue_size=1,latch=True)
    rospy.loginfo('Map ready: %d points on %s, frame=%s',len(points),topic,frame)
    rate=rospy.Rate(1)
    while not rospy.is_shutdown():
        header=Header(stamp=rospy.Time.now(),frame_id=frame)
        pub.publish(point_cloud2.create_cloud_xyz32(header,points.tolist()))
        rate.sleep()

# 脚本入口：正常处理 ROS 关闭引发的中断。
if __name__=='__main__':
    try: main()
    except rospy.ROSInterruptException: pass