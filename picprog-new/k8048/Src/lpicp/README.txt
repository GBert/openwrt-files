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
	Removed udly_pgd_val_to_clk_rise since no such value
	exists in the datasheets from mc-iscp.
	Fixed module unload/reload issue with mc-icsp-custom.
	Added 12/14-bit word microcontroller and bit I/O support
	to mc-icsp module.
	Removed from mc-icsp mclr/pgm control on file open.
	Refactored mc-icsp and restored 16-bit `data only'
	IOCTL as per the original.
	Changed input timing in mc-icsp to make it more reliable
	with P5B=0, however, on the R-PI P5B must be >0 and
	the delay overhead is long.
