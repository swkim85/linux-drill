// private4.c
#include <stdio.h>
#include <omp.h>

int main() {
  int tid, total_threads;
  int i, last_value = 0;
  omp_set_num_threads(2);
  #pragma omp parallel private(tid, i)
  {
    tid = omp_get_thread_num();
    total_threads = omp_get_num_threads();
    #pragma omp lastprivate(last_value)
    for (i = 0; i < 10; i++) {
      printf("Hello from thread %d of %d. i=%d\n", tid, total_threads, i);
      last_value = tid * 100 + i;
    }
  }
  printf("program terminated. tid=%d last_value=%d\n", tid, last_value);
  return 0;
}




