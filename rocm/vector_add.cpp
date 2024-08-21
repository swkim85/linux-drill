#include <hip/hip_runtime.h>
#include <stdio.h>

#define N 100

__global__ void vector_add(const float *A, const float *B, float *C, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        C[idx] = A[idx] + B[idx];
    }
}

int main() {
    float *A, *B, *C;
    float *d_A, *d_B, *d_C;
    
    A = (float*)malloc(N * sizeof(float));
    B = (float*)malloc(N * sizeof(float));
    C = (float*)malloc(N * sizeof(float));
    
    hipMalloc(&d_A, N * sizeof(float));
    hipMalloc(&d_B, N * sizeof(float));
    hipMalloc(&d_C, N * sizeof(float));
    
    for (int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

    hipMemcpy(d_A, A, N * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_B, B, N * sizeof(float), hipMemcpyHostToDevice);
    
    dim3 threadsPerBlock(256);
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x);
    
    hipLaunchKernelGGL(vector_add, blocksPerGrid, threadsPerBlock, 0, 0, d_A, d_B, d_C, N);
    
    hipMemcpy(C, d_C, N * sizeof(float), hipMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        printf("C[%d] = %f\n", i, C[i]);
    }
    
    free(A);
    free(B);
    free(C);
    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);
    
    return 0;
}

