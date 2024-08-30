// ballot.hip
#include <hip/hip_runtime.h>
#include <iostream>

__global__ void ballotExample(int* input, int* output, int size) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int warpId = idx / 32;
  int laneId = idx % 32;

  int predicate = (idx < size) ? (input[idx] > 0) : 0; // 조건 확인: 입력 값이 0보다 큰지
  unsigned int ballot = __ballot(predicate);
  if (laneId == 0) { // 워프의 첫 번째 스레드만 결과를 기록
    output[blockIdx.x * (blockDim.x / 32) + warpId] = ballot;
  }
}

void print_vec(std::string title, int *data, int n, int hex) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  if (hex) for (int i = 0; i < n; ++i) printf("0x%08x ", data[i]);
  else for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
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
  print_vec("h_data", h_data, size, 0);

  hipMalloc(&d_data, size * sizeof(int));
  hipMalloc(&d_result, (size / 32) * sizeof(int));
  hipMemcpy(d_data, h_data, size * sizeof(int), hipMemcpyHostToDevice);

  constexpr int threadsPerBlock = 256;
  constexpr int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

  ballotExample<<<blocksPerGrid, threadsPerBlock>>>(d_data, d_result, size); 
  hipMemcpy(h_result, d_result, (size / 32) * sizeof(int), hipMemcpyDeviceToHost);
  print_vec("ballotExample", h_result,  size/32, 1);

  hipFree(d_data);
  hipFree(d_result);
  return 0;
}
