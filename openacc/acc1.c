// acc1.c
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int main() {
  float *a = (float*) malloc(N * sizeof(float));
  float *b = (float*) malloc(N * sizeof(float));
  float *c = (float*) malloc(N * sizeof(float));

  for (int i = 0; i < N; i++) {
    a[i] = i * 1.0f;
    b[i] = i * 2.0f;
  }

  #pragma acc data copyin(a[0:N], b[0:N]) copyout(c[0:N])
  {
    #pragma acc parallel loop
    for (int i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
    }
  }

  for (int i = 0; i < 10; i++) printf("%f ", c[i]);
  printf("\n");
  free(a);
  free(b);
  free(c);
  return 0;
}

