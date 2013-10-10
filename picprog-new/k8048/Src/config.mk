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
K14=0
K16=1
KTEST=0
KIO=0

# backends (where available) (enable:1 disable:0)
TTY=0
RPI=0
MCP23017=0
LPICP=1

# gdb else strip (gdb:1 strip:0)
GDB=0
