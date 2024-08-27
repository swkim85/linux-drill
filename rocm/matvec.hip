// matvec.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>

//#define BLOCKSIZE 1024
//#define ROWS 4096
//#define COLS 4096

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

__global__ void matrixVectorMulKernel1(float* matrix, float* vector, float* result, int rows, int cols) {
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  if (row < rows) {
    float sum = 0.0f;
    for (int col = 0; col < cols; ++col) {
      sum += matrix[row * cols + col] * vector[col];
    }
    result[row] = sum;
  }
}

template<unsigned int Rows>
__global__ void matrixVectorMulKernel2(float* matrix, float* vector, float* result, const int rows, const int cols) {
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  //__shared__ float  s_vector[Rows];  // static shared memory
  extern __shared__ float  s_vector[];

  if (row < rows) s_vector[row] = vector[row];
  __syncthreads(); 

  if (row < rows) {
    float sum = 0.0f;
    for (int col = 0; col < cols; ++col) {
      sum += matrix[row * cols + col] * s_vector[col];
    }
    result[row] = sum;
  }
}


void cpuMatrixVectorMul(const float* matrix, const float* vector, float* result, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    float sum = 0.0f;
    for (int j = 0; j < cols; ++j) {
      sum += matrix[i * cols + j] * vector[j];
    }
    result[i] = sum;
  }
}

void initialize_matrix(float *matrix, int rows, int cols) {
  float v;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i == j) v = i + j; else v = 1;
      matrix[i*rows+j] = v;
    }
  }
}
void initialize_vector(float *vec, int n) {
  for (int i = 0; i < n; i++) {
    vec[i] = 1.0;
  }
}

//const int print_max = 64;
const int print_max = 1024;
void print_matrix(float *data, int rows, int cols) {
  if (rows <= print_max && cols <= print_max) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        std::cout.width(4);
        std::cout << data[i*rows + j] << "  ";
      }
      std::cout << std::endl;
    }
  }
}
void print_vec(float *data, int n) {
  if (n <= print_max) {
    for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
    std::cout << std::endl;
  }
}

bool compare_vector(float *v1, float *v2, int n) {
  float diff;
  constexpr float eps    = 1.0E-6f; // 허용 오차 epsilon
  for (int i = 0; i < n; ++i) {
    diff = std::fabs(v1[i] - v2[i]);
    if (diff > eps) {
      std::cout << "diff : " << diff << std::endl;
      return false;
    }
  }
  return true; // equal 
}

int main()
{
  const int rows = ROWS; const int cols = COLS;
  float *h_matrix = new float[rows * cols];
  float *h_vector = new float[cols];
  float *h_result = new float[rows];
  float *h_result2 = new float[rows];

  initialize_matrix(h_matrix, rows, cols);
  initialize_vector(h_vector, cols);
  print_matrix(h_matrix, rows, cols);
  print_vec(h_vector, cols);

  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<double> t1;
if (1) {
  // cpu에서 계산 (h_result2)
  start = std::chrono::high_resolution_clock::now(); // 시간측정 시작
  cpuMatrixVectorMul(h_matrix, h_vector, h_result2, rows, cols);
  end = std::chrono::high_resolution_clock::now(); // 시간측정 종료
  t1 = end - start;
  std::cout << "cpu execution time:" << t1.count() << " s" << std::endl;
}

  // allocate device memory
  float *d_matrix, *d_vector, *d_result;
  HIP_CHECK(hipMalloc(&d_matrix, rows * cols * sizeof(float)));
  HIP_CHECK(hipMalloc(&d_vector, cols * sizeof(float)));
  HIP_CHECK(hipMalloc(&d_result, rows * sizeof(float)));

  // 호스트에서 디바이스로 데이터 복사
  HIP_CHECK(hipMemcpy(d_matrix, h_matrix, rows * cols * sizeof(float), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(d_vector, h_vector, cols * sizeof(float), hipMemcpyHostToDevice));

  dim3 blockSize(BLOCKSIZE);
  dim3 gridSize((rows + blockSize.x - 1) / blockSize.x);
  std::cout << "blockSize.x : " << blockSize.x << std::endl;
  std::cout << "gridSize.x  : " << gridSize.x << std::endl;
  std::cout << "rows : " << rows << std::endl;

if (1) {
  // kernel1
  start = std::chrono::high_resolution_clock::now(); // 시간측정 시작
  matrixVectorMulKernel1<<<gridSize, blockSize, 0, 0>>>(d_matrix, d_vector, d_result, rows, cols); // 커널 실행
  end = std::chrono::high_resolution_clock::now(); // 시간측정 종료
  t1 = end - start;
  std::cout << "kernel1 execution time:" << t1.count() << " s" << std::endl;

  // 디바이스에서 호스트로 결과 복사
  HIP_CHECK(hipMemcpy(h_result, d_result, rows * sizeof(float), hipMemcpyDeviceToHost));
  print_vec(h_result, rows);

  if (compare_vector(h_result, h_result2, rows)) std::cout << "Varification sucess!" << std::endl;
  else std::cout << "Varification FAIL!!!!!" << std::endl;
}

  // kernel2
  size_t sharedMemSize = rows * sizeof(float);
  start = std::chrono::high_resolution_clock::now(); // 시간측정 시작
  matrixVectorMulKernel2<rows>    // 커널 실행
    <<<gridSize, blockSize, sharedMemSize>>>(d_matrix, d_vector, d_result, rows, cols);
  end = std::chrono::high_resolution_clock::now(); // 시간측정 종료
  t1 = end - start;
  std::cout << "kernel2 execution time:" << t1.count() << " s" << std::endl;

  // 디바이스에서 호스트로 결과 복사
  HIP_CHECK(hipMemcpy(h_result, d_result, rows * sizeof(float), hipMemcpyDeviceToHost));
  print_vec(h_result, rows);

  if (compare_vector(h_result, h_result2, rows)) std::cout << "Varification sucess!" << std::endl;
  else std::cout << "Varification FAIL!!!!!" << std::endl;

  HIP_CHECK(hipFree(d_matrix));
  HIP_CHECK(hipFree(d_vector));
  HIP_CHECK(hipFree(d_result));
  return 0;
}

