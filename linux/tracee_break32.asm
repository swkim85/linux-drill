; tracee_break32.asm
section    .data
    msg1    db      'Hello,', 0xa
    len1    equ     $ - msg1
    msg2    db      'world!', 0xa
    len2    equ     $ - msg2
section    .text
    global _start
_start:
    mov     ecx, msg1
    mov     edx, len1
    mov     ebx, 1
    mov     eax, 4
    int     0x80  ; sys_write

    mov     ecx, msg2
    mov     edx, len2
    mov     ebx, 1
    mov     eax, 4
    int     0x80  ; sys_write

    mov   eax,1
    int     0x80
