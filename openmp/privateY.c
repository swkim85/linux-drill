// privateX.c
#include <stdio.h>
#include <omp.h>
int main() {
  int tid, total_threads;
  int i;
  omp_set_num_threads(2);
  #pragma omp parallel private(tid, i)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    for (i = 0; i < 5; i++) {
      printf("Hello from thread %d of %d. i=%d\n", tid, total_threads, i);
    }
  }
  printf("program terminated.\n");
  return 0;
}
