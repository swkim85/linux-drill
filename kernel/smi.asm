
section .text
global _start

_start:
    ; 0xB2 포트에 값을 쓰기 - 이는 SMI를 트리거합니다.
    mov al, 0x01   ; 트리거 값을 레지스터 AL에 로드 (값은 예시)
    out 0xB2, al   ; 0xB2 포트에 AL 값을 출력하여 SMI 트리거

    ; 종료 루틴
    mov eax, 1     ; 시스템 호출 번호 (exit)
    xor ebx, ebx   ; 종료 코드 0
    int 0x80       ; 시스템 호출

