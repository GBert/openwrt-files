/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "k8048.h"

/*
 * Get hex nibble
 */
unsigned char
inhx32_gethexn(char c)
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
 * Get hex byte
 */
unsigned char
inhx32_gethexb(char *s)
{
	if (strlen(s) > 1)
		return inhx32_gethexn(s[0]) << 4 | inhx32_gethexn(s[1]);
	return 0;
}

/*
 * Compare inhx32 data
 */
int
inhx32_compare(const void *d1, const void *d2)
{
	inhx32_data *data1 = (inhx32_data *)(d1);
	inhx32_data *data2 = (inhx32_data *)(d2);

	if (data1->address < data2->address)
		return -1;
	else if (data1->address == data2->address)
		return 0;
	return 1;
}

/*
 * Create ordered list of inhx32 data
 */
void 
inhx32_order(const void *entry, VISIT order, int level) 
{ 
	inhx32_data *inhx32 = *(inhx32_data **)(entry);

	if (order == postorder || order == leaf) 
		inhx32_pdata[inhx32_index++] = inhx32;
} 

/*
 * Format parser
 *
 * Return program data line count
 */
int
inhx32(char *filename)
{
	FILE *f1;
	char *line;
	unsigned int extended_address = 0, bb, ix;
	int n;
	unsigned char tt = TT_DATA, cc;
	unsigned short aaaa;
	inhx32_data *root = NULL, *inhx32;

	inhx32_count = 0;
	inhx32_pdata = NULL;
	
	f1 = fopen(filename, "rb");
	if (f1 == NULL) {
		printf("%s: error: file open failed [%s] [%s]\n", __func__, filename, strerror(errno));
		return -1;
	}
	line = (char *)calloc(STRLEN, sizeof(char));
	if (line == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	while (tt != TT_EOF && fgets(line, STRLEN, f1) != NULL)
	{
		line[STRMAX] = '\0';

		/* Strip CRLF */
		n = strlen(line) - 1;
		while (n >= 0 && (line[n] == '\n' || line[n] == '\r'))
			line[n--] = '\0';

		/* Validate line prefix and length */
		if (line[0] != ':' || (strlen(line) & 1) == 0 || strlen(line) < 11) {
			printf("%s: warning: ignoring malformed line [%s] invalid format\n",
			__func__, line);
			continue;
		}

		/* Validate checksum */
		cc = 0;
		for (n = 1; line[n]; n += 2)
			cc += inhx32_gethexb(&line[n]); 
		if (cc != 0) {
			printf("%s: warning: ignoring malformed line [%s] invalid checksum [%02X]\n",
			__func__, line, cc);
			continue;
		}

		/* Determine number of data bytes in this line */
		bb = inhx32_gethexb(&line[BB]);

		/* Validate line length */
		if (strlen(line) != (2 * bb + 11)) {
			printf("%s: warning: ignoring malformed line [%s] invalid length [%d]\n",
			__func__, line, (int ) strlen(line));
			continue;
		}

		/* Determine data address for this line */
		aaaa = (inhx32_gethexb(&line[AAAA]) << 8) | inhx32_gethexb(&line[AAAA + 2]);

		/* Determine record type */
		tt = inhx32_gethexb(&line[TT]);

		switch (tt) {
		case TT_DATA:   if (bb == 0) {
					printf("%s: warning: ignoring empty line [%s]\n",
					__func__, line);
					break;
				}

				/* Allocate new array entry for this line */
				inhx32 = (inhx32_data *)calloc(1, sizeof(inhx32_data));
				if (inhx32 == NULL) {
					printf("%s: fatal error: calloc failed\n", __func__);
					exit(EX_OSERR); /* Panic */
				}

				/* Increment line counter */
				inhx32_count++;

				/* Save address and word count */
				inhx32->address = extended_address | aaaa;
				inhx32->nbytes = bb;

				/* Extract data */
				ix = HHHH;
				for (n = 0; n < bb; n++) {
					inhx32->bytes[n] = inhx32_gethexb(&line[ix]);
					ix += 2;
				}
				
				/* Find entry in tree */
				if (tfind((void *)(inhx32), (void **)(&root), inhx32_compare) != NULL) {
					printf("%s: fatal error: duplicate address [%08X]\n",
					__func__, inhx32->address);
					exit(EX_SOFTWARE); /* Panic */
				}

				/* Add entry into tree */
				if (tsearch((void *)(inhx32), (void **)(&root), inhx32_compare) == NULL) {
					printf("%s: fatal error: tsearch failed\n", __func__);
					exit(EX_OSERR); /* Panic */
				}
				break;

		case TT_EOF:	break;

		case TT_EXTENDED_LINEAR_ADDRESS:
				if (aaaa == 0 && bb == 2)
					extended_address = (inhx32_gethexb(&line[HHHH]) << 24) | (inhx32_gethexb(&line[HHHH + 2]) << 16);
				else
					printf("%s: warning: ignoring invalid extended linear address [aaaa=%04X, bb=%d]\n", __func__, aaaa, bb);
				break;

		case TT_EXTENDED_SEGMENT_ADDRESS:
				printf("%s: warning: ignoring unhandled extended segment address\n", __func__);
				break;

		case TT_START_LINEAR_ADDRESS:
				printf("%s: warning: ignoring unhandled start linear address\n", __func__);
				break;

		case TT_START_SEGMENT_ADDRESS:
				printf("%s: warning: ignoring unhandled start segment address\n", __func__);
				break;

		default:	printf("%s: warning: ignoring unknown record type [%d]\n", __func__, tt);
				break;
		}
	}
	free(line);
	fclose(f1);

	/* Return error if no program data lines found */
	if (inhx32_count == 0) {
		printf("%s: error: file contains no data records [%s]\n", __func__, filename);
		return -5;
	}
	
	/* Create and populate program line data array from tree */
	inhx32_pdata = (inhx32_data **)calloc(inhx32_count, sizeof(inhx32_data *));
	if (inhx32_pdata == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	inhx32_index = 0;
	twalk((void *)(root), inhx32_order); 

	/* Free tree nodes */
	for (n = 0; n < inhx32_count; n++) {
		if (tdelete((void *)(inhx32_pdata[n]), (void **)(&root), inhx32_compare) == NULL) {
			printf("%s: fatal error: tdelete failed\n", __func__);
			exit(EX_OSERR); /* Panic */
		}
	}
	if (root != NULL) {
		printf("%s: fatal error: tree destroy failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	/* Return the program data line count */
	return inhx32_count;
}
		
/*
 * Free memory used by program data line array
 */
void
inhx32_free()
{
	if (inhx32_count) {
		for (int i = 0; i < inhx32_count; i++)
			free(inhx32_pdata[i]);
		free(inhx32_pdata);
		inhx32_count = 0;
		inhx32_pdata = NULL;
	} else {
		printf("%s: warning: nothing to free\n", __func__);
	}
}
