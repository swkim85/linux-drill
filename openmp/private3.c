// private3.c
#include <stdio.h>
#include <omp.h>

int total = 10;
#pragma omp threadprivate(total)

int main() {
  int tid, total_threads;
  omp_set_num_threads(2);
  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    for (; total > 0; total--) {
      printf("Hello from thread %d of %d. total=%d(%p)\n", tid, total_threads, total, &total);
    }
  }
  printf("program terminated. tid=%d total=%d(%p)\n", tid, total, &total);
  return 0;
}
