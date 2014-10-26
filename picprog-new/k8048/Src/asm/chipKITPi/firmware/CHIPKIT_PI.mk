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

CPU     = 32MX250F128B
BOARD	= CHIPKIT_PI

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

TARGET  = $(BOARD).hex

CSOURCE = main.c
CHEADER = main.h BoardConfig.h serial.h util.h
COBJECT = $(BOARD)/main.o

ASOURCE = crt0.S
AOBJECT = $(BOARD)/crt0.o

ELF     = $(TARGET:.hex=.elf)
MAP     = $(TARGET:.hex=.map)

CFLAGS  = -Os -mips16 -mprocessor=$(CPU) -std=gnu99 -pedantic-errors -Wall -fverbose-asm -save-temps=obj
CFLAGS += -mno-smart-io -w -ffunction-sections -fdata-sections -g3 -mdebugger -Wcast-align -fno-short-double -fframe-base-loclist
CFLAGS += -D_BOARD_$(BOARD)_
CLINK   = -mno-peripheral-libs -nostartfiles -nostdlib -Wl,-TMX1-2-boot-linkerscript.ld,-Map=$(MAP),--gc-sections

build:init $(TARGET)

init:
	@mkdir -p $(BOARD)

$(TARGET):$(ELF)
	@echo -n "[HX] "
	$(HX) $(ELF)

$(ELF):$(AOBJECT) $(COBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(CPPFLAGS) $(AOBJECT) $(COBJECT) -o $(ELF) $(CLINK)

$(COBJECT):$(CHEADER) Makefile MX1-2-boot-linkerscript.ld

program:build
	k32 program $(TARGET)

verify:
	k32 verify $(TARGET)

$(BOARD)/%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BOARD)/%.o:%.S
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(AFLAGS) $(CPPFLAGS) -c $< -o $@

install:build

uninstall:clean

clean:
	@echo -n "[RM] "
	$(RM) -Rf $(BOARD)
	@echo -n "[RM] "
	$(RM) -f *.elf *.map *.hex *~
