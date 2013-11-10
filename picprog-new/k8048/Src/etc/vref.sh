#!/bin/bash
kio WATCHDOG D
kio DIRECTION A 0xFF
VREF="0"
while test 1;
do
	kio VREF "${VREF}" 

	VREF=`expr ${VREF} + 1`

	if test "${VREF}" -ge "16"; then
		VREF="0"
	fi
done
exit 0
