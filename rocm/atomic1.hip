// atomic1.hip
#include <hip/hip_runtime.h>
#include <stdio.h>

#define N 1024
#define THREADS_PER_BLOCK 256

__global__ void sumKernel(int *input, int *sum1, int *sum2) {
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  int stride = gridDim.x * blockDim.x;
  for (int i = tid; i < N; i += stride) {
    sum1 += input[i];
    atomicAdd(sum2, input[i]);
  }
}

int main() {
  int *d_input, *d_sum1, *d_sum2;
  int h_input[N], h_sum1 = 0, h_sum2 = 0;

  for (int i = 0; i < N; i++) h_input[i] = 1;

  hipMalloc(&d_input, N * sizeof(int));
  hipMalloc(&d_sum1, sizeof(int));
  hipMalloc(&d_sum2, sizeof(int));
  hipMemcpy(d_input, h_input, N * sizeof(int), hipMemcpyHostToDevice);
  hipMemcpy(d_sum1, &h_sum1, sizeof(int), hipMemcpyHostToDevice);
  hipMemcpy(d_sum2, &h_sum2, sizeof(int), hipMemcpyHostToDevice);

  int blocks = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
  sumKernel<<<blocks, THREADS_PER_BLOCK>>>(d_input, d_sum1, d_sum2);

  hipMemcpy(&h_sum1, d_sum1, sizeof(int), hipMemcpyDeviceToHost);
  hipMemcpy(&h_sum2, d_sum2, sizeof(int), hipMemcpyDeviceToHost);
  printf("sum1: %d  sum2: %d\n", h_sum1, h_sum2);

  hipFree(d_input); hipFree(d_sum1); hipFree(d_sum2);
  return 0;
}
