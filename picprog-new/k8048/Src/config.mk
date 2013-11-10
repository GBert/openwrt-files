#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

# build targets (enable:1 disable:0)
K12=0
K14=1
K16=1
K24=1
K32=0
KTEST=1
KIO=1

# backends (where available) (enable:1 disable:0)
TTY=0
RPI=0
MCP23017=0
BITBANG=1

# gdb else strip (gdb:1 strip:0)
GDB=0

# output verbose config (enable:1 disable:0)
VERBOSE=1
