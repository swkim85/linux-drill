// par2.c
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#define N 10

int main() {
  int i;
  int array[N], result[N];

  for (i = 0; i < N; i++) { array[i] = i + 1; result[i] = 0; }

  #pragma omp parallel shared(array, result) private(i)
  {
    printf ("loop#1 thread ID=%d num_thds=%d\n", omp_get_thread_num(), omp_get_num_threads());
    //if (omp_get_thread_num() == 0) sleep(1);

    #pragma omp for nowait
    for (i = 0; i < N; i++) {
      //printf ("loop#2 thread ID=%d num_thds=%d i=%d\n", omp_get_thread_num(), omp_get_num_threads(), i);
      result[i] = array[i] * array[i];
    }

    #pragma omp barrier   // 모든 스레드가 여기서 대기

    #pragma omp critical
    {
      for (i = 0; i < N; i++) printf("%d ", result[i]);
      printf(" <== tid=%d\n", omp_get_thread_num());
    }

    #pragma omp for
    for (i = 1; i < N; i++) {
      //printf ("loop#3 thread ID=%d num_thds=%d i=%d\n", omp_get_thread_num(), omp_get_num_threads(), i);
      result[i] += result[i-1];
    }
  }

  for (i = 0; i < N; i++) printf("%d ", result[i]);
  printf(" <= result \n");

  return 0;
}
