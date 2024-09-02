// sort1.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <algorithm>
#include <random>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << " at line " << __LINE__ << std::endl; \
    exit(1); \
  }

const int ARRAY_SIZE = 1024;

__global__ void bubbleSortKernel(int* data, int n, int step) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = gridDim.x * blockDim.x;

  for (int i = idx; i < n - 1; i += stride) {
    if (i % 2 == step) {
      if (data[i] > data[i + 1]) {
        int temp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = temp;
      }
    }
  }
}

void print_vec(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_vector(int *data, int n) {
  std::random_device  rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10000);
  for (int i = 0; i < n; i++) {
    data[i] = dis(gen);
  }
}

bool is_sorted(int *arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
}

void hipSort(int* data, int n) {
  int* d_data;
  HIP_CHECK(hipMalloc(&d_data, n * sizeof(int)));
  HIP_CHECK(hipMemcpy(d_data, data, n * sizeof(int), hipMemcpyHostToDevice));
  constexpr int BLOCK_SIZE = 256;

  dim3 block(BLOCK_SIZE);
  dim3 grid((n + block.x - 1) / block.x);

  for (int i = 0; i < n; i++) {
    bubbleSortKernel<<<grid, block>>>(d_data, n, i % 2);
    HIP_CHECK(hipGetLastError());
  }

  HIP_CHECK(hipMemcpy(data, d_data, n * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipFree(d_data));
}

int main() {
  constexpr unsigned int n = ARRAY_SIZE;
  int data[n];
  initialize_vector(data, n); // random number initialize

  print_vec("before ", data, n);
  hipSort(data, ARRAY_SIZE); // HIP을 사용한 정렬 수행
  print_vec("after ", data, n);

  if (is_sorted(data, n)) std::cout << "Sorting success" << std::endl;
  else std::cout << "Sorting FAIL!!" << std::endl;
  return 0;
}
