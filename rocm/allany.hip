// allany.hip
#include <hip/hip_runtime.h>
#include <iostream>

__global__ void checkAllPositive(int* data, int* result, int size) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int warpId = idx / 32;
  bool isPositive = (idx < size) ? (data[idx] > 0) : true;
  bool allPositive = __all(isPositive); // 워프 내의 모든 스레드가 양수를 가지고 있는지 확인
  if (threadIdx.x % 32 == 0) {  // 워프의 첫 번째 스레드만 결과를 기록
    result[warpId] = allPositive;
  }
}
__global__ void checkAnyPositive(int* data, int* result, int size) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int warpId = idx / 32;
  bool isPositive = (idx < size) ? (data[idx] > 0) : false;
  bool anyPositive = __any(isPositive); // 워프 내의 어느 스레드가 양수를 가지고 있는지 확인
  if (threadIdx.x % 32 == 0) {  // 워프의 첫 번째 스레드만 결과를 기록
    result[warpId] = anyPositive;
  }
}

void print_vec(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

int main() {
  constexpr int size = 128;
  int h_data[size];
  int h_result[size/32];
  int *d_data, *d_result;

  for (int i = 0; i < size; i++) {
    if (i >= 40 & i <= 123) h_data[i] = -1; // 인덱스 40~123 이면 음수
    else h_data[i] = i + 1; // 데이터 초기화 모두 양수
  }
  print_vec("h_data", h_data, size);

  hipMalloc(&d_data, size * sizeof(int));
  hipMalloc(&d_result, (size / 32) * sizeof(int));
  hipMemcpy(d_data, h_data, size * sizeof(int), hipMemcpyHostToDevice);

  constexpr int threadsPerBlock = 256;
  constexpr int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

  checkAllPositive<<<blocksPerGrid, threadsPerBlock>>>(d_data, d_result, size); 
  hipMemcpy(h_result, d_result, (size / 32) * sizeof(int), hipMemcpyDeviceToHost);
  print_vec("checkAllPositive", h_result,  size/32);

  checkAnyPositive<<<blocksPerGrid, threadsPerBlock>>>(d_data, d_result, size); 
  hipMemcpy(h_result, d_result, (size / 32) * sizeof(int), hipMemcpyDeviceToHost);
  print_vec("checkAnyPositive", h_result,  size/32);

  hipFree(d_data);
  hipFree(d_result);
  return 0;
}
