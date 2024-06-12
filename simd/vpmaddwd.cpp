// vpmaddwd.cpp
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
         1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1,  1,  1,  1,  1,  1,
         2,  2,  2,  2,  2,  2,  2,  2, 2,  2,  2,  2,  2,  2,  2,  2 };

    // load vector
    __m512i vec_a = _mm512_load_epi32(a); // extended packed integer
    __m512i vec_b = _mm512_load_epi32(b);

    // _mm512_madd_epi16 : multiply and add
    __m512i result = _mm512_madd_epi16(vec_a, vec_b);

    print_m512i(result);
    return 0;
}

