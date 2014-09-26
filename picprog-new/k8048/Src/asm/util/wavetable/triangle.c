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

#include "triangle.h"

/* 1 quarter 10-bit triangle table */
#define TABLERES (10)
#define TABLESIZE (1024)
#define TABLE4TH (TABLESIZE / 4)

int
main(void)
{
	uint16_t x;
	int16_t y;
	
	printf("/* Triangle wave table */\n"
	"const fractional triangletable[0x%04X] = {\n", TABLE4TH);

	for (x = 0; x < TABLE4TH;) {

		y = round((double)(x) / TABLE4TH * INT16_MAX);

		if (++x % 8) {
			printf("%6d,", y);
		} else {
			printf("%6d,\n", y);
		}
	}
	printf("};\n\n"

	"/* Triangle wave look-up */\n"
	"static inline fractional\n"
	"trianglelookup(uint16_t a)\n"
	"{\n"
	"\tregister uint16_t x, y;\n\n"

	"\tx = a >> %d;\n"
	"\ty = x & 0x%04X;\n\n"

	"\tif (x < 0x%04X)\n"
	"\t\treturn triangletable[y];\n"
	"\tif (x < 0x%04X)\n"
	"\t\treturn triangletable[0x%04X - y];\n"
	"\tif (x < 0x%04X)\n"
	"\t\treturn -triangletable[y];\n"
	"\treturn -triangletable[0x%04X - y];\n"
	"}\n", 16 - TABLERES, TABLE4TH - 1, TABLE4TH, TABLE4TH * 2, TABLE4TH - 1, TABLE4TH * 3, TABLE4TH - 1);

	return 0;
}
