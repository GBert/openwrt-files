#!/bin/sh
modprobe mc-icsp
#P2B:DS39592F P2B TsclkH 40..400ns (default:0ns) (int)
#P2A:DS39592F P2A TsclkL 40..400ns (default:0ns) (int)
#P5:DS39592F P5 Tdly1 20ns (default:0us) (int)
#P15:DS39592F P15 Tset3 2us (default:2us) (int)
#mc_icsp:mc_icsp config -> mclr,pgd,pgc,pgm (array of uint)
modprobe mc-icsp-gpio-custom mc_icsp=4,24,25,22 P2B=200 P2A=0
mknod /dev/icsp0 c 245 0
chmod 666 /dev/icsp0
exit 0
