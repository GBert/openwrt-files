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

build:
	$(MAKE) -f CHIPKIT_PI.mk build
	$(MAKE) -f CHIPKIT_PI_MX270.mk build
	$(MAKE) -f CHIPKIT_PI_MX270_48.mk build

install:build
	$(MAKE) -f CHIPKIT_PI.mk install
	$(MAKE) -f CHIPKIT_PI_MX270.mk install
	$(MAKE) -f CHIPKIT_PI_MX270_48.mk install

uninstall:
	$(MAKE) -f CHIPKIT_PI.mk uninstall
	$(MAKE) -f CHIPKIT_PI_MX270.mk uninstall
	$(MAKE) -f CHIPKIT_PI_MX270_48.mk uninstall

clean:
	$(MAKE) -f CHIPKIT_PI.mk clean
	$(MAKE) -f CHIPKIT_PI_MX270.mk clean
	$(MAKE) -f CHIPKIT_PI_MX270_48.mk clean
