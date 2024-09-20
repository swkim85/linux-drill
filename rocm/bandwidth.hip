// bandwidth.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
    exit(1); \
  }

#ifndef  DSIZE
#define  DSIZE  1024*1024*1024
#endif

// 데이터 크기
const size_t   DATA_SIZE =  DSIZE;

int main() {
  float *h_data, *d_data;
  
  h_data = (float*)malloc(DATA_SIZE); // 호스트 메모리 할당
  HIP_CHECK(hipMalloc(&d_data, DATA_SIZE)); // 디바이스 메모리 할당
  std::cout << "Data Size: " << DATA_SIZE/1024/1024 << " MB" << std::endl;
  
  // 데이터 초기화
  for (size_t i = 0; i < DATA_SIZE / sizeof(float); ++i) {
    h_data[i] = static_cast<float>(i);
  }
  
  // 호스트 -> 디바이스 전송 시간 측정(1)
  auto start = std::chrono::high_resolution_clock::now();
  HIP_CHECK(hipMemcpy(d_data, h_data, DATA_SIZE, hipMemcpyHostToDevice));
  HIP_CHECK(hipDeviceSynchronize());
  auto end = std::chrono::high_resolution_clock::now();

  // 호스트 -> 디바이스 전송 시간 측정(2)
  start = std::chrono::high_resolution_clock::now();
  HIP_CHECK(hipMemcpy(d_data, h_data, DATA_SIZE, hipMemcpyHostToDevice));
  HIP_CHECK(hipDeviceSynchronize());
  end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> h2d_time = end - start;
  
  // 디바이스 -> 호스트 전송 시간 측정
  start = std::chrono::high_resolution_clock::now();
  HIP_CHECK(hipMemcpy(h_data, d_data, DATA_SIZE, hipMemcpyDeviceToHost));
  HIP_CHECK(hipDeviceSynchronize());
  end = std::chrono::high_resolution_clock::now();
  
  std::chrono::duration<double> d2h_time = end - start;
  
  // 대역폭 계산 (GB/s)
  double h2d_bandwidth = DATA_SIZE / (1024.0 * 1024.0 * 1024.0) / h2d_time.count();
  double d2h_bandwidth = DATA_SIZE / (1024.0 * 1024.0 * 1024.0) / d2h_time.count();
  
  std::cout << "Host to Device Bandwidth: " << h2d_bandwidth << " GB/s" << std::endl;
  std::cout << "Device to Host Bandwidth: " << d2h_bandwidth << " GB/s" << std::endl;
  
  // 메모리 해제
  free(h_data);
  HIP_CHECK(hipFree(d_data));
  
  return 0;
}
