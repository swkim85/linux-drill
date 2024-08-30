// shfl3.hip
#include <hip/hip_runtime.h>
#include <iostream>

//#define N 64
//#define WIDTH 8

__global__ void shflKernel(float* in, float* out, int mod) {
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx < N) {
    float val = in[idx];
    val = __shfl_up(val, 2, WIDTH);
    out[idx] = val;
  }
}

void print_vec(std::string title, float *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_vector(float *vec, int n, float value) {
  for (int i = 0; i < n; i++) {
    vec[i] = i * value;
  }
}

__managed__ float a[N], b[N];
int main() {

  initialize_vector(a, N, 1);
  print_vec("a", a, N);

  int blockSize = 256;
  int numBlocks = (N + blockSize - 1) / blockSize;

  int mod = 2;
  shflKernel <<<dim3(numBlocks), dim3(blockSize), 0, 0>>>(a, b, mod);
  hipDeviceSynchronize();
  std::cout << " mod=" << mod << std::endl;
  print_vec("b", b, N);

  return 0;
}
