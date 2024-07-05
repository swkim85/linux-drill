// private2.c
#include <stdio.h>
#include <omp.h>

int main() {
  int tid, total_threads;
  int i = 10;
  #pragma omp parallel private(tid) firstprivate(i)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    for (; i < 13; i++) {
      printf("Hello from thread %d of %d. i=%d\n", tid, total_threads, i);
    }
  }
  printf("program terminated.\n");
  return 0;
}
