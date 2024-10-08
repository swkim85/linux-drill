// hip_hello.cpp
#include <hip/hip_runtime.h>
#include <stdio.h>

__global__ void gpuHello() {
  int id = blockIdx.x * blockDim.x + threadIdx.x;
  printf("Hello World from thread %d\n", id);
}

int main() {
  gpuHello<<<1, 1>>>();
  //hipLaunchKernelGGL(gpuHello, dim3(2), dim3(4), 0, 0);
  hipDeviceSynchronize();
  return 0;
}
