#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

AS=gpasm -a inhx32 -e on -r hex -w 1 -I ../include/

OBJECT=demo1.hex demo2.hex

build:$(OBJECT)
	cd demo1 && $(MAKE)

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build
	cd demo1 && $(MAKE) install

uninstall:clean
	cd demo1 && $(MAKE) uninstall

clean:
	cd demo1 && $(MAKE) clean
	rm -f *.lst *.hex *.cod *~
