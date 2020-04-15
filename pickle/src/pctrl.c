/*
 * Copyright (C) 2005-2019 Darron Broad
 * All rights reserved.
 *
 * This file is part of Pickle Microchip PIC ICSP.
 *
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 *
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "VERSION"

#include "pctrl.h"

/*
 * pctrl session
 */
struct pickle p = {0};

static void
usage(char *execname, char *msg)
{
	printf("USAGE: %s RUN|STOP|RESTORE [BEFORE][AFTER][DURING]\n", execname);
	printf("Control master clear.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n"
		" pctrl RUN 100\n"
		"\t\tWait 100ms then raise master clear to take the device out of reset.\n"
		" pctrl STOP 0 100\n"
		"\t\tLower master clear to put the device in reset then wait 100ms.\n"
		" pctrl RESTORE 0 0 500\n"
		"\t\tLower master clear for 500ms before raising to reset the device.\n"

		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}

int
main(int argc, char *argv[])
{
	char *execdup = NULL, *execname = NULL;
	uint32_t before = 0, after = 0, during = 10000;

	/* Get exec name */
	execdup = (char *)strdup(argv[0]);
	if (execdup == NULL) {
		printf("%s: fatal error: strdup failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}
	execname = basename(execdup);
	if (execname == NULL) {
		printf("%s: fatal error: basename failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}

	/* Get configuration */
	getconf();

	/* Determine back-end */
	if (io_backend() == 0)
		usage(execname, "Unsupported backend device in config. Run `pickle` to list");

	/* Open device */
	if (io_open() < 0)
		usage(execname, io_error());

	/* Reset uid */
	if (getuid() != geteuid()) {
		if (setuid(getuid()) < 0) {
			printf("%s: fatal error: setuid failed\n", __func__);
			io_exit(EX_OSERR); /* Panic */
		}
	}

	/* Get args */
	if (argc < 2)
		usage(execname, "Missing arg");
	if (argc > 2)
		before = 1000 * strtoul(argv[2], NULL, 0);
	if (argc > 3)
		after = 1000 * strtoul(argv[3], NULL, 0);
	if (argc > 4)
		during = 1000 * strtoul(argv[4], NULL, 0);
	if (argc > 5)
		usage(execname, "Too many args");

	io_usleep(before);

	/* RUN/STOP RESTORE */
	if (strcasecmp(argv[1], "RUN") == 0) {
		io_close(HIGH);
		printf("RUN\n");
	} else if (strcasecmp(argv[1], "STOP") == 0) {
		io_close(LOW);
		printf("STOP\n");
	} else if (strcasecmp(argv[1], "RESTORE") == 0) {
		io_set_vpp(LOW);
		io_usleep(during);
		io_close(HIGH);
		printf("RESTORE\n");
	} else {
		io_close(HIGH);
		printf("RUN [DEFAULT]\n");
	}

	io_usleep(after);

	io_exit(EX_OK);
}
