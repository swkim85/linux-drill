// gv1.c
#include <stdio.h>
#include <openacc.h>
#define N 1000
int main() {
  float a[N], b[N], c[N];
  printf("OpenACC version: %d\n", _OPENACC);
  for (int i = 0; i < N; i++) {
    a[i] = i * 1.0f;
    b[i] = i * 2.0f;
  }
  #pragma acc parallel loop
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
  }
  for (int i = 0; i < 10; i++) {
    printf("c[%d] = %f\n", i, c[i]);
  }
  return 0;
}
