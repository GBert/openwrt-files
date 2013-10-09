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
	cd RKP28sb-USB && $(MAKE)

install:
	cd RKP28sb-USB && $(MAKE) install

uninstall:
	cd RKP28sb-USB && $(MAKE) uninstall

clean:
	rm -f *~
	cd RKP28sb-USB && $(MAKE) clean
