// transpose.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <cstring>
#include <random>

#define SIZE  8
//#define SIZE  32

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

__global__ void matrix_transpose_kernel(float* out, const float* in, const unsigned int width)
{
  int x = blockDim.x * blockIdx.x + threadIdx.x;
  int y = blockDim.y * blockIdx.y + threadIdx.y;
  out[y * width + x] = in[x * width + y];
}

void print_matrix(float *data, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout.width(4);
      std::cout << data[i*n + j] << "  ";
    }
    std::cout << std::endl;
  }
}

void initialize(float *data, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      data[i * n + j] = i - j;
    }
  }
}

float h_A[SIZE][SIZE];
float h_B[SIZE][SIZE];

int main() {

  float *d_A;
  float *d_B;
  int buffer_size = SIZE * SIZE * sizeof(float);
  int width = SIZE;

  initialize((float *)h_A, width); // 매트릭스 초기화
  std::cout << "A:" << std::endl;
  print_matrix(&h_A[0][0], width); // 매트릭스 출력(A)

  hipMalloc((void**)&d_A, buffer_size); // 디바이스 메모리 할당
  hipMemcpy(d_A, h_A, buffer_size, hipMemcpyHostToDevice); // host -> device
  hipMalloc((void**)&d_B, buffer_size); // 디바이스 메모리 할당

  constexpr unsigned int threads_per_block_x = 8;
  constexpr unsigned int threads_per_block_y = 8;
  matrix_transpose_kernel<<<dim3(width / threads_per_block_x, width / threads_per_block_y),
                            dim3(threads_per_block_x, threads_per_block_y),
                            0,
                            hipStreamDefault>>>(d_B, d_A, width);
  HIP_CHECK(hipGetLastError());

  hipMemcpy(h_B, d_B, buffer_size, hipMemcpyDeviceToHost); // device -> host
  std::cout << "B = transpose A:" << std::endl;
  print_matrix(&h_B[0][0], SIZE); // 매트릭스 출력(B)

  hipFree(d_A);
  return 0;
}
