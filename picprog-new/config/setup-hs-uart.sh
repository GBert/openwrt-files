#!/bin/sh

echo "set GPIO3 as input"
io -4 -o 0x18040000 0x00000008
echo "set GPIO0 as output"
io -4 -a 0x18040000 0xfffffffe
echo "set GPIO 2 on"
io -4 -o 0x18040008 0x00000004

echo "set GPIO3 as UART1_RD"
io -4 0x18040068 0x00030000
echo "set GPIO0 as UART1_TD"
io -4 -o 0x1804002c 0x0000004f
echo "set GPIO0 Output"
io -4 -a 0x18040000 0xfffffffe
io -4 0x18040000
echo "set GPIO0 as UART1_TD"
io -4 -o 0x1804002c 0x0000004f

