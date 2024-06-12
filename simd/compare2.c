#include <immintrin.h>
#include <stdio.h>

int main() {
  __m512 vec_a = _mm512_set_ps(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
  __m512 vec_b = _mm512_set_ps(3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3);
  //float mem[16] __attribute__((aligned(64)));
  //_mm512_store_ps(mem, mem_vals);

  __mmask16 k2 = 0xFFFF;
  __mmask16 k3;

/*
  __asm__ volatile ( "vcmpgtps 0xc0(%1), %2, %0 %{%3%}"
    : "=k" (k3)
    : "r"(mem), "x"(zmm0), "k"(k2)
  );
*/

  __asm__ volatile ( "vcmpltps %[value_b], %[value_zmm0], %[value_k3] %{%%k2%}"
    : [value_k3] "=m" (k3)
    : [value_b] "m"(vec_b), [value_zmm0] "r"(vec_a), "r"(k2)
  );


  printf("(k3 mask): 0x%04x\n", k3);
  return 0;
}

