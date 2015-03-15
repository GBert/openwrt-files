#!/bin/sh

echo "set GPIO3 as GPIO"
io -4 -a 0x18040068 0xff00ffff
echo "set GPIO0 as GPIO"
io -4 -a 0x1804002c 0xffffff00

modprobe gpio-bb
mknod /dev/gpio-bb c 180 0
chmod 666 /dev/gpio-bb
