#!/usr/bin/env python3
"""由 demo.launch.py 调用，从 YAML（米、world 坐标）生成障碍物点云。
输入：map_file 参数；输出：PointCloud2；模式：ROS 2 仿真地图。
依赖：rclpy、sensor_msgs_py、NumPy、PyYAML。
"""
import os
import yaml
import numpy as np
import rclpy
from rclpy.node import Node
from std_msgs.msg import Header
from sensor_msgs.msg import PointCloud2
from sensor_msgs_py import point_cloud2

# 在闭区间内均匀采样，包含两端点，采样间距不超过 res（要求 a < b、res > 0）。
def axis(a,b,res):
    return np.linspace(a,b,max(2,int(np.ceil((b-a)/res))+1),dtype=np.float32)

# 按配置生成各障碍物的实体采样点，返回形状为 (N, 3) 的 float32 数组。
def build_points(cfg):
    """按米制地图配置生成 world 下 Nx3 点；拒绝非法尺寸或未知类型。"""
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
            raise ValueError(f"Unsupported obstacle type: {ob['type']}")
        parts.append(pts.astype(np.float32))
    # 合并所有障碍物；无障碍物时保留合法的空点云形状。
    return np.concatenate(parts) if parts else np.empty((0,3),dtype=np.float32)

class MapPublisher(Node):
    def __init__(self):
        super().__init__('custom_map_publisher')
        self.declare_parameter('map_file',''); self.declare_parameter('cloud_topic','/map_generator/global_cloud')
        with open(os.path.expanduser(self.get_parameter('map_file').value),encoding='utf-8') as f: cfg=yaml.safe_load(f)
        self.points=build_points(cfg); self.frame=cfg['map'].get('frame_id','world')
        self.pub=self.create_publisher(PointCloud2,self.get_parameter('cloud_topic').value,1); self.create_timer(1.,self.publish)
    def publish(self):
        """使用节点时钟标记静态点云，周期发布以支持后加入的订阅者。"""
        h=Header(stamp=self.get_clock().now().to_msg(),frame_id=self.frame); self.pub.publish(point_cloud2.create_cloud_xyz32(h,self.points.tolist()))
def main():
    rclpy.init(); n=MapPublisher(); rclpy.spin(n); n.destroy_node(); rclpy.shutdown()
if __name__=='__main__': main()
