#include <hip/hip_runtime.h>
#include <iostream>

#define N 10

__global__ void myKernel(int *data, int *output) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < N) {
        // 각 스레드가 output 배열에 자신의 데이터를 문자열 형태로 저장
        output[idx * 2] = idx;
        output[idx * 2 + 1] = data[idx];
        //sprintf(&output[idx * 32], "Thread %d: data = %d\n", idx, data[idx]);
    }
}

int main() {
    int hostData[N] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *deviceData;
    int *deviceOutput;
    int hostOutput[N * 2];

    // Device 메모리 할당
    hipMalloc((void**)&deviceData, N * sizeof(int));
    hipMalloc((void**)&deviceOutput, N * 2 * sizeof(int));

    // Host에서 Device로 데이터 복사
    hipMemcpy(deviceData, hostData, N * sizeof(int), hipMemcpyHostToDevice);

    // Kernel 실행: 10개의 스레드를 1개의 블록에서 실행
    myKernel<<<1, N>>>(deviceData, deviceOutput);

    // Kernel 실행이 완료될 때까지 대기
    hipDeviceSynchronize();

    // Device에서 Host로 출력 결과 복사
    hipMemcpy(hostOutput, deviceOutput, N * 2 * sizeof(int), hipMemcpyDeviceToHost);

    // 출력 결과 표시
    //std::cout << hostOutput;
    // CPU에서 포맷팅하여 출력
    for (int i = 0; i < N; i++) {
      std::cout << "Thread " << hostOutput[i * 2] << ": data = " << hostOutput[i * 2 + 1] << std::endl;
    }

    // Device 메모리 해제
    hipFree(deviceData);
    hipFree(deviceOutput);

    return 0;
}
