// breakpoint.c
int main() {
    int i;     
    for(i=0; i<3;i++) {
        __asm__("int3");
    }
}
