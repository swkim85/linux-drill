// signzero.c
#include <stdio.h>
#include <math.h>
int main() {
  float positive_zero = 0.0;
  float negative_zero = -0.0;
  printf("1 / +0.0 = %f\n", 1.0 / positive_zero);  // +∞
  printf("1 / -0.0 = %f\n", 1.0 / negative_zero);  // -∞
  if (positive_zero == negative_zero) printf("+0.0 equals to -0.0\n");
  else printf("+0.0 not equals to -0.0\n");
}
