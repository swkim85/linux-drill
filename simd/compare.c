// compare.c
#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>

void print_m512i(__m512 vec) {
  float result[16] __attribute__((aligned(64)));
  _mm512_store_ps(result, vec);
  for (int i = 0; i < 16; ++i) {
    printf("%f ", result[i]);
  }
  printf("\n");
}
int main() {
  float a[16] __attribute__((aligned(64)))
     = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
  float b[16] __attribute__((aligned(64)))
     = { 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1 };
  float c[16] __attribute__((aligned(64)))
     = { 8,8,8,8, 0,0,0,0, 0,0,0,0, 8,8,8,8 };
  __m512 vec_a = _mm512_loadu_ps(a);
  __m512 vec_b = _mm512_loadu_ps(b);
  __m512 vec_c = _mm512_loadu_ps(c);
  __mmask16 mask ;
  mask = _mm512_cmp_ps_mask (vec_a, vec_b, _CMP_LT_OS);
  printf("mask : %04X \n", mask);   // a가 b 보다 작은지 비교(하위 8비트는 true)

  mask = _mm512_cmp_ps_mask (vec_b, vec_c, _CMP_GT_OS);
  printf("mask : %04X \n", mask); // b가 c보다 큰지 비교(상위 4비트는 false)
  return 0;
}
