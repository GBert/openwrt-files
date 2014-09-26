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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

/*
 * Natural frequency table for PIC16 16-bit timers
 *
 * F = 1 / (2 * (1 / OSC) * (65536 - RELOAD))
 *
 * RELOAD = 65536 - ((1 / F) / (2 * (1 / OSC)))
 */

/* System oscillator and timer prescale */
#define FOSC	  (64000000)
#define FOSCDIV4  (FOSC / 4)
#define PRESCALER (4)
#define OSC	  (FOSCDIV4 / PRESCALER)

/*
 * dsPIC33Fj128GP802 NCO Control Word for software table lookup generator.
 *
 * CW = 2^N / Fsample * F
 *
 * 2^32 = 4294967296 2^31 = 2147483648 2^30 = 1073741824 2^29 = 536870912
 * 2^28 = 268435456  2^27 = 134217728  2^26 = 67108864   2^25 = 33554432
 * 2^24 = 16777216   2^23 = 8388608    2^22 = 4194304    2^21 = 2097152
 * 2^20 = 1048576    2^19 = 524288     2^18 = 262144     2^17 = 131072
 * 2^16 = 65536
 */

#define NPOWER  (262144.)
#define FSAMPLE (78000.)

#if 0

/*
 * Midi notes range from 21 to 108 for an 88 key piano
 *	See: http://www.tonalsoft.com/pub/news/pitch-bend.aspx for a midi notes and frequencies.
 */

/*  21 = OCTAVE -2 A 27.5 Hz */
#define MIDI_FIRST (21)
#define FREQ_FIRST (27.5)
/* 108 = OCTAVE +3 C 4,186.0 Hz */
#define MIDI_LAST (108)

#endif

#if 0

/*
 * IBM PC speaker supports 84 notes of 7 octaves in BASIC
 *
 * IBM basic duration parameter is based on the IBM PC hardware
 * timer interrupt which occured approximately 18.2 times per
 * second (every 54.9 milliseconds) (14318181 / 12 / 65536).
 */

/*  24 = OCTAVE -3 C 32.7 Hz */
#define MIDI_FIRST (24)
#define FREQ_FIRST (32.7031956626)
/* 107 = OCTAVE +3 B 3,951 Hz */
#define MIDI_LAST (107)

#endif

#if 1

/*
 * Full midi note range 0..127
 */

/*   0 = OCTAVE -5 C 8.176 Hz */
#define MIDI_FIRST (0)
#define FREQ_FIRST (8.1757989156)
/* 127 = OCTAVE +5 G 12,544 Hz */
#define MIDI_LAST (127)

#endif
