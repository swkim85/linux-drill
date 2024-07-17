// maxofsix.s
.global  maxofsix
.text
maxofsix:
  mov   %rdi, %rax  # rdi  1st argument
  cmp   %rsi, %rax  # rsi  2nd argument
  cmovl %rsi, %rax
  cmp   %rdx, %rax  # rdx  3rd argument
  cmovl %rdx, %rax
  cmp   %rcx, %rax  # rcx  4th argument
  cmovl %rcx, %rax
  cmp   %r8, %rax  # r8  5th argument
  cmovl %r8, %rax
  cmp   %r9, %rax  # r9  6th argument
  cmovl %r9, %rax
  ret
