#!/bin/bash
make
sudo insmod fortune.ko
echo "aaa" > /proc/fortuneDir/fortuneFile
echo "bbb" > /proc/fortuneDir/fortuneFile
echo "ccc" > /proc/fortuneDir/fortuneFile

cat /proc/fortuneDir/fortuneFile
cat /proc/fortuneDir/fortuneFile
cat /proc/fortuneDir/fortuneFile

sudo rmmod fortune.ko

