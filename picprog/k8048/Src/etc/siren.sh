#!/bin/bash
#
# PLAY ANNOYING ALARM SOUND
#
while test 1;
do
	N=71
	while test $N != "87";
	do
		P=$N
		i2cset -y 1 0x40 0x00 $P

		P=`expr 12 + $N`
		i2cset -y 1 0x40 0x01 $P

	        P=`expr 157 - $N`
		i2cset -y 1 0x40 0x02 $P

	        P=`expr 169 - $N`
		i2cset -y 1 0x40 0x03 $P

	        N=`expr $N + 1`
	done
done
exit 0
