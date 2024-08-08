
// fftw_example.c 
#include <stdio.h>
#include <complex.h>
#include <fftw3.h>

#define N 8

int main() {
    fftw_complex *in, *out;
    fftw_plan p;

    // 입력과 출력을 위한 메모리 할당
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    // 입력 데이터 초기화
    for (int i = 0; i < N; i++) {
        in[i] = i + 0.0 * I;  // 실수부는 i, 허수부는 0
    }

    // FFTW 계획 생성
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // FFT 실행
    fftw_execute(p);

    // 결과 출력
    printf("FFT 결과:\n");
    for (int i = 0; i < N; i++) {
        printf("%d: %f + %fi\n", i, creal(out[i]), cimag(out[i]));
    }

    // 메모리 해제 및 정리
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return 0;
}
