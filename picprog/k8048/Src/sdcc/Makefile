#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#
# REQUIRES: gpasm sdcc
#
build:
	cd pic18f2550 && $(MAKE)

install:
	cd pic18f2550 && $(MAKE) install

uninstall:
	cd pic18f2550 && $(MAKE) uninstall

clean:
	rm -f *~
	cd pic18f2550 && $(MAKE) clean
