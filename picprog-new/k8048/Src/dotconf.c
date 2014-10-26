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

/*
 * Dot path
 */
int
getdotpath(struct k8048 *k)
{
	struct stat st;

	if (stat(k->dotfile, &st) < 0) {
		if (errno == ENOENT)
			return -1;
		printf("%s: fatal error: stat failed [%s]\n", __func__, k->dotfile);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (S_ISREG(st.st_mode)) {
		return access(k->dotfile, R_OK);
	}
	if (S_ISDIR(st.st_mode)) {
		char *dotdup = (char *)strdup(k->dotfile);
		if (dotdup == NULL) {
			printf("%s: fatal error: strdup failed\n", __func__);
			io_exit(k, EX_OSERR); /* Panic */
		}
		snprintf(k->dotfile, STRLEN, "%s/%s", dotdup, DOTCONFIGNAME);
		free(dotdup);
		return access(k->dotfile, R_OK);
	}
	return -1;
}

/*
 * Dot file
 */
void
getdotfile(struct k8048 *k)
{
	char *filename = getenv("K8048");
	if (filename != NULL) {
		snprintf(k->dotfile, STRLEN, "%s", filename);
		if (getdotpath(k) == 0)
			return;
	}

	char dir[STRLEN];
	if (getcwd(dir, STRLEN) == NULL) {
		printf("%s: fatal error: getcwd failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}
	snprintf(k->dotfile, STRLEN, "%s/%s", dir, DOTFILENAME);
	if (getdotpath(k) == 0)
		return;

	char *homedir = getenv("HOME");
	if (homedir != NULL) {
		snprintf(k->dotfile, STRLEN, "%s/%s", homedir, DOTFILENAME);
		if (getdotpath(k) == 0)
			return;
	}

	char *username = getenv("USER");
	if (username != NULL) {
		snprintf(k->dotfile, STRLEN, "/home/%s/%s", username, DOTFILENAME);
		if (getdotpath(k) == 0)
			return;
	}

	/* No dot file found */
	k->dotfile[0] = '\0';
}

/*
 * Configuration
 */
void
getconf(struct k8048 *k)
{
	FILE *f1;
	char line[STRLEN];

	/* Configure defaults */
	io_config(k);

	/* Determine dot file name */
	getdotfile(k);

	/* Load dot file when available */
	if (k->dotfile[0] && (f1 = fopen(k->dotfile, "rb")) != NULL) {
		while (fgets(line, STRLEN, f1) != NULL) {
			/* Remove CRLF */
			rmcrlf(line, STRLEN);

			if (mystrcasestr(line, "DEVICE=") == line) {
				strncpy(k->device, &line[7], STRLEN);
			}
#ifdef TTY
			else if (mystrcasestr(line, "BAUDRATE=") == line) {
				k->baudrate = strtoul(&line[9], NULL, 0);
			}
#endif
			else if (mystrcasestr(line, "SLEEP=") == line) {
				k->sleep_low = strtoul(&line[6], NULL, 0);
				k->sleep_high = k->sleep_low;
			}
			else if (mystrcasestr(line, "SLEEP_LOW=") == line) {
				k->sleep_low = strtoul(&line[10], NULL, 0);
			}
			else if (mystrcasestr(line, "SLEEP_HIGH=") == line) {
				k->sleep_high = strtoul(&line[11], NULL, 0);
			}
			else if (mystrcasestr(line, "BITRULES=") == line) {
				k->bitrules = strtoul(&line[9], NULL, 0);
			}
			else if (mystrcasestr(line, "BUSY=") == line) {
				k->busy = strtoul(&line[5], NULL, 0);
			}
#if defined(RPI) || defined(BITBANG)
			else if (mystrcasestr(line, "VPP=") == line) {
				k->vpp = strtoul(&line[4], NULL, 0);
			}
			else if (mystrcasestr(line, "PGM=") == line) {
				k->pgm = strtoul(&line[4], NULL, 0);
			}
			else if (mystrcasestr(line, "PGC=") == line) {
				k->pgc = strtoul(&line[4], NULL, 0);
			}
			else if (mystrcasestr(line, "PGD=") == line) {
				k->pgdi = strtoul(&line[4], NULL, 0);
				k->pgdo = k->pgdi;
			}
			else if (mystrcasestr(line, "PGDO=") == line) {
				k->pgdo = strtoul(&line[5], NULL, 0);
			}
			else if (mystrcasestr(line, "PGDI=") == line) {
				k->pgdi = strtoul(&line[5], NULL, 0);
			}
#endif /* RPI || BITBANG */

#ifdef MCP23017
			else if (mystrcasestr(line, "MCP=") == line) {
				k->mcp = strtoul(&line[4], NULL, 0);
			}
#endif /* MCP23017 */
			else if (mystrcasestr(line, "FWSLEEP=") == line) {
				k->fwsleep = strtoul(&line[8], NULL, 0);
			}
			else if (mystrcasestr(line, "DEBUG=") == line) {
				k->debug = strtoul(&line[6], NULL, 0);
			}
			else if (mystrcasestr(line, "ETC=") == line) {
				strncpy(k->etc, &line[4], STRLEN);
			}
		}
		fclose(f1);
	} else { /* We are using the defaults */
		snprintf(k->dotfile, STRLEN, "Using defaults (override in %s)", DOTFILENAME);
	}
}
