
// fibonacci.c
#include <stdio.h>
static int fibonacci(int n) {
  if (n <= 1) return 1;
  return fibonacci(n-1) + fibonacci(n-2);
}
int main(void) {
  printf("Inside main()\n");
  int i = 0, value;
  for(;i<40;i++) {
    value = fibonacci(i);
    printf("%d %d\n", i, value);
  }
  return 0;
}
