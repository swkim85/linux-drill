// loop.c
#include <stdio.h>
#include <omp.h>

int main() {
  int i, tid;
  omp_set_num_threads(4);

#pragma omp parallel private(tid)
{
  int tid = omp_get_thread_num();

  #pragma omp for
  for (i = 0; i < 20; i++) {
    printf("Hello World %d %d\n", tid, i);
  }
}

  return 0;
}




