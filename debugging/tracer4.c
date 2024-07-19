// tracer4.c
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <string.h>

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

void run_debugger(pid_t child_pid) {
  int wait_status;
  struct user_regs_struct regs;
  procmsg("debugger started\n");
  wait(&wait_status);  // child가 첫번째 명령에서 stop될때까지 대기
  
#if __x86_64__

  // 중단점을 설정할 주소에서 데이터를 읽음
  long unsigned addr = TARGETADDRESS;  
  long unsigned data = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
  procmsg("peek : [0x%08x] ==> 0x%08x\n", addr,  data);

  // 트랩 명령(int 3)을 해당 주소에 쓰기
  long unsigned data_with_trap = (data & 0xffffffffffffff00) | 0xcc;
  procmsg("poke : [0x%08x] <== 0x%08x\n", addr,  data_with_trap);
  ptrace(PTRACE_POKETEXT, child_pid, addr, data_with_trap);
  data_with_trap = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
  procmsg("peek : [0x%08x] ==> 0x%08x\n", addr,  data_with_trap);

#else

  // 중단점을 설정할 주소에서 데이터를 읽음
  unsigned addr = TARGETADDRESS;  
  unsigned data = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
  procmsg("peek : [0x%08x] ==> 0x%08x\n", addr,  data);

  // 트랩 명령(int 3)을 해당 주소에 쓰기
  unsigned data_with_trap = (data & 0xffffff00) | 0xcc;
  procmsg("poke : [0x%08x] <== 0x%08x\n", addr,  data_with_trap);
  ptrace(PTRACE_POKETEXT, child_pid, addr, data_with_trap);
  data_with_trap = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
  procmsg("peek : [0x%08x] ==> 0x%08x\n", addr,  data_with_trap);

#endif

  // child를 재시작한다. 
  ptrace(PTRACE_CONT, child_pid, 0, 0); // continue

  wait(&wait_status); // 위 주소 번에서 트랩이 발생될때까지 대기
  if (WIFSTOPPED(wait_status)) {
    procmsg("Child got a signal: %s\n", strsignal(WSTOPSIG(wait_status)));
  } else {
    perror("wait"); return;
  }

  // see where the child is now 
#if __x86_64__
  ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
  procmsg("Child stopped at rip = 0x%08x\n", regs.rip);
#else
  ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
  procmsg("Child stopped at eip = 0x%08x\n", regs.eip);
#endif

  // remove the breakpoint by restoring the previous data
  procmsg("poke : [0x%08x] <== 0x%08x\n", addr,  data);
  ptrace(PTRACE_POKETEXT, child_pid, addr, data);

#if __x86_64__
  regs.rip = addr;
  procmsg("continue at rip <= 0x%08x\n", regs.rip);
  ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
#else
  regs.eip = addr;
  procmsg("continue at eip <= 0x%08x\n", regs.eip);
  ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
#endif

  ptrace(PTRACE_CONT, child_pid, 0, 0);

  procmsg("parent wait.. \n");
  pid_t waitPid;
  waitPid = wait(&wait_status);

  procmsg("parent terminate.\n");
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
