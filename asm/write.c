// write.c
#include <unistd.h>

int main() {
  const char *message = "Hello, world!\n";
  size_t length = 14;  // length of the message
  syscall(1, 1, message, length);
  syscall(60, 99); //return 99;
}
