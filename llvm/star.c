#include <stdio.h>
void star(void) {
  putchar('*');
}
double printd(double X) {
  fprintf(stderr, "%f\n", X);
  return 0;
}

double putchard(double X) {
  fputc((char)X, stderr);
  return 0;
}
