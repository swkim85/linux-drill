// pattern3.hip
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

struct MyStruct {
  int idx;
  int thread_id;
  int block_id;
  int blockdim_id;
  int n_count;
};

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

char pattern[PATTERN_SIZE];

// pattern at device (managed variable)
__managed__ char               d_pattern[PATTERN_SIZE];

__global__ void patternKernel(
   MyStruct *kinfo, char *d_data, int ds, int chunksize, int n_chunk, char *pattern, int ps)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;

  if (idx < n_chunk) {
    kinfo[idx].idx = idx;
    kinfo[idx].thread_id   = threadIdx.x;
    kinfo[idx].block_id    = blockIdx.x;
    kinfo[idx].blockdim_id = blockDim.x;
    kinfo[idx].n_count = 0;

    unsigned int start = idx * chunksize;

    // find pattern in data
    int count = 0;
    for (int i = start; i < start+chunksize; i++) { // start 부터 chunksize 만큼
      bool flag = true;
      for (int j = 0; j < ps; j++) {
        if (d_data[i+j] != pattern[j]) {
          flag = false;
          break;
        }
      }
      if (flag) {
        count++;
        //atomicAdd(d_find_count, 1); 
      }
    }
    kinfo[idx].n_count = count;
  }
}

void print_vec_int(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void print_vec(std::string title, char *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) {
    std::cout << data[i] << "";
  }
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
void print_krn_info(MyStruct *h_krnInfo, int n) {
  std::cout << "idx threadID  blockID  blockdimID count" << std::endl;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    std::cout << ""
       << "" << h_krnInfo[i].idx
       << "\t" << h_krnInfo[i].thread_id
       << "\t" << h_krnInfo[i].block_id
       << "\t" << h_krnInfo[i].blockdim_id
       << "\t" << h_krnInfo[i].n_count
       << std::endl;
    sum += h_krnInfo[i].n_count;
  }
  std::cout << "sum : " << sum << std::endl;
}

int main(void) {

  constexpr unsigned int ds = DATA_SIZE;
  constexpr unsigned int ps = PATTERN_SIZE;
  constexpr unsigned int chunksize = CHUNK_SIZE ; // data를 chunk 단위로 각 스레드에 할당
  constexpr unsigned int n_chunk = ds/chunksize; // chunk 개수
  MyStruct *d_krnInfo;
  MyStruct *h_krnInfo;

  char *h_data = new char[ds];
  initialize_vector_random(h_data, ds);
  initialize_vector_random(pattern, ps);

  print_vec2("data", h_data, ds, chunksize); // 데이터를 출력
  std::cout << "  data length : " << ds << std::endl;
  print_vec("pattern", pattern, ps);
  std::cout << "  pattern length : " << ps << std::endl;

  copy_vec(pattern, d_pattern, ps); // 패턴을 디바이스 메모리로 복사
  //print_vec("d_pattern", d_pattern, ps);

  char *d_data;
  HIP_CHECK(hipMalloc(&d_data, ds * sizeof(char))); // 디바이스 메모리 할당

  h_krnInfo = new MyStruct[n_chunk];      // 호스트 메모리 할당 (kernel info)
  hipMalloc((void**)&d_krnInfo, n_chunk * sizeof(MyStruct)); // 디바이스 메모리 할당(kernel info)

  // 호스트에서 디바이스로 데이터 복사
  HIP_CHECK(hipMemcpy(d_data, h_data, ds * sizeof(char), hipMemcpyHostToDevice));

  constexpr unsigned int blocksize = 256;
  dim3 blockSize(blocksize);
  dim3 gridSize((n_chunk + blockSize.x - 1) / blockSize.x);
  std::cout
        << "  chunk size: " << chunksize
        << "  chunks: " << n_chunk
        << "  blockSize.x: " << blockSize.x
        << "  gridSize.x : " << gridSize.x 
        << std::endl;

  // kernel
  patternKernel<<<gridSize, blockSize, 0, 0>>> (d_krnInfo, d_data, ds, chunksize, n_chunk, d_pattern, ps);

  hipMemcpy(h_krnInfo, d_krnInfo, n_chunk * sizeof(MyStruct), hipMemcpyDeviceToHost); // device -> host (krninfo)
  print_krn_info(h_krnInfo, n_chunk);


  hipFree(d_data);
  hipFree(d_krnInfo);
  delete[] h_data;
  return 0;
}
