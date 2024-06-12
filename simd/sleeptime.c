// sleeptime.c 
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
// RDTSC를 사용하여 타임스탬프를 읽는 함수
static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));  // EDX(hi):EAX(lo)
    return ((uint64_t)hi << 32) | lo;
}
int main(int argc, char **argv) {
    uint64_t start, end;
    long long unsigned int elapsed;
    start = rdtsc();
    sleep(1); // sleep 1
    end = rdtsc();
    elapsed = end - start;
    printf("sleep Elapsed Time (cycles): %llu\n", elapsed);
}
