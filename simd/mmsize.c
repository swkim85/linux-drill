
// mmsize.c 
#include <immintrin.h>
#include <stdio.h>
int main() {
  printf("size of __m256 %ld \n", sizeof(__m256));
  printf("size of __m512 %ld \n", sizeof(__m512));
  return 0;
}
