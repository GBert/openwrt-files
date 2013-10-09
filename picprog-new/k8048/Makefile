#
# Velleman K8048 Programmer for FreeBSD and others.
#
# Copyright (c) 2005-2013 Darron Broad
# All rights reserved.
#
# Licensed under the terms of the BSD license, see file LICENSE for details.
#

#
# COMMAND LINE TOOL ONLY
#
# DEBIAN:
#  sudo apt-get install mercurial
#  hg clone http://hg.kewl.org/pub/k8048/
#  cd k8048
#  make build
#  sudo make install
#
build:
	cd Doc && $(MAKE)
	cd Src && $(MAKE)

install:
	cd Src && $(MAKE) install

uninstall:
	cd Src && $(MAKE) uninstall

clean:
	rm -f *~
	cd Doc && $(MAKE) clean
	cd Src && $(MAKE) clean
#
# COMMAND LINE TOOL & ASSEMBLER DEMOS
# 
# REQUIRES: gpasm gEDA gEDA-utils
#
# DEBIAN:
#  sudo apt-get install gputils
#  sudo apt-get install geda
#  sudo apt-get install geda-utils
#  sudo apt-get install libreadline-dev
#  sudo apt-get install libncurses-dev
#  make build-all
#  sudo make install-all
#
build-all:
	cd Doc && $(MAKE)
	cd Src && $(MAKE) build-all

install-all:
	cd Src && $(MAKE) install-all

uninstall-all:
	cd Src && $(MAKE) uninstall-all

clean-all:
	rm -f *~
	cd Doc && $(MAKE) clean
	cd Src && $(MAKE) clean-all
