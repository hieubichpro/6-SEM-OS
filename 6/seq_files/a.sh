#!/bin/bash
make
sudo insmod single_file.ko
echo "aaa" > /proc/seqfiles/seqfile
echo "bbb" > /proc/seqfiles/seqfile
echo "ccc" > /proc/seqfiles/seqfile

cat /proc/seqfiles/seqfile

# sudo rmmod seq_file.ko

