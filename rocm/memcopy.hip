// memcopy.hip
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <hip/hip_runtime.h>

#define PATTERN_SIZE 3
#define CHUNK_SIZE  64

#define DATA_SIZE    1024
//#define DATA_SIZE    1073741824 // 1GB
//#define DATA_SIZE    2147483648 // 2GB

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }


// pattern at device (managed variable)
__managed__ char               d_pattern[PATTERN_SIZE];
__managed__ unsigned int       d_ps;


__global__ void memcopy_kernel(
     char *d_data, char *d_data2,
     int ds, int chunksize, int n_chunk, char *d_pattern, int d_ps)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;

  if (idx < n_chunk) {
    unsigned int start = idx * chunksize;

    // copy : d_data -> d_data2
    if (0) {
      for (int i = 0; i < chunksize; i++) {
        d_data2[start + i] = d_data[start + i];
      }
    }

    // fill pattern : d_pattern -> d_data2
    if (1) {
      for (int i = 0; i < chunksize; i++) {
        int j = i % d_ps;
        d_data2[start + i] = d_pattern[j];
      }
    }

  }
}


void print_vec(std::string title, char *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void print_vec2(std::string title, char *data, int n, int chunksize) {
  if (title.length()) std::cout << title << std::endl;
  for (int i = 0; i < n; ++i) {
    if (i % chunksize == 0) {
      std::cout << std::setw(5) << i << " ";
    }
    std::cout << data[i] << "";
    if (i % chunksize == chunksize-1) std::cout << std::endl;
  }
}

void copy_vec(char *src, char *dst, int n) {
  for (int i = 0; i < n; ++i) {
    dst[i] = src[i];
  }
}


void initialize_vector_random(char *data, int n) {
  std::random_device  rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 4);
  int x = (int)'A';
  //std::cout << " x " << x  << std::endl;
  for (int i = 0; i < n; i++) {
    int rand = dis(gen);
    data[i] = (char)(x+rand);
  }
}

int main(void) {

  constexpr unsigned int ds = DATA_SIZE;
  constexpr unsigned int ps = PATTERN_SIZE;
  constexpr unsigned int chunksize = CHUNK_SIZE ; // data를 chunk 단위로 각 스레드에 할당
  constexpr unsigned int n_chunk = ds/chunksize; // chunk 개수

  char *h_data = new char[ds];
  char *h_data2 = new char[ds];
  char *h_pattern = new char[ps];

  std::cout << "random initialize " << std::endl;
  initialize_vector_random(h_data, ds);
  initialize_vector_random(h_pattern, ps);

  print_vec2("data", h_data, ds, chunksize); // 데이터를 출력
  std::cout << "  data length : " << ds << std::endl;

  ////////////////////////////////////////////////////////////
  std::cout << "== kernel " << std::endl;

  copy_vec(h_pattern, d_pattern, ps); d_ps = ps; // 패턴을 디바이스 메모리로 복사
  print_vec("d_pattern", d_pattern, ps);

  char *d_data;
  char *d_data2;
  HIP_CHECK(hipMalloc(&d_data, ds * sizeof(char))); // 디바이스 메모리 할당
  HIP_CHECK(hipMalloc(&d_data2, ds * sizeof(char))); // 디바이스 메모리 할당

  // 호스트에서 디바이스로 데이터 복사
  HIP_CHECK(hipMemcpy(d_data, h_data, ds * sizeof(char), hipMemcpyHostToDevice));

  constexpr unsigned int blocksize = 256;
  dim3 blockSize(blocksize);
  dim3 gridSize((n_chunk + blockSize.x - 1) / blockSize.x);
  std::cout
        << "  chunk size: " << chunksize
        << "  n_chunks: " << n_chunk
        << "  blockSize.x: " << blockSize.x
        << "  gridSize.x : " << gridSize.x 
        << std::endl;

  memcopy_kernel<<<gridSize, blockSize, 0, 0>>>
     (d_data, d_data2, ds, chunksize, n_chunk, d_pattern, ps);

  hipMemcpy(h_data2, d_data2, ds * sizeof(char), hipMemcpyDeviceToHost); // 호스트로 데이터복사
  print_vec2("h_data2", h_data2, ds, chunksize); // 검증을 위해 데이터를 출력
  std::cout << "  data2 length : " << ds << std::endl;

  HIP_CHECK(hipFree(d_data));
  HIP_CHECK(hipFree(d_data2));

  ////////////////////////////////////////////////////////////
  delete[] h_data;
  delete[] h_data2;
  return 0;
}
