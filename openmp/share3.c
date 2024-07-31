// share3.c
#include <stdio.h>
#include <omp.h>

#define N 10

int main() {
  int i, array[N], result[N];
  for (i = 0; i < N; i++) {
    array[i] = i + 1;
  }
  #pragma omp parallel shared(array, result) private(i) 
  {
    int tid = omp_get_thread_num();
    #pragma omp for
    for (i = 3; i < N; i++) {
      printf("tid=%d i=%d\n", tid, i);
      result[i] = array[i] * array[i];
      //if (i > 5) break;
    }
  }

  printf("Squared elements of array:\n");
  for (i = 0; i < N; i++) {
    printf("%d ", result[i]);
  }
  printf("\n");
  return 0;
}

