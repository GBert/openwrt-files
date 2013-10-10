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

CLIB=
CTARGET=
CSOURCE=
COBJECT=
CHEADER=

AS=gpasm -a inhx32 -e on -r dec -I ../include/
AOBJECT=pic16f84a.hex pic16f627.hex pic16f88.hex pic16f648a.hex \
	pic16f819.hex pic16f676.hex pic12f675.hex pic16f877a.hex \
	pic18f1320.hex pic18f2550.hex pic18f252.hex pic16f716.hex \
	pic16f505.hex pic10f200.hex pic10f202.hex pic18f4550.hex \
	pic18f2320.hex pic12f683.hex pic16f688.hex pic16f628a.hex \
	pic16f726.hex pic12f508.hex pic12f615.hex pic16f872.hex \
	pic16f57.hex pic16f54.hex pic16f506.hex pic10f220.hex \
	pic16f887.hex pic12f519.hex pic16f73.hex pic16f886.hex \
	pic18f4620.hex pic16f876a.hex pic18f4520.hex pic16f917.hex \
	pic16f84.hex pic18f1330.hex pic18f2431.hex

ASTEST:=$(shell gpasm -l | grep -i 10f320)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic10f320.hex
endif

ASTEST:=$(shell gpasm -l | grep -i 12f617)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic12f617.hex
endif

ASTEST:=$(shell gpasm -l | grep -i 16f1507)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic16f1507.hex
endif

ASTEST:=$(shell gpasm -l | grep -i 18f25k22)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic18f25k22.hex
endif

ASTEST:=$(shell gpasm -l | grep -i 18lf27j53)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic18lf27j53.hex
endif

ASTEST:=$(shell gpasm -l | grep -i 18f26k80)
ifneq "$(strip $(ASTEST))" ""
AOBJECT+=pic18f26k80.hex
endif

AHEADER=../include/device.inc ../include/const.inc ../include/delay.inc \
	../include/icspio.inc ../include/macro.inc ../include/shadow.inc \
	../include/common.inc ../include/ampire-ac162d.inc ../include/ps2.inc

build:$(AOBJECT) $(COBJECT) $(CTARGET)

$(AOBJECT):$(AHEADER)

$(COBJECT):$(CHEADER)

$(CTARGET):$(COBJECT)
	$(CC) $(CFLAGS) $(COBJECT) -o $(CTARGET) $(CLIB)

%.hex:%.asm
	$(AS) $< -o $@

c.o:	$(CC) -c $< -o $@

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~ $(CTARGET)
