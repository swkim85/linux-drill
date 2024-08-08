
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#define N 1000  // 데이터 포인트 수
#define PI 3.14159265358979323846

void  initialize(fftw_complex *vec, int n) {
    // 주파수 성분 설정
    double freq1 = 50.0;  // 첫 번째 주파수 (Hz)
    double freq2 = 30.0; // 두 번째 주파수 (Hz)
    double freq3 = 40.0; // 세 번째 주파수 (Hz)
    double sample_rate = 1000.0; // sampling rate

    for (int i = 0; i < N; i++) {
        double t = (double)i / sample_rate;
        double signal = sin(2*PI * freq1 * t) + sin(2*PI * freq2 * t) + sin(2*PI * freq3 * t);
        vec[i] = signal + 0.0 * I;
    }
}

int main() {
    fftw_complex *in, *out;
    fftw_plan p;

    // 입력과 출력을 위한 메모리 할당
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    initialize(in, N);

    // FFTW 계획 생성
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p); // FFT 실행

    printf("FFT 결과:\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%f\t%f\t%f\t%f\n", i, creal(in[i]), cimag(in[i]), creal(out[i]), cimag(out[i]) );
    }

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    return 0;
}
