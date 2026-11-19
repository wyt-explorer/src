#!/usr/bin/env python3
"""文件作用：重复生成上游机群起终点、启动规划、触发运行并结束本轮进程。
调用关系：调用 gen_start_goal.py、gen_primitive_planner_launch.py 和 pub_trigger.sh。
输入：实验次数、无人机数量、运行时长、预热与轮间等待时间。
输出：生成的起终点/启动配置和各轮 ROS 2 运行日志。
运行模式：ROS 2 批量实验，停止时仅清理本脚本创建的进程组。
主要依赖：Python subprocess、signal、argparse、ros2 launch/topic。
"""
import argparse
from pathlib import Path
import os
import signal
import subprocess
import sys
import time


def stop(process):
    if process.poll() is not None:
        return
    process.send_signal(signal.SIGINT)
    try:
        process.wait(timeout=15)
    except subprocess.TimeoutExpired:
        os.killpg(process.pid, signal.SIGTERM)
        process.wait(timeout=5)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--runs', type=int, default=30)
    parser.add_argument('--drones', type=int, default=80)
    parser.add_argument('--duration', type=float, default=50.)
    parser.add_argument('--warmup', type=float, default=20.)
    parser.add_argument('--cooldown', type=float, default=35.)
    args = parser.parse_args()
    scripts = Path(__file__).resolve().parent
    launch_file = scripts.parent / 'planner/plan_manage/launch/primitive_swarm.launch'
    for index in range(args.runs):
        print(f'Experiment {index + 1}/{args.runs}: {args.drones} drones', flush=True)
        subprocess.run([sys.executable, str(scripts/'gen_start_goal.py'), str(args.drones)], cwd=scripts, check=True)
        subprocess.run([sys.executable, str(scripts/'gen_primitive_planner_launch.py')], cwd=scripts, check=True)
        process = subprocess.Popen(['ros2', 'launch', str(launch_file)], start_new_session=True)
        try:
            time.sleep(args.warmup)
            subprocess.run(['bash', str(scripts/'pub_trigger.sh')], check=True)
            time.sleep(args.duration)
        finally:
            stop(process)
        if index + 1 < args.runs:
            time.sleep(args.cooldown)


if __name__ == '__main__':
    main()
