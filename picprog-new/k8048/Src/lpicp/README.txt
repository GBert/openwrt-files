Origin:
	https://github.com/GBert/openwrt-files

Added:
        gpl-2.0.txt

Changes:
        Removed some files.
	Added kernel makefile.
	Added msleep command config argument as an optional
	replacement to busy waiting.
	Adjusted timing which was too fast for reliable
	operation on the R-PI.
	Removed udly_pgd_val_to_clk_rise since no such value
	exists in the datasheets.
	Fixed module unload/reload issue with mc-icsp-custom.
