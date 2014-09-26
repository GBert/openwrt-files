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

#ifndef _DAC_H
#define _DAC_H

#include <stdlib.h>	/* rand() */
#include <stdbool.h>	/* true || false */
#include <stdint.h>
#include <p33Fxxxx.h>
#include <pps.h>
#include <dsp.h>

#if 1

/*
 * FVCO = FRC(7,372,800) / PLLPRE(3) * PLLDIV(65) = 159,744,000 Hz
 * FOSC = FVCO / PLLPOST(2) = 79,872,000 Hz
 */
#define PLL_PRE  (1)            /* PLLPRE divide by 3  */
#define PLL_DIV  (65 - 2)       /* PLLDIV = 65         */
#define PLL_POST (0)            /* PLLPOST divide by 2 */

/*
 * ACLK    = FVCO / APSTCLR(1) = 159,744,000
 * DACFDIV = ACLK / (Fs * 256)
 *
 * Fs = 48000
 * 	DACFDIV = 159744000 / (48000 * 256) = 13
 * Fs = 24000
 * 	DACFDIV = 159744000 / (24000 * 256) = 26
 * Fs = 12000
 * 	DACFDIV = 159744000 / (12000 * 256) = 52
 *
 * Fs = 12000, DACFDIV = 52-1
 * Fs = 12480, DACFDIV = 50-1
 * Fs = 13000, DACFDIV = 48-1
 * Fs = 15600, DACFDIV = 40-1
 * Fs = 16000, DACFDIV = 39-1
 * Fs = 19500, DACFDIV = 32-1
 * Fs = 20800, DACFDIV = 30-1
 * Fs = 24000, DACFDIV = 26-1
 * Fs = 24960, DACFDIV = 25-1
 * Fs = 26000, DACFDIV = 24-1
 * Fs = 31200, DACFDIV = 20-1
 * Fs = 39000, DACFDIV = 16-1
 * Fs = 41600, DACFDIV = 15-1
 * Fs = 48000, DACFDIV = 13-1
 * Fs = 52000, DACFDIV = 12-1
 * Fs = 62400, DACFDIV = 10-1
 * Fs = 78000, DACFDIV =  8-1
 */
#define DAC_FDIV (8 - 1)	/* DACFDIV = 8 */
#define FS (78000)		/* Fs = 78000  */

/*
 * FCY = FOSC / 2 = 39,936,000 Hz
 */
#define FCY 39936000UL
#include <libpic30.h>	/* __delay32() */

/*
 * UART BRGH = 0, BRGx = (FCY / (16 * BAUD RATE)) - 1
 *
 * BAUD RATE(38400) = 39936000 / 16 / 38400 - 1 = 64
 */
#define BRG1_PC2 (64)

#else

/*
 * FVCO = FRC(7,372,800) / PLLPRE(2) * PLLDIV(42) = 154,828,800 Hz
 * FOSC = FVCO / PLLPOST(2) = 77,414,400 Hz
 */
#define PLL_PRE  (0)            /* PLLPRE divide by 2  */
#define PLL_DIV  (42 - 2)       /* PLLDIV = 42         */
#define PLL_POST (0)            /* PLLPOST divide by 2 */

/*
 * ACLK    = FVCO / APSTCLR(1) = 154,828,800
 * DACFDIV = ACLK / (Fs * 256)
 *
 * DACFDIV = 14, Fs = 154828800 / 256 / 14 = 43200
 *
 * Fs(24192) = 25
 * Fs(25200) = 24
 * Fs(28800) = 21
 * Fs(30240) = 20
 * Fs(33600) = 18
 * Fs(37800) = 16
 * Fs(40320) = 15
 */
#define DAC_FDIV (14 - 1)	/* DACFDIV = 14, Fs = 43200 */
#define FS (43200)

/*
 * FCY = FOSC / 2 = 38,707,200 Hz
 */
#define FCY 38707200UL
#include <libpic30.h>	/* __delay32() */

/*
 * UART BRGH = 0, BRGx = (FCY / (16 * BAUD RATE)) - 1
 *
 * BAUD RATE(115200) = 38707200 / 16 / 115200 - 1 = 20
 *
 * BAUD RATE(38400)  = 38707200 / 16 / 38400  - 1 = 62
 */
#define BRG1_PC2 (62)

#endif

/*
 * DAC FIFO
 */

#define FIFO_SIZE (4) /* Hardware FIFO Size */

typedef struct dac_t {
	fractional fifo[FIFO_SIZE];
	fractional volume;	/* DAC volume level */
} DAC;

/*
 * Waveform
 *
 * Fosc = Fs / 2^N * CW
 * Fosc = Fs * CW / 2^N
 *
 * CW = 2^N / Fs * Fosc
 * CW = (Fosc / Fs) * 2^N
 */

#define FUNC_NONE     (0)
#define FUNC_SINE     (1)
#define FUNC_TRIANGLE (2)
#define FUNC_SAW      (3)
#define FUNC_SQUARE   (4)
#define FUNC_NOISE    (5)

/*
 * 2^32 = 4294967296 2^31 = 2147483648 2^30 = 1073741824 2^29 = 536870912
 * 2^28 = 268435456  2^27 = 134217728  2^26 = 67108864   2^25 = 33554432
 * 2^24 = 16777216   2^23 = 8388608    2^22 = 4194304    2^21 = 2097152
 * 2^20 = 1048576    2^19 = 524288     2^18 = 262144     2^17 = 131072
 * 2^16 = 65536
 */
#define NSHIFT (18 - 16)
#define NPOWER (262144.)

#define CONTROL_1000HZ (0.5 + (NPOWER / FS * 1000.))
#define CONTROL_500HZ  (0.5 + (NPOWER / FS *  500.))

#define DUTY_SQUARE (0x8000)

typedef struct waveform_t {
	uint32_t acc;		/* Accumulator           */
	uint32_t control;	/* Control Word          */
	uint16_t func;		/* Function              */
	uint16_t duty;		/* Square wave duty      */
	fractional volume;	/* Waveform volume level */
} WAVEFORM;

/*
 * EG (Envelope Generator)
 *
 * 1ms tick = 0.001 / ((1 / Fs) * FIFO(4)))
 *
 * Fs(39000) =  9.75
 * Fs(78000) = 19.50
 */

#if FS == 39000
#define MILLISECOND (10)
#elif FS == 78000
#define MILLISECOND (20)
#else
#error "1MS TICK UNCALCULATED"
#endif

typedef struct envelope_t {
	int32_t a;		/* Attack increment      */
	int32_t d;		/* Decay decrement       */
	fractional s;		/* Sustain level         */
	int32_t r;		/* Release decrement     */

	int32_t volume;		/* Envelope volume level */
	uint16_t state;		/* NONE, A, D, S or R    */
} ENVELOPE;

#define STATE_NONE    (0)
#define STATE_ATTACK  (1)
#define STATE_DECAY   (2)
#define STATE_SUSTAIN (3)
#define STATE_RELEASE (4)

/*
 * A Voice
 */

#define VOICE_SIZE (4) /* Number of software voices */

typedef struct voice_t {
	WAVEFORM waveform;
	ENVELOPE envelope;
	/*
	 * Voice volume level =
	 * 	DAC volume level * Waveform volume level * Envelope volume level
	 */
	fractional volume;	/* Voice volume level */
} VOICE;

/* Envelope Generator */
extern void eg(void);

/* DAC FIFO Update */
extern void fifo(void);

#endif
