#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

AS=gpasm -a inhx32 -e on -r hex -w 1 -I ../include/

OBJECT=a2d.hex bttn.hex i2c.hex intrp.hex pwm.hex table.hex tmrrw.hex usart.hex

build:$(OBJECT)

%.hex:%.asm
	@echo -n "[AS] "
	$(AS) $< -o $@

install:

uninstall:

clean:
	rm -f *.lst *.hex *.cod *~

