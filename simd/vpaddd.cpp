// vpaddd.cpp
#include <immintrin.h>
#include <iostream>

void print_m512i(__m512i vec) {
    alignas(64) int32_t result[16];
    _mm512_store_epi32(result, vec);
    for (int i = 0; i < 16; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
  alignas(64) int32_t a[16] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
  alignas(64) int32_t b[16] = {
      16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
  alignas(64) int32_t c[16] = {
      11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

  __m512i vec_a = _mm512_load_epi32(a);
  __m512i vec_b = _mm512_load_epi32(b);
  __m512i vec_c = _mm512_load_epi32(c);

  __m512i result;
  result = _mm512_add_epi32 (vec_a, vec_b); // add two vectors
  print_m512i(result);

  __mmask16 k = 0x00ff;
  result = _mm512_mask_add_epi32 (vec_c, k, vec_a, vec_b);
  print_m512i(result);
  return 0;
}

