// serial_pi.c
#include <stdio.h>
#include <omp.h>
#ifndef N
#define N 100000
#endif
static long num_steps = N;
double step;
int main() {
  int i;
  double x, pi, sum = 0.0; 
  double start_time, end_time, elapsed_time;
  step = 1.0 / (double) num_steps;
  start_time = omp_get_wtime();
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum = sum + 4.0/(1.0 + x*x);
  }
  pi = step * sum;
  end_time = omp_get_wtime();
  elapsed_time = end_time - start_time;
  printf("pi = %.30f \n", pi);
  printf("Elapsed Time: %f seconds\n", elapsed_time);
  return 0;
}
