#!/bin/bash
#
# SETUP: 16F886 FIRMWARE ON XINO
#
# CONNECT LD1..LD5     ON PORT A RA0..RA5
# CONNECT BI-COLOR LED ON PORT C RC0 & RC1
#
DELAY="0.05"
kio WATCHDOG DISABLE
#PORT C DATA DIRECTION
kio DIRECTION C 0xFC
while test 1;
do
	#LD1..LD5
	kio LED `expr $RANDOM % 64`

	#GREEN
	kio OUTPUT C 1
	sleep $DELAY

	#LD1..LD5
	kio LED `expr $RANDOM % 64`

	#RED
	kio OUTPUT C 2
	sleep $DELAY

	#LD1..LD5
	kio LED `expr $RANDOM % 64`

	#GREEN + RED
	kio OUTPUT C 3
	sleep $DELAY
done
exit 0
