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

#include "k8048.h"

/******************************************************************************
 * UTILITY FUNCTIONS
 *****************************************************************************/

/*
 * are you sure?
 */
int
areyousure(const char *s)
{
	int c;

	printf("%s: are you sure [y/N]? ", s);
	c = fgetc(stdin);
	if (c == 'y' || c == 'Y')
		return 1;

	return 0;
}

/*
 * strcasestr implementation for cygwin
 */
const char *
mystrcasestr(const char *haystack, const char *needle)
{
	int i, j;
	int nlen = strlen(needle);
	int hlen = strlen(haystack) - nlen;

	for (i = 0; i <= hlen; i++) {
		for (j = 0; j < nlen; j++) {
			if (tolower((int)haystack[i + j]) != tolower((int)needle[j]))
				break;
		}
		if (j == nlen) {
			return &haystack[i];
		}
	}
	return NULL;
}

/*
 * return the value of a hex nibble
 */
uint8_t
hex2nibble(const char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

/*
 * return the value of a hex byte
 */
uint8_t
hex2byte(const char *s)
{
	if (s[0] && s[1])
		return hex2nibble(s[0]) << 4 | hex2nibble(s[1]);
	return 0;
}

/*
 * return string with CRLF removed from end
 */
char *
rmcrlf(char *s, int slen)
{
	int l;

	s[slen - 1] = '\0';

	l = strlen(s) - 1;

	while (l >= 0 && (s[l]=='\r' || s[l]=='\n'))
		s[l--]= '\0';

	return s;
}
