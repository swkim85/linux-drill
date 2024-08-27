#include <hip/hip_runtime.h>
#include <iostream>
#include <rocblas.h>

int main() {
    // 초기화
    rocblas_handle handle;
    rocblas_create_handle(&handle);

    // 행렬 크기 설정
    const int m = 3, n = 3, k = 3;
    const float alpha = 1.0f, beta = 0.0f;

    // 호스트 메모리 할당 및 초기화
    float *h_A = new float[m * k]{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    float *h_B = new float[k * n]{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    float *h_C = new float[m * n]{0.0f};

    // 디바이스 메모리 할당
    float *d_A, *d_B, *d_C;
    hipMalloc(&d_A, m * k * sizeof(float));
    hipMalloc(&d_B, k * n * sizeof(float));
    hipMalloc(&d_C, m * n * sizeof(float));

    // 데이터를 디바이스로 복사
    hipMemcpy(d_A, h_A, m * k * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_B, h_B, k * n * sizeof(float), hipMemcpyHostToDevice);

    // 행렬 곱셈 수행
    rocblas_sgemm(handle, rocblas_operation_none, rocblas_operation_none,
                  m, n, k, &alpha, d_A, m, d_B, k, &beta, d_C, m);

    // 결과를 호스트로 복사
    hipMemcpy(h_C, d_C, m * n * sizeof(float), hipMemcpyDeviceToHost);

    // 결과 출력
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << h_C[i * n + j] << " ";
        }
        std::cout << std::endl;
    }

    // 정리
    rocblas_destroy_handle(handle);
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);

    return 0;
}
