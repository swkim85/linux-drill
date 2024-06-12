#include <immintrin.h>
#include <stdio.h>

int main() {
  __m512 vec_a = _mm512_set_ps(1,2,3,4, 5,6,7,8, 1,2,3,4, 5,6,7,8);
  __m512 vec_b = _mm512_set_ps(3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3);

  __mmask16 mask1 = 0xFF00;
  __mmask16 mask2;

// vec_a가 vec_b 보다 작은지 비교, mask1에 의해서 상위 8개 요소만 비교대상으로함
  __asm__ volatile ( "vcmpltps %[value_b], %[value_a], %[mask2]%{%[mask1]%}"
    : [mask2] "=Yk" (mask2)
    : [value_b] "x"(vec_b),
      [value_a] "x"(vec_a),
      [mask1] "Yk" (mask1)
  );

  printf("(mask2): 0x%04x\n", mask2);
  return 0;
}

