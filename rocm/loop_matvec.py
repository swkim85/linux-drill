#!/usr/bin/env python3

rows = cols = 1024
rows = cols = 32
rows = cols = 64

value = 1
while value <= 128:
  blocksize = value
  print("echo  blocksize=%d" % blocksize)
  cmd = "hipcc -w matvec.hip  -o matvec  -DROWS=%s -DCOLS=%s -DBLOCKSIZE=%s ; ./matvec" % (rows, cols, blocksize);
  print(cmd)
  value *= 2
  #value += 1
