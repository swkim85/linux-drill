// hello_omp.c
#include <stdio.h>
#include <omp.h>

int main() {
  int tid, total_threads;
  #pragma omp parallel
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    printf("Hello from thread %d/%d\n", tid, total_threads);
  }
  return 0;
}
