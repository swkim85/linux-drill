#include <hip/hip_runtime.h>
#include <iostream>

#define N 10

#define HIP_ENABLE_PRINTF 
#define HCC_ENABLE_PRINTF 

#include <hip/hip_runtime.h>

__global__ void run_printf() { printf("Hello World\n"); }

int main() {
  run_printf<<<dim3(1), dim3(1), 0, 0>>>();
}

/*
__global__ void myKernel(int *data) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < N) {
        printf("Thread %d: data = %d\n", idx, data[idx]);
    }
}

int main() {
    int hostData[N] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *deviceData;

    // Device 메모리 할당
    hipMalloc((void**)&deviceData, N * sizeof(int));

    // Host에서 Device로 데이터 복사
    hipMemcpy(deviceData, hostData, N * sizeof(int), hipMemcpyHostToDevice);

    // Kernel 실행: 10개의 스레드를 1개의 블록에서 실행
    myKernel<<<1, N>>>(deviceData);
    //hipLaunchKernelGGL(HIP_KERNEL_NAME(myKernel), dim3(1), dim3(N), 0, 0);

    // Kernel 실행이 완료될 때까지 대기
    hipDeviceSynchronize();

    // Device 메모리 해제
    hipFree(deviceData);

    return 0;
}
*/
