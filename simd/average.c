// average.c
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include "rdtsc.h"

#define N 100000

static void init_buffer (float *buf, int n) {
  int i;
  for (i = 0; i < n; i++) {
    buf[i] = i;
  }
}
static void print_buffer (float *buf, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%3.1f ", buf[i]);
  }
  printf("\n");
}

int main() {
  int i;
  uint64_t start, end;
  long long unsigned int elapsed;
  float  vec1[N];
  float  vec2[N];

  init_buffer(vec1, N);
  //printf("vec1 = "); print_buffer(vec1, N);

  int idx1, idx2, idx3;
  start = rdtsc();
  for (i = 0; i < N; i++) {
         if (i == 0) { idx1 = 0; idx2 = 0; idx3 = 1; }
    else if (i == N-1) { idx1 = N-2; idx2 = N-1; idx3 = N-1; }
    else { idx1 = i-1; idx2 = i; idx3 = i+1; }
    vec2[i] = (vec1[idx1] + vec1[idx2] + vec1[idx3])/3;
  }
  end = rdtsc();
  elapsed = end - start;
  printf("Elapsed Time (cycles): %llu\n", elapsed);
  //printf("vec2 = "); print_buffer(vec2, N);
  return 0;
}

