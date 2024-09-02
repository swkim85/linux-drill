// launchkernel.hip
#include <hip/hip_runtime.h>
#include <iostream>
#include <random>
//#define GRIDSIZE   10
//#define BLOCKSIZE  10
//#define N 100

struct MyStruct {
  int idx;
  int thread_id;
  int block_id;
  int blockdim_id;
};

#define MAX_THREADS_PER_BLOCK 1000
#define MIN_WARPS_PER_EXECUTION_UNIT 10
__global__ void  __launch_bounds__(MAX_THREADS_PER_BLOCK, MIN_WARPS_PER_EXECUTION_UNIT) myKernel(int *data, MyStruct *kinfo) {
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx < N) {
    kinfo[idx].idx = idx;
    kinfo[idx].thread_id   = threadIdx.x;
    kinfo[idx].block_id    = blockIdx.x;
    kinfo[idx].blockdim_id = blockDim.x;
  }
}
void initialize(int *data, int n) {
  std::mt19937 gen(std::time(0));
  std::uniform_int_distribution<> dis(1, 10000);  // 1~10000 사이의 랜덤 넘버
  for (int i = 0; i < n; ++i) {
    data[i] = dis(gen); // random distribution
  }
}
void print_data(int *data, int n) {
  for (int i = 0; i < 10; ++i) std::cout << data[i] << " "; // first 10 items
  std::cout << " ... ";
  for (int i = n-10; i < n; ++i) std::cout << data[i] << " "; // last 10 items
  std::cout << std::endl;
}

int main() {
  int *hostData;
  int *deviceData;
  MyStruct *d_krnInfo;
  MyStruct *h_krnInfo;

  int data_buffer_size = N * sizeof(int);
  hostData = new int[N];
  //initialize(hostData, N); // hostData 를 초기화 (랜덤데이터)
  //print_data(hostData, N);

  hipMalloc((void**)&deviceData, data_buffer_size); // 디바이스 메모리 할당(data)
  hipMemcpy(deviceData, hostData, data_buffer_size, hipMemcpyHostToDevice); // host -> device

  h_krnInfo = new MyStruct[N];
  int krninfo_size = N * sizeof(struct MyStruct);
  hipMalloc((void**)&d_krnInfo, krninfo_size); // 디바이스 메모리 할당(kernel info)

  // Kernel 실행: N개의 스레드를 1개의 블록에서 실행
  myKernel<<<GRIDSIZE, BLOCKSIZE>>>(deviceData, d_krnInfo);
  //hipLaunchKernelGGL(myKernel, GRIDSIZE, BLOCKSIZE, 0, 0, deviceData, d_krnInfo);
  hipDeviceSynchronize();

  hipMemcpy(h_krnInfo, d_krnInfo, krninfo_size, hipMemcpyDeviceToHost); // device -> host (krninfo)

  std::cout << "\tidx\t\tthread_id\tblock_id\tblockdim_id" << std::endl;
  for (int i = 0; i < N; i++) {
    std::cout << ""
       << "\t" << h_krnInfo[i].idx
       << "\t\t" << h_krnInfo[i].thread_id
       << "\t\t" << h_krnInfo[i].block_id
       << "\t\t" << h_krnInfo[i].blockdim_id
       << std::endl;
  }

  hipFree(deviceData);
  hipFree(d_krnInfo);
  return 0;
}
