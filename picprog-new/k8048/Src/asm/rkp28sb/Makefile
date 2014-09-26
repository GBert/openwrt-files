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

SYSTEM:=$(shell uname -s)

CC=sdcc
CFLAGS=--use-non-free --fomit-frame-pointer --pstack-model=small -mpic16 -p18f2550 -DDEBUG=1
CLIB=-llibc18f.lib

TARGET=pic18F2550.hex
SOURCE=pic18F2550.c PICHID/usb.c PICHID/main.c
OBJECT=$(SOURCE:.c=.o)
HEADER=$(SOURCE:.c=.h) PICHID/callbacks.h

build:$(TARGET)
ifeq ($(SYSTEM),Linux)
	cd HidComL && $(MAKE)
endif

$(TARGET):$(OBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET) $(CLIB)

$(OBJECT):$(HEADER) Makefile

install:build

uninstall:clean

clean:
ifneq (,$(findstring BSD,$(SYSTEM)))
	find -E . -regex ".*\.(o|asm|cod|err|hex|log|lst)" -delete
else
	find . -regextype posix-extended -regex ".*\.(o|asm|cod|err|hex|log|lst)" -delete
endif
	rm -f *~ .*~ PICHID/*~ PICHID/.*~ etc/*~ etc/.*~
ifeq ($(SYSTEM),Linux)
	cd HidComL && $(MAKE) clean
endif

program:build
	k16 program $(TARGET)

verify:build
	k16 verify $(TARGET)
	
%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@
