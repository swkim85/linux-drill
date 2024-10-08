
//#include <x86intrin.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef N
#define N 200
#endif

void buffer_print(float *buf, int n) {
  float v;
  printf("buffer_print n=%d \n", n);
  #pragma GCC unroll 0
  for (int i = 0; i < n; i++)  {
    v = buf[i];
    printf("%3.1f ", v);
  }
  printf("\n");
}
void init_buffer(float *vec, int n) {
  #pragma GCC unroll 0
  for (int i = 0; i < n; ++i) {
    vec[i] = 1.0f; 
  }    
}

int main() {
  const int noTests = 1; //Number of tests
  float a[N], b[N], c[N], result[N];

  init_buffer(a, N);
  init_buffer(b, N);
  init_buffer(c, N);

  for (int i = 0; i < noTests; ++i) {
    for (int j = 0; j < N; ++j) {
      result[j] = a[j] + b[j] * c[j];
    }
  }

#ifdef PRINT
  buffer_print(result, N);
#endif

  return 0;
}

