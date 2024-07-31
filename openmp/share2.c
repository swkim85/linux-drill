// share2.c
#include <stdio.h>
#include <omp.h>

int main() {
  omp_set_num_threads(3); 
  #pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    printf("This is executed by all threads. Thread ID: %d\n", thread_id);
    #pragma omp single nowait
    {
      for (int i ; i < 10000000; i++) ;
      int total_threads = omp_get_num_threads();
      printf("total threads %d. Thread ID=%d\n", total_threads, thread_id);
    }
    printf("done. Thread ID: %d\n", thread_id);
  }
  return 0;
}
