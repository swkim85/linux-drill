
value = 4
while value <= 1024*1024*1024:
    cmd = "hipcc -w saxpy2.hip  -o saxpy2  -DNSIZE=%d ; ./saxpy2" % (value);
    print(cmd)
    value *= 2
