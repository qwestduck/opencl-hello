#ifndef OPENCLOPS_H_
#define OPENCLOPS_H_

#include <vector>

#define CL_TARGET_OPENCL_VERSION 120
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

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
