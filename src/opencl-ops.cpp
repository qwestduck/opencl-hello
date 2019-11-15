#include <iostream>
#include <vector>

#include "OpenCLOPS.h"

static const char source[] =
    "#if defined(cl_khr_fp64)\n"
    "#  pragma OPENCL EXTENSION cl_khr_fp64: enable\n"
    "#elif defined(cl_amd_fp64)\n"
    "#  pragma OPENCL EXTENSION cl_amd_fp64: enable\n"
    "#else\n"
    "#  error double precision is not supported\n"
    "#endif\n"
    "kernel void add(\n"
    "       ulong n,\n"
    "       global const double *a,\n"
    "       global const double *b,\n"
    "       global double *c\n"
    "       )\n"
    "{\n"
    "    size_t i = get_global_id(0);\n"
    "    if (i < n) {\n"
    "       c[i] = a[i] + b[i];\n"
    "    }\n"
    "}\n";

OpenCLOPS::OpenCLOPS() {
    try {
        // Get list of OpenCL platforms.
        cl::Platform::get(&platform);

        if (platform.empty()) {
            std::cerr << "OpenCL platforms not found." << std::endl;
        }

        get_double_precision_gpu_device();

        context = cl::Context(device);

        // Create command queue.
        queue = cl::CommandQueue(context, device[0]);

        // Compile OpenCL program for found device.
        program = cl::Program(context, cl::Program::Sources(
            1, std::make_pair(source, strlen(source))
            ));

        try {
            program.build(device);
        } catch (const cl::Error&) {
            std::cerr
            << "OpenCL compilation error" << std::endl
            << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
            << std::endl;
        }
    } catch (const cl::Error &err) {
        std::cerr
            << "OpenCL error: "
            << err.what() << "(" << err.err() << ")"
            << std::endl;
    }
}

void OpenCLOPS::print_gpu_device_info() {
    std::vector<cl::Device> pldev;

    for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
        p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

        for(auto d = pldev.begin(); d != pldev.end(); d++) {
            std::cout << d->getInfo<CL_DEVICE_NAME>() << std::endl;
        }
    }
}

int OpenCLOPS::get_double_precision_gpu_device() {
    std::vector<cl::Device> pldev;

    for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
        try {
            p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

            for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
                if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

                std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

                if (
                    ext.find("cl_khr_fp64") == std::string::npos &&
                    ext.find("cl_amd_fp64") == std::string::npos
                ) continue;

                device.push_back(*d);
            }
        } catch(...) {
            device.clear();
        }
    }

    if (device.empty()) {
        std::cerr << "GPUs with double precision not found." << std::endl;
        return 1;
    }

    return 0;
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