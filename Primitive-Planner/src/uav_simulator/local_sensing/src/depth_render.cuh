/**
 * @file depth_render.cuh
 * 文件作用：声明 GPU 深度渲染参数与 DepthRender 数据上传、位姿渲染接口。
 * 调用关系：遗留 pcl_render_node.cpp、euroc.cpp 调用；实现位于 depth_render.cu。
 * 输入：米制点云、像素制相机内参、图像尺寸及世界到相机变换。
 * 输出：毫米制整数深度缓冲。
 * 运行模式：CUDA 工具类，当前 ROS 2 CPU 感知节点不使用此接口。
 * 主要依赖：CUDA runtime、device_image.cuh、cuda_exception.cuh、helper_math.h。
 */
#ifndef DEPTH_RENDER_CUH
#define DEPTH_RENDER_CUH

#include <cuda_runtime.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <iostream>
//#include <Eigen/Eigen>

#include "device_image.cuh"
#include "cuda_exception.cuh"
#include "helper_math.h"

using namespace std;
//using namespace Eigen;

struct Parameter
{
	int point_number;
	float fx, fy, cx, cy;
	int width, height;
	float r[3][3];
	float t[3];
};

class DepthRender
{
public:
	DepthRender();
	void set_para(float _fx, float _fy, float _cx, float _cy, int _width, int _height);
	~DepthRender();
	void set_data(vector<float> &cloud_data);
	//void render_pose( Matrix3d &rotation, Vector3d &translation, int *host_ptr);
	//void render_pose( Matrix4d &transformation, int *host_ptr);
	void render_pose( double * transformation, int *host_ptr);
	
private:
	int cloud_size;

	//data
	float3 *host_cloud_ptr;
	float3 *dev_cloud_ptr;
	bool has_devptr;

	//camera
	Parameter parameter;
	Parameter* parameter_devptr;	
};

#endif