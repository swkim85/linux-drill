// opsth.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rdtsc.h"
typedef struct {
  int thread_no;
  int iteration;
} ThreadData;
void *worker(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  printf("thread no=%d iteration=%d\n", data->thread_no, data->iteration);
  double da = 1.0, db = 1.0, dc = 0.0;
  for (int i = 0; i < data->iteration; i++) {
    dc = (da * db) + dc;
  }
  pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
  int i;
  if (argc < 3) { fprintf(stderr, "Usage: %s <iteration> <#threads>\n", argv[0]); return 1; }
  int iteration = atoi(argv[1]);
  int nthreads = atoi(argv[2]);

  pthread_t  threads[100];
  ThreadData thread_data[100];
  uint64_t start, end;
  long long unsigned int elapsed;

  start = rdtsc();
  for (int i = 0; i < nthreads; i++) { // create threads
    thread_data[i].thread_no = i;
    thread_data[i].iteration = iteration/nthreads;
    pthread_create(&threads[i], NULL, worker, (void *)&thread_data[i]);
  }
  for (int i = 0; i < nthreads; i++) pthread_join(threads[i], NULL);
  end = rdtsc();
  elapsed = end - start;
  printf("iteration=%d nthreads=%d Elapsed Time (cycles): %llu\n",
       iteration, nthreads, elapsed);
  return 0;
}
