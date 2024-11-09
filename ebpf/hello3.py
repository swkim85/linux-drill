#!/usr/bin/python3
from bcc import BPF
prog = """
int hello(void *ctx) {
  bpf_trace_printk("Hello, world!\\n");
  return 0;
}
"""
b = BPF(text=prog, debug=False)
b.attach_kprobe(event=b.get_syscall_fnname("openat"), fn_name="hello")
print("CTRL-C to stop")
try:
  while True: pass
except KeyboardInterrupt:
  exit()

