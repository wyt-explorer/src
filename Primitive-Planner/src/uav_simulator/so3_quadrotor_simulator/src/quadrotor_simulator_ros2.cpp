/**
 * @file quadrotor_simulator_ros2.cpp
 * 文件作用：保留旧 ROS 2 模拟器源码路径，转用完整 SO3 动力学实现。
 * 调用关系：包含 quadrotor_simulator_so3.cpp；CMake 直接构建后者。
 * 输入：同完整实现的 SO3 指令、扰动力和物理参数。
 * 输出：同完整实现的 odom、imu。
 * 运行模式：兼容入口，无固定状态模拟逻辑。
 * 主要依赖：quadrotor_simulator_so3.cpp。
 */
#include "quadrotor_simulator_so3.cpp"
