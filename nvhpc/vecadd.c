//vecadd.c
#include <omp.h>
#include <stdio.h>
#define N 1000
int main() {
  int i;
  float a[N], b[N], c[N];

  for (i = 0; i < N; i++) {
    a[i] = i * 1.0f;
    b[i] = i * 2.0f;
  }

  #pragma omp parallel
  {
    printf("thread %d/%d \n", omp_get_thread_num(), omp_get_num_threads());

    #pragma omp target teams distribute parallel for
    for (i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
    }
  }

  for (i = 0; i < 10; i++) {
    printf("%f + %f = %f\n", a[i], b[i], c[i]);
  }
  return 0;
}

