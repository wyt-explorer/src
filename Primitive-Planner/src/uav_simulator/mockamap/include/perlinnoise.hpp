/**
 * @file perlinnoise.hpp
 * 文件作用：声明带种子置换表的三维 Perlin 噪声生成器。
 * 调用关系：Maps::perlin3D() 调用 PerlinNoise::noise() 构造地图噪声场。
 * 输入：随机种子或默认置换表，以及三维无量纲采样坐标。
 * 输出：连续噪声值，供地图阈值筛选使用。
 * 运行模式：纯 C++ 数学辅助模块。
 * 主要依赖：C++ 标准库 vector、random、cmath。
 */
#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

#include <vector>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see
// http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT
// PRESENT IN THE ORIGINAL IMPLEMENTATION)

class PerlinNoise
{
  // The permutation vector
  std::vector<int> p;

public:
  // Initialize with the reference values for the permutation vector
  PerlinNoise();
  // Generate a new permutation vector based on the value of seed
  PerlinNoise(unsigned int seed);
  // Get a noise value, for 2D images z can have any value
  double noise(double x, double y, double z);

private:
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};

#endif // PERLINNOISE_HPP
