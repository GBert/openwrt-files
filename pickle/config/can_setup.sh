#!/bin/sh

rmmod gpio-bb
echo "GPIO input/output register"
io -4 0x18040000

echo "set GPIO3 as input"
io -4 -o 0x18040000 0x00000008
echo "set GPIO0 as output"
io -4 -a 0x18040000 0xfffffffe

echo "set GPIO3 as UART1_RD"
io -4 0x18040068 0x00030000
echo "set GPIO0 as UART1_TD"
io -4 -o 0x1804002c 0x0000004f
echo "set GPIO0 Output"
io -4 -a 0x18040000 0xfffffffe
io -4 0x18040000
echo "set GPIO0 as UART1_TD"
io -4 -o 0x1804002c 0x0000004f

echo "GPIO 2 on"
io -4 -o 0x18040008 0x00000004

# slcand -S 500000 ttyATH0 can0
# ifconfig can0 up

# echo "GPIO 2 on / MCLR high - restart PIC"
# io -4 -a 0x18040008 0xfffffffb ; io -4 -o 0x18040008 0x00000004

# echo "UART_CLOCK"
# io -4 0x18500008

# modprobe ar934x_hs_uart
# echo "UART_CLOCK"
# io -4 0x18500008

# io -4 -o 0x1804000c 0x00000004
# stty -F /dev/ttyATH0 921600 raw clocal cread cs8 -cstopb -parenb
# /usr/bin/stty -F /dev/ttyATH0 460800 raw clocal cread cs8 -cstopb -parenb
# echo "UART_CLOCK"
# io -4 0x18500008
# show-gpio | head -4

# modprobe can
# modprobe can-raw
# insmod slcan
modprobe /lib/modules/3.*/ar934x_hs_uart.ko
slcand -c -o -S500000 /dev/ttyATH0 can0
sleep 1
ifconfig can0 up
ip -s -d link show can0
