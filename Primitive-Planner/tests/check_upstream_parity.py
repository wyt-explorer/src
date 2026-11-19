#!/usr/bin/env python3
"""文件作用：对比固定上游版本的算法体，验证迁移没有替换核心规划和控制公式。
调用关系：开发者传入上游只读检出路径后运行；无需 ROS 节点或网络访问。
输入：--upstream 指定 cab5aa2 上游检出，及本地 Primitive-Planner 源码。
输出：逐项差异或通过结果；不比较消息 API、时钟、日志和启动语法。
运行模式：离线 Python 3 差分检查。
主要依赖：argparse、pathlib、re、ast。
"""
import argparse
import ast
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LEXER = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/|\s+|[^\s]', re.S)


def uncomment(source):
    return ''.join(m.group() for m in LEXER.finditer(source) if not m.group().startswith(('//', '/*')))


def function(source, name):
    """提取函数体；注释与字符串不会影响大括号边界。"""
    source = uncomment(source)
    definition = re.search(re.escape(name) + r'\([^;{}]*?\)\s*(?:const\s*)?\{', source, re.S)
    if definition is None:
        raise ValueError('Missing definition: ' + name)
    first = definition.end() - 1
    level = 0
    for match in LEXER.finditer(source, first):
        token = match.group()
        if token == '{':
            level += 1
        elif token == '}':
            level -= 1
            if level == 0:
                return source[first:match.end()]
    raise ValueError(name)


def normalized(source):
    source = re.sub(r'#include\s*<ros/ros.h>', '', source)
    source = source.replace('ros::Time::now()', 'clock_now()').replace('nh_->now()', 'clock_now()').replace('node->now()', 'clock_now()').replace('.toSec()', '.seconds()')
    source = source.replace('ros::Time', 'rclcpp::Time')
    source = source.replace('nav_msgs::Odometry', 'nav_msgs::msg::Odometry').replace('sensor_msgs::Imu', 'sensor_msgs::msg::Imu')
    source = source.replace('cmd->kR[', 'cmd->k_r[').replace('cmd->kOm[', 'cmd->k_om[')
    source = re.sub(r'(ROS_(?:WARN|ERROR|INFO)|(?:std::)?fprintf)\([^;]*;', '', source)
    for package in ('std_msgs', 'quadrotor_msgs', 'traj_utils'):
        source = re.sub(package + r'::(?!msg::)([A-Z]\w*)', package + r'::msg::\1', source)
    source = re.sub(r'(\w+)\.publish\(', r'\1->publish(', source)
    return ''.join(m.group() for m in LEXER.finditer(source) if not m.group().isspace())


def python_tree(source):
    tree = ast.parse(source)
    for node in ast.walk(tree):
        if isinstance(node, (ast.Module, ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
            if node.body and isinstance(node.body[0], ast.Expr) and isinstance(node.body[0].value, ast.Constant) and isinstance(node.body[0].value.value, str):
                node.body.pop(0)
    return ast.dump(tree, include_attributes=False)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--upstream', required=True, type=Path)
    upstream = parser.parse_args().upstream
    checks = {
        'src/planner/plan_manage/src/planner_manager.cpp': [
            'PPPlannerManager::labelObsCollisionPaths', 'PPPlannerManager::labelAgentCollisionPaths',
            'PPPlannerManager::scorePaths', 'PPPlannerManager::readPathList', 'PPPlannerManager::readPathAll',
            'PPPlannerManager::readCorrespondences', 'PPPlannerManager::readAgentCorrespondences', 'PPPlannerManager::trajReplan'],
        'src/planner/plan_manage/src/traj_server.cpp': ['calculate_yaw', 'limit_yaw', 'yaw_diff', 'trajCallback', 'polytrajCallback', 'cmdCallback'],
        'src/uav_simulator/fake_drone/src/poscmd_2_odom.cpp': ['pubOdom'],
        'src/uav_simulator/so3_control/src/SO3Control.cpp': ['SO3Control::calculateControl'],
        'src/uav_simulator/so3_quadrotor_simulator/src/quadrotor_simulator_so3.cpp': ['getControl', 'stateToOdomMsg', 'quadToImuMsg'],
    }
    failures = []
    count = 0
    for relative, names in checks.items():
        old, new = (upstream / relative).read_text(), (ROOT / relative).read_text()
        for name in names:
            if normalized(function(old, name)) != normalized(function(new, name)):
                failures.append(f'{relative}: {name}')
            count += 1
    for relative in ['src/scripts/swarm_path.py', 'src/uav_simulator/so3_quadrotor_simulator/src/dynamics/Quadrotor.cpp']:
        old, new = (upstream / relative).read_text(), (ROOT / relative).read_text()
        # 上游的 -1 起始路径 ID 与其读取器要求的非负连续 ID 不兼容；
        # 只允许这一个已有修正，几何、TOPPRA 约束和体素对应公式仍须一致。
        if relative.endswith('.py'):
            old = old.replace('path_id = -1', 'path_id = 0')
        equal = python_tree(old) == python_tree(new) if relative.endswith('.py') else normalized(uncomment(old)) == normalized(uncomment(new))
        if not equal:
            failures.append(relative)
        count += 1
    if failures:
        raise SystemExit('Upstream parity differences:\n' + '\n'.join(failures))
    print(f'PASS: {count} upstream algorithm checks (planning, yaw, control allocation, dynamics, primitive generation).')


if __name__ == '__main__':
    main()
