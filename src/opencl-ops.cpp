#include <iostream>
#include <vector>
#include <string>

#include "OpenCLOPS.h"

static const std::string source {R"CLC(
    #if defined(cl_khr_fp64)
    #  pragma OPENCL EXTENSION cl_khr_fp64: enable
    #else
    #  error double precision is not supported
    #endif
    kernel void add(
           ulong n,
           global const double *a,
           global const double *b,
           global double *c
           )
    {
        size_t i = get_global_id(0);
        if (i < n) {
           c[i] = a[i] + b[i];
        }
    }
)CLC"};

OpenCLOPS::OpenCLOPS() {
    // Find NVidia platform
    std::vector<cl::Platform> platform;
    cl::Platform nvp;
    bool nvidia_platform_present = false;

    cl::Platform::get(&platform);
    for(auto p = platform.begin(); p != platform.end(); p++) {
        if(p->getInfo<CL_PLATFORM_NAME>() == "NVIDIA CUDA") {
            nvp = cl::Platform(*p);

            nvidia_platform_present = true;
            break;
        }
    }

    if(!nvidia_platform_present) {
        std::cerr << "No NVidia OpenCL platform found." << std::endl;
    }

    // Build a context of all double precision gpus from the nvidia platform
    std::vector<cl::Device> nvp_gpus;
    std::vector<cl::Device> nvp_double_gpus;
    nvp.getDevices(CL_DEVICE_TYPE_GPU, &nvp_gpus);

    for(auto d = nvp_gpus.begin(); d != nvp_gpus.end(); d++) {
        if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

        std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

        if (ext.find("cl_khr_fp64") == std::string::npos) {
            continue;
        }

        nvp_double_gpus.push_back(*d);
    }

    if (nvp_double_gpus.empty()) {
        std::cerr << "Nvidia GPUs with double precision not found." << std::endl;
    }

    context = cl::Context(nvp_double_gpus);

    // Create command queue.
    queue = cl::CommandQueue(context, nvp_double_gpus[0]);

    // Compile OpenCL program for found device.
    program = cl::Program(context, cl::Program::Sources(1, source));

    try {
        program.build(nvp_double_gpus);
    } catch (const cl::Error&) {
        std::cerr
        << "OpenCL compilation error" << std::endl
        << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(nvp_double_gpus[0])
        << std::endl;
    }
}

void OpenCLOPS::print_gpu_devices() {
    std::vector<cl::Platform> platform;
    std::vector<cl::Device> pldev;

    cl::Platform::get(&platform);
    for(auto p = platform.begin(); p != platform.end(); p++) {
        p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

        for(auto d = pldev.begin(); d != pldev.end(); d++) {
            std::cout << d->getInfo<CL_DEVICE_NAME>() << std::endl;
        }
    }
}

void OpenCLOPS::print_platforms() {
    std::vector<cl::Platform> platform;

    cl::Platform::get(&platform);
    for(auto p = platform.begin(); p != platform.end(); p++) {
        std::cout << "Platform Name: " << p->getInfo<CL_PLATFORM_NAME>() << std::endl;
        std::cout << "Platform Vendor: " << p->getInfo<CL_PLATFORM_VENDOR>() << std::endl;
    }
}

std::vector<double> OpenCLOPS::vec_add(std::vector<double> &a, std::vector<double> &b) {
    cl::Kernel add(program, "add");

    size_t N = a.size();

    std::vector<double> c(N);

    // Allocate device buffers and transfer input data to device.
    cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        a.size() * sizeof(double), a.data());

    cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        b.size() * sizeof(double), b.data());

    cl::Buffer C(context, CL_MEM_READ_WRITE,
        c.size() * sizeof(double));

    // Set kernel parameters.
    add.setArg(0, static_cast<cl_ulong>(N));
    add.setArg(1, A);
    add.setArg(2, B);
    add.setArg(3, C);
    
    // Launch kernel on the compute device.
    queue.enqueueNDRangeKernel(add, cl::NullRange, N, cl::NullRange);

    // Get result back to host.
    queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(double), c.data());

    return c;
}
