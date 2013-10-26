#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#
build:
	cd blink      && $(MAKE)
	cd example1.1 && $(MAKE)
	
install:
	cd blink      && $(MAKE) install
	cd example1.1 && $(MAKE) install
	
uninstall:
	cd blink      && $(MAKE) uninstall
	cd example1.1 && $(MAKE) uninstall
	
clean:
	cd blink      && $(MAKE) clean
	cd example1.1 && $(MAKE) clean
	rm -f *.o *~ .*~
