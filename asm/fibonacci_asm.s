; fibonacci_asm.s
    .text
    .globl    fibonacci_asm
fibonacci_asm:
    pushq    %rbp
    movq     %rsp, %rbp
    pushq    %rbx
    subq    $16, %rsp
    movq    %rdi, -16(%rbp)
    cmpq    $0, -16(%rbp)
    jne    .L2
    movl    $0, %eax
    jmp    .L3
.L2:
    cmpq    $1, -16(%rbp)
    jne    .L4
    movl    $1, %eax
    jmp    .L3
.L4:
    movq    -16(%rbp), %rax
    subq    $1, %rax
    movq    %rax, %rdi
    call    fibonacci_asm ;; recursive call
    movq    %rax, %rbx
    movq    -16(%rbp), %rax
    subq    $2, %rax
    movq    %rax, %rdi
    call    fibonacci_asm ;; recursive call
    addq    %rbx, %rax
.L3:
    addq    $16, %rsp
    popq    %rbx
    popq    %rbp
    ret
