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

#FreeBSD IBMPC FreeBSD 9.0-RELEASE   amd64
#Debian  IBMPC Linux   3.2.0-4-amd64 x86_64
#Debian  R-PI  Linux   3.2.27+       armv6l
SYSTEM:=$(shell uname -s)
RELEASE:=$(shell uname -r)
PLATFORM:=$(shell uname -m)

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -I. -Wall
ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
endif
ifneq (,$(findstring CYGWIN,$(SYSTEM)))
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
endif

CTARGET=notetable
CSOURCE=notetable.c
COBJECT=$(CSOURCE:.c=.o)
CHEADER=$(CSOURCE:.c=.h)
CLIB=-lm

build:$(COBJECT) $(CTARGET)

$(AOBJECT):$(AHEADER) Makefile

$(CTARGET):$(COBJECT)
	@echo -n "[CC] "
	$(CC) $(CFLAGS) $(COBJECT) -o $(CTARGET) $(CLIB)

$(COBJECT):$(CHEADER) Makefile

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@

install:build

uninstall:clean

clean:
	rm -f *.lst *.hex *.cod *.log *.o *~ $(CTARGET)
