// sort2.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <random>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << " at line " << __LINE__ << std::endl; \
    exit(1); \
  }

const int ARRAY_SIZE = 32;
//const int ARRAY_SIZE = 1024;
//const int ARRAY_SIZE = 2048;
//const int ARRAY_SIZE = 4096;

void print_vec(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_vector(int *data, int n) {
  for (int i = 0; i < n; i++) {
    data[i] = n - i;
  }
}

void initialize_vector_random(int *data, int n) {
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


//  i =   0  1  2  3  4  5 ...
// step = 0  1  0  1  0  1 ...
__global__ void bubbleSortKernel(int* data, int n, int step) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = gridDim.x * blockDim.x;

  // n=1024  blocksize=256  blocksize=4 일때, stride 는 1024 이 된다.
  // n=2048  blocksize=256  blocksize=8 일때, stride 는 2048 이 된다.
  for (int i = idx; i < n - 1; i += stride) {

    // idx=0번 스레드는 step=0임, i=0 이므로 data[0] data[1] 을 정렬
    // idx=1번 스레드는 step=1임, i=1 이므로 data[1] data[2] 을 정렬
    // idx=2번 스레드는 step=0임, i=2 이므로 data[2] data[3] 을 정렬
    // ...
    // idx=1022번 스레드는 step=0임, i=1022 이므로 data[1022] data[1023] 을 정렬
    // idx=1023번 스레드는 step=1임, i=1023 인데 i<n-1 조건에 걸림으로 for loop이 종료됨

    if (i % 2 == step) {
      if (data[i] > data[i + 1]) {
        int temp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = temp;
      }
    }
  }
}

void hipSort(int* data, int n) {
  int* d_data;
  HIP_CHECK(hipMalloc(&d_data, n * sizeof(int)));
  HIP_CHECK(hipMemcpy(d_data, data, n * sizeof(int), hipMemcpyHostToDevice));
  constexpr int BLOCK_SIZE = 256;

  dim3 block(BLOCK_SIZE);
  dim3 grid((n + block.x - 1) / block.x);

  // n 개의 커널을 동시에 실행(비동기)
  // step 값은  i % 2 임.
  //  i =   0  1  2  3  4  5 ...
  // step = 0  1  0  1  0  1 ...
  for (int i = 0; i < n; i++) {
    bubbleSortKernel<<<grid, block>>>(d_data, n, i % 2);

    hipMemcpy(data, d_data, n * sizeof(int), hipMemcpyDeviceToHost);
    print_vec("", data, n);
  }

  //hipMemcpy(data, d_data, n * sizeof(int), hipMemcpyDeviceToHost);
  hipFree(d_data);
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
