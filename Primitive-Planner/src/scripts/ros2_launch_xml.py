"""文件作用：将上游离线生成器的启动 XML 转成对应的 ROS 2 启动语法。
调用关系：起终点、随机目标与里程计显示生成器在写出配置时调用。
输入：生成器产生的 ROS 1 XML 文本或文件路径，不执行其中节点。
输出：ROS 2 XML，节点、布局参数和目标坐标保持原值。
运行模式：离线 Python 3，仅适配本仓库生成器涉及的标签和入口。
主要依赖：ElementTree、re、pathlib。
"""
from pathlib import Path
import re
import xml.etree.ElementTree as ET


def convert_text(text):
    text = re.sub(r'\$\(find ([^)]+)\)', r'$(find-pkg-share \1)', text)
    text = text.replace('$(arg ', '$(var ')
    for old, new in [('run_in_sim.xml', 'run_in_sim.launch.py'), ('bridge_udp.launch', 'bridge_udp.launch.py'),
                     ('run_vis_rotate.launch', 'run_vis_rotate.launch.xml')]:
        text = text.replace('/' + old + '"', '/' + new + '"')
    root = ET.fromstring(text)
    for arg in root.findall('arg'):
        if 'value' in arg.attrib:
            arg.set('default', arg.attrib.pop('value'))
    for node in root.iter('node'):
        if 'type' in node.attrib:
            node.set('exec', node.attrib.pop('type'))
        if node.get('pkg') == 'rviz':
            node.set('pkg', 'rviz2')
            node.set('exec', 'rviz2')
        if node.get('pkg') == 'odom_visualization' and node.get('exec') == 'odom_visualization':
            node.set('namespace', node.get('name', 'odom_visualization'))
        for remap in node.findall('remap'):
            for key in ('from', 'to'):
                if remap.get(key, '').startswith('~'):
                    remap.set(key, remap.get(key)[1:])
        for old in list(node.findall('rosparam')):
            if 'param' not in old.attrib:
                raise ValueError('Unsupported rosparam file in generated launch')
            node.append(ET.Element('param', {'name': old.get('param'), 'value': (old.text or '').strip()}))
            node.remove(old)
    ET.indent(root, space='  ')
    return ET.tostring(root, encoding='unicode') + '\n'


def convert_file(path):
    path = Path(path)
    path.write_text(convert_text(path.read_text()))
