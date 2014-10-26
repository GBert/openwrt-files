Origin:
	https://github.com/chipKIT32/PIC32-avrdude-bootloader

Changes:
	Added Makefile, CHIPKIT_PI.mk, CHIPKIT_PI_MX270.mk
	Added _BOARD_CHIPKIT_PI_MX270_ section and __32MX270F256B__ CPU to BoardConfig.
	Edited section sizes in linker script, reducing reset and increasing program.

TODO:
	50Mhz CPU support.
