// dft.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define PI 3.14159265358979323846
#define  SIZE  1000

void DFT(double *in_real, double *in_imag, double *out_real, double *out_imag, int N) {
  #pragma omp parallel
  {
    #pragma omp for
    for (int k = 0; k < N; k++) {
      out_real[k] = 0;
      out_imag[k] = 0;

      for (int n = 0; n < N; n++) {
        double angle = 2 * PI * k * n / N;
        out_real[k] += in_real[n] * cos(angle) + in_imag[n] * sin(angle);
        out_imag[k] += -in_real[n] * sin(angle) + in_imag[n] * cos(angle);
      }
    }
  }
}

void IDFT(double *in_real, double *in_imag, double *out_real, double *out_imag, int N) {
  #pragma omp parallel
  {
    #pragma omp for
    for (int n = 0; n < N; n++) {
      out_real[n] = 0;
      out_imag[n] = 0;

      for (int k = 0; k < N; k++) {
        double angle = 2 * PI * k * n / N;
        out_real[n] += in_real[k] * cos(angle) - in_imag[k] * sin(angle);
        out_imag[n] += in_real[k] * sin(angle) + in_imag[k] * cos(angle);
      }
      out_real[n] /= N;
      out_imag[n] /= N;
    }
  }
}

void initialize(double *real, double *imag, int n) {
  // 주파수 성분 설정
  double freq1 = 20.0;
  double freq2 = 30.0;
  double freq3 = 40.0;
  double sample_rate = 1000.0; // sampling rate

  // 입력 데이터 초기화 (3개의 주파수 성분 혼합)
  for (int i = 0; i < n; i++) {
    double t = (double)i / sample_rate;
    double signal = sin(2*PI * freq1 * t) + sin(2*PI * freq2 * t) + sin(2*PI * freq3 * t);
    real[i] = signal; // real part
    imag[i] = 0; // imag is zero
  }
}

int main() {
  int N = SIZE;
  double in_real[SIZE], in_imag[SIZE];
  double out_real[SIZE], out_imag[SIZE];
  double inv_real[SIZE], inv_imag[SIZE];
  double start_time, end_time;

  initialize(in_real, in_imag, N);

  // DFT
  start_time = omp_get_wtime();
  DFT(in_real, in_imag, out_real, out_imag, N);
  end_time = omp_get_wtime();
  printf("Elapsed time: %f seconds\n", end_time - start_time);

  // IDFT
  IDFT(out_real, out_imag, inv_real, inv_imag, N);

  for (int i = 0; i < N; i++) {
    printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\n", i, in_real[i], in_imag[i], out_real[i], out_imag[i], inv_real[i], inv_imag[i]);
  }

  return 0;
}
