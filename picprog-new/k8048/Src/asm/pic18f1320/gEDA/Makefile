#
# Velleman K8048 Makefile
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

TS=tragesym

OBJECT=pic18f1320.sym tlc5620.sym ds1267.sym

SYMDIR=/usr/share/gEDA/sym/local

build:$(OBJECT)

%.sym: %.src
	$(TS) $< $@

install:build
	mkdir -p $(SYMDIR)
	cp ds1267.sym     $(SYMDIR)
	cp pic18f1320.sym $(SYMDIR)
	cp tlc5620.sym    $(SYMDIR)

uninstall:clean
	rm -f $(SYMDIR)/ds1267.sym
	rm -f $(SYMDIR)/pic18f1320.sym
	rm -f $(SYMDIR)/tlc5620.sym

clean:
	rm -f *.sym *.log *~
