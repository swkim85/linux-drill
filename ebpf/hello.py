#!/usr/bin/python3

from bcc import BPF

prog = """
int kprobe__sys_openat(void *ctx) {
  bpf_trace_printk("Hello, World!\\n");
  return 0;
}
"""

b = BPF(text=prog, debug=True)
b.trace_print()

