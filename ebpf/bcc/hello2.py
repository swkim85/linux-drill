#!/usr/bin/python3

from bcc import BPF

prog = """
int hello(void *ctx) {
  bpf_trace_printk("Hello, World!\\n");
  return 0;
}
"""

b = BPF(text=prog, debug=True)
b.attach_kprobe(event=b.get_syscall_fnname("openat"), fn_name="hello")
print("PID MESSAGE")
try:
  b.trace_print(fmt="{0}/{1}/{2}/{3}/{4}/{5}")
except KeyboardInterrupt:
  exit()

