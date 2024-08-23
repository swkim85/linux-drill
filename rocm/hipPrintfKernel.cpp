
#define HIP_ENABLE_PRINTF
#define HCC_ENABLE_PRINTF

#include "test_common.h"

__global__ void run_printf() { printf("Hello World\n"); }

int main() {
    int device_count = 0;
    hipGetDeviceCount(&device_count);
    for (int i = 0; i < device_count; ++i) {
        hipSetDevice(i);
        hipLaunchKernelGGL(HIP_KERNEL_NAME(run_printf), dim3(1), dim3(1), 0, 0);
        hipDeviceSynchronize();
    }
    passed();
}
