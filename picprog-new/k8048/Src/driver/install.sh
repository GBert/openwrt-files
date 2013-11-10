#!/bin/sh

ID=`id -u`
if test $ID -ne "0"; then
        echo "Need to be root"
        exit 1
fi

rmmod bit-bang-gpio 1>/dev/null 2>/dev/null
rm -f /dev/bit-bang-gpio

mkdir -p /lib/modules/3.6.11+/kernel/local/
cp ~/bit-bang-gpio.ko /lib/modules/3.6.11+/kernel/local/
chown -R 0:0 /lib/modules/3.6.11+/kernel/local/
depmod -a
sync

modprobe bit-bang-gpio

exit 0
