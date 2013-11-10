#
# Copyright (C) 2013 Darron Broad
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

ARCH    = PIC24F
MCU     = 24FJ64GA002
SCRIPT  = p24FJ64GA002.gld

HEAP    = 1024
STACK   = 16

PIC30   = /opt/pic30-tools
COMPILE = $(PIC30)/bin/pic30-coff-

AS      = $(COMPILE)as
LD      = $(COMPILE)ld
CC      = $(COMPILE)gcc-4.0.3
AR      = $(COMPILE)ar
NM      = $(COMPILE)nm
HX      = $(COMPILE)bin2hex
STRIP   = $(COMPILE)strip
OBJCOPY = $(COMPILE)objcopy
OBJDUMP = $(COMPILE)objdump

RM      = /bin/rm

TARGET  = example1.1.hex
SOURCE  = example1.1.s
OBJECT  = $(SOURCE:.s=.o)

COF     = $(TARGET:.hex=.cof)
MAP     = $(TARGET:.hex=.map)

AFLAGS  = -p$(MCU) -I$(PIC30)/pic30-nonfree/support/$(ARCH)/inc -g
ALINK   = -p$(MCU) -L$(PIC30)/pic30-nonfree/support/$(ARCH)/gld -T$(SCRIPT) -Map=$(MAP) -no-data-init --heap=$(HEAP) --stack=$(STACK)

build:$(TARGET)

$(TARGET):$(COF)
	@echo -n "[HX] "
	$(HX) $(COF)

$(COF):$(OBJECT)
	@echo -n "[LINK] "
	$(LD) -o $(COF) $(OBJECT) $(ALINK)

$(OBJECT):Makefile

%.o:%.s
	@echo -n "[AS] "
	$(AS) $(AFLAGS) $< -a=$(<:.s=.lst) -o $@

install:build

uninstall:clean

clean:
	@echo -n "[RM] "
	$(RM) -f *.o *.cof *.map *.lst *.hex *~
