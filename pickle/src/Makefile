#
# Copyright (C) 2005-2020 Darron Broad
# All rights reserved.
#
# This file is part of Pickle Microchip PIC ICSP.
#
# Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation.
#
# Pickle Microchip PIC ICSP is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
#

# build targets and i/o configuration, see config directory
include config.mk

#FreeBSD IBMPC: FreeBSD 9.0-RELEASE   amd64
#Debian  IBMPC: Linux   3.2.0-4-amd64 x86_64
#Debian  RPi:   Linux   3.2.27+       armv6l
SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)

ifeq "$(SYSTEM)" "FreeBSD"
 ifneq (,$(findstring 10.,$(RELEASE)))
  CC=cc
 else
  CC=gcc
 endif
else
 CC=gcc
endif

CFLAGS=-Os -std=gnu99 -pedantic-errors -I. -Wall -fverbose-asm -save-temps
ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
endif

ifeq "$(VERBOSE)" "1"
CFLAGS+=-DVERBOSE
endif

ifeq "$(LOADER)" "1"
CFLAGS+=-DLOADER
endif

ifneq (,$(findstring CYGWIN,$(SYSTEM)))
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
endif

BINDIR=/usr/local/bin

#CLIB0=$(CLIB)
#TARGET0=pickle
#SOURCE0=pickle.c

CLIB1=$(CLIB)
TARGET1=pickle
SOURCE1=pickle.c dotconf.c inhx32.c io.c pic.c util.c

CLIB2=$(CLIB)
TARGET2=pio
SOURCE2=pio.c icspio.c dotconf.c io.c util.c

CLIB3=$(CLIB)
TARGET3=ptest
SOURCE3=ptest.c icspio.c dotconf.c io.c util.c

CLIB4=$(CLIB)
TARGET4=pctrl
SOURCE4=pctrl.c dotconf.c io.c util.c

CLIB5=$(CLIB)
TARGET5=pload
SOURCE5=pload.c dotconf.c inhx32.c io.c util.c stk500v2.c stk500v2_load.c ip.c rw.c serial_posix.c

ifeq "$(DRYRUN)" "1"
CFLAGS+=-DDRYRUN
endif

ifeq "$(P12)" "1"
CFLAGS+=-DP12
SOURCE1+=pic12.c
endif

ifeq "$(P14)" "1"
CFLAGS+=-DP14
SOURCE1+=pic14.c
endif

ifeq "$(N14)" "1"
CFLAGS+=-DN14
SOURCE1+=pic14n.c
endif

ifeq "$(P16)" "1"
CFLAGS+=-DP16
SOURCE1+=pic16.c
endif

ifeq "$(N16)" "1"
CFLAGS+=-DN16
SOURCE1+=pic16n.c
endif

ifeq "$(P24)" "1"
CFLAGS+=-DP24
SOURCE1+=pic24.c
endif

ifeq "$(P32)" "1"
CFLAGS+=-DP32
SOURCE1+=pic32.c
endif

ifeq "$(PCTRL)" "1"
CFLAGS+=-DPCTRL
endif

ifeq "$(PTEST)" "1"
CFLAGS+=-DPTEST
endif

ifeq "$(PIO)" "1"
CFLAGS+=-DPIO
endif

ifeq "$(PLOAD)" "1"
CFLAGS+=-DPLOAD
endif

$(info $$CONFIG_TARGET_PROFILE is [${CONFIG_TARGET_PROFILE}])

ifeq "$(PICKLE_RPI)" "1"
 SOURCE1+=raspi.c
 SOURCE2+=raspi.c
 SOURCE3+=raspi.c
 SOURCE4+=raspi.c
 SOURCE5+=raspi.c
endif

ifeq "$(PICKLE_BPI)" "1"
 SOURCE1+=allwinner.c
 SOURCE2+=allwinner.c
 SOURCE3+=allwinner.c
 SOURCE4+=allwinner.c
 SOURCE5+=allwinner.c
endif

ifeq "$(SYSTEM)" "Linux"
 ifeq "$(PCF8574)" "1"
 CFLAGS+=-DPCF8574
 SOURCE1+=pcf8574.c
 SOURCE2+=pcf8574.c
 SOURCE3+=pcf8574.c
 SOURCE4+=pcf8574.c
 SOURCE5+=pcf8574.c
 endif

 ifeq "$(MCP23016)" "1"
 CFLAGS+=-DMCP23016
 SOURCE1+=mcp23016.c
 SOURCE2+=mcp23016.c
 SOURCE3+=mcp23016.c
 SOURCE4+=mcp23016.c
 SOURCE5+=mcp23016.c
 endif

 ifeq "$(MCP230XX)" "1"
 CFLAGS+=-DMCP230XX
 SOURCE1+=mcp230xx.c
 SOURCE2+=mcp230xx.c
 SOURCE3+=mcp230xx.c
 SOURCE4+=mcp230xx.c
 SOURCE5+=mcp230xx.c
 endif

 ifeq "$(MCP23SXX)" "1"
 CFLAGS+=-DMCP23SXX
 SOURCE1+=mcp23sxx.c
 SOURCE2+=mcp23sxx.c
 SOURCE3+=mcp23sxx.c
 SOURCE4+=mcp23sxx.c
 SOURCE5+=mcp23sxx.c
 endif

 ifeq "$(MCP2221)" "1"
 CFLAGS+=-DMCP2221
 SOURCE1+=mcp2221.c
 SOURCE2+=mcp2221.c
 SOURCE3+=mcp2221.c
 SOURCE4+=mcp2221.c
 SOURCE5+=mcp2221.c
 endif

 ifeq "$(CP2104)" "1"
 CFLAGS+=-DCP2104
 SOURCE1+=cp2104-bb.c
 SOURCE2+=cp2104-bb.c
 SOURCE3+=cp2104-bb.c
 SOURCE4+=cp2104-bb.c
 SOURCE5+=cp2104-bb.c
 endif

 ifeq "$(BITBANG)" "1"
 CFLAGS+=-DBITBANG
 SOURCE1+=gpio-bb.c
 SOURCE2+=gpio-bb.c
 SOURCE3+=gpio-bb.c
 SOURCE4+=gpio-bb.c
 SOURCE5+=gpio-bb.c
 endif

 ifeq "$(SYSFSGPIO)" "1"
 CFLAGS+=-DSYSFSGPIO
 SOURCE1+=sysfs_gpio.c rw.c
 SOURCE2+=sysfs_gpio.c rw.c
 SOURCE3+=sysfs_gpio.c rw.c
 SOURCE4+=sysfs_gpio.c rw.c
 SOURCE5+=sysfs_gpio.c
 endif

 ifeq "$(FTDI)" "1"
 CFLAGS+=-DFTDI
 SOURCE1+=ftdi-bb.c
 SOURCE2+=ftdi-bb.c
 SOURCE3+=ftdi-bb.c
 SOURCE4+=ftdi-bb.c
 SOURCE5+=ftdi-bb.c
 CLIB+=-lftdi1
 endif
endif

SERIAL_POSIX=0

ifeq "$(SERIAL)" "1"
CFLAGS+=-DSERIAL
SOURCE1+=serial-bb.c
SOURCE2+=serial-bb.c
SOURCE3+=serial-bb.c
SOURCE4+=serial-bb.c
SOURCE5+=serial-bb.c
SERIAL_POSIX=1
endif

ifeq "$(STK500)" "1"
CFLAGS+=-DSTK500
SOURCE1+=stk500v2.c stk500v2_listen.c ip.c rw.c
SERIAL_POSIX=1
endif

ifeq "$(SERIAL_POSIX)" "1"
SOURCE1+=serial_posix.c
SOURCE2+=serial_posix.c
SOURCE3+=serial_posix.c
SOURCE4+=serial_posix.c
endif

ifeq "$(QSORT)" "1"
CFLAGS+=-DQSORT
endif

OBJECT1=$(SOURCE1:.c=.o)
HEADER1=$(SOURCE1:.c=.h)

OBJECT2=$(SOURCE2:.c=.o)
HEADER2=$(SOURCE2:.c=.h)

OBJECT3=$(SOURCE3:.c=.o)
HEADER3=$(SOURCE3:.c=.h)

OBJECT4=$(SOURCE4:.c=.o)
HEADER4=$(SOURCE4:.c=.h)

OBJECT5=$(SOURCE5:.c=.o)
HEADER5=$(SOURCE5:.c=.h)

#
# COMMAND LINE TOOL(S)
#
TARGETS:=$(TARGET1)
ifeq "$(PIO)" "1"
TARGETS:=$(TARGETS) $(TARGET2)
endif
ifeq "$(PTEST)" "1"
TARGETS:=$(TARGETS) $(TARGET3)
endif
ifeq "$(PCTRL)" "1"
TARGETS:=$(TARGETS) $(TARGET4)
endif
ifeq "$(PLOAD)" "1"
TARGETS:=$(TARGETS) $(TARGET5)
endif
build:$(TARGETS)

$(TARGET1):$(OBJECT1)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1) -Wl,-Map,$(TARGET1).map
ifneq "$(GDB)" "1"
#	@echo -n "[STRIP] "
#	strip $(TARGET1)
endif

$(OBJECT1):$(HEADER1) Makefile config.mk VERSION

$(TARGET2):$(OBJECT2)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2) -Wl,-Map,$(TARGET2).map
ifneq "$(GDB)" "1"
#	@echo -n "[STRIP] "
#	strip $(TARGET2)
endif

$(OBJECT2):$(HEADER2) Makefile config.mk VERSION pickle.h

$(TARGET3):$(OBJECT3)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT3) -o $(TARGET3) $(CLIB3) -Wl,-Map,$(TARGET3).map
ifneq "$(GDB)" "1"
#	@echo -n "[STRIP] "
#	strip $(TARGET3)
endif

$(OBJECT3):$(HEADER3) Makefile config.mk VERSION pickle.h

$(TARGET4):$(OBJECT4)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT4) -o $(TARGET4) $(CLIB4) -Wl,-Map,$(TARGET4).map
ifneq "$(GDB)" "1"
#	@echo -n "[STRIP] "
#	strip $(TARGET4)
endif

$(OBJECT4):$(HEADER4) Makefile config.mk VERSION pickle.h

$(TARGET5):$(OBJECT5)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT5) -o $(TARGET5) $(CLIB5) -Wl,-Map,$(TARGET5).map
ifneq "$(GDB)" "1"
#	@echo -n "[STRIP] "
#	strip $(TARGET5)
endif

$(OBJECT5):$(HEADER5) Makefile config.mk VERSION pickle.h

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@

install:build
	mkdir -p $(BINDIR)
	cp $(TARGET1) $(BINDIR)/$(TARGET1)
ifeq "$(P12)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p12
endif
ifeq "$(P14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p14
endif
ifeq "$(N14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/n14
endif
ifeq "$(P16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p16
endif
ifeq "$(N16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/n16
endif
ifeq "$(P24)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p24
endif
ifeq "$(P32)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p32
endif
ifeq "$(PIO)" "1"
	cp $(TARGET2) $(BINDIR)/$(TARGET2)
endif
ifeq "$(PTEST)" "1"
	cp $(BINDIR)/$(TARGET1) $(BINDIR)/ptest
endif
ifeq "$(PCTRL)" "1"
	cp $(BINDIR)/$(TARGET1) $(BINDIR)/pctrl
endif
ifeq "$(TTY)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/pload
endif
ifeq (,$(findstring CYGWIN,$(SYSTEM)))
	chown 0:0 $(BINDIR)/$(TARGET1)
	chmod u+s $(BINDIR)/$(TARGET1)
 ifeq "$(PIO)" "1"
	chown 0:0 $(BINDIR)/$(TARGET2)
	chmod u+s $(BINDIR)/$(TARGET2)
 endif
endif

uninstall:
	rm -f $(BINDIR)/pickle
	rm -f $(BINDIR)/p12
	rm -f $(BINDIR)/p14
	rm -f $(BINDIR)/n14
	rm -f $(BINDIR)/p16
	rm -f $(BINDIR)/n16
	rm -f $(BINDIR)/p24
	rm -f $(BINDIR)/p32
	rm -f $(BINDIR)/pio
	rm -f $(BINDIR)/ptest
	rm -f $(BINDIR)/pctrl
	rm -f $(BINDIR)/pload

clean:
	cd dotconf && $(MAKE) clean
	rm -f pickle pio ptest pctrl pload *.o *~ .*~ *.err *.log *.map *.s *.i
