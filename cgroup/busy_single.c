#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>

#define COUNTER_SIZE  0xFFFFFFF

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

void *loop(  ) {
  int count = 0;
  double cpu_usage = calculate_cpu_usage(); // initialize

  while (1) {
    count++;
    if (count == COUNTER_SIZE) {
      cpu_usage = calculate_cpu_usage();
      int nice_val = getpriority(PRIO_PROCESS, 0);
      printf("nice:%d CPU Usage: %.2f%%\n", nice_val, cpu_usage);
    }
    if (count == COUNTER_SIZE) count = 0;
  }
}

int main(int argc, char **argv) {
  pid_t pid = getpid();
  printf("pid: %d\n", pid);
  loop();
  return 0;
}

