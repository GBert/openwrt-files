#!/bin/sh

ID=`id -u`
if test $ID -ne "0"; then
        echo "Need to be root"
        exit 1
fi

modprobe mc-icsp debug=0

#P2B:DS39592F P2B TsclkH 40..400ns (default:0ns) (int)
#P2A:DS39592F P2A TsclkL 40..400ns (default:0ns) (int)
#P5:DS39592F P5 Tdly1 20ns (default:0us) (int)
#mc_icsp:mc_icsp config -> mclr,pgd,pgc,pgm (array of uint)

# ndelay may not exist on your machine architecture
# and P2A/B will be rounded up the microsecond in
# that instance.

modprobe mc-icsp-gpio-custom mc_icsp=4,24,25,22 P2B=400 P2A=0

if test ! -c "/dev/icsp0"; then
	mknod /dev/icsp0 c 245 0
	chmod 666 /dev/icsp0
fi

exit 0
