
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/utsname.h>

int main() {
  struct utsname buffer;

  if (uname(&buffer) != 0) { perror("uname"); return 1; }

  printf("System Name: %s\n", buffer.sysname);
  printf("Node Name: %s\n", buffer.nodename);
  printf("Release: %s\n", buffer.release);
  printf("Version: %s\n", buffer.version);
  printf("Machine: %s\n", buffer.machine);

  #ifdef _GNU_SOURCE
  printf("Domain Name: %s\n", buffer.domainname);
  #endif

  return 0;
}
