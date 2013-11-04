/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _INHX32_H
#define _INHX32_H

/*
 * intel hex format 32 definitions
 */
#define BB (1)
#define AAAA (3)
#define TT (7)
#define HHHH (9)
#define TT_DATA (0)
#define TT_EOF (1)
#define TT_EXTENDED_SEGMENT_ADDRESS (2)
#define TT_START_SEGMENT_ADDRESS (3)
#define TT_EXTENDED_LINEAR_ADDRESS (4)
#define TT_START_LINEAR_ADDRESS (5)

/*
 * a program data line
 */
#define BYTESMAX (256)
typedef struct
{
	unsigned int address;
	unsigned short bytes[BYTESMAX];
	unsigned short nbytes;
} inhx32_data;

/*
 * program data lines array (ordered)
 */
inhx32_data **inhx32_pdata;

/*
 * index used to build array (twalk)
 */
int inhx32_index;

/*
 * count of items in array
 */
int inhx32_count;

/*
 * prototypes
 */
unsigned char inhx32_gethexn(char);
unsigned char inhx32_gethexb(char *);
int inhx32_compare(const void *, const void *);
void inhx32_order(const void *, VISIT, int); 
int inhx32(const char *);
void inhx32_free(void);

#endif /* !_INHX32_H */
