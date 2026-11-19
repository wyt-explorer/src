/**
 * @file plugin_probe.cpp
 * 文件作用：检查两个迁移后的 RViz2 工具可由 pluginlib 发现、加载和实例化。
 * 调用关系：创建 QApplication 后，通过 rviz_common::Tool 工厂构造各工具。
 * 输入：AMENT_PREFIX_PATH 中已安装的插件描述与共享库。
 * 输出：插件名称及通过信息；加载异常时返回非零退出码。
 * 运行模式：QT_QPA_PLATFORM=offscreen，无窗口、无鼠标操作验证。
 * 主要依赖：Qt Widgets、pluginlib、rviz_common、rclcpp。
 */
#include <QApplication>
#include <pluginlib/class_loader.hpp>
#include <rviz_common/tool.hpp>
#include <rclcpp/rclcpp.hpp>
#include <iostream>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  rclcpp::init(argc, argv);
  try {
    pluginlib::ClassLoader<rviz_common::Tool> loader("rviz_common", "rviz_common::Tool");
    for (const auto* name : {"rviz_plugins/Goal3DTool", "selected_points_publisher/SelectedPointsPublisher"}) {
      auto tool = loader.createSharedInstance(name);
      if (!tool) return 1;
      std::cout << name << ": PASS" << std::endl;
    }
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
