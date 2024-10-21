#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>

#define COUNTER_SIZE  0x1FFFFFF

void *loop(  ) {
  int count = 0;

  while (1) {
    count++;
    if (count == COUNTER_SIZE) count = 0;
  }
}

int main(int argc, char **argv) {
  loop();
  return 0;
}

