// getcpu.c
#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>

int main() {
  int cpu;
  cpu = sched_getcpu();
  
  if (cpu == -1) {
      perror("sched_getcpu");
      return 1;
  }
  printf("현재 실행 중인 CPU 코어 번호: %d\n", cpu);
  return 0;
}

