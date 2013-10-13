#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

# build targets and backend configuration
include config.mk

#FreeBSD IBMPC: FreeBSD 9.0-RELEASE   amd64
#Debian  IBMPC: Linux   3.2.0-4-amd64 x86_64
#Debian  R-PI:  Linux   3.2.27+       armv6l
SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)

CC=gcc
CFLAGS=-Os -std=gnu99 -I. -Wall
ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
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
SOURCE1=k8048.c dotconf.c inhx32.c io.c pic.c util.c
OBJECT1=k8048.o dotconf.o inhx32.o io.o pic.o util.o
HEADER1=k8048.h dotconf.h inhx32.h io.h pic.h util.h
HEADER1+=VERSION Makefile config.mk

ifeq "$(K12)" "1"
CFLAGS+=-DK12
SOURCE1+=pic12.c
OBJECT1+=pic12.o
HEADER1+=pic12.h
endif
ifeq "$(K14)" "1"
CFLAGS+=-DK14
SOURCE1+=pic14.c
OBJECT1+=pic14.o
HEADER1+=pic14.h
endif
ifeq "$(K16)" "1"
CFLAGS+=-DK16
SOURCE1+=pic16.c
OBJECT1+=pic16.o
HEADER1+=pic16.h
endif
ifeq "$(KTEST)" "1"
CFLAGS+=-DKTEST
endif
ifeq "$(KIO)" "1"
CFLAGS+=-DKIO
endif

CLIB2=$(CLIB)
TARGET2=kio
SOURCE2=kio.c   dotconf.c io.c util.c
OBJECT2=kio.o   dotconf.o io.o util.o
HEADER2=k8048.h dotconf.h io.h util.h
HEADER2+=VERSION Makefile config.mk

ifeq "$(TTY)" "1"
CFLAGS+=-DTTY
SOURCE1+=serial_posix.c
OBJECT1+=serial_posix.o
HEADER1+=serial_posix.h
SOURCE2+=serial_posix.c
OBJECT2+=serial_posix.o
HEADER2+=serial_posix.h
endif

ifeq "$(RPI)" "1"
 ifeq "$(PLATFORM)" "armv6l"
 CFLAGS+=-DRPI
 SOURCE1+=raspi.c
 OBJECT1+=raspi.o
 HEADER1+=raspi.h
 SOURCE2+=raspi.c
 OBJECT2+=raspi.o
 HEADER2+=raspi.h
 endif
endif

ifeq "$(SYSTEM)" "Linux"
 ifeq "$(MCP23017)" "1"
 CFLAGS+=-DMCP23017
 SOURCE1+=mcp23017.c
 OBJECT1+=mcp23017.o
 HEADER1+=mcp23017.h
 SOURCE2+=mcp23017.c
 OBJECT2+=mcp23017.o
 HEADER2+=mcp23017.h
 endif

 ifeq "$(LPICP)" "1"
 CFLAGS+=-DLPICP
 SOURCE1+=lpicp/lpicp_icsp.c
 OBJECT1+=lpicp/lpicp_icsp.o
 HEADER1+=lpicp/lpicp_icsp.h
 SOURCE2+=lpicp/lpicp_icsp.c
 OBJECT2+=lpicp/lpicp_icsp.o
 HEADER2+=lpicp/lpicp_icsp.h
 endif
endif

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
ifeq "$(KIO)" "1"
build: $(TARGET1) $(TARGET2)
else
build: $(TARGET1)
endif

$(TARGET1):$(OBJECT1)
	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1)
# ifneq "$(GDB)" "1"
# 	strip $(TARGET1)
# endif

$(TARGET2):$(OBJECT2)
	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2)
# ifneq "$(GDB)" "1"
# 	strip $(TARGET2)
# endif

$(OBJECT1):$(HEADER1)

$(OBJECT2):$(HEADER2)

c.o: $(CC) -c $< -o $@

install:build
	mkdir -p $(BINDIR)
	cp $(TARGET1) $(BINDIR)/$(TARGET1)
ifeq "$(K12)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k12
endif
ifeq "$(K14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k14
endif
ifeq "$(K16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k16
endif
ifeq "$(KTEST)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/ktest
endif
ifeq "$(KIO)" "1"
	cp $(TARGET2) $(BINDIR)/$(TARGET2)
endif
ifeq (,$(findstring CYGWIN,$(SYSTEM)))
	chown 0:0 $(BINDIR)/$(TARGET1)
	chmod u+s $(BINDIR)/$(TARGET1)
ifeq "$(KIO)" "1"
	chown 0:0 $(BINDIR)/$(TARGET2)
	chmod u+s $(BINDIR)/$(TARGET2)
endif
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
