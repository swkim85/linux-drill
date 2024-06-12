
// cpuid.c
#include <stdio.h>
void cpuid(int code1, int code2, unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d) {
    __asm__ volatile( "cpuid"
                     : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                     : "a"(code1), "c"(code2)  );
}
int main() {
    unsigned int eax, ebx, ecx, edx;
    cpuid(0, 0, &eax, &ebx, &ecx, &edx); // EAX=0: 프로세서 제조사 문자열

    char vendor[13];
    ((unsigned int *)vendor)[0] = ebx;
    ((unsigned int *)vendor)[1] = edx;
    ((unsigned int *)vendor)[2] = ecx;
    vendor[12] = '\0';
    printf("Processor Vendor: %s\n", vendor);
    
    cpuid(1, 0, &eax, &ebx, &ecx, &edx); // EAX=1: 프로세서 버전 및 특징 정보
    printf("Processor Version: %08x\n", eax);
    printf("CPUID(EAX=01H):ECX: %08x  EDX: %08x\n", ecx, edx);

    cpuid(7, 0, &eax, &ebx, &ecx, &edx); // EAX=7, ECX=0: AVX-512 features
    printf("CPUID(EAX=07H):EBX: %08x\n", ebx);
    return 0;
}

