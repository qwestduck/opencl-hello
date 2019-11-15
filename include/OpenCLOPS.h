#ifndef OPENCLOPS_H_
#define OPENCLOPS_H_

#include <vector>

#define CL_TARGET_OPENCL_VERSION 120
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class OpenCLOPS {
    std::vector<cl::Platform> platform;
    std::vector<cl::Device> device;
    cl::Program program;
    cl::Context context;
    cl::CommandQueue queue;

public:
    OpenCLOPS();
    void print_gpu_device_info();
    int get_double_precision_gpu_device();
    std::vector<double> vec_add(std::vector<double>&,std::vector<double>&);
};

#endif
