#include <hip/hip_runtime.h>
#include <stdio.h>

int main() {
    int deviceCount;
    hipError_t err = hipGetDeviceCount(&deviceCount);
    
    if (err != hipSuccess) {
        printf("hipGetDeviceCount failed: %s\n", hipGetErrorString(err));
        return 1;
    }

    if (deviceCount == 0) {
        printf("No HIP-capable devices found.\n");
        return 1;
    }

    for (int dev = 0; dev < deviceCount; ++dev) {
        hipDeviceProp_t deviceProp;
        hipGetDeviceProperties(&deviceProp, dev);
        printf("Device %d: %s\n", dev, deviceProp.name);

        int major, minor;
        hipDeviceGetAttribute(&major, hipDeviceAttributeComputeCapabilityMajor, dev);
        hipDeviceGetAttribute(&minor, hipDeviceAttributeComputeCapabilityMinor, dev);
        printf("  Compute capability: %d.%d\n", major, minor);

        int multiProcessorCount;
        hipDeviceGetAttribute(&multiProcessorCount, hipDeviceAttributeMultiprocessorCount, dev);
        printf("  Number of multiprocessors: %d\n", multiProcessorCount);

        int maxThreadsPerBlock;
        hipDeviceGetAttribute(&maxThreadsPerBlock, hipDeviceAttributeMaxThreadsPerBlock, dev);
        printf("  Max threads per block: %d\n", maxThreadsPerBlock);

        int warpSize;
        hipDeviceGetAttribute(&warpSize, hipDeviceAttributeWarpSize, dev);
        printf("  Warp size: %d\n", warpSize);

        int memoryClockRate;
        hipDeviceGetAttribute(&memoryClockRate, hipDeviceAttributeMemoryClockRate, dev);
        printf("  Memory clock rate (kHz): %d\n", memoryClockRate);

        int memoryBusWidth;
        hipDeviceGetAttribute(&memoryBusWidth, hipDeviceAttributeMemoryBusWidth, dev);
        printf("  Memory bus width (bits): %d\n", memoryBusWidth);

        int l2CacheSize;
        hipDeviceGetAttribute(&l2CacheSize, hipDeviceAttributeL2CacheSize, dev);
        printf("  L2 cache size (bytes): %d\n", l2CacheSize);
    }

    return 0;
}

