/**
 * @file comm_types.h
 * 文件作用：定义飞控通信类型码和定点字段布局。
 * 调用关系：encode_msgs.cpp 和 decode_msgs.cpp 使用这些结构体打包、解包。
 * 输入：推力、姿态、增益和状态量；缩放因子见字段注释。
 * 输出：SO3_CMD_INPUT、STATUS_DATA、OUTPUT_DATA 等协议结构。
 * 运行模式：遗留飞控协议定义，当前 ROS 2 消息生成不编译编解码实现。
 * 主要依赖：定宽整数类型及 C++ 结构体/位域。
 */
#ifndef __QUADROTOR_MSGS_COMM_TYPES_H__
#define __QUADROTOR_MSGS_COMM_TYPES_H__

#define TYPE_SO3_CMD 's'
struct SO3_CMD_INPUT
{
  // Scaling factors when decoding
  int16_t force[3]; // /500
  int8_t des_qx, des_qy, des_qz, des_qw; // /125
  uint8_t kR[3]; // /50
  uint8_t kOm[3]; // /100
  int16_t cur_yaw; // /1e4
  int16_t kf_correction; // /1e11;
  uint8_t angle_corrections[2]; // roll,pitch /2500
  uint8_t enable_motors:1;
  uint8_t use_external_yaw:1;
  uint8_t seq;
};

#define TYPE_STATUS_DATA 'c'
struct STATUS_DATA
{
  uint16_t loop_rate;
  uint16_t voltage;
  uint8_t seq;
};

#define TYPE_OUTPUT_DATA 'd'
struct OUTPUT_DATA
{
  uint16_t loop_rate;
  uint16_t voltage;
  int16_t roll, pitch, yaw;
  int16_t ang_vel[3];
  int16_t acc[3];
  int16_t dheight;
  int32_t height;
  int16_t mag[3];
  uint8_t radio[8];
  //uint8_t rpm[4];
  uint8_t seq;
};

#define TYPE_TRPY_CMD 'p'
struct TRPY_CMD
{
  int16_t thrust;
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
  int16_t current_yaw;
  uint8_t enable_motors:1;
  uint8_t use_external_yaw:1;
};

#define TYPE_PPR_OUTPUT_DATA 't'
struct PPR_OUTPUT_DATA
{
  uint16_t time;
  int16_t des_thrust;
  int16_t des_roll;
  int16_t des_pitch;
  int16_t des_yaw;
  int16_t est_roll;
  int16_t est_pitch;
  int16_t est_yaw;
  int16_t est_angvel_x;
  int16_t est_angvel_y;
  int16_t est_angvel_z;
  int16_t est_acc_x;
  int16_t est_acc_y;
  int16_t est_acc_z;
  uint16_t pwm1;
  uint16_t pwm2;
  uint16_t pwm3;
  uint16_t pwm4;
};

#define TYPE_PPR_GAINS 'g'
struct PPR_GAINS
{
  int16_t Kp;
  int16_t Kd;
  int16_t Kp_yaw;
  int16_t Kd_yaw;
};
#endif
