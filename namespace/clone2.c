#define _GNU_SOURCE
#include <sched.h>   // /usr/include/linux/sched.h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE 1024*1024  // 1MB

int childFunc(void *arg) {
  printf("child pid = %d\n", getpid());
  printf("Hello from the child !\n");
  sleep(30);
  return 0;
}

int main() {
  char *stack;
  char *stackTop;
  pid_t pid;

  stack = malloc(STACK_SIZE);
  if (stack == NULL) { perror("malloc"); exit(1); }
  
  stackTop = stack + STACK_SIZE;

//CLONE_NEWCGROUP

  pid = clone(childFunc, stackTop, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, NULL);
  if (pid == -1) { perror("clone"); exit(EXIT_FAILURE); }
  sleep(1);

  printf("Hello from the parent process! pid=%d\n", pid);

  printf("wait....\n");
  if (waitpid(pid, NULL, 0) == -1) perror("waitpid"); // wait for the child

  free(stack);
  printf("parent process exit.\n");
  return 0;
}
