// vec2.c
#include <stdio.h>
#include <stdlib.h>
#include "rdtsc.h"

#ifndef N
#define N 200
#endif

void init_buffer(float *vec, int n) {
  for (int i = 0; i < n; ++i) {
    vec[i] = 1.0f; 
  }    
}
void buffer_print(float *buf, int n) {
  float v;
  printf("buffer_print n=%d \n", n);
  for (int i = 0; i < n; i++)  {
    v = buf[i];
    printf("%3.1f ", v);
  }
  printf("\n");
}
int main() {
  float a[N], b[N], c[N], result[N];
  init_buffer(a, N);
  init_buffer(b, N);
  init_buffer(c, N);

  uint64_t start, end;
  long long unsigned int elapsed;

  start = rdtsc();
  #pragma GCC unroll 0
  for (int j = 0; j < N; ++j) {
    result[j] = a[j] + b[j] * c[j];
  }
  end = rdtsc();
  elapsed = end - start;
  printf("loop Elapsed Time (cycles): %llu\n", elapsed);
  buffer_print(result, N);
  return 0;
}
