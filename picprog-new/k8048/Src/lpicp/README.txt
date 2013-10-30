Origin:
	https://github.com/GBert/openwrt-files
	https://github.com/pavius/lpicp
	http://www.pavius.net/2011/06/lpicp-the-embedded-linux-pic-programmer/

Added:
        gpl-2.0.txt

Changes:
        Removed some files.

	Added kernel makefile.

	Added msleep command config argument as an optional
	replacement to busy waiting in mc-icsp.

	Added mc-icsp-custom kernel module paramaters for clock
	timing.

	Removed udly_pgd_val_to_clk_rise from mc-icsp since
	no such value exists in the 12/14/16-bit word datasheets.
	Nb. This value does appear in the PIC24 data sheet which
	latches data input on clock high rather than clock low like
	it's predecessors.

	Fixed module unload/reload issue with mc-icsp-custom.
	Added 12/14-bit word microcontroller and bit I/O support
	to mc-icsp module.

	Removed from mc-icsp mclr/pgm control on file open.

	Refactored mc-icsp and restored 16-bit `data only'
	IOCTL as per the original.

	Changed input timing in mc-icsp to make it more reliable
	with P5B=0, however, on the R-PI P5B must be >0 and
	the delay is long since there is no ndelay on this
	platform AFAICT.

	Created a best fix for a PGD I/O contention issue
	where PGD was set to O/P when owned by the PICMicro
	and not set to O/P until the next clock high transition.
	This is a best fix because some PIC18K devices fail
	to operate correctly if we set data direction after
	raising the clock signal.

	Changed data direction to model that of linux gpio
	where setting a pin to output also sets the output
	level.

	Added an option to set the voltage level on pgd prior
	to changing the data direction register to input. This
	may assist an external pull-up resistor.
