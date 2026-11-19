/**
 * @file cuda_exception.cuh
 * 文件作用：封装 CUDA 错误码与上下文描述为 C++ 异常。
 * 调用关系：DeviceImage 和 DepthRender 在 CUDA 分配或拷贝失败时抛出。
 * 输入：错误描述字符串和 cudaError。
 * 输出：CudaException 异常及 what() 描述接口。
 * 运行模式：CUDA 辅助类型，保留原 REMODE 许可证。
 * 主要依赖：CUDA runtime、exception、sstream。
 */
// This file is part of REMODE - REgularized MOnocular Depth Estimation.
//
// Copyright (C) 2014 Matia Pizzoli <matia dot pizzoli at gmail dot com>
// Robotics and Perception Group, University of Zurich, Switzerland
// http://rpg.ifi.uzh.ch
//
// REMODE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or any later version.
//
// REMODE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef RMD_CUDA_EXCEPTION_CUH_
#define RMD_CUDA_EXCEPTION_CUH_

#include <sstream>
#include <cuda_runtime.h>


struct CudaException : public std::exception
{
  CudaException(const std::string& what, cudaError err)
    : what_(what), err_(err) {}
  virtual ~CudaException() throw() {}
  virtual const char* what() const throw()
  {
    std::stringstream description;
    description << "CudaException: " << what_ << std::endl;
    if(err_ != cudaSuccess)
    {
      description << "cudaError code: " << cudaGetErrorString(err_);
      description << " (" << err_ << ")" << std::endl;
    }
    return description.str().c_str();
  }
  std::string what_;
  cudaError err_;
};

#endif /* RMD_CUDA_EXCEPTION_CUH_ */
