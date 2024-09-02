#include <hip/hip_runtime.h>
#include <stdio.h>

__global__ void resourceIntensiveKernel() {
    int largeArray[409600];
    
    // 배열 초기화
    for (int i = 0; i < 1024; i++) {
        largeArray[i] = i;
    }
    
    // 복잡한 계산 시뮬레이션
    int sum = 0;
    for (int i = 0; i < 1024; i++) {
        sum += largeArray[i] * largeArray[1023 - i];
    }
    
    printf("Thread %d, Sum: %d\n", threadIdx.x, sum);
}

int main() {
    // 큰 블록 크기 설정
    dim3 blockSize(1024, 1, 1);
    dim3 gridSize(1, 1, 1);
    
    // 커널 실행
    hipLaunchKernelGGL(resourceIntensiveKernel, gridSize, blockSize, 0, 0);
    
    // 동기화
    hipDeviceSynchronize();
    
    // 오류 확인
    hipError_t error = hipGetLastError();
    if (error != hipSuccess) {
        printf("HIP error: %s\n", hipGetErrorString(error));
    }
    
    return 0;
}
