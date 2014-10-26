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
 *
 * Intel hex 32 parser
 *
 *  Two algorithms exist for parsing input. Quick sort or tree sort.
 *
 *  Tree sort is the original algorithm and quick sort a new addition that
 *  in relatively untested (2014-10-19).
 *
 *  Selection of which algorithm to use is done in config.mk
 *
 *****************************************************************************/

/*
 * Get line
 *
 * Reads input line and updates the byte count and address fields.
 *
 * The record type field (*tt) is also updated as:
 *
 *	TT_EOF		EOF record
 *	TT_DATA		Data record
 *	~TT_DATA	Not a data record (255)
 */
char *
inhx32_fgets(struct k8048 *k, char *line, FILE *fp,
	uint8_t *bb, uint16_t *aaaa, uint8_t *tt, uint16_t *extended_addr)
{
	size_t len;
	uint8_t cc, _tt;
	uint16_t i;

	*bb = 0;
	*aaaa = 0;
	*tt = ~TT_DATA; /* Not a data record (255) */

	/* Get line */
	if (fgets(line, STRLEN, fp) == NULL)
		return NULL; /* EOF */

	/* Remove CRLF */
	rmcrlf(line, STRLEN);

	/* Validate line prefix */
	if (line[0] != ':')
		return line;

	/* Validate line length */
	len = strlen(line);
	if ((len & 1) == 0 || len < 11)
		return line;

	/* Validate checksum */
	cc = 0;
	for (i = 1; line[i]; i += 2)
		cc += hex2byte(&line[i]); 
	if (cc != 0)
		return line;

	/* Determine number of bytes in this line */
	*bb = hex2byte(&line[BB]);
#if 0
	/* Validate number of bytes */
	if (*bb > PIC_BYTLEN)
		return line;
#endif
	/* Validate line length */
	if (len != (2 * *bb + 11))
		return line;

	/* Determine address for this line */
	*aaaa = (hex2byte(&line[AAAA]) << 8) |
		hex2byte(&line[AAAA + 2]);

	/* Determine record type */
	_tt = hex2byte(&line[TT]);

	/* Process data record */
	if (_tt == TT_DATA) {
		/* Validate line length */
		if (*bb == 0)
			return line;

		/* Validate alignment */
		if (k->pic && (*bb % k->pic->align))
			return line;
	}
	/* Process extended address record */
	else if (_tt == TT_EXTENDED_LINEAR_ADDRESS) {
		/* Validate extended address */
		if (*aaaa != 0 || *bb != 2)
			return line;

		/* Determine extended address */
		*extended_addr = (hex2byte(&line[HHHH]) << 8) |
				hex2byte(&line[HHHH + 2]);
		return line;
	}
	/* Ignore other records */
	else
		return line;

	/* Return data line */
	*tt = TT_DATA;

	return line;
}

/*
 * Get data
 *  
 *  Returns with either a data record or NULL for EOF
 */
pic_data *
inhx32_get(struct k8048 *k, FILE *fp)
{
	char line[STRLEN];
	uint8_t bb, tt;
	uint16_t aaaa, ix, i;
	static uint16_t extended_addr = 0;
	pic_data *data;

	do {
		/* Get line */
		if (inhx32_fgets(k, line, fp, &bb, &aaaa, &tt, &extended_addr) == NULL)
			return NULL;	/* EOF */
		if (tt == TT_EOF)
			return NULL;	/* EOF */
	}
	while (tt != TT_DATA);		/* Not a data record */

	/* Allocate data */
	data = (pic_data *)calloc(1, sizeof(pic_data));
	if (data == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	/* Store address and number of bytes */
	data->address = (extended_addr << 16) | aaaa;
	data->nbytes = bb;

	/* Extract and store bytes */
	ix = HHHH;
	for (i = 0; i < bb; ++i) {
		data->bytes[i] = hex2byte(&line[ix]);
		ix += 2;
	}

	/* Return data */
	return data;
}

/******************************************************************************
 *
 * Qsort allocator
 *
 *****************************************************************************/
#ifdef QSORT
/*
 * Stage 1: Input & Rewind
 *
 *  Read and copy `stdin' to a temporary file or read input file.
 *
 *  During read, determine the total number of hex data bytes and
 *  data line count.
 *
 *  Rewind input.
 *
 * Stage 2: Input & Alloc
 *
 *  Allocate array of pointers to data records.
 *
 *  Re-read input populating data records array.
 *
 *  Quick sort data records array.
 */

/*
 * Stage 1: Input & Rewind
 *
 *  filename should either be `-' for `stdin' else an accessible
 *  disk file.
 *
 *  tempname contains the temporary file template on entry and any
 *  temporary file name on return.
 */
uint32_t
inhx32_qsort_input(struct k8048 *k, FILE **fp, const char *filename, char *tempname, uint32_t *count)
{
	char line[STRLEN];
	uint8_t bb, tt;
	uint16_t dummy;;

	FILE *ftemp = NULL;
	int rc;
	
	uint32_t nbytes = 0;
	*count = 0;

	/* Open file or stdin */
	if (strcmp(filename, "-") == 0) {
		*fp = stdin;
		rc = mkstemp(tempname);
		if (rc < 0) {
			tempname[0] = '\0';
			printf("%s: error: mkstemp failed [%s]\n",
				__func__, tempname);
			return 0; /* Empty */
		}
		ftemp = fdopen(rc, "rb+");
		if (ftemp == NULL) {
			close(rc);
			unlink(tempname);
			tempname[0] = '\0';
			printf("%s: error: fdopen failed [%s]\n",
				__func__, tempname);
			return 0; /* Empty */
		}
	} else {
		*fp = fopen(filename, "rb");
		if (*fp == NULL) {
			printf("%s: error: fopen failed [%s]\n",
				__func__, filename);
			return 0; /* Empty */
		}
		tempname[0] = '\0';
	}

	/* While not EOF, process data */
	while (inhx32_fgets(k, line, *fp, &bb, &dummy, &tt, &dummy) != NULL) {
		if (tt == TT_DATA) {
			nbytes += bb;
			(*count)++;
		}
		if (ftemp)
			fprintf(ftemp, "%s\n", line);
	}

	/* Rewind input */
	if (ftemp)
		*fp = ftemp;
	if (fseek(*fp, 0L, SEEK_SET) < 0) {
		fclose(*fp);
		if (tempname[0]) {
			unlink(tempname);
			tempname[0] = '\0';
		}
		printf("%s: error: fseek failed [%s] [%s]\n",
			__func__, filename, tempname);
		return 0; /* Empty */
	}

	/* Information */
	if (nbytes == 0) {
		fclose(*fp);
		printf("%s: information: no valid input data\n", __func__);
		if (tempname[0]) {
			unlink(tempname);
			tempname[0] = '\0';
		}
	}

	/* Return number of bytes */
	return nbytes;
}

/******************************************************************************
 *
 * Qsort array allocator
 *
 *****************************************************************************/

/*
 * Compare entries
 */
int
inhx32_array_compare(const void *d1, const void *d2)
{
	pic_data *data1 = *(pic_data **)d1;
	pic_data *data2 = *(pic_data **)d2;

	if (data1->address < data2->address)
		return -1;
	else if (data1->address == data2->address)
		return 0;
	return 1;
}

/*
 * Allocate array
 */
uint32_t
inhx32_array_create(struct k8048 *k, pic_data ***pdata, const char *filename, uint32_t *count)
{
	FILE *fp;
	char tempname[] = "/tmp/inhx32.XXXXXX";
	uint32_t nbytes, index = 0;
	pic_data *data;

	*count = 0;
	
	/*
	 * Stage 1.
	 */

	/* Determine number of bytes and line count */
	nbytes = inhx32_qsort_input(k, &fp, filename, tempname, count);
	if (nbytes == 0)
		return 0; /* Empty */

	/*
	 * Stage 2.
	 */

	/* Allocate array */
	*pdata = (pic_data **)calloc(*count, sizeof(pic_data *));
	if (*pdata == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	/* While not EOF, process data */
	while ((data = inhx32_get(k, fp))) {
		(*pdata)[index++] = data;
	}
	fclose(fp);

	/* Remove any temporary file created in stage 1 */
	if (tempname[0])
		unlink(tempname);

	/* Quick sort array */
	qsort(*pdata, *count, sizeof(pic_data *), inhx32_array_compare);

	/* Return number of bytes */
	return nbytes;
}

/*
 * Free array
 */
void
inhx32_array_free(struct k8048 *k, pic_data **pdata, uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i) {
		free(pdata[i]);
	}
	if (count) {
		free(pdata);
	}
}

/******************************************************************************
 *
 * Qsort memory allocator (PIC32 PE)
 *
 *****************************************************************************/
#ifdef K32
/*
 * Allocate memory
 */
uint32_t
inhx32_memory_create(struct k8048 *k, uint8_t **pmem, const char *filename)
{
	FILE *fp;
	char tempname[] = "/tmp/inhx32.XXXXXX";
	uint32_t nbytes, count = 0, index = 0;
	pic_data *data;
	uint8_t i;

	/*
	 * Stage 1.
	 */

	/* Determine number of bytes and line count */
	nbytes = inhx32_qsort_input(k, &fp, filename, tempname, &count);
	if (nbytes == 0)
		return 0; /* Empty */

	/*
	 * Stage 2.
	 */

	/* Allocate memory */
	*pmem = (uint8_t *)calloc(nbytes, sizeof(uint8_t));
	if (*pmem == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	/* While not EOF, process data */
	while ((data = inhx32_get(k, fp))) {
		for (i = 0; i < data->nbytes; ++i)
			(*pmem)[index++] = data->bytes[i];
	}
	fclose(fp);

	/* Remove any temporary file created in stage 1 */
	if (tempname[0])
		unlink(tempname);

	/* Return number of bytes */
	return nbytes;
}
#endif /* K32   */
#endif /* QSORT */
/******************************************************************************
 *
 * Tree allocator
 *
 *****************************************************************************/
#ifndef QSORT
/*
 * Stage 1: Input
 *
 *  Read input file creating binary tree.
 *
 *  During read, determine the total number of hex data bytes and
 *  data line count.
 *
 *  Close input.
 *
 * Stage 2: Alloc
 *
 *  Allocate array of pointers to data records.
 *
 *  Walk tree populating data records array.
 */

/*
 * Compare entries
 */
int
inhx32_tree_compare(const void *d1, const void *d2)
{
	pic_data *data1 = (pic_data *)d1;
	pic_data *data2 = (pic_data *)d2;

	if (data1->address < data2->address)
		return -1;
	else if (data1->address == data2->address)
		return 0;
	return 1;
}

/*
 * Stage 1: Input
 *
 *  filename should either be `-' for `stdin' else an accessible
 *  disk file.
 */
uint32_t
inhx32_tree_input(struct k8048 *k, void **root, const char *filename, uint32_t *count)
{
	FILE *fp = stdin;
	uint32_t nbytes = 0;
	void *entry;
	pic_data *data;
	
	*count = 0;

	/* Open file or stdin */
	if (strcmp(filename, "-") && (fp = fopen(filename, "rb")) == NULL) {
		printf("%s: error: fopen failed [%s] [%s]\n",
			__func__, filename, strerror(errno));
		return 0; /* Empty */
	}

	/* While not EOF, process data */
	while ((data = inhx32_get(k, fp)) != NULL) {
		/* Detect duplicate data in tree */
		entry = tfind((void *)data, root, inhx32_tree_compare);
		if (entry != NULL) {
			printf("%s: fatal error: duplicate address [%08X]\n",
				__func__, data->address);
			io_exit(k, EX_SOFTWARE); /* Panic */
		}

		/* Add new data to tree */
		entry = tsearch((void *)data, root, inhx32_tree_compare);
		if (entry == NULL) {
			printf("%s: fatal error: tsearch failed\n", __func__);
			io_exit(k, EX_OSERR); /* Panic */
		}

		/* Update counters */
		(*count)++;
		nbytes += data->nbytes;
	}

	/* Close file or stdin */
	if (strcmp(filename, "-"))
		fclose(fp);

	/* Information */
	if (nbytes == 0)
		printf("%s: information: no valid input data\n", __func__);

	/* Return number of bytes */
	return nbytes;
}

/*
 * Match entries
 */
int
inhx32_tree_match(const void *d1, const void *d2)
{
	return 0;
}

/*
 * Free tree (entries and/or data)
 */
void
inhx32_tree_free(struct k8048 *k, void **root, int flag)
{
	void *entry;
	pic_data *data;

	while ((entry = tfind(NULL, root, inhx32_tree_match))) {
		data = *(pic_data **)entry;
		tdelete(data, root, inhx32_tree_match);
		if (flag)
			free(data);
	}
	if (*root != NULL) {
		printf("%s: fatal error: tree free failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}
}

/******************************************************************************
 *
 * Tree array allocator
 *
 *****************************************************************************/

/*
 * `twalk' array
 */
static pic_data **inhx32_pdata;

/*
 * `twalk' index
 */
static uint32_t inhx32_index;

/*
 * Walk array
 */
void 
inhx32_array_walk(const void *entry, VISIT order, int level) 
{ 
	pic_data *data = *(pic_data **)entry;

	if (order == postorder || order == leaf) {
		inhx32_pdata[inhx32_index++] = data;
	}
} 

/*
 * Allocate array
 */
uint32_t
inhx32_array_create(struct k8048 *k, pic_data ***pdata, const char *filename, uint32_t *count)
{
	uint32_t nbytes;
	void *root = NULL;

	/*
	 * Stage 1.
	 */

	/* Create tree */
	nbytes = inhx32_tree_input(k, &root, filename, count);
	if (nbytes == 0)
		return 0; /* Empty */

	/*
	 * Stage 2.
	 */

	/* Allocate array */
	*pdata = (pic_data **)calloc(*count, sizeof(pic_data *));
	if (*pdata == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	/* Populate array */
	inhx32_pdata = *pdata;
	inhx32_index = 0;
	twalk(root, inhx32_array_walk);

	/* Free tree (entries, not data) */
	inhx32_tree_free(k, &root, 0);

	/* Return number of bytes */
	return nbytes;
}
		
/*
 * Free array
 */
void
inhx32_array_free(struct k8048 *k, pic_data **pdata, uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i) {
		free(pdata[i]);
	}
	if (count) {
		free(pdata);
	}
}

/******************************************************************************
 *
 * Tree memory allocator (PIC32 PE)
 *
 *****************************************************************************/
#ifdef K32
/*
 * `twalk' memory
 */
static uint8_t *inhx32_pmem;

/*
 * Walk memory
 */
void 
inhx32_memory_walk(const void *entry, VISIT order, int level) 
{ 
	pic_data *data = *(pic_data **)entry;
	uint8_t i;

	if (order == postorder || order == leaf) {
		for (i = 0; i < data->nbytes; ++i)
			inhx32_pmem[inhx32_index++] = data->bytes[i];
	}
} 

/*
 * Allocate memory
 */
uint32_t
inhx32_memory_create(struct k8048 *k, uint8_t **pmem, const char *filename)
{
	uint32_t nbytes, count = 0;
	void *root = NULL;

	/*
	 * Stage 1.
	 */

	/* Create tree */
	nbytes = inhx32_tree_input(k, &root, filename, &count);
	if (nbytes == 0)
		return 0; /* Empty */

	/*
	 * Stage 2.
	 */

	/* Allocate memory */
	*pmem = (uint8_t *)calloc(nbytes, sizeof(uint8_t));
	if (*pmem == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	/* Populate memory */
	inhx32_pmem = *pmem;
	inhx32_index = 0;
	twalk(root, inhx32_memory_walk);

	/* Free tree (entries and data) */
	inhx32_tree_free(k, &root, 1);

	/* Return number of bytes */
	return nbytes;
}
#endif /* K32   */
#endif /* !QSORT */
