"""文件作用：生成 circle 布局的目标参数 XML 启动文件。
调用关系：离线运行脚本；生成配置供 random_goals_node 使用。
输入：命令行目标数量；在半径 4 米、高度 1 米的圆周上采样目标。
输出：脚本 fname 指定路径的 random_goals.launch.xml，包含 goalN 坐标参数。
运行模式：离线 Python 工具，生成的是 ROS 2 launch XML。
主要依赖：Python 标准库、NumPy。
"""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "scripts"))
from ros2_launch_xml import convert_file
import numpy as np
import os
import math

script_dir = os.path.dirname(os.path.abspath(__file__))
fname = str(Path(script_dir) / "random_goals.launch.xml")
def main(argv):
    goal_num_in = int(argv[1])
    height = 1.0
    theta = np.linspace(-3.14159, 3.14159, goal_num_in+1)
    r = 4.0
    file = open(fname, "w")

    str_head = "<launch>\n\
    <node pkg=\"random_goals\" name=\"random_goals_node\" type=\"random_goals_node\" output=\"screen\">\n\
        <param name=\"drone_num\" value=\"10\"/>\n\
        <param name=\"goal_num\" value=\"{goal_num}\"/>\n".format(goal_num=goal_num_in)
    file.write(str_head)

    for i in range(goal_num_in):
        str_goals = "        <rosparam param=\"goal{goal_id}\">[{p0},{p1},{p2}]</rosparam>\n".format(goal_id=i, p0=r * math.sin(theta[i]), p1=r * math.cos(theta[i]), p2=height, \
        target0_x=-r * math.sin(theta[i]), target0_y=-r * math.cos(theta[i]), target0_z=height) 
        file.write(str_goals)

    str_tail = "    </node>\n</launch>"
    file.write(str_tail)
    
    file.close()
    convert_file(fname)

if __name__ == '__main__':
    main(sys.argv)

