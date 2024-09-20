// sgemv.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>

#define BLOCKSIZE 1024
//#define ROWS 4096
//#define COLS 4096
//#define ROWS 16
//#define COLS 16

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

__global__ void sgemv_kernel(int m, int n, const float alpha, const float* A, int lda, 
                             const float* x, int incx, const float beta, float* y, int incy) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < m) {
    float dot = 0.0f;
    for (int j = 0; j < n; ++j) {
      dot += A[i * lda + j] * x[j * incx];
    }
    y[i * incy] += alpha * dot + beta * y[i * incy];
  }
}

// y = alpha * A * x + beta * y
void cpu_sgemv(int m, int n, const float alpha, const float *A, int lda,
        const float *x, int incx, 
        const float beta, float *y, int incy) {
  for (int i = 0; i < m; i++) {
    float dot = 0.0f;
    for (int j = 0; j < n; j++) {
      dot += A[i * lda + j] * x[j * incx];
    }
    y[i * incy] += alpha * dot + beta * y[i * incy];
  }
}

void initialize_matrix(float *matrix, int rows, int cols) {
  float v;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (i == j) v = i + j; else v = 1;
      //if (i == j) v = 1; else v = 0;
      matrix[i*rows+j] = v;
    }
  }
}
void initialize_vector(float *vec, int n) {
  for (int i = 0; i < n; i++) {
    vec[i] = 1.0;
  }
}

const int print_max = 64;
void print_matrix(std::string title, float *data, int rows, int cols) {
  if (title.length()) std::cout << title << std::endl;
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
void print_vec(std::string title, float *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  if (n <= print_max) {
    for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
    std::cout << " ]" << std::endl;
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
  float *h_A = new float[rows * cols];
  float *h_x = new float[cols];
  float *h_y = new float[rows];
  float *h_y2 = new float[rows];
  float alpha = 2.0f;
  float beta = 10.0f;
  int lda = cols;   // leading dimension of A (row-major 에서는 보통 열의 수와 같음)
  int incx = 1, incy = 1;

  initialize_matrix(h_A, rows, cols);
  initialize_vector(h_x, cols);
  initialize_vector(h_y, cols);
  initialize_vector(h_y2, cols);
  print_matrix("A", h_A, rows, cols);
  print_vec("x", h_x, cols);
  print_vec("y", h_y, cols);
  print_vec("y2", h_y2, cols);
  std::cout << " incx: " << incx << "  incy: " << incy 
          << " alpha: " << alpha << "  beta: " << beta << std::endl;

if (1) {
  // cpu에서 계산
  cpu_sgemv(rows, cols, alpha, h_A, lda, h_x, incx, beta, h_y2, incy);
  std::cout << "cpu result : " << std::endl;
  print_vec("", h_y2, rows);
}

  // allocate device memory
  float *d_A, *d_x, *d_y;
  HIP_CHECK(hipMalloc(&d_A, rows * cols * sizeof(float)));
  HIP_CHECK(hipMalloc(&d_x, cols * sizeof(float)));
  HIP_CHECK(hipMalloc(&d_y, rows * sizeof(float)));

  // 호스트에서 디바이스로 데이터 복사
  HIP_CHECK(hipMemcpy(d_A, h_A, rows * cols * sizeof(float), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(d_x, h_x, cols * sizeof(float), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(d_y, h_y, rows * sizeof(float), hipMemcpyHostToDevice));

  dim3 blockSize(BLOCKSIZE);
  dim3 gridSize((rows + blockSize.x - 1) / blockSize.x);
  std::cout << "  blockSize.x: " << blockSize.x 
            << "  gridSize.x : " << gridSize.x
            << "  rows: " << rows << std::endl;

  sgemv_kernel<<<gridSize, blockSize, 0, 0>>>
      (rows, cols, alpha, d_A, lda, d_x, incx, beta, d_y, incy);

  // 디바이스에서 호스트로 결과 복사
  HIP_CHECK(hipMemcpy(h_y, d_y, rows * sizeof(float), hipMemcpyDeviceToHost));
  std::cout << "gpu result : " << std::endl;
  print_vec("", h_y, rows);

  if (compare_vector(h_y, h_y2, rows)) std::cout << "Varification sucess!" << std::endl;
  else std::cout << "Varification FAIL!!!!!" << std::endl;

  HIP_CHECK(hipFree(d_A));
  HIP_CHECK(hipFree(d_x));
  HIP_CHECK(hipFree(d_y));
  return 0;
}

