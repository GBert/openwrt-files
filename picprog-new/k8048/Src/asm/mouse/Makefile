#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

AS=gpasm -a inhx32 -e on -r dec -I ../include/

OBJECT=pic16f688.hex pic16f628a.hex

HEADER=../include/device.inc ../include/const.inc ../include/delay.inc ../include/icspio.inc ../include/macro.inc ../include/shadow.inc ../include/ps2.inc

build:$(OBJECT)
	cd gEDA && $(MAKE)

$(OBJECT):$(HEADER)

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build
	cd gEDA && $(MAKE) install

uninstall:clean
	cd gEDA && $(MAKE) uninstall

clean:
	rm -f *.lst *.hex *.hxh *.hxl *.cod *~
	cd gEDA && $(MAKE) clean
