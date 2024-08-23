#include <hip/hip_runtime.h>
#include <iostream>
#include <cstring>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
    exit(1); \
  }

#define SIZE 20

void print_data(int *data) {
  for (int i = 0; i < SIZE; ++i) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
    int  hostData[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        hostData[i] = static_cast<int>(i);  // 0부터 99까지의 값으로 초기화
    }
    print_data(hostData);

    int  hostDataCopy[SIZE];
    int *deviceData;
    int buffer_size = SIZE * sizeof(int);

    hipMalloc((void**)&deviceData, buffer_size); // alloc device memory 

    hipMemcpy(deviceData, hostData, buffer_size, hipMemcpyHostToDevice); // host -> device
    HIP_CHECK(hipDeviceSynchronize());

    hipMemcpy(hostDataCopy, deviceData, buffer_size, hipMemcpyDeviceToHost); // device -> host
    print_data(hostDataCopy);

    bool isIdentical = (std::memcmp(hostData, hostDataCopy, buffer_size) == 0);
    if (isIdentical) {
        std::cout << "Data matches!" << std::endl;
    } else {
        std::cout << "Data does not match!" << std::endl;
    }

    hipFree(deviceData);

    return 0;
}
