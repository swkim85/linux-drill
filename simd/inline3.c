#include <stdio.h>

int main() {
  int a = 5;
  int b = 3;

  // a의 메모리 주소를 참조하여 b의 값을 더함
  __asm__ (
      "addl %[val_b], %[val_a];"
      : [val_a] "=m" (a)  // a의 메모리 주소를 출력 피연산자로 지정
      : "m" (a), [val_b] "r" (b) // a와 b를 입력 피연산자로 지정
  );

  printf("Result: %d\n", a);  // 결과 출력
  return 0;
}

