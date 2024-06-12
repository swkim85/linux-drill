// ops.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char *argv[]) {
  uint64_t start, end;
  long long unsigned int elapsed;
  int i;
  if (argc < 2) { fprintf(stderr, "Usage: %s <iteration> \n", argv[0]); return 1; }
  int iteration = atoi(argv[1]);

  double da = 1.0, db = 1.0, dc = 0.0;
  start = rdtsc();
  for (i = 0; i < iteration; i++) {
    dc = (da * db) + dc;
  }
  end = rdtsc();
  elapsed = end - start;
  printf("iter=%d double mul+sum Elapsed Time (cycles): %llu\n", iteration, elapsed);

  start = rdtsc();
  for (i = 0; i < iteration; i++) {
    dc = (da / db) + dc;
  }
  end = rdtsc();
  elapsed = end - start;
  printf("iter=%d double div+sum Elapsed Time (cycles): %llu\n", iteration, elapsed);

  int ia = 1.0, ib = 1.0, ic = 0.0;
  start = rdtsc();
  for (i = 0; i < iteration; i++) {
    ic = (ia * ib) + ic;
  }
  end = rdtsc();
  elapsed = end - start;
  printf("iter=%d int mult+sum   Elapsed Time (cycles): %llu\n", iteration, elapsed);
 
  return 0;
}

