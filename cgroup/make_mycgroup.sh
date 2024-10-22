#!/bin/sh

mkdir -p /sys/fs/cgroup/mycgroup
sleep 1
echo "50000 100000" >  /sys/fs/cgroup/mycgroup/cpu.max


