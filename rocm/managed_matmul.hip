// managed_matmul.hip
#include <hip/hip_runtime.h>
#include <iostream>

//#define N 4
#define N 16
//#define N 32
//#define N 33
//#define N 64
//#define N 1024

__global__ void matrixMulKernel(const float* A, const float* B, float* C, int width) {
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  float sum = 0.0f;
  if (row < width && col < width) {
    for (int k = 0; k < width; k++) {
      sum += A[row * width + k] * B[k * width + col];
    }
    __syncthreads();
    C[row * width + col] = sum;
  }
}

__global__ void transposeKernel(int *out, int *in, int width, int height) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x < width && y < height) {
    int in_idx = y * width + x;
    int val = in[in_idx];
    val = __shfl(val, threadIdx.y * blockDim.x + threadIdx.x);
    int out_idx = x * height + y;
    out[out_idx] = val;
  }
}

__global__ void addKernel(float* a, float* b, float* c) {
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx < N) {
    c[idx] = a[idx] + b[idx];
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
      if (i == j) v = i+j; else v = 1;
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
  float *a, *b, *c;
  float *A, *B, *C;

  //hipMallocManaged(&a, N * sizeof(float));
  //hipMallocManaged(&b, N * sizeof(float));
  //hipMallocManaged(&c, N * sizeof(float));
  hipMallocManaged(&A, N*N * sizeof(float));
  hipMallocManaged(&B, N*N * sizeof(float));
  hipMallocManaged(&C, N*N * sizeof(float));

  //initialize_vector(a, N, 1);
  //initialize_vector(b, N, 2);
  //print_vec("a", a, N);
  //print_vec("b", b, N);

  initialize_matrix(A, N, N);
  initialize_matrix(B, N, N);
  print_matrix("A", A, N, N);
  print_matrix("B", A, N, N);

  int blockSize = 256;
  int numBlocks = (N + blockSize - 1) / blockSize;

  //addKernel <<<dim3(numBlocks), dim3(blockSize), 0, 0>>>(a, b, c);
  //hipDeviceSynchronize();
  //print_vec("c", c, N);

  constexpr unsigned int threads_per_block_x = 8;
  constexpr unsigned int threads_per_block_y = 8;
  constexpr unsigned int gridsize_x = (N + threads_per_block_x - 1) / threads_per_block_x;
  constexpr unsigned int gridsize_y = (N + threads_per_block_y - 1) / threads_per_block_y;

  matrixMulKernel<<<dim3(gridsize_x, gridsize_y),
                    dim3(threads_per_block_x, threads_per_block_y), 0, 0>>>(A, B, C, N);
  hipDeviceSynchronize();
  print_matrix("C", C, N, N);

  hipFree(a); hipFree(b); hipFree(c);
  return 0;
}
