#!/bin/bash
#
# TMP36 on AN8
# 
#      +-----+
#      |LM35 | 1 +V
#      |DZ   | 2 VOUT
#      ++-+-++ 3 GND
#       | | |
#       1 2 3
#       | | |
# 5V----+ +----10K--+--o AN8
#       |   |       |
# 100n ===  | 100n ===
#       |   |       |
# GND---+---+-------+--o GND
#
kio WATCHDOG DISABLE 2>/dev/null
kio ANALOG 8         2>/dev/null
while test 1;
do
	TMP1=`kio SAMPLE 2>/dev/null`
	TMP2=`printf "%d\n" ${TMP1}`
	TMP3=`expr \( ${TMP2} - 100 \) / 2`
	echo "TMP36 = ${TMP3}'C"
	sleep 1
done
exit 0
