#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBLOCKS  2048  // 2048 * 1MB = 2GB (max)

int main( int argc, char *argv[] ) {
  if (argc != 2) {
    printf("사용법: %s <메모리_크기(MB)>\n", argv[0]);
    return 1;
  }
  size_t size = atoi(argv[1]);
  int *ptr[MAXBLOCKS];

  size_t size1m = 1024 * 1024; // 1 MB
  int i;

  for (i = 0; i < size; i++) {
    printf("malloc 1MB memory ... %d\n", i);
    ptr[i] = (int *)malloc(size1m);
    if (ptr[i] == NULL) {
      printf("메모리 할당에 실패했습니다.\n");
      return 1;
    }
  }
  printf("1MB * %zu 메모리가 성공적으로 할당되었습니다.\n", size);

  // 할당된 메모리에 쓰기
  for (i = 0; i < size; i++) {
    printf("write block ... %d\n", i);
    for (size_t j = 0; j < size1m / sizeof(int); j++) {
      ptr[i][j] = j;
    }
  }
  //sleep(100);

  for (i = 0; i < size; i++) {
    free(ptr[i]);
  }
  printf("메모리가 해제되었습니다.\n");
  return 0;
}
