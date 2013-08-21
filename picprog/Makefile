#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)

CC=mipsel-openwrt-linux-gcc
CFLAGS=-std=c99 -I. -ggdb -Wall -Wno-implicit-function-declaration
GCCVERSIONGTEQ4:=$(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 4)
ifeq "$(GCCVERSIONGTEQ4)" "1"
CFLAGS+=-Wno-int-to-pointer-cast
endif

#DMALLOC
#CFLAGS=-std=c99 -Wall -Wno-implicit-function-declaration -Wno-int-to-pointer-cast -I. -ggdb -I/usr/local/include -DDMALLOC
#CLIB=-L/usr/local/lib -ldmalloc

BINDIR=/usr/local/bin

CLIB1=$(CLIB)
TARGET1=k8048
#SOURCE1=k8048.c inhx32.c pic12.c pic14.c pic16.c pic.c util.c
#HEADER1=k8048.h inhx32.h pic12.h pic14.h pic16.h pic.h util.h
#OBJECT1=k8048.o inhx32.o pic12.o pic14.o pic16.o pic.o util.o
SOURCE1=lpicp_icsp.c k8048.c inhx32.c io.c  pic16.c pic.c util.c
HEADER1=lpicp_icsp.h k8048.h inhx32.h io.h pic16.h pic.h util.h
OBJECT1=lpicp_icsp.o k8048.o inhx32.o io.o pic16.o pic.o util.o

#
# COMMAND LINE TOOL
#
# DEBIAN:
#  sudo apt-get install mercurial
#  hg clone http://hg.kewl.org/pub/k8048/
#  cd k8048
#  make build
#  sudo make install
#
build: $(TARGET1) $(TARGET2)

$(TARGET1):$(OBJECT1)
	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1)

$(TARGET2):$(OBJECT2)
	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2)

$(OBJECT1):$(HEADER1)

$(OBJECT2):$(HEADER2)

c.o:	$(CC) -c $< -o $@

install:build
	mkdir -p $(BINDIR)
	cp $(TARGET1) $(BINDIR)/$(TARGET1)
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k12
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k14
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k16
	cp $(TARGET2) $(BINDIR)/$(TARGET2)
ifeq (,$(findstring CYGWIN,$(SYSTEM)))
	chown 0:0 $(BINDIR)/$(TARGET1)
	chmod u+s $(BINDIR)/$(TARGET1)
	chown 0:0 $(BINDIR)/$(TARGET2)
	chmod u+s $(BINDIR)/$(TARGET2)
endif

uninstall:
	rm -f $(BINDIR)/$(TARGET1)
	rm -f $(BINDIR)/k12
	rm -f $(BINDIR)/k14
	rm -f $(BINDIR)/k16
	rm -f $(BINDIR)/$(TARGET2)

clean:
	rm -f $(TARGET1) $(TARGET2) *.o *~ .*~ etc/*~
#
# COMMAND LINE TOOL & ASSEMBLER DEMOS
#
# REQUIRES: gpasm gEDA gEDA-utils
#
# DEBIAN:
#  sudo apt-get install gputils
#  sudo apt-get install geda
#  sudo apt-get install geda-utils
#  sudo apt-get install libreadline-dev
#  sudo apt-get install libncurses-dev
#  make build-all
#  sudo make install-all
#
build-all:build build-asm

install-all:install install-asm

uninstall-all:uninstall uninstall-asm

clean-all:clean clean-asm
