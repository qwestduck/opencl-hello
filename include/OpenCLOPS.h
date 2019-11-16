#ifndef OPENCLOPS_H_
#define OPENCLOPS_H_

#include <vector>

#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_CL_1_2_DEFAULT_BUILD
#include <CL/cl2.hpp>

class OpenCLOPS {
    cl::Program program;
    cl::Context context;
    cl::CommandQueue queue;

public:
    OpenCLOPS();
    void print_gpu_devices();
    void print_platforms();
    std::vector<double> vec_add(std::vector<double>&,std::vector<double>&);
};

#endif
