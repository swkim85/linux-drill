#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] ) {
  if (argc != 2) {
    printf("사용법: %s <메모리_크기(MB)>\n", argv[0]);
    return 1;
  }
  size_t size = atoi(argv[1]);
  int *ptr;

  size = size * 1024 * 1024; // MB

  ptr = (int *)malloc(size);

  if (ptr == NULL) {
    printf("메모리 할당에 실패했습니다.\n");
    return 1;
  }

  // 할당된 메모리에 쓰기
  for (size_t i = 0; i < size / sizeof(int); i++) {
     ptr[i] = i;
  }
  printf("%zu 바이트의 메모리가 성공적으로 할당되었습니다.\n", size);
  //sleep(100);

  free(ptr);
  printf("메모리가 해제되었습니다.\n");
  return 0;
}
