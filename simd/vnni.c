// vnni.c
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include "rdtsc.h"

void print_m512i(__m512i vec) {
  int32_t result[16]  __attribute__((aligned(64)));
  _mm512_store_epi32(result, vec);
  for (int i = 0; i < 16; ++i) {
    printf("%d ", result[i]);
  }
  printf("\n");
}
int main() {
  int i;
  uint64_t start, end;
  long long unsigned int elapsed;
  int16_t a[32]   __attribute__((aligned(64))) = {
       1,  2,  3,  4,  5,  6,  7,  8, 9, 10, 11, 12, 13, 14, 15, 16,
      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
  int16_t b[32]  __attribute__((aligned(64))) = {
       1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1,  1,  1,  1,  1,  1,
       2,  2,  2,  2,  2,  2,  2,  2, 2,  2,  2,  2,  2,  2,  2,  2 };
  int32_t c[16]  __attribute__((aligned(64))) = {
       100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

  __m512i vec_a, vec_b, vec_c, tmp;
  __m512i result;

  start = rdtsc();
  vec_a  = _mm512_load_epi32(a); // a, b:  16bit x 32
  vec_b  = _mm512_load_epi32(b);
  for (i = 0; i < N; i++) {
    tmp    = _mm512_madd_epi16(vec_a, vec_b); // tmp : 32bit x 16
    result = _mm512_add_epi32(vec_c, tmp); // c = c + tmp
  }
  end = rdtsc();
  elapsed = end - start;
  printf("sleep Elapsed Time (cycles): %llu\n", elapsed);
  print_m512i(result);

  start = rdtsc();
  for (i = 0; i < N; i++) {
    result = _mm512_dpwssds_epi32(vec_c, vec_a, vec_b);
  }
  end = rdtsc();
  elapsed = end - start;
  printf("sleep Elapsed Time (cycles): %llu\n", elapsed);
  print_m512i(result);

  return 0;
}

