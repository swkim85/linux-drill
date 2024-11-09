// openat.c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024  // 버퍼 크기 정의

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
    return EXIT_FAILURE;
  }
  pid_t pid = getpid();
  printf("Current Process ID: %d\n", pid);
  sleep(20);

  int dir_fd = AT_FDCWD; // 현재 작업 디렉토리 기준
  int fd = openat(dir_fd, argv[1], O_RDONLY);
  if (fd == -1) { perror("openat"); return EXIT_FAILURE; }

  char buffer[BUF_SIZE];
  ssize_t bytesRead;
  while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0) {
    write(STDOUT_FILENO, buffer, bytesRead);
  }

  if (bytesRead == -1) { perror("read"); close(fd); return EXIT_FAILURE; }
  close(fd);
  return EXIT_SUCCESS;
}
