// private3.c
#include <stdio.h>
#include <omp.h>

int total = 10;
#pragma omp threadprivate(total)

int main() {
  int tid, total_threads;
  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    for (; total > 0; total--) {
      printf("Hello from thread %d of %d. total=%d\n", tid, total_threads, total);
    }
  }
  printf("program terminated. tid=%d total=%d\n", tid, total);
  return 0;
}
