#
# Velleman K8048 Makefile
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

TS=tragesym

OBJECT=pic18f25k22.sym

SYMDIR=/usr/share/gEDA/sym/local

build:$(OBJECT)

%.sym:%.src
	@echo -n "[TRAGESYM] "
	$(TS) $< $@

install:build
	mkdir -p $(SYMDIR)
	cp pic18f25k22.sym $(SYMDIR)

uninstall:clean
	rm -f $(SYMDIR)/pic18f25k22.sym

clean:
	rm -f *.sym *.log *~
