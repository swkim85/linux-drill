#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1048576 // 1MB in bytes

int main() {
  FILE *fp;
  char *buffer;
  clock_t start, end;
  double cpu_time_used;
  int seconds_to_run = 10; // 프로그램 실행 시간 (초)
  int bytes_written = 0;

  // 파일 열기
  fp = fopen("output.bin", "wb");
  if (fp == NULL) { perror("파일을 열 수 없습니다"); return 1; }

  // 1MB 버퍼 할당
  buffer = (char*)malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    perror("메모리 할당 실패"); fclose(fp); return 1;
  }

  // 버퍼를 임의의 데이터로 채우기
  memset(buffer, 'A', BUFFER_SIZE);

  printf("초당 1MB를 %d초 동안 파일에 쓰는 중...\n", seconds_to_run);

  start = clock();
  end = start + seconds_to_run * CLOCKS_PER_SEC;

  while (clock() < end) {
    size_t written = fwrite(buffer, 1, BUFFER_SIZE, fp);
    if (written != BUFFER_SIZE) {
      perror("파일 쓰기 오류");
      break;
    }
    bytes_written += written;

    // 1초 대기
    while (clock() - start < CLOCKS_PER_SEC) {
      // 대기
    }
    start = clock();
  }

  cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;

  printf("총 %d 바이트를 %.2f 초 동안 썼습니다.\n", bytes_written, cpu_time_used);
  printf("평균 쓰기 속도: %.2f MB/s\n", (double)bytes_written / (1024 * 1024) / cpu_time_used);

  free(buffer);
  fclose(fp);
  return 0;
}
