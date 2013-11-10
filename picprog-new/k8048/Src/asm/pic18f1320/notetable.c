/*
 * Velleman K8048 PWM Natural Frequency Generator
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

/*
 * create a natural frequency table for pic pwm
 *
 * due to range of the pwm timer, an external (cd4040) post scaler and mux
 * is needed to select the correct output frequency.
 *
 * this example if for a 10MHz system clock (10Mhz HSPLL= 40/4) with a /16 prescaler.
 *
 * XXX the top most octave suffers from resolution errors on the PIC PWM.
 */

/* mux selector range start bit (3 bit range, 8 inputs) */
#define MUX (0)

/* default system oscillator and timer2 prescaling */
#define HSPLL (10000000)	/* 10MHz HSPLL (40MHz/4) */
#define PRESCALER (16)		/* /16 */

/* midi note range 0 to 127. only notes 21 to 108 (88 piano keys) are generally used */
#define MIDI_FIRST (0)
#define MIDI_LAST (127)
#define FREQ_FIRST (8.1757989156)/* Hz */

#define MUXSHIFT (5)	/* mux control shift. if RB0..RB2 shift=0, if RB5..RB7 shift=5 */

#define STRLEN (1024)

int
main(int argc, char **argv)
{
	/* calculations and table */
	int midi, pr2, ccpr1l, post, limit, octave, oldoctave = INT_MAX, note;
	float freq, div, log;
	static const char notes[][12] = {"C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#,Ab", "A", "A#/Bb", "B"};
	int firstline = 0;

	/* defaults */
	int sclk = HSPLL;
	int prescaler = PRESCALER;
	int mux = MUX;
	int muxshift = MUXSHIFT;

	/* readline */
	char *line, prompt[STRLEN];
	int valid;

	/* determine FSPLL */
	do {
		snprintf(prompt, STRLEN, "HSPLL (%d): ", sclk);
		line = readline(prompt);
		if (line[0]) {
			add_history(line);
			sclk = strtoul(line, NULL, 0);
		}
		valid = 1;
	} while (!valid);
	
	/* determine PRESCALER */
	do {
		snprintf(prompt, STRLEN, "PRE (%d): ", prescaler);
		line = readline(prompt);
		if (line[0]) {
			add_history(line);
			prescaler = strtoul(line, NULL, 0);
			if (!(prescaler == 1 || prescaler == 4 || prescaler == 16)) {
				printf("Error: Invalid prescaler\n");
				prescaler = PRESCALER;
				valid = 0;
			} else {
				valid = 1;
			}
		}
		else
			valid = 1;
	} while (!valid);

	/*
	 * determine mux start
	 *
	 * mux input is from pic and cd4040 (13 available inputs, 1(pic) + 12(cd4040) 8 are used
	 */
	do {
		snprintf(prompt, STRLEN, "MUX INPUT RANGE START BIT (0..5) (%d): ", mux);
		line = readline(prompt);
		if (line[0]) {
			add_history(line);
			mux = strtoul(line, NULL, 0);
			/* 
			 * mux input ranges available are:
			 *
			 * 0..7 1..8 2..9 3..10 4..11 5..12
			 */
			if (!(mux >= 0 && mux <= 5)) {
				printf("Error: Invalid mux input range\n");
				mux = MUX;
				valid = 0;
			} else {
				valid = 1;
			}
		} else {
			valid = 1;
		}
	} while (!valid);
	
	/*
	 * determine mux shift
	 *
	 * mux is controlled by 3 bits on port output lines. mux shift is the first port bit.
	 */
	do {
		snprintf(prompt, STRLEN, "MUX CONTROL PORT START BIT (0..5) (%d): ", muxshift);
		line = readline(prompt);
		if (line[0]) {
			add_history(line);
			muxshift = strtoul(line, NULL, 0);
			/* 
			 * mux control ranges available are:
			 *
			 * 0,1,2 1,2,3 2,3,4 4,5,6 5,6,7 
			 */
			if (!(muxshift >= 0 && muxshift <= 5)) {
				printf("Error: Invalid mux control range\n");
				muxshift = MUXSHIFT;
				valid = 0;
			} else {
				valid = 1;
			}
		} else {
			valid = 1;
		}
	} while (!valid);
	
	for (midi = MIDI_FIRST; midi <= MIDI_LAST; midi++) {
		/* determine note frequency */
		freq = FREQ_FIRST * pow(2.0, ((midi - MIDI_FIRST) / 12.0));

		/* determine divisor needed for this frequency */
		div = (sclk / prescaler) / freq;

		/* scale divisor to fit into 8 bits and determine post scaler */
		post = 0;
		limit = 256;
		while (div >= limit) {
			limit *= 2;
			post++;
		}

		/* determine mark/space time for timer2 */
		pr2 = div / pow(2, post);
		ccpr1l = pr2 / 2;

		/* determine octave (infomational) */
		log = log2(freq);
		octave = (int)(log) - 8;

		/* determine note (informational) */
		note = midi % 12;

		/* output details if within postscaler mux range */
		if (post >= mux && post < (mux + 8)) {
			if (++firstline == 1) {
				fprintf(stdout, "NOTESTART   EQU .%d\n", midi);
				fprintf(stdout, "NOTETABLE\n");
			}
			if (oldoctave != octave)
				fprintf(stdout, ";OCTAVE %d\n", octave);
			oldoctave = octave;

			fprintf(stdout, ";MIDI=%3d(0x%02x) NOTE=%-5s HSPLL=%d PRESCALER=%d PR2=0x%02x CCPR1L=0x%02x POSTSCALER=2^%d (f=%.3f)\n",
				midi, midi, notes[note], sclk, prescaler, pr2, ccpr1l, post, freq);
			fprintf(stdout, "	DW  0x%02x%02x\n", pr2, post << muxshift);
		}
	}
}
