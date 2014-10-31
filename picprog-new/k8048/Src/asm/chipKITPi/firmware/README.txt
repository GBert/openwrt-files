Origin:
	https://github.com/chipKIT32/PIC32-avrdude-bootloader

Changes:
	Added Makefile, CHIPKIT_PI.mk, CHIPKIT_PI_MX270.mk & CHIPKIT_PI_MX270_48.mk

	Added __32MX270F256B__ CPU to BoardConfig.

	Added _BOARD_CHIPKIT_PI_MX270_ & _BOARD_CHIPKIT_PI_MX270_48_ to BoardConfig.

	Edited section sizes in linker script, reducing reset and increasing program.
