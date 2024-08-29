// shfl_transpose.hip
#include <hip/hip_runtime.h>
#include <iostream>

//#define N 16
#define N 32
//#define N 33
//#define N 64
//#define N 1024


__global__ void transposeKernel(float *out, float *in, int width, int height) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x < width && y < height) {
    int in_idx = y * width + x;
    float val = in[in_idx];
    val = __shfl(val, threadIdx.y * blockDim.x + threadIdx.x);
    int out_idx = x * height + y;
    out[out_idx] = val;
  }
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

void print_vec(std::string title, float *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_matrix(float *matrix, int rows, int cols) {
  float v;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i == j) v = i+2*j; else v = (i+3*j)%8;
      matrix[i*rows+j] = v;
    }
  }
}
void initialize_vector(float *vec, int n, float value) {
  for (int i = 0; i < n; i++) {
    vec[i] = i * value;
  }
}

int main() {
  float *A, *B, *C;
  hipMallocManaged(&A, N*N * sizeof(float));
  hipMallocManaged(&B, N*N * sizeof(float));
  hipMallocManaged(&C, N*N * sizeof(float));

  initialize_matrix(A, N, N);
  print_matrix("A", A, N, N);

  int blockSize = 256;
  int numBlocks = (N + blockSize - 1) / blockSize;

  constexpr unsigned int threads_per_block_x = 8;
  constexpr unsigned int threads_per_block_y = 8;
  constexpr unsigned int gridsize_x = (N + threads_per_block_x - 1) / threads_per_block_x;
  constexpr unsigned int gridsize_y = (N + threads_per_block_y - 1) / threads_per_block_y;
  transposeKernel<<<dim3(gridsize_x, gridsize_y),
                    dim3(threads_per_block_x, threads_per_block_y), 0, 0>>>(C, A, N, N);
  hipDeviceSynchronize();
  print_matrix("C", C, N, N);

  hipFree(A); hipFree(B); hipFree(C);
  return 0;
}
