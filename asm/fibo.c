// fibo.c
#include <stdio.h>
#include <inttypes.h>
uint64_t fibonacci_asm(uint64_t n);
int main() {
  uint64_t n = 0;  uint64_t f = 1;
  n = 20; f = fibonacci_asm(n); printf("f(%lu) = %lu\n", n, f);
  return 0;
}
