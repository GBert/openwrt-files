#!/bin/sh

rmmod gpio_bb
insmod gpio-bb
sleep 1
./pic-flash.sh 
k16 lvp p /tmp/pic18f26k80-wr841.hex
./ser_setup.sh 

