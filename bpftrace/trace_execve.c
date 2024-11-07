
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("kprobe/sys_execve")
int trace_execve(struct pt_regs *ctx) {
    char comm[16];
    bpf_get_current_comm(&comm, sizeof(comm));
    bpf_printk("execve called by process: %s\n", comm);
    return 0;
}

char _license[] SEC("license") = "GPL";
