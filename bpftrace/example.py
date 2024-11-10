from bcc import BPF

# BPF 프로그램 정의
bpf_program = """
#include <uapi/linux/ptrace.h>

BPF_HASH(reads, u64);

int kprobe__vfs_read(struct pt_regs *ctx) {
    u64 key = 0;
    reads.increment(key);
    return 0;
}
"""

# BPF 객체 생성 및 프로그램 로드
b = BPF(text=bpf_program)

# 데이터 수집 및 출력
try:
    while True:
        b.perf_buffer_poll()
except KeyboardInterrupt:
    print("Reads:", b["reads"][0].value)
