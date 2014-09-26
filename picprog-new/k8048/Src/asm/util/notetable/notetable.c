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

#include "notetable.h"

static const char notes[][12] = {
	"C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#,Ab", "A", "A#/Bb", "B"
};

void
pic16timer(uint8_t midi, const char *note, double frequency, int8_t octave)
{
	/* Determine reload */
	uint16_t reload = (0.5 + (65536. - ((1. / frequency) / (2. * (1. / OSC)))));

	/* Determine actual frequency */
	double actual = 1. / (2. * (1. / OSC) * (65536. - reload));

	if (midi == MIDI_FIRST) {
		fprintf(stdout, "NOTEFIRST EQU .%d\n", MIDI_FIRST);
		fprintf(stdout, "NOTELAST  EQU .%d\n", MIDI_LAST);
		fprintf(stdout, "NOTETABLE\n");
	}
	fprintf(stdout, "    DW 0x%04X ;MIDI=%3d(0x%02X) OCTAVE=%-2d NOTE=%-5s f=%-8.2f (%.2f)\n",
		reload, midi, midi, octave, note, frequency, actual);
}

void
dsPICnco(uint8_t midi, const char *note, double frequency, int8_t octave)
{
	/* Determine control word */
	uint16_t cw = (0.5 + (NPOWER / FSAMPLE * frequency));

	/* Determine actual frequency */
	double actual = FSAMPLE * cw / NPOWER;

	if (midi == MIDI_FIRST) {
		fprintf(stdout, "#define NOTEFIRST (%d)\n", MIDI_FIRST);
		fprintf(stdout, "#define NOTELAST  (%d)\n", MIDI_LAST);
		fprintf(stdout, "const uint16_t notetable[%d] = {\n",
			1 + MIDI_LAST - MIDI_FIRST);
	}
	fprintf(stdout, "0x%04X, // MIDI=%3d(0x%02X) OCTAVE=%-2d NOTE=%-5s f=%-8.2f (%.2f)\n",
		cw, midi, midi, octave, note, frequency, actual);

	if (midi == MIDI_LAST)
		fprintf(stdout, "};\n");
}

int
main(int argc, char **argv)
{
	for (uint8_t midi = MIDI_FIRST; midi <= MIDI_LAST; ++midi) {
                /* Determine note */
                const char *note = notes[midi % 12];

		/* Determine note frequency */
		double frequency = FREQ_FIRST * pow(2., ((midi - MIDI_FIRST) / 12.));

                /* Determine note octave */
                int8_t octave = (int8_t)(log2(frequency)) - 8;
#if 0
		pic16timer(midi, note, frequency, octave);
#else
		dsPICnco(midi, note, frequency, octave);
#endif
	}
}
