
#include <fftw3.h>
#include <stdio.h>

int main() {
    int N = 100;  // 데이터 포인트 수
    fftw_complex *in, *out;
    fftw_plan p;

    // 메모리 할당
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    // 입력 데이터 설정 (예시)
    for (int i = 0; i < N; i++) {
       if (i >= 49 && i <= 50) {
         in[i][0] = 0;  // 실수부
         in[i][1] = 1;  // 허수부
       }
    }

    // 역변환 계획 생성
    p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // 역변환 실행
    fftw_execute(p);

    // 결과 출력
    for (int i = 0; i < N; i++) {
        printf("%d\t%f\t%f\n", i, out[i][0] / N, out[i][1] / N);
    }

    // 정리
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return 0;
}
