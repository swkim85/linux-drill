// par1.c
#include <stdio.h>
#include <omp.h>

int main (void) {
#ifdef NESTED
  omp_set_nested(1);
#endif
  #pragma omp parallel
  {
    printf ("thread ID=%d num_thds=%d level=%d\n",
      omp_get_thread_num(), omp_get_num_threads(), omp_get_level());
    #pragma omp parallel
    {
      printf ("thread ID=%d num_thds=%d level=%d\n",
        omp_get_thread_num(), omp_get_num_threads(), omp_get_level());
    }
  }
  return 0;
}
