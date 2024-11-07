#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>
#include <sys/resource.h>

#define MAX_THREADS  16
#define COUNTER_SIZE  0xfffffff

void curr_time(char *buf) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
}

double calculate_cpu_usage() {
  static clock_t last_cpu, last_sys_cpu, last_user_cpu;
  static int initialized = 0;

  struct tms time_sample;
  clock_t now;

  now = times(&time_sample);
  if (!initialized) {
    last_cpu = now;
    last_sys_cpu = time_sample.tms_stime;
    last_user_cpu = time_sample.tms_utime;
    initialized = 1;
    return 0.0;
  }

  double percent = ((time_sample.tms_stime - last_sys_cpu) +
                    (time_sample.tms_utime - last_user_cpu)) /
                   (double)(now - last_cpu);
  percent *= 100;

  last_cpu = now;
  last_sys_cpu = time_sample.tms_stime;
  last_user_cpu = time_sample.tms_utime;

  return percent;
}

void *loop( void *arg ) {
  int threadId = *(int *)arg;
  printf("threadId = %d\n", threadId);

  int count = INT_MIN;
  double cpu_usage = calculate_cpu_usage(); // initialize
  char buf[20];

  while (1) {
    count++;
    if (threadId == 0) {
      if (count == COUNTER_SIZE) {
        curr_time(buf);
        cpu_usage = calculate_cpu_usage();
        int nice_val = getpriority(PRIO_PROCESS, 0);
        printf("%s nice:%d CPU Usage: %.2f%%\n", buf, nice_val, cpu_usage);
      }
    }
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

