// helloworld.c
#include <stdio.h>

int add(int a, int b) {
  int sum;
  sum = a + b;
  return sum;
}

int main() {
  int number = add(123, 456);
  printf("Hello, number %d!\n", number);
  return 0;
}
