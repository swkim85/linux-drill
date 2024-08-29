// managed.hip
#include <hip/hip_runtime.h>
#include <iostream>

#define N 1024

__global__ void addKernel(float* a, float* b, float* c) {
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx < N) {
    c[idx] = a[idx] + b[idx];
  }
}
void print_vec(float *data, int n) {
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << std::endl;
}
int main() {
  float *a, *b, *c;

  hipMallocManaged(&a, N * sizeof(float));
  hipMallocManaged(&b, N * sizeof(float));
  hipMallocManaged(&c, N * sizeof(float));

  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = i * 2;
  }

  int blockSize = 256;
  int numBlocks = (N + blockSize - 1) / blockSize;
  hipLaunchKernelGGL(addKernel, dim3(numBlocks), dim3(blockSize), 0, 0, a, b, c);
  hipDeviceSynchronize();
  print_vec(c, N);

  hipFree(a); hipFree(b); hipFree(c);
  return 0;
}
