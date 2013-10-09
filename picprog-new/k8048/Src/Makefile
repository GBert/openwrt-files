#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

#FreeBSD IBMPC FreeBSD 9.0-RELEASE   amd64
#Debian  IBMPC Linux   3.2.0-4-amd64 x86_64
#Debian  R-PI  Linux   3.2.27+       armv6l
SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)
ifeq "$(PLATFORM)" "armv6l"
RASPI=1
else
RASPI=0
endif

CC=gcc
CFLAGS=-std=c99 -DENABLE_RPI=$(RASPI) -I. -ggdb -Wall -Wno-implicit-function-declaration
GCCVERSIONGTEQ4:=$(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 4)
ifeq "$(GCCVERSIONGTEQ4)" "1"
CFLAGS+=-Wno-int-to-pointer-cast -Wno-char-subscripts
endif
ifneq (,$(findstring CYGWIN,$(SYSTEM)))
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
endif

#DMALLOC
#CFLAGS=-std=c99 -Wall -Wno-implicit-function-declaration -Wno-int-to-pointer-cast -I. -ggdb -I/usr/local/include -DDMALLOC
#CLIB=-L/usr/local/lib -ldmalloc

BINDIR=/usr/local/bin

CLIB1=$(CLIB)
TARGET1=k8048
SOURCE1=k8048.c dotconf.c inhx32.c io.c pic12.c pic14.c pic16.c pic.c serial_posix.c util.c raspi.c mcp23017.c lpicp/lpicp_icsp.c
OBJECT1=k8048.o dotconf.o inhx32.o io.o pic12.o pic14.o pic16.o pic.o serial_posix.o util.o raspi.o mcp23017.o lpicp/lpicp_icsp.o
HEADER1=k8048.h dotconf.h inhx32.h io.h pic12.h pic14.h pic16.h pic.h serial_posix.h util.h raspi.h mcp23017.h lpicp/lpicp_icsp.h
HEADER1+=VERSION

CLIB2=$(CLIB)
TARGET2=kio
SOURCE2=kio.c   dotconf.c io.c serial_posix.c util.c raspi.c mcp23017.c lpicp/lpicp_icsp.c
OBJECT2=kio.o   dotconf.o io.o serial_posix.o util.o raspi.o mcp23017.o lpicp/lpicp_icsp.o
HEADER2=k8048.h dotconf.h io.h serial_posix.h util.h raspi.h mcp23017.h lpicp/lpicp_icsp.h
HEADER2+=VERSION

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
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/ktest
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
	rm -f $(BINDIR)/ktest
	rm -f $(BINDIR)/$(TARGET2)

clean:
	cd etc   && $(MAKE) clean
	cd lpicp && $(MAKE) clean
	rm -f $(TARGET1) $(TARGET2) *.o *~ .*~
#
# ASSEMBLER DEMOS
#
# REQUIRES: gpasm gEDA gEDA-utils
#
# DEBIAN:
#  sudo apt-get install gputils
#  sudo apt-get install geda
#  sudo apt-get install geda-utils
#  sudo apt-get install libreadline-dev
#  sudo apt-get install libncurses-dev
#  make build-asm
#
build-asm:
	cd asm && $(MAKE)

install-asm:build-asm
	cd asm && $(MAKE) install

uninstall-asm:
	cd asm && $(MAKE) uninstall

clean-asm:
	cd asm && $(MAKE) clean
#
# SDCC DEMOS
#
# REQUIRES: gpasm sdcc
#
# DEBIAN:
#  view README.tools
#  make build-sdcc
#
build-sdcc:
	cd sdcc && $(MAKE)

install-sdcc:build-asm
	cd sdcc && $(MAKE) install

uninstall-sdcc:
	cd sdcc && $(MAKE) uninstall

clean-sdcc:
	cd sdcc && $(MAKE) clean
#
# COMMAND LINE TOOL, ASSEMBLER AND SDCC DEMOS
#
# REQUIRES: gpasm sdcc gEDA gEDA-utils
#
# DEBIAN:
#  view README.tools
#  sudo apt-get install geda
#  sudo apt-get install geda-utils
#  sudo apt-get install libreadline-dev
#  sudo apt-get install libncurses-dev
#  make build-all
#  sudo make install-all
#
build-all:build build-asm build-sdcc

install-all:install install-asm install-sdcc

uninstall-all:uninstall uninstall-asm uninstall-sdcc

clean-all:clean clean-asm clean-sdcc
