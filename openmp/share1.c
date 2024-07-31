// share1.c
#include <stdio.h>
#include <omp.h>

int main() {
  int a = 0;
  #pragma omp parallel
  {
    if (omp_get_thread_num() == 0) {
      int total_threads = omp_get_num_threads();
      printf("total threads %d\n", total_threads);
    }
    #pragma omp sections firstprivate(a) lastprivate(a)
    {
      #pragma omp section
      {
        printf("section 1: a 초기값 = %d\n", a);
        a = 1;
        printf("section 1: a 수정값 = %d\n", a);
      }
      #pragma omp section
      {
        printf("section 2: a 초기값 = %d\n", a);
        a = 2;
        printf("section 2: a 수정값 = %d\n", a);
      }
      #pragma omp section
      {
        printf("section 3: a 초기값 = %d\n", a);
        a = 3;
        printf("section 3: a 수정값 = %d\n", a);
      }
    }
  }
  printf("a 최종값 = %d\n", a);
  return 0;
}
