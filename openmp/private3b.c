// private3b.c
#include <stdio.h>
#include <omp.h>

int total = 10;

#ifdef TEST
#pragma omp threadprivate(total)
#endif

int main() {
  int tid, total_threads;
  omp_set_num_threads(2);
  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    for (; total > 0; total--) {
      printf("Hello from thread %d of %d. total=%2d &total=%p\n", tid, total_threads, total, (void *)&total);
    }
  }
  printf("program terminated. tid=%d total=%d\n", tid, total);
  return 0;
}
