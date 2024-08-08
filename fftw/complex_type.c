
// complex_type.c

#include <stdio.h>
#include <complex.h>

int main() {

  printf("sizeof double complex = %ld\n", sizeof(double complex));
  printf("sizeof float complex = %ld\n", sizeof(float complex));
  printf("sizeof int = %ld\n", sizeof(int));

  float complex a = 1.0 + 2.0 * I;
  printf("a = %.2f + %.2fi\n", creal(a), cimag(a));

  int *ai;
  ai = (int *)&a;
  printf("ai[0] = %x \n", *ai);
  printf("ai[1] = %x \n", *(ai+1));

  return 0;
}


