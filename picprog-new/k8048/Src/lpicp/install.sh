#!/bin/sh

ID=`id -u`
if test $ID -ne "0"; then
        echo "Need to be root"
        exit 1
fi

mkdir -p /lib/modules/3.6.11+/kernel/local/

cp ~/mc-icsp*ko /lib/modules/3.6.11+/kernel/local/

chown -R 0:0 /lib/modules/3.6.11+/kernel/local/

depmod -a

sync

exit 0
