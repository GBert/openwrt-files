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

BUILD_DIR=/embedded/raspbian/build
HAS_BUILD_DIR := $(firstword $(wildcard $(BUILD_DIR)))
ifeq (,$(HAS_BUILD_DIR))
 $(error build directory not found)
endif

KERNEL_DIR=$(BUILD_DIR)/linux
HAS_KERNEL_DIR := $(firstword $(wildcard $(KERNEL_DIR)))
ifeq (,$(HAS_KERNEL_DIR))
 $(error kernel directory not found)
endif

COMPILER_DIR=$(BUILD_DIR)/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin
HAS_COMPILER_DIR := $(firstword $(wildcard $(COMPILER_DIR)))
ifeq (,$(HAS_COMPILER_DIR))
 $(error cross-compiler directory not found)
endif

CROSS_COMPILE=$(COMPILER_DIR)/arm-bcm2708hardfp-linux-gnueabi-

PWD := $(shell pwd)

obj-m := bit-bang-gpio/src/bit-bang-gpio.o

modules:header
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -j 8 -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

temps:header
	$(MAKE) KCFLAGS=-save-temps ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -j 8 -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

header:
	cp bit-bang-gpio/src/bit-bang-gpio.h ../bit-bang-gpio.h

clean:
	find . -name "*~" -delete
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean
