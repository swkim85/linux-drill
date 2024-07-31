// gomp_version.c
#include <stdio.h>
#include <omp.h>
int main() {
  printf("OpenMP version: %d\n", _OPENMP);
  return 0;
}

