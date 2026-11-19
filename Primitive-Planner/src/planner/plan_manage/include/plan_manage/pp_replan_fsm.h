/**
 * @file pp_replan_fsm.h
 * 文件作用：声明重规划状态机、消息回调和轨迹缓存。
 * 调用关系：primitive_planner_node 创建 PPReplanFSM；其调用 PPPlannerManager 筛选基元。
 * 输入：里程计、触发、目标、群体轨迹和轨迹服务反馈；位置使用世界系，米、秒制。
 * 输出：基元选择、心跳、停车、偏航及多项式轨迹消息接口。
 * 运行模式：ROS 2；共享内存通信由 USE_SHARED_MEMORY 编译开关选择。
 * 主要依赖：rclcpp、Eigen、planner_manager、traj_utils、quadrotor_msgs。
 */
#ifndef _PP_REPLAN_FSM_H_
#define _PP_REPLAN_FSM_H_

#include <Eigen/Eigen>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <plan_manage/planner_manager.h>
#include <traj_utils/planning_visualization.h>
#include <nav_msgs/msg/odometry.hpp>
#include <std_msgs/msg/empty.hpp>
#include <std_msgs/msg/int8_multi_array.hpp>
// #include <traj_utils/primitiveTraj.h>
#include <quadrotor_msgs/msg/goal_set.hpp>
#include <traj_utils/msg/swarm_primitive_traj.hpp>
#include <traj_utils/msg/polynomial.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <quadrotor_msgs/msg/position_command.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include <plan_manage/CSVLogger.h>

namespace primitive_planner
{
    class PPReplanFSM
    {
        public:
            PPReplanFSM() {}
            ~PPReplanFSM();

            void init(rclcpp::Node::SharedPtr nh);

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        
        private:
            enum FSM_EXEC_STATE
            {
            INIT,
            WAIT_TARGET,
            GEN_NEW_TRAJ,
            REPLAN_TRAJ,
            EXEC_TRAJ,
            EMERGENCY_STOP,
            APPROACH_GOAL,
            CRASH_RECOVER
            };

            /* planning utils */
            PPPlannerManager::Ptr planner_manager_;
            PlanningVisualization::Ptr visualization_;

            FSM_EXEC_STATE exec_state_;

            bool flag_realworld_experiment_, have_trigger_, have_odom_, have_target_, have_log_files_;

            rclcpp::TimerBase::SharedPtr exec_timer_;
            rclcpp::SubscriptionBase::SharedPtr trigger_sub_, odom_sub_, mandatory_stop_sub_, select_path_end_sub_, cmd_sub_, broadcast_primitive_sub_, waypoint_sub_;

            rclcpp::Node::SharedPtr nh_;
            rclcpp::Publisher<traj_utils::msg::SwarmPrimitiveTraj>::SharedPtr path_id_pub_, broadcast_primitive_pub_;
            rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr stop_pub_, global_pub_;
            rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr heartbeat_pub_;
            rclcpp::Publisher<traj_utils::msg::Polynomial>::SharedPtr poly_pub_;
            rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr yaw_cmd_pub_;

            double waypoints_[10][3];
            int waypoint_num_;
            int goal_id_;
            int target_type_; // 1 mannual select, 2 hard code
            std::vector<Eigen::Vector3d> all_goal_;
            Eigen::Vector3d global_goal_;
            Eigen::Vector3d odom_pos_, odom_vel_, odom_x_dir_;
            Eigen::Quaterniond odom_q_;
            double odom_yaw_;

            vector<vector<std::pair<double, vector<Eigen::Vector3d>>>> primitve_pos_;

            Eigen::Vector3d latest_safe_pt_;
            bool have_latest_safe_pt_;

            const int TOTAL_DRONE_NUM_ = 1000;
            typedef std::pair<bool, traj_utils::msg::SwarmPrimitiveTraj> SharedMemory;
            SharedMemory* swarm_traj_ptr_ = nullptr;
            void* shared_memory_ = nullptr;
            int shm_id_ = -1;

            CSVLogger odom_logger_, computing_time_logger_;

            Eigen::Vector3d start_pt_, start_v_;
            Eigen::Quaterniond start_q_;

            double no_replan_thresh_, replan_thresh_;
            rclcpp::Time start_time_;

            bool enable_fail_safe_, flag_escape_emergency_;
            bool trigger_pending_{false}, target_yaw_pending_{false};
            rclcpp::Time trigger_ready_time_{0, 0, RCL_ROS_TIME};
            double target_yaw_{0.0};
            
            // crash recovery
            rclcpp::Time crash_rec_start_time_;
            bool flag_wait_crash_rec_;
            int crash_rec_stage_;
            double final_yaw_des_;
            bool flag_pub_first_yaw_;
            int yaw_cmd_count_;
            vector<double> past_yaw_list_;
            int keep_fails_;

            Eigen::Vector3d select_path_end_, select_path_end_last_;

            std::string primitiveFolder_;

            LocalTrajData myself_traj_;

            /* state machine functions */
            void execFSMCallback();
            void changeFSMExecState(FSM_EXEC_STATE new_state, std::string pos_call);
            void printFSMExecState();

            bool planPrimitive(bool first_plan, double xV_offset = 0.0);
            bool callEmergencyStop(Eigen::Vector3d stop_pos);
            void mandatoryStopCallback(const std_msgs::msg::Empty &msg);
            void pubPolyTraj(const Eigen::Vector3d &start_p, const Eigen::Vector3d &start_v, const Eigen::Vector3d &end_p, const double dura);

            void triggerCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
            void RecvBroadcastPrimitiveCallback(const traj_utils::msg::SwarmPrimitiveTraj::ConstSharedPtr &msg);
            void odometryCallback(const nav_msgs::msg::Odometry::ConstSharedPtr &msg);
            void pathEndCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);
            void cmdCallback(const quadrotor_msgs::msg::PositionCommand::SharedPtr cmd);
            void waypointCallback(const quadrotor_msgs::msg::GoalSet::SharedPtr msg);

            bool readLocalTrajPos(Eigen::Vector3d& start_pos, int& vel_id, Eigen::Matrix<double, 3, 3>& Rwv, std::vector<int>& path_id, std::vector<Eigen::Vector3d>& traj_pos, double& traj_duration);
            bool checkCollision(int recv_id);
            bool readPrimitivePos();
    };

} // namespace primitive_planner

#endif