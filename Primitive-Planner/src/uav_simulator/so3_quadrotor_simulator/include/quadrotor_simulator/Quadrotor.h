/**
 * @file Quadrotor.h
 * 文件作用：声明四旋翼状态、物理参数、转速输入和动力学积分接口。
 * 调用关系：Quadrotor.cpp 实现，动力学测试和完整 ROS 2 模拟器调用。
 * 输入：电机转速、外力/力矩、初始状态和秒制积分步长；状态含世界系位置/速度与机体姿态。
 * 输出：积分后的位姿、速度、角速度与电机转速；米、秒、kg 制。
 * 运行模式：独立 C++ 动力学模型。
 * 主要依赖：Eigen、Boost array。
 */
#ifndef __QUADROTOR_SIMULATOR_QUADROTOR_H__
#define __QUADROTOR_SIMULATOR_QUADROTOR_H__

#include <Eigen/Core>
#include <boost/array.hpp>

namespace QuadrotorSimulator
{

class Quadrotor
{
public:
  struct State
  {
    Eigen::Vector3d x;
    Eigen::Vector3d v;
    Eigen::Matrix3d R;
    Eigen::Vector3d omega;
    Eigen::Array4d  motor_rpm;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
  };

  Quadrotor();

  const Quadrotor::State& getState(void) const;

  void setState(const Quadrotor::State& state);

  void setStatePos(const Eigen::Vector3d& Pos);

  double getMass(void) const;
  void setMass(double mass);

  double getGravity(void) const;
  void setGravity(double g);

  const Eigen::Matrix3d& getInertia(void) const;
  void setInertia(const Eigen::Matrix3d& inertia);

  double getArmLength(void) const;
  void setArmLength(double d);

  double getPropRadius(void) const;
  void setPropRadius(double r);

  double getPropellerThrustCoefficient(void) const;
  void setPropellerThrustCoefficient(double kf);

  double getPropellerMomentCoefficient(void) const;
  void setPropellerMomentCoefficient(double km);

  double getMotorTimeConstant(void) const;
  void setMotorTimeConstant(double k);

  const Eigen::Vector3d& getExternalForce(void) const;
  void setExternalForce(const Eigen::Vector3d& force);

  const Eigen::Vector3d& getExternalMoment(void) const;
  void setExternalMoment(const Eigen::Vector3d& moment);

  double getMaxRPM(void) const;
  void setMaxRPM(double max_rpm);

  double getMinRPM(void) const;
  void setMinRPM(double min_rpm);

  // Inputs are desired RPM for the motors
  // Rotor numbering is:
  //   *1*    Front
  // 3     4
  //    2
  // with 1 and 2 clockwise and 3 and 4 counter-clockwise (looking from top)
  void setInput(double u1, double u2, double u3, double u4);

  // Runs the actual dynamics simulation with a time step of dt
  void step(double dt);

  // For internal use, but needs to be public for odeint
  typedef boost::array<double, 22> InternalState;
  void operator()(const Quadrotor::InternalState& x,
                  Quadrotor::InternalState&       dxdt, const double /* t */);

  Eigen::Vector3d getAcc() const;

private:
  void updateInternalState(void);

  double          alpha0; // AOA
  double          g_;     // gravity
  double          mass_;
  Eigen::Matrix3d J_; // Inertia
  double          kf_;
  double          km_;
  double          prop_radius_;
  double          arm_length_;
  double          motor_time_constant_; // unit: sec
  double          max_rpm_;
  double          min_rpm_;

  Quadrotor::State state_;

  Eigen::Vector3d acc_;

  Eigen::Array4d  input_;
  Eigen::Vector3d external_force_;
  Eigen::Vector3d external_moment_;

  InternalState internal_state_;
};
}
#endif
