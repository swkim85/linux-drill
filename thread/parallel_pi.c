// parallel_pi.c
#include <stdio.h>
#include <omp.h>
#ifndef N
#define N 100000
#endif
#ifndef NUM_THREADS
#define NUM_THREADS 2
#endif
static long num_steps = N;
double step;

int main() {
  int i, nthreads;
  double pi, sum[NUM_THREADS];
  double start_time, end_time, elapsed_time;
  step = 1.0 / (double) num_steps;

  start_time = omp_get_wtime();
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel
  {
    int i, id,nthrds;
    double x;
    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();
    if (id == 0) nthreads = nthrds;
    sum[id] = 0;
    for (i = id; i < num_steps; i = i + nthrds) {
      x = (i+0.5)*step;
      sum[id] += 4.0/(1.0+x*x);
    }
  }
  pi = 0;
  for (i=0; i < nthreads; i++) pi += sum[i] * step;
  end_time = omp_get_wtime();
  elapsed_time = end_time - start_time;
  printf("pi = %.30f \n", pi);
  printf("Elapsed Time: %f seconds\n", elapsed_time);
  return 0;
}
