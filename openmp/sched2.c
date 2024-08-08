// sched1.c
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define N 10
#define CHUNKSIZE 1

int main() {
  int i;
  time_t start_time = time(NULL);
  #pragma omp parallel  private(i)
  {
    int sec;
    #pragma omp for schedule(runtime)
    for (i = 0; i < N; i++) {
      time_t t = time(NULL);
      sec = (i+1) * 2; // 2,4,6,8,...
      printf("t=%ld i=%d thread=%d worktime=%d \n", t-start_time, i, omp_get_thread_num(), sec);
      sleep(sec);
    }
  }
  time_t t = time(NULL);
  printf("t=%ld done. \n", t-start_time);
  return 0;
}
