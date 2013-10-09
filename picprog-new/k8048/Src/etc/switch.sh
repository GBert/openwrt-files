#!/bin/bash
#
# SETUP: K8048
#
LED="0"
LASTSW="0xFF"
while test 1;
do
	#GET SW1..SW4
	SWITCH=`kio SWITCH 2>/dev/null`

	if test "${LASTSW}" != "${SWITCH}"; then
		LASTSW="${SWITCH}"

		#LD1..LD5 = SWITCH
		kio LED "${SWITCH}"
	fi

	sleep 0.050
done
exit 0
