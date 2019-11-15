# Hello world in OpenCL using Nvidia hardware on Debian 10

## Setup development environment in Debian 10
To build OpenCL applications, you will need the following:
* An OpenCL ICD Loader
* OpenCL header files
* OpenCL development files (symlinks)
* Vendor-specific ICD

Although individual vendors offer their own implementations for the first three items,
these items are standardized according to a Khronos specification. It is sufficient
(and likely better due to vendor-specific implementation bugs) to use vendor-agnostic
versions of these items.

The final item requires a vendor specific implementation.

For OpenCL on nvidia devices, these packages satisfy the requirements:
* ocl-icd-libopencl1
* opencl-headers
* ocl-icd-opencl-dev
* nvidia-opencl-icd

## Nvidia quirks relevant to Debian 10
The nvidia ICD does not support OpenCL 2.x. The latest version for which the ICD
provides full support is OpenCL 1.2. Target this release with

```
#define CL_TARGET_OPENCL_VERSION 120
```

Nvidia provides its own ICD loader packaged as nvidia-libopencl1. This package
should not be used. Instead use the vendor-agnostic ICD loader packaged as ocl-icd-libopencl1.

## Build

```
mkdir _build
cd _build
cmake .. 'Unix Makefiles'
make
```

## Test

```
cd _build
make test
```

