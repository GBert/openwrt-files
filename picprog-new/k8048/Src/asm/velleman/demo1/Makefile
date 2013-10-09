#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

AS=gpasm -a inhx32 -e on -w1

OBJECT=pic16f676.hex

build:$(OBJECT)

%.hex:%.asm
	$(AS) $< -o $@

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *~
