
#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

  int result = unshare(CLONE_NEWPID);
  if (result == -1) { perror("unshare failed"); return 1; }
    
  pid_t pid = fork();
  if (pid == 0) { // child
    printf("child pid = %d\n", getpid());
    printf("child sleep \n");
    sleep(10);

  } else if (pid > 0) { // parent
    printf("pid = %d, Parent getpid() : %d\n", pid, getpid());
    wait(NULL);
    printf("exit program\n");
  }
    
  return 0;
}
