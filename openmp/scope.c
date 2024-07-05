// scope.c
#define _GNU_SOURCE
#include <stdio.h>
#include <omp.h>
#include <sched.h>

int main() {
  int i = 10, tid, a[12];

  omp_set_num_threads(4);

#pragma omp parallel shared(a) private(tid) firstprivate(i)
{
    int tid = omp_get_thread_num();
    int total_threads = omp_get_num_threads();
    int cpu = sched_getcpu();
    printf("Hello from thread %d out of %d threads. core %d \n", tid, total_threads, cpu);

    int idx = tid * 3;
    i = i + idx;
    a[idx] = i;
    a[idx+1] = i+1; 
    a[idx+2] = i+2; 
}

  for (i = 0; i < 12; i++) {
     printf("a[%d] = %d\n", i, a[i]);
  }

  return 0;
}




