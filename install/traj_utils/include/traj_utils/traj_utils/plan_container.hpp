/**
 * @file plan_container.hpp
 * 文件作用：定义单机轨迹记录 LocalTrajData 和群体轨迹容器 SwarmTrajData。
 * 调用关系：PPReplanFSM 写入轨迹，PPPlannerManager 读取以查询机间碰撞。
 * 输入：无人机 ID、起始时间、时长及世界系位置采样序列。
 * 输出：内存数据结构；位置为米，时间为秒，轨迹使用方按 0.01 秒采样解释。
 * 运行模式：纯 C++ 数据容器，无节点或消息发布。
 * 主要依赖：Eigen、vector。
 */
#ifndef _PLAN_CONTAINER_H_
#define _PLAN_CONTAINER_H_

#include <Eigen/Eigen>
#include <vector>

using std::vector;

namespace primitive_planner
{

  struct LocalTrajData
  {
    int drone_id{-1};
    double start_time{0.0};
    double traj_duration{0.0};
    std::vector<Eigen::Vector3d> traj_pos;
  };

  typedef std::vector<LocalTrajData> SwarmTrajData;


} // namespace primitive_planner

#endif