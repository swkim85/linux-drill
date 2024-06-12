// vpdpwssd.cpp
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
    alignas(64) int16_t a[32] = {
         1,  2,  3,  4,  5,  6,  7,  8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    alignas(64) int16_t b[32] = {
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,
        16, 15, 14, 13, 12, 11, 10,  9, 8,  7,  6,  5,  4,  3,  2,  1 };
    alignas(64) int32_t c[16] = {
        100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

    __m512i vec_a = _mm512_load_epi32(a);
    __m512i vec_b = _mm512_load_epi32(b);
    __m512i vec_c = _mm512_load_epi32(c);
    __m512i result = _mm512_dpwssds_epi32(vec_c, vec_a, vec_b);
    print_m512i(result);
    return 0;
}

