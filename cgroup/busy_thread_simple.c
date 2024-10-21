#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>
#include <sys/resource.h>

#define MAX_THREADS  16
#define COUNTER_SIZE  0xfffffff

void *loop( void *arg ) {
  int threadId = *(int *)arg;
  printf("threadId = %d\n", threadId);

  int count = INT_MIN;

  while (1) {
    count++;
    if (count == COUNTER_SIZE) count = 0;
  }
}

int main(int argc, char **argv) {
  pthread_t   thread[MAX_THREADS]; 
  int         threadIds[MAX_THREADS]; 

  int i = 0;
  int num_threads = 1;
  if (argc == 2) num_threads = atoi(argv[1]);

  pid_t pid = getpid();
  printf("pid: %d\n", pid);
  printf("threads: %d\n", num_threads);

  for (i = 0; i < num_threads; i++) {
    int ret;
    threadIds[i] = i;
    ret = pthread_create(&thread[i], NULL, loop, (void *)&threadIds[i]);
    if (ret != 0) {
      printf("pthread_create() failed. Error code %d\n", ret);
      exit(1);
    }
  }
  for (i = 0; i < num_threads; i++) {
    pthread_join(thread[i], NULL);
  }
  return 0;
}

