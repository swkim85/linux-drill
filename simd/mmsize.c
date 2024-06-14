
// mmsize.c 
#include <immintrin.h>
#include <stdio.h>
int main() {
  printf("size of __m256 %ld \n", sizeof(__m256));
  printf("size of __m512 %ld \n", sizeof(__m512));
  printf("size of float %ld \n", sizeof(float));
  printf("size of double %ld \n", sizeof(double));
  return 0;
}
