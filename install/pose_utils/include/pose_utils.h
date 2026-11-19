/**
 * @file pose_utils.h
 * 文件作用：声明姿态转换、位姿组合和滤波状态/雅可比计算接口。
 * 调用关系：odom_visualization 等模块包含本头文件；实现在 pose_utils.cpp。
 * 输入：Armadillo 向量/矩阵、位姿、输入量和时间步长；欧拉角为弧度。
 * 输出：转换后的姿态、组合位姿、预测状态及雅可比矩阵。
 * 运行模式：纯 C++ 数学库，无 ROS 订阅或发布。
 * 主要依赖：Armadillo、C++ 标准库。
 */
#ifndef POSE_UTILS_H
#define POSE_UTILS_H

#include <iostream>
#include "armadillo"

#define PI 3.14159265359
#define NUM_INF 999999.9

using namespace arma;
using namespace std;

// Rotation ---------------------
mat ypr_to_R(const colvec& ypr);

mat yaw_to_R(double yaw);

colvec R_to_ypr(const mat& R);

mat quaternion_to_R(const colvec& q);

colvec R_to_quaternion(const mat& R);

colvec quaternion_mul(const colvec& q1, const colvec& q2);

colvec quaternion_inv(const colvec& q);

// General Pose Update ----------
colvec pose_update(const colvec& X1, const colvec& X2);

colvec pose_inverse(const colvec& X);

colvec pose_update_2d(const colvec& X1, const colvec& X2);

colvec pose_inverse_2d(const colvec& X);

// For Pose EKF -----------------
mat Jplus1(const colvec& X1, const colvec& X2);

mat Jplus2(const colvec& X1, const colvec& X2);

// For IMU EKF ------------------
colvec state_update(const colvec& X, const colvec& U, double dt);

mat jacobianF(const colvec& X, const colvec& U, double dt);

mat jacobianU(const colvec& X, const colvec& U, double dt);

colvec state_measure(const colvec& X);

mat jacobianH();

#endif
