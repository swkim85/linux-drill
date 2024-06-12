// hello_avx.c
#include <immintrin.h>
#include <stdio.h>
#include "rdtsc.h"
int main() {
   uint64_t start, end;
   long long unsigned int elapsed;

  // 8개의 float32(single precision) 값을 256비트 벡터에 할당함
  __m256 evens = _mm256_set_ps (2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);
  __m256 odds  = _mm256_set_ps (1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0);

  start = rdtsc();
  __m256 result = _mm256_sub_ps (evens, odds); // 두 벡터의 차이(subtract)를 구함
  end = rdtsc();
  elapsed = end - start;
  printf("Elapsed Time (cycles): %llu\n", elapsed);

  float* f = (float*)&result; // 결과를 출력
  printf("%f %f %f %f %f %f %f %f \n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
  return 0;
}
