// atomic3.hip
#include <hip/hip_runtime.h>
#include <iostream>

#define N 500

__global__ void incrementKernel(int* counter) {
  int* mutex = counter + 1;  // 첫 번째 요소를 카운터로, 두 번째 요소를 뮤텍스로 사용

  // critical section start
  while (atomicCAS(mutex, 0, 1) != 0);

  //(*counter)++;
  atomicAdd(counter, 1);

  // critical section end
  atomicExch(mutex, 0);
}


int main() {
  int *d_counter;
  int h_counter[2];

  hipMalloc(&d_counter, 2 * sizeof(int));

  hipMemcpy(d_counter, &h_counter, 2 * sizeof(int), hipMemcpyHostToDevice);

  constexpr int blocksize = 256;
  dim3 blockSize(blocksize);
  dim3 gridSize((N + blockSize.x - 1) / blockSize.x);
  std::cout << "  blockSize.x: " << blockSize.x
            << "  gridSize.x : " << gridSize.x
            << std::endl;

  int blocks = (N + blocksize - 1) / blocksize;
  incrementKernel<<<gridSize, blockSize>>>(d_counter);

  hipMemcpy(&h_counter, d_counter, 2*sizeof(int), hipMemcpyDeviceToHost);

  std::cout << "Final counter value: " << h_counter[0] << std::endl;

  hipFree(d_counter);
  return 0;
}
