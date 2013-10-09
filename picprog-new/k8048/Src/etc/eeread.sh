#!/bin/bash
Y=0
while test $Y != "16";
do
	ADDRESS=`expr 16 \* $Y`
	printf "[%02X] " $ADDRESS
	X=0
	while test $X != "16";
	do
		ADDRESS=`expr 16 \* $Y + $X`
		DATA=`kio EEREAD $ADDRESS 2>/dev/null | cut -b 3,4`
		echo -n "$DATA "
		X=`expr $X + 1`
	done
	echo
	Y=`expr $Y + 1`
done
exit 0
