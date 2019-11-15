Hello world in OpenCL

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

For OpenCL on nvidia devices on Debian 10, these packages satisfy the requirements:
* ocl-icd-libopencl1
* opencl-headers
* ocl-icd-opencl-dev
* nvidia-opencl-icd

Nvidia quirks:
The nvidia ICD does not support OpenCL 2.x. The latest version for which the ICD
provides full support is OpenCL 1.2. Target this release with
#define CL_TARGET_OPENCL_VERSION 120

Nvidia provides its own ICD loader packaged as nvidia-libopencl1 in Debian 10. This package
should not be used. Instead use the vendor-agnostic ICD loader packaged as ocl-icd-libopencl1.
