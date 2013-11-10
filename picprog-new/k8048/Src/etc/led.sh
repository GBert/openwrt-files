#!/bin/bash
#
# SETUP: K8048
#
LED="0"
while test 1;
do
	#LD1..LD5
	kio LED "${LED}" 

	#LED = LED + 1
	LED=`expr ${LED} + 1`

	if test "${LED}" -ge "16"; then
		LED="0"
	fi

	#sleep 0.1
done
exit 0
