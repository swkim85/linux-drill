// saxpy.hip
#include <hip/hip_runtime.h>
#include <iostream>
#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

__global__ void saxpy_kernel(const float a, const float* d_x, float* d_y, const unsigned int size)
{
  const unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (idx < size) {
    d_y[idx] = a * d_x[idx] + d_y[idx];
  }
}

void initialize(float *vec, int n, float value) {
  for (int i = 0; i < n; ++i) {
    vec[i] = value;
  }
}
void print_data(float *data, int n) {
  for (int i = 0; i < 10; ++i) std::cout << data[i] << " "; // first 10 items
  std::cout << " ... ";
  for (int i = n-10; i < n; ++i) std::cout << data[i] << " "; // last 10 items
  std::cout << std::endl;
}

int main()
{
  constexpr unsigned int size = 1000000; // 벡터의 크기
  constexpr size_t size_bytes = size * sizeof(float); // 벡터의 메모리 용량
  constexpr unsigned int block_size = 256; // 블록당 스레드 개수
  constexpr unsigned int grid_size = (size + block_size - 1) / block_size; // 블럭의 개수 ceil(size/block_size).
  constexpr float a = 3.14f; // 상수 a (Y += a*X)

  float *h_x, *h_y;
  h_x = new float[size];
  h_y = new float[size];
  initialize(h_x, size, 2.0);
  initialize(h_y, size, 1.0);
  print_data(h_x, size);
  print_data(h_y, size);

  // Allocate and copy vectors to device memory.
  float* d_x;
  float* d_y;
  HIP_CHECK(hipMalloc(&d_x, size_bytes));
  HIP_CHECK(hipMalloc(&d_y, size_bytes));
  HIP_CHECK(hipMemcpy(d_x, h_x, size_bytes, hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(d_y, h_y, size_bytes, hipMemcpyHostToDevice));

  std::cout << "Calculating y[i] = a * x[i] + y[i] over " << size << " elements." << std::endl;

  // Launch the kernel on the default stream.
  saxpy_kernel<<<dim3(grid_size), dim3(block_size), 0, hipStreamDefault>>>(a, d_x, d_y, size);
  HIP_CHECK(hipGetLastError());

  HIP_CHECK(hipMemcpy(h_y, d_y, size_bytes, hipMemcpyDeviceToHost)); // device->host

  print_data(h_y, size);

  HIP_CHECK(hipFree(d_x));
  HIP_CHECK(hipFree(d_y));
  return 0;
}

