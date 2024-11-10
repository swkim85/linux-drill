// bpf_sys_openat.c
#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <bpf/bpf_helpers.h>
//#include <linux/uaccess.h>

SEC("kprobe/sys_openat")
int kprobe_sys_openat(struct pt_regs *ctx) {
  //char filename[256];
  //bpf_probe_read_user_str(filename, sizeof(filename), (void *)PT_REGS_PARM2(ctx));
  //bpf_probe_read_user_str(filename, sizeof(filename), (void *)ctx->di);

  static const char fmt[] = "Hello, World!\\n";
  bpf_trace_printk(fmt, sizeof(fmt));
  return 0;
}

char _license[] SEC("license") = "GPL";

