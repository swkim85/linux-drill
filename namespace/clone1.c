
#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Start function for cloned child 
static int childFunc(void *arg) {
  struct utsname uts;

  printf("child pid = %d\n", getpid());
  // Change hostname in UTS namespace of child 
  if (sethostname(arg, strlen(arg)) == -1) errExit("sethostname");

  // Retrieve and display hostname 
  if (uname(&uts) == -1) errExit("uname");
  printf("uts.nodename in child:  %s\n", uts.nodename);

  // Keep the namespace open for a while, by sleeping.
  //   This allows some experimentation--for example, another
  //   process might join the namespace. 
  sleep(200);
  return 0; 
}

#define STACK_SIZE (1024 * 1024)    // Stack size for cloned child 

int main(int argc, char *argv[]) {
  char *stack;
  char *stackTop;
  pid_t pid;
  struct utsname uts;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  // Allocate stack for child 
  stack = malloc(STACK_SIZE);
  if (stack == NULL) errExit("malloc");
  stackTop = stack + STACK_SIZE;  // Assume stack grows downward 

  // Create child that has its own UTS namespace;
  //    child commences execution in childFunc() 
  pid = clone(childFunc, stackTop, CLONE_NEWUTS | SIGCHLD, argv[1]);
  if (pid == -1) errExit("clone");
  printf("clone() returned %ld\n", (long) pid);

  printf("parent pid = %d\n", getpid());
  // Parent falls through to here 
  sleep(1);    // Give child time to change its hostname 

  // Display hostname in parent's UTS namespace. This will be
  //   different from hostname in child's UTS namespace. 
  if (uname(&uts) == -1) errExit("uname");
  printf("uts.nodename in parent: %s\n", uts.nodename);

  if (waitpid(pid, NULL, 0) == -1) errExit("waitpid");
  printf("child has terminated\n");

  exit(EXIT_SUCCESS);
}

