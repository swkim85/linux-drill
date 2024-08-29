// streams.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }


template<unsigned int Width>
__global__ void matrix_transpose_static_shared(float* out, const float* in)
{
    __shared__ float shared_mem[Width * Width]; // Shared memory of constant size
    const unsigned int x = blockDim.x * blockIdx.x + threadIdx.x;
    const unsigned int y = blockDim.y * blockIdx.y + threadIdx.y;
    shared_mem[y * Width + x] = in[x * Width + y];
    __syncthreads();
    out[y * Width + x] = shared_mem[y * Width + x];
}

__global__ void matrix_transpose_dynamic_shared(float* out, const float* in, const int width)
{
    extern __shared__ float shared_mem[]; // Dynamic shared memory
    const unsigned int x = blockDim.x * blockIdx.x + threadIdx.x;
    const unsigned int y = blockDim.y * blockIdx.y + threadIdx.y;
    shared_mem[y * width + x] = in[x * width + y];
    __syncthreads();
    out[y * width + x] = shared_mem[y * width + x];
}

void print_matrix(std::string title, float *data, int rows, int cols) {
  if (title.length()) std::cout << title << std::endl;
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        std::cout.width(4);
        std::cout << data[i*rows + j] << "  ";
      }
      std::cout << std::endl;
    }
}

template<unsigned int Width, unsigned int Size>
void deploy_multiple_stream(const float* h_in, float **h_transpose_matrix, const int  num_streams)
{
  // Set the block dimensions
  constexpr unsigned int threads_per_block_x = 4;
  constexpr unsigned int threads_per_block_y = 4;

  // Create streams
  std::vector<hipStream_t> streams(num_streams);
  for (int i = 0; i < num_streams; i++) {
    HIP_CHECK(hipStreamCreate(&streams[i]));
  }

  // Allocate device input and output memory and copy host input data to device memory
  std::vector<float*> d_in(num_streams);
  std::vector<float*> d_transpose_matrix(num_streams);

  // Size in bytes for memory management
  const size_t size_in_bytes = sizeof(float) * Size;

  // Allocate device input memory
  HIP_CHECK(hipMalloc(&d_in[0], size_in_bytes));
  HIP_CHECK(hipMalloc(&d_in[1], size_in_bytes));

  // Allocate device output memory
  HIP_CHECK(hipMalloc(&d_transpose_matrix[0], size_in_bytes));
  HIP_CHECK(hipMalloc(&d_transpose_matrix[1], size_in_bytes));

  for (int i = 0; i < num_streams; i++) {
      // Note: If the host memory is not pinned at allocation time using hipHostMalloc then hipMemcpyAsync
      // will behave as synchronous.
      HIP_CHECK(hipMemcpyAsync(d_in[i], h_in, size_in_bytes, hipMemcpyHostToDevice, streams[i]));
  }

  // Make sure that Width is evenly divisible by threads_per_block_x and threads_per_block_y
  static_assert(Width % threads_per_block_x == 0);
  static_assert(Width % threads_per_block_y == 0);

  // Launch kernel with stream[0]
  matrix_transpose_static_shared<Width>
        <<<dim3(Width / threads_per_block_x, Width / threads_per_block_y),
           dim3(threads_per_block_x, threads_per_block_y),
           0,
           streams[0]>>>(d_transpose_matrix[0], d_in[0]);

  // Launch kernel with stream[1]
  matrix_transpose_dynamic_shared<<<dim3(Width / threads_per_block_x, Width / threads_per_block_y),
                                    dim3(threads_per_block_x, threads_per_block_y),
                                    sizeof(float) * Width * Width,
                                    streams[1]>>>(d_transpose_matrix[1], d_in[1], Width);

  // Asynchronously copy the results from device to host
  for(int i = 0; i < num_streams; i++) {
        HIP_CHECK(hipMemcpyAsync(h_transpose_matrix[i],
                                 d_transpose_matrix[i],
                                 size_in_bytes,
                                 hipMemcpyDeviceToHost,
                                 streams[i]));
  }
  // Wait for all tasks in both the streams to complete on the device
  HIP_CHECK(hipDeviceSynchronize());

  // Destroy the streams
  for(int i = 0; i < num_streams; i++) {
      HIP_CHECK(hipStreamDestroy(streams[i]))
  }

  // Free device memory
  for(int i = 0; i < num_streams; i++) {
      HIP_CHECK(hipFree(d_in[i]));
      HIP_CHECK(hipFree(d_transpose_matrix[i]));
  }
}

int main() {

  // Dimension of the input square matrix is width x width
  constexpr unsigned int width = 8;
  //constexpr unsigned int width = 128;
  constexpr unsigned int size  = width * width;
  constexpr unsigned int num_streams = 2; // number of streams
  const size_t size_in_bytes = sizeof(float) * size;

  float* h_in = nullptr; // Host input memory
  HIP_CHECK(hipHostMalloc(&h_in, size_in_bytes)); // host pinned memory allocation

  float *h_transpose_matrix[num_streams]; // Host output memory
  HIP_CHECK(hipHostMalloc(&h_transpose_matrix[0], size_in_bytes));
  HIP_CHECK(hipHostMalloc(&h_transpose_matrix[1], size_in_bytes));

  // Initialize the host input matrix
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      h_in[ i * width + j ] = i;
    }
  }
  print_matrix("h_in", h_in, width, width);

  deploy_multiple_stream<width, size>(h_in, h_transpose_matrix, num_streams);
  print_matrix("transpose[0]",  h_transpose_matrix[0], width, width);
  print_matrix("transpose[1]",  h_transpose_matrix[1], width, width);

  // Free host memory
  HIP_CHECK(hipHostFree(h_in));
  for (int i = 0; i < num_streams; i++) {
    HIP_CHECK(hipHostFree(h_transpose_matrix[i]));
  }

  std::cout << "streams completed!" << std::endl;

  return 0;
}
