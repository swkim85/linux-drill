// hellokernel.hip
#include <hip/hip_runtime.h>
#include <iostream>
#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
    exit(1); \
  }

// __device__ 표시된 함수는 디바이스에서만 실행됨
__device__ unsigned int get_thread_idx()
{
    // Built-in threadIdx returns the 3D coordinate of the active work item in the block of threads.
    return threadIdx.x;
}

// __host__ 함수는 호스트에서 실행
__host__ void print_hello_host()
{
    std::cout << "Hello world from host!" << std::endl;
}

// __device__ and __host__ 표시된 함수는 호스트와 디바이스용으로 컴파일됨
// These functions cannot use coordinate built-ins.
__device__ __host__ void print_hello(int thread_idx, int block_idx)
{
    // 디바이스 에서 printf 를 지원
    printf("Hello world from device or host! thread_idx=%d block_idx=%d\n", thread_idx, block_idx);
}

// __global__ 함수는 디바이스에서 실행되며, 호스트에서 호출된다.
__global__ void helloworld_kernel()
{
    unsigned int thread_idx = get_thread_idx();
    unsigned int block_idx = blockIdx.x;
    //print_hello(thread_idx, block_idx);
    // Only printf is supported for printing from device code.
    printf("Hello world from device kernel block %u thread %u!\n", block_idx, thread_idx);
}

int main()
{
    print_hello_host();
    //print_hello(0);

    // Launch the kernel.
/*
    helloworld_kernel<<<dim3(2), // 3D grid specifying number of blocks to launch: (2, 1, 1)
                        dim3(2), // 3D grid specifying number of threads to launch: (2, 1, 1)
                        0, // number of bytes of additional shared memory to allocate
                        hipStreamDefault // stream where the kernel should execute: default stream
                        >>>();
*/
    hipLaunchKernelGGL(helloworld_kernel, dim3(2), dim3(2), 0, 0);

    // Wait on all active streams on the current device.
    HIP_CHECK(hipDeviceSynchronize());
}
