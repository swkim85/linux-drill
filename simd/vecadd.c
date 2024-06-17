// vecadd.c
#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rdtsc.h"


#ifndef N
#define N 128
#endif

static void init_buffer (float *buf, int n, float value) {
  for (int i = 0; i < n; i++) {
    buf[i] = value;
  }
}
static void print_buffer (float *buf, int n) {
  for (int i = 0; i < n; i++) {
    printf("%3.1f ", buf[i]);
  }
  printf("\n");
}

void vector_add1 (float* a, float* b, float* result, int n) {
  assert(n % 4 == 0);
  for (int i = 0; i < n; i += 4) {
    __m128 va = _mm_loadu_ps(&a[i]);
    __m128 vb = _mm_loadu_ps(&b[i]);
    __m128 vresult = _mm_add_ps(va, vb);
    _mm_storeu_ps(&result[i], vresult);
  }
}

void vector_add2 (float a[], float b[], float result[], int n) {
  for (int i = 0; i < n; i++) {
    result[i] = a[i] + b[i];
  }
}

int main() {
  int i;
  uint64_t start, end;
  long long unsigned int elapsed;
  float va[N];
  float vb[N];
  float r1[N];
  float r2[N];

  printf("sizeof va = %lu\n", sizeof(va));

  init_buffer(va, N, 100);
  init_buffer(vb, N, 2);
  //printf("va = "); print_buffer(va, N);
  //printf("vb = "); print_buffer(vb, N);

  //start = rdtsc();
  //vector_add1(va, vb, r1, N);
  //end = rdtsc();
  //elapsed = end - start;
  //printf("with sse Elapsed Time (cycles): %llu\n", elapsed);

  //start = rdtsc();
  vector_add2(va, vb, r2, N);
  //end = rdtsc();
  //elapsed = end - start;
  //printf("without sse Elapsed Time (cycles): %llu\n", elapsed);

  //printf("r1 = "); print_buffer(r1, N);
  //printf("r2 = "); print_buffer(r2, N);
  return 0;
}

