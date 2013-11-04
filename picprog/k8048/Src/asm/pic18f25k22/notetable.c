/*
 * Velleman K8048 16-bit Timer Natural Frequency Generator
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
 * create a natural frequency table for pic 16-bit timers
 *
 * F = 1 / (2 * (1 / OSC) * (65536 - RELOAD))
 *
 * RELOAD = 65536 - ((1 / F) / (2 * (1 / OSC)))
 */

/* system oscillator and timer prescaling */
#define FOSC (64000000)
#define FOSC4 (FOSC / 4)
#define PRESCALER (4)
#define OSC (FOSC4 / PRESCALER)

/*
 * midi notes range from 21 to 108 for an 88 key piano
 *
 * see http://www.tonalsoft.com/pub/news/pitch-bend.aspx
 * for a midi notes and frequencies.
 */

#if 0
#define MIDI_FIRST (21)
#define FREQ_FIRST (27.5)	/* 27.5Hz (MIDI_FIRST) */
#define MIDI_LAST (108)
#endif

/*
 * IBM PC speaker supports 84 notes and 7 octaves in BASIC
 *
 * IBM basic duration parameter is based on the IBM PC hardware
 * timer interrupt which occured approximately 18.2 times per
 * second (every 54.9 milliseconds) (14318181 / 12 / 65536).
 */
#if PRESCALER == 4
#define MIDI_FIRST (24)
#define FREQ_FIRST (32.7031956626)
#define MIDI_LAST (107)
#endif

#define MIDDLEC (60)

#define STRLEN (1024)

int
main(int argc, char **argv)
{
	/* calculations and table */
	int midi, reload, octave, oldoctave = INT_MAX, note;
	float freq, log, actual;
	static const char notes[][12] = {"C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#,Ab", "A", "A#/Bb", "B"};

	fprintf(stdout, "NOTEFIRST EQU .%d\n", MIDI_FIRST);
	fprintf(stdout, "NOTELAST  EQU .%d\n", MIDI_LAST);
	fprintf(stdout, "NOTETABLE\n");

	for (midi = MIDI_FIRST; midi <= MIDI_LAST; midi++) {
		/* determine note frequency */
		freq = FREQ_FIRST * pow(2.0, ((midi - MIDI_FIRST) / 12.0));

		/* determine reload needed for this frequency */
		reload = (int)(0.5 + ( 65536.0 - ((1.0 / freq) / (2.0 * (1.0 / OSC))) ));

		/* determine actual note frequency */
		actual = 1.0 / (2.0 * (1.0 / OSC) * (65536 - reload));

                /* determine octave (infomational) */
                log = log2(freq);
                octave = (int)(log) - 8;

                /* determine note (informational) */
                note = midi % 12;

		if (oldoctave != octave) {
			if (midi == MIDDLEC)
				fprintf(stdout, ";OCTAVE %d (%d) MIDDLE C\n", octave, octave + 3);
			else
				fprintf(stdout, ";OCTAVE %d (%d)\n", octave, octave + 3);
			oldoctave = octave;
		}
		fprintf(stdout, ";MIDI=%3d(0x%02x) NOTE=%-5s RELOAD=0x%04X (f=%.3f~%.3f)\n", midi, midi, notes[note], reload, freq, actual);
		fprintf(stdout, "       DW  0x%04X\n", reload);
	}
}
