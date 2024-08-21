#include <hip/hip_runtime.h>
#include <stdio.h>

//#define WIDTH  16
#define WIDTH  4

// GPU에서 실행되는 커널 함수
__global__ void matrixMul(const float* A, const float* B, float* C, int width) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    float sum = 0.0f;

    if (row < width && col < width) {
        for (int k = 0; k < width; k++) {
            sum += A[row * width + k] * B[k * width + col];
        }
        C[row * width + col] = sum;
    }
}

int main() {
    // 호스트 메모리 할당
    float A[WIDTH * WIDTH];
    float B[WIDTH * WIDTH];
    float C[WIDTH * WIDTH];

    // 행렬 초기화
    for (int i = 0; i < WIDTH * WIDTH; i++) {
        A[i] = 1.0f;
        B[i] = 2.0f;
    }

    // 디바이스 메모리 할당
    float *d_A, *d_B, *d_C;
    hipMalloc(&d_A, WIDTH * WIDTH * sizeof(float));
    hipMalloc(&d_B, WIDTH * WIDTH * sizeof(float));
    hipMalloc(&d_C, WIDTH * WIDTH * sizeof(float));

    // 호스트에서 디바이스로 데이터 복사
    hipMemcpy(d_A, A, WIDTH * WIDTH * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_B, B, WIDTH * WIDTH * sizeof(float), hipMemcpyHostToDevice);

    // 그리드와 블록 차원 설정
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((WIDTH + threadsPerBlock.x - 1) / threadsPerBlock.x, 
                       (WIDTH + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // 커널 실행
    hipLaunchKernelGGL(matrixMul, blocksPerGrid, threadsPerBlock, 0, 0, d_A, d_B, d_C, WIDTH);

    // 디바이스에서 호스트로 결과 복사
    hipMemcpy(C, d_C, WIDTH * WIDTH * sizeof(float), hipMemcpyDeviceToHost);

    // 결과 출력
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%f ", C[i * WIDTH + j]);
        }
        printf("\n");
    }

    // 메모리 해제
    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);

    return 0;
}

