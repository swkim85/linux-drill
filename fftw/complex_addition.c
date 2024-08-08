
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>

int main() {
  int x;
  float complex z1, z2, result;

  // 1에서 100 사이의 난수 생성
  int z1r = rand() % 100 + 1;
  int z1i = rand() % 100 + 1;
  int z2r = rand() % 100 + 1;
  int z2i = rand() % 100 + 1;

  z1 = z1r + z1i * I;
  z2 = z2r + z2i * I;
  printf("z1 = %.2f + %.2fi\n", creal(z1), cimag(z1));
  printf("z2 = %.2f + %.2fi\n", creal(z2), cimag(z2));

  x = 6;
  result = z1 + z2;
  x = 6;
  printf("z1 + z2 = %.2f + %.2fi\n", creal(result), cimag(result));

  return 0;
}
