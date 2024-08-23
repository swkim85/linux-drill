// copy.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <cstring>
#include <random>

#define SIZE  1048576   // 1024*1024

void print_data(int *data, int n) {
  for (int i = 0; i < 10; ++i) std::cout << data[i] << " "; // first 10 items
  std::cout << " ... ";
  for (int i = n-10; i < n; ++i) std::cout << data[i] << " "; // last 10 items
  std::cout << std::endl;
}

void initialize(int *data, int n) {
  std::mt19937 gen(std::time(0));
  std::uniform_int_distribution<> dis(1, 10000);
  for (int i = 0; i < n; ++i) {
    data[i] = dis(gen); // random distribution
  }
}

int  hostData[SIZE];
int  hostDataCopy[SIZE];

int main() {

  int *deviceData;
  int buffer_size = SIZE * sizeof(int);

  initialize(hostData, SIZE); // hostData 를 초기화
  print_data(hostData, SIZE);

  hipMalloc((void**)&deviceData, buffer_size); // 디바이스 메모리 할당

  hipMemcpy(deviceData, hostData, buffer_size, hipMemcpyHostToDevice); // host -> device

  hipMemcpy(hostDataCopy, deviceData, buffer_size, hipMemcpyDeviceToHost); // device -> host
  print_data(hostDataCopy, SIZE);

  bool isIdentical = (std::memcmp(hostData, hostDataCopy, buffer_size) == 0);
  if (isIdentical) {
    std::cout << "Data matches!" << std::endl;
  } else {
    std::cout << "Data does not match!" << std::endl;
  }

  hipFree(deviceData);
  return 0;
}
