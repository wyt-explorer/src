/**
 * @file SO3Control.h
 * 文件作用：声明基于位置、速度误差及期望加速度的 SO3 控制计算接口。
 * 调用关系：SO3 ROS 2 节点及遗留 nodelet 调用 calculateControl()，再读取推力和姿态。
 * 输入：世界系状态/期望位置、速度、加速度、偏航及增益；米、秒、弧度制，质量 kg。
 * 输出：期望力向量（N）和姿态四元数。
 * 运行模式：独立 C++ 控制算法，无 ROS 回调。
 * 主要依赖：Eigen。
 */
#ifndef __SO3_CONTROL_H__
#define __SO3_CONTROL_H__

#include <Eigen/Geometry>

class SO3Control
{
public:
  SO3Control();

  void setMass(const double mass);
  void setGravity(const double g);
  void setPosition(const Eigen::Vector3d& position);
  void setVelocity(const Eigen::Vector3d& velocity);
  void setAcc(const Eigen::Vector3d& acc);

  void calculateControl(const Eigen::Vector3d& des_pos,
                        const Eigen::Vector3d& des_vel,
                        const Eigen::Vector3d& des_acc, const double des_yaw,
                        const double des_yaw_dot, const Eigen::Vector3d& kx,
                        const Eigen::Vector3d& kv);

  const Eigen::Vector3d&    getComputedForce(void);
  const Eigen::Quaterniond& getComputedOrientation(void);


  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
  // Inputs for the controller
  double          mass_;
  double          g_;
  Eigen::Vector3d pos_;
  Eigen::Vector3d vel_;
  Eigen::Vector3d acc_;

  // Outputs of the controller
  Eigen::Vector3d    force_;
  Eigen::Quaterniond orientation_;
};

#endif
