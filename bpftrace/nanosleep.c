#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <bpf/bpf_helpers.h>

SEC("kprobe/do_nanosleep");
int bpf_prog(struct pt_regs *ctx) {
    // 프로그램 로직
    return 0;
}

char _license[] SEC("license") = "GPL";

