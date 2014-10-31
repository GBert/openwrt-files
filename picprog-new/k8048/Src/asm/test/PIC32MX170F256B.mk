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

CPU     = 32MX170F256B

HEAP    =
STACK   =

PIC32   = /opt/pic32-tools
ifeq (,$(wildcard $(PIC32)))
$(error $(PIC32) unavailable)
endif

COMPILE = $(PIC32)/bin/pic32-

AS      = $(COMPILE)as
LD      = $(COMPILE)ld
CC      = $(COMPILE)gcc
AR      = $(COMPILE)ar
NM      = $(COMPILE)nm
HX      = $(COMPILE)bin2hex
STRIP   = $(COMPILE)strip
OBJCOPY = $(COMPILE)objcopy
OBJDUMP = $(COMPILE)objdump

RM      = /bin/rm

TARGET  = PIC32MX170F256B.hex

CSOURCE = PIC32MX170F256B.c
COBJECT = $(CSOURCE:.c=.o)
CHEADER = ../include/icspio.h
CTEMPS  = $(CSOURCE:.c=.s)

ELF     = $(TARGET:.hex=.elf)
MAP     = $(TARGET:.hex=.map)

CFLAGS  = -Os -mprocessor=$(CPU) -std=gnu99 -pedantic-errors -Wall -g -fno-short-double -fverbose-asm -save-temps -I../include
CFLAGS += -mno-smart-io -ffunction-sections -fdata-sections -mdebugger -Wcast-align -fframe-base-loclist
CLINK   = -Wl,-T../lib/p$(CPU).ld,-Map=$(MAP),--gc-sections -mno-peripheral-libs -lm

PIHOST  = pi3

build:$(TARGET)

$(TARGET):$(ELF)
	@echo -n "[HX] "
	$(HX) $(ELF)

$(ELF):$(COBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(CPPFLAGS) $(AOBJECT) $(COBJECT) -o $(ELF) $(CLINK)

$(COBJECT):$(CHEADER) PIC32MX170F256B.mk ../lib/p$(CPU).ld

program:build
	k32 program $(TARGET)

verify:build
	k32 verify $(TARGET)

program-pi:build
	ssh ${PIHOST} k32 program < $(TARGET)

verify-pi:build
	ssh ${PIHOST} k32 verify < $(TARGET)

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
	$(RM) -f $(CTEMPS) *.o *.i *.elf *.map *.hex *~
