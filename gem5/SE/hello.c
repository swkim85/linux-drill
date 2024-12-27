#include <stdio.h>
#include <unistd.h>

int foo() {
  int i = 0;
  for (i = 0; i < 1000; i++) {
    usleep(10);
    printf("%d, Hello world!\n", i);
  }
}

int main(int argc, char* argv[]) {
  foo();
  return 0;
}
