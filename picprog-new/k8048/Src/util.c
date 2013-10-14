/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "k8048.h"

/******************************************************************************
 * UTILITY FUNCTIONS
 *****************************************************************************/

/*
 * Are you sure?
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
 * Get quoted command line argument
 */
#if 0
char *
getarg(const char *line)
{
	int len = strlen(line);
	if (len == 0)
		return NULL;

	char *arg = calloc(1 + len, sizeof(char));
	if (arg == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	int i = 0;
	while (line[i] && line[i] == ' ')
		i++;

	char q;
	if (line[i] == '\'' || line[i] == '"')
		q = line[i++];
	else
		q = ' ';

	int j = 0;
	while (line[i] && line[i] != q)
		arg[j++] = line[i++];

	if (j == 0) {
		free(arg);
		return NULL;
	}
	arg[j] = '\0';

	return arg;
}
#endif
