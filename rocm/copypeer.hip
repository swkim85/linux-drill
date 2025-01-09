
#include <hip/hip_runtime.h>

int main() {
    int *d_A, *d_B;
    int size = 1024 * sizeof(int);

    // GPU 0에 메모리 할당
    hipSetDevice(0);
    hipMalloc(&d_A, size);

    // GPU 1에 메모리 할당
    hipSetDevice(1);
    hipMalloc(&d_B, size);

    // GPU 0에서 GPU 1로 데이터 전송
    hipMemcpyPeer(d_B, 1, d_A, 0, size);

    // 메모리 해제
    hipFree(d_A);
    hipFree(d_B);

    return 0;
}

