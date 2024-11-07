
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

static int child_fn() {
    printf("자식 프로세스 PID: %d\n", getpid());
    printf("네트워크 네임스페이스 분리 후\n");
    system("ip link");
    system("ip addr");
    return 0;
}

int main() {
    printf("부모 프로세스 PID: %d\n", getpid());
    printf("네트워크 네임스페이스 분리 전\n");
    system("ip link");
    system("ip addr");

    pid_t child_pid = clone(child_fn, child_stack + STACK_SIZE, 
                            CLONE_NEWNET | SIGCHLD, NULL);

    if (child_pid == -1) {
        perror("clone");
        exit(1);
    }

    waitpid(child_pid, NULL, 0);
    return 0;
}
