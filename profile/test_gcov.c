// test_gcov.c
#include <stdio.h>
void func1() {
    printf("Hello World \n");
}
void func2(int delay) {
    printf("Delay: %d", delay);
    while (delay--);
}
int main() {
    for (int i = 0; i < 10; ++i) {
        func1();
    }
    for (int i = 0; i < 100; ++i) {
        func2(i);
    }
    return 0;
}
