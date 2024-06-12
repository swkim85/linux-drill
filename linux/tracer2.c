// tracer2.c
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>

// process ID와 함께 메시지를 출력
void procmsg(const char* format, ...) {
    va_list ap;
    fprintf(stdout, "[%d] ", getpid());
    va_start(ap, format);
    vfprintf(stdout, format, ap);
    va_end(ap);
}
// child(=target = tracee) 프로세스
void run_target(const char* programname) {
    procmsg("target started. will run '%s'\n", programname);
    // 이 프로세스가 parent 에 의해 추적될 것을 OS에게 알림
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");  return;
    }
    // 프로세스 이미지를 주어진 프로그램으로 대체하여 실행
    execl(programname, programname, NULL);
}

void run_debugger(pid_t child_pid)
{
    int wait_status;
    unsigned icounter = 0;
    procmsg("debugger started\n");

    wait(&wait_status);   // child가 첫번째 명령에서 중지될때까지 대기

    while (WIFSTOPPED(wait_status)) {
      icounter++;

      struct user_regs_struct regs;
      ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
#if __x86_64__
        unsigned instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rip, 0);
        procmsg("icounter = %u.  rip = 0x%08x.  instr = 0x%08x\n",
          icounter, regs.rip, instr);
# else
        unsigned instr = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip, 0);
        procmsg("icounter = %u.  eip = 0x%08x.  instr = 0x%08x\n",
          icounter, regs.eip, instr);
#endif
      // child가 하나의 명령을 실행한 후 멈추도록 한다.
      if (ptrace(PTRACE_SINGLESTEP, child_pid, 0, 0) < 0) {
          perror("ptrace");
          return;
      }
      // 다음 명령에서 멈출때까지 기다림
      wait(&wait_status);
    }
    procmsg("the child executed %u instructions\n", icounter);
}

int main(int argc, char** argv) {
    pid_t  child_pid;
    if (argc < 2) {
        fprintf(stderr, "Expected a program name as argument\n"); return -1;
    }
    child_pid = fork();
    if (child_pid == 0) // child process
        run_target(argv[1]);
    else if (child_pid > 0) // parent process
        run_debugger(child_pid);
    else {
        perror("fork"); return -1;
    }
    return 0;
}
