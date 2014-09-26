Origin:
	https://github.com/chipKIT32/PIC32-avrdude-bootloader

Build date:
	August 27, 2014

Usage:
	PRESS BOOTLOAD-EN
	PRESS RESET
	RELEASE RESET
	RELEASE BOOTLOAD-EN
	avrdude -v -v -P /dev/ttyAMA0 -b 115200 -p pic32-250-128 -c stk500v2 -U flash:w:chipKIT_Pi_SerialTest.hex
