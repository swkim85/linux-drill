// rng.hip
#include <hip/hip_runtime.h>
#include <rocrand/rocrand_kernel.h>
#include <iostream>
#include <chrono>

#define N  10
#define MAX 10

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
    exit(1); \
  }

struct MyStruct {   // 스레드 내부의 인덱스 값을 저장
  int idx;
  int thread_id;
  int block_id;
  int blockdim_id;
  int number;
  rocrand_state_xorwow  rng; // random number generator
};

__managed__  unsigned long long seed;

__global__ void myKernel(MyStruct *kinfo, int n)
{
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx >= n) return;

  kinfo[idx].idx = idx;  // 현재 스레드의 정보를 저장 
  kinfo[idx].thread_id   = threadIdx.x;
  kinfo[idx].block_id    = blockIdx.x;
  kinfo[idx].blockdim_id = blockDim.x;

  rocrand_init(seed, idx, 0, &(kinfo[idx].rng)); // XORWOW 상태 초기화

  int rand = floor(rocrand_uniform (&(kinfo[idx].rng)) * (MAX+1));
  kinfo[idx].number = rand;
}

int main() {

  MyStruct *d_krnInfo, *h_krnInfo;
  h_krnInfo = new MyStruct[N];
  int krninfo_size = N * sizeof(struct MyStruct);
  hipMalloc((void**)&d_krnInfo, krninfo_size); // 디바이스 메모리 할당(kernel info)

  // 현재 시간을 seed로 사용
  seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

  // Kernel 실행
  constexpr unsigned int block_size = 256; // 블록당 스레드 개수
  unsigned int grid_size = (N + block_size - 1) / block_size; // 블럭 개수
  myKernel<<<grid_size, block_size>>>(d_krnInfo, N);
  hipDeviceSynchronize();

  hipMemcpy(h_krnInfo, d_krnInfo, krninfo_size, hipMemcpyDeviceToHost); // 호스트로 복사
  std::cout << "idx  .number " << std::endl;
  for (int i = 0; i < N; i++) {
    std::cout << ""
       << "" << h_krnInfo[i].idx
       << "  " << h_krnInfo[i].number
       << std::endl;
  }

  hipFree(d_krnInfo);
  return 0;
}
