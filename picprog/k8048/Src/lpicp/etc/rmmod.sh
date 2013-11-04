#!/bin/sh

ID=`id -u`
if test $ID -ne "0"; then
        echo "Need to be root"
        exit 1
fi

rmmod mc_icsp_gpio_custom

rmmod mc_icsp

rm -f /dev/icsp0

exit 0
