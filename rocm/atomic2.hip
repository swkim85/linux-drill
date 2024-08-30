// atomic2.hip
#include <hip/hip_runtime.h>
#include <stdio.h>

#define N 500
#define THREADS_PER_BLOCK 256

__global__ void incrementKernel(int* counter) {
  int old = *counter;
  int assumed;
  do {
    assumed = old;
    old = atomicCAS(counter, assumed, assumed + 1);
  } while (assumed != old);
}

int main() {
  int *d_counter;
  int h_counter = 0;
  hipMalloc(&d_counter, sizeof(int));
  hipMemcpy(d_counter, &h_counter, sizeof(int), hipMemcpyHostToDevice);
  int blocks = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
  incrementKernel<<<blocks, THREADS_PER_BLOCK>>>(d_counter);
  hipMemcpy(&h_counter, d_counter, sizeof(int), hipMemcpyDeviceToHost);
  printf("Final counter value: %d\n", h_counter);
  hipFree(d_counter);
  return 0;
}
