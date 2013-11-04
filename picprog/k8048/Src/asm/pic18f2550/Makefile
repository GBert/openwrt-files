#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

AS=gpasm -a inhx32 -e on -r dec -w 1 -I ../include/

OBJECT=triangle.hex

build:$(OBJECT)
	cd gEDA && $(MAKE)

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:build
	cd gEDA && $(MAKE) install

uninstall:clean
	cd gEDA && $(MAKE) uninstall

clean:
	rm -f *.lst *.hex *.cod *~
	cd gEDA && $(MAKE) clean
