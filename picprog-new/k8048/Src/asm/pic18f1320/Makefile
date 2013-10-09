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
CFLAGS=-std=c99 -DENABLE_RPI=$(RASPI) -I. -I../.. -ggdb -Wall -Wno-implicit-function-declaration
GCCVERSIONGTEQ4:=$(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 4)
ifeq "$(GCCVERSIONGTEQ4)" "1"
CFLAGS+=-Wno-int-to-pointer-cast
endif

CLIB=-lm -lreadline -lncurses

CTARGET=notetable
CSOURCE=notetable.c
COBJECT=notetable.o
CHEADER=

AS=gpasm -a inhx32 -e on -r dec -w 1 -I ../include/ -I ./include/
AOBJECT=pwm.hex serial.hex synth.hex
AHEADER=./include/notetable.inc

build:$(AOBJECT) $(COBJECT) $(CTARGET)
	cd gEDA && $(MAKE)

$(CTARGET):$(COBJECT)
	$(CC) $(CFLAGS) $(COBJECT) -o $(CTARGET) $(CLIB)

$(AOBJECT):$(AHEADER)

$(COBJECT):$(CHEADER)

%.hex:%.asm
	$(AS) $< -o $@

c.o:    $(CC) -c $< -o $@

install:build
	cd gEDA && $(MAKE) install

uninstall:clean
	cd gEDA && $(MAKE) uninstall

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~ $(CTARGET)
	cd gEDA && $(MAKE) clean
