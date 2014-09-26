#
# Copyright (C) 2005-2014 Darron Broad
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name `Darron Broad' nor the names of any contributors
#    may be used to endorse or promote products derived from this
#    software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

ARCH    = dsPIC33F
CPU     = 33FJ128GP802

HEAP    = 64
STACK   = 64

PIC30   = /opt/pic30-tools
ifeq (,$(wildcard $(PIC30)))
$(error $(PIC30) unavailable)
endif

COMPILE = $(PIC30)/bin/pic30-coff-
GCC     = gcc-4.5.1

AS      = $(COMPILE)as
LD      = $(COMPILE)ld
CC      = $(COMPILE)$(GCC)
AR      = $(COMPILE)ar
NM      = $(COMPILE)nm
HX      = $(COMPILE)bin2hex
STRIP   = $(COMPILE)strip
OBJCOPY = $(COMPILE)objcopy
OBJDUMP = $(COMPILE)objdump

RM      = /bin/rm

TARGET  = led.hex

CSOURCE = led.c
COBJECT = $(CSOURCE:.c=.o)
CHEADER = $(CSOURCE:.c=.h)
CTEMPS  = $(CSOURCE:.c=.s)

ASOURCE = blink.s
AOBJECT = $(ASOURCE:.s=.o)

COF     = $(TARGET:.hex=.cof)
MAP     = $(TARGET:.hex=.map)

SIM30   = /opt/microchip/xc16/v1.21/bin/sim30
SIM     = $(TARGET:.hex=.sim)

AFLAGS  = -p$(CPU) -g
CFLAGS  = -mcpu=$(CPU) -Os -std=gnu99 -pedantic-errors -Wall -g -fno-short-double -fverbose-asm -save-temps -merrata=exch
CLINK   = -Wl,-Tp${CPU}.gld,-Map=$(MAP),--heap=$(HEAP),--stack=$(STACK) -L$(PIC30)/non-free/lib

PIHOST  = pi3

build:$(TARGET)

offsetof.h:offsetof.s
	@echo -n "[AWK] "
	awk '($$1 == "OFFSETOF") { print "#define " toupper($$2) " (" $$3 ")" }' offsetof.s > offsetof.h

offsetof.inc:offsetof.s
	@echo -n "[AWK] "
	awk '($$1 == "OFFSETOF") { print ".equiv      " toupper($$2) ", " $$3 }' offsetof.s > offsetof.inc

offsetof.s:offsetof.c Makefile
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -S offsetof.c -o offsetof.s

$(TARGET):$(COF)
	@echo -n "[HX] "
	$(HX) $(COF)

$(COF):$(AOBJECT) $(COBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(CPPFLAGS) $(AOBJECT) $(COBJECT) -o $(COF) $(CLINK)

$(COBJECT):$(CHEADER) Makefile

$(AOBJECT):Makefile

program:build
	k24 lvp program $(TARGET)

verify:build
	k24 lvp verify $(TARGET)

program-pi:build
	ssh ${PIHOST} k24 lvp program < $(TARGET)

verify-pi:build
	ssh ${PIHOST} k24 lvp verify < $(TARGET)

sim:build
	$(SIM30) $(SIM)

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:%.S
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(AFLAGS) $(CPPFLAGS) -c $< -o $@

%.o:%.s
	@echo -n "[AS] "
	$(AS) $(AFLAGS) $< -a=$(<:.s=.lst) -o $@

install:build

uninstall:clean

clean:
	@echo -n "[RM] "
	$(RM) -f $(CTEMPS) offsetof.s offsetof.h offsetof.inc *.o *.i *.cof *.map *.hex *.lst UartOut.txt *~
