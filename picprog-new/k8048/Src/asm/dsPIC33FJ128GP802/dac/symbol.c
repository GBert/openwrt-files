/*
 * Copyright (C) 2005-2014 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "dac.h"

/*
 * Translate C structure offsets and definitions for use in assembly code.
 *
 * Method based on http://docs.blackfin.uclinux.org/doku.php?id=toolchain:gas:structs
 */

#define SYMBOL(symbol)         asm volatile("\nSYMBOL " #symbol " %[s] " : : [s] "i" (symbol))
#define SYMBOL2(symbol, value) asm volatile("\nSYMBOL " #symbol " %[v] " : : [v] "i" (value))

#define OFFSETOF(type, member) SYMBOL2(type ## _ ## member, offsetof(type, member))

int
main(void)
{
	OFFSETOF(DAC, fifo);
	OFFSETOF(DAC, volume);
	SYMBOL(FIFO_SIZE);

	OFFSETOF(WAVEFORM, acc);
	OFFSETOF(WAVEFORM, control);
	OFFSETOF(WAVEFORM, func);
	OFFSETOF(WAVEFORM, duty);
	OFFSETOF(WAVEFORM, volume);

	OFFSETOF(ENVELOPE, a);
	OFFSETOF(ENVELOPE, d);
	OFFSETOF(ENVELOPE, s);
	OFFSETOF(ENVELOPE, r);
	OFFSETOF(ENVELOPE, volume);
	OFFSETOF(ENVELOPE, state);
	SYMBOL(STATE_NONE);
	SYMBOL(STATE_ATTACK);
	SYMBOL(STATE_DECAY);
	SYMBOL(STATE_SUSTAIN);
	SYMBOL(STATE_RELEASE);

	OFFSETOF(VOICE, waveform);
	OFFSETOF(VOICE, envelope);
	OFFSETOF(VOICE, volume);
	SYMBOL(VOICE_SIZE);

	return 0;
}
