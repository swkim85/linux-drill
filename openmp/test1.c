// test1.c
#include <stdio.h>

int main() {
#ifdef _OPENMP
    printf("OpenMP is enabled.\n");
#else
    printf("OpenMP is not enabled.\n");
#endif
    return 0;
}
