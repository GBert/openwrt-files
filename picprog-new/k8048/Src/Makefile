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
CFLAGS=-Os -std=gnu99 -pedantic-errors -I. -Wall -fverbose-asm -save-temps
ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
endif
ifeq "$(VERBOSE)" "1"
CFLAGS+=-DVERBOSE
endif
ifneq (,$(findstring CYGWIN,$(SYSTEM)))
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
endif

BINDIR=/usr/local/bin

CLIB1=$(CLIB)
TARGET1=k8048
SOURCE1=k8048.c dotconf.c inhx32.c io.c pic.c util.c

CLIB2=$(CLIB)
TARGET2=kio
SOURCE2=kio.c dotconf.c io.c util.c

ifeq "$(K12)" "1"
CFLAGS+=-DK12
SOURCE1+=pic12.c
endif
ifeq "$(K14)" "1"
CFLAGS+=-DK14
SOURCE1+=pic14.c
endif
ifeq "$(K16)" "1"
CFLAGS+=-DK16
SOURCE1+=pic16.c
endif
ifeq "$(K24)" "1"
CFLAGS+=-DK24
SOURCE1+=pic24.c
endif
ifeq "$(K32)" "1"
CFLAGS+=-DK32
SOURCE1+=pic32.c
endif
ifeq "$(KTEST)" "1"
CFLAGS+=-DKTEST
endif
ifeq "$(KIO)" "1"
CFLAGS+=-DKIO
endif

ifeq "$(TTY)" "1"
CFLAGS+=-DTTY
SOURCE1+=serial_posix.c
SOURCE2+=serial_posix.c
endif

ifeq "$(RPI)" "1"
 ifeq "$(PLATFORM)" "armv6l"
 CFLAGS+=-DRPI
 SOURCE1+=raspi.c
 SOURCE2+=raspi.c
 endif
endif

ifeq "$(SYSTEM)" "Linux"
 ifeq "$(MCP23017)" "1"
 CFLAGS+=-DMCP23017
 SOURCE1+=mcp23017.c
 SOURCE2+=mcp23017.c
 endif

 ifeq "$(BITBANG)" "1"
 CFLAGS+=-DBITBANG
 SOURCE1+=bit-bang-gpio.c
 SOURCE2+=bit-bang-gpio.c
 endif
endif

OBJECT1=$(SOURCE1:.c=.o)
HEADER1=$(SOURCE1:.c=.h)

OBJECT2=$(SOURCE2:.c=.o)
HEADER2=$(SOURCE2:.c=.h)

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
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1) -Wl,-Map=$(TARGET1).map
# ifneq "$(GDB)" "1"
# 	@echo -n "[STRIP] "
# 	strip $(TARGET1)
# endif

$(OBJECT1):$(HEADER1) Makefile config.mk VERSION

$(TARGET2):$(OBJECT2)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2) -Wl,-Map=$(TARGET2).map
# ifneq "$(GDB)" "1"
# 	@echo -n "[STRIP] "
# 	strip $(TARGET2)
# endif

$(OBJECT2):$(HEADER2) Makefile config.mk VERSION

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@

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
ifeq "$(K24)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k24
endif
ifeq "$(K32)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/k32
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
	rm -f $(BINDIR)/k24
	rm -f $(BINDIR)/k32
	rm -f $(BINDIR)/ktest
	rm -f $(BINDIR)/$(TARGET2)

clean:
	cd etc      && $(MAKE) clean
	rm -f $(TARGET1) $(TARGET2) *.o *~ .*~ *.err *.log *.map *.s *.i
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
# COMMAND LINE TOOL, AND DEMOS
#
# REQUIRES: gpasm sdcc pic30-tools gEDA gEDA-utils
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
build-all:build build-asm

install-all:install install-asm

uninstall-all:uninstall uninstall-asm

clean-all:clean clean-asm
