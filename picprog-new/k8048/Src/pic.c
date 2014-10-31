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
 * DETERMINE ARCH
 *
 *  k12 | k14 | k16 | k24 | k32
 */
uint32_t
pic_arch(struct k8048 *k, const char *execname)
{
#ifdef K12
	if (strcmp(execname, "k12") == 0)
		return pic12_arch(k);	/* 12-bit word PIC10F/PIC12F/PICF */
#endif
#ifdef K14
	if (strcmp(execname, "k14") == 0)
		return pic14_arch(k);	/* 14-bit word PIC10F/PIC12F/PICF */
#endif
#ifdef K16
	if (strcmp(execname, "k16") == 0)
		return pic16_arch(k);	/* 16-bit word PIC18F */
#endif
#ifdef K24
	if (strcmp(execname, "k24") == 0)
		return pic24_arch(k);	/* 24-bit word PIC24/dsPIC */
#endif
#ifdef K32
	if (strcmp(execname, "k32") == 0)
		return pic32_arch(k);	/* 32-bit word PIC32 */
#endif
	return 0;
}

/*
 * COMPARE DEVICE NAMES FOR QSORT
 */
int
pic_cmp(const void *p1, const void *p2)
{
	char *s1 = *(char **)p1, *s2 = *(char **)p2;

	return strcasecmp(s1, s2);
}

/*
 * DUMP DEVICE SELECTION
 */
void
pic_selector(struct k8048 *k)
{
	if (k->pic->selector)
		k->pic->selector();
	else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * BEGIN PROGRAMMING
 */
void
pic_program_begin(struct k8048 *k)
{
	if (k->pic->program_begin)
		k->pic->program_begin(k);
}

/*
 * PROGRAM DATA
 */
uint32_t
pic_program_data(struct k8048 *k, uint32_t current_region, pic_data *pdata)
{
	if (k->pic->program_data)
		return k->pic->program_data(k, current_region, pdata);

	return PIC_REGIONNOTSUP;
}

/*
 * END PROGRAMMING
 */
void pic_program_end(struct k8048 *k, int config)
{
	if (k->pic->program_end)
		k->pic->program_end(k, config);
}

/*
 * BEGIN VERIFY
 */
void
pic_verify_begin(struct k8048 *k)
{
	if (k->pic->verify_begin)
		k->pic->verify_begin(k);
}

/*
 * VERIFY DATA
 */
uint32_t
pic_verify_data(struct k8048 *k, uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	if (k->pic->verify_data)
		return k->pic->verify_data(k, current_region, pdata, fail);

	return PIC_REGIONNOTSUP;
}

/*
 * END VERIFY
 */
void pic_verify_end(struct k8048 *k)
{
	if (k->pic->verify_end)
		k->pic->verify_end(k);
}

/*
 * READ PE FILE
 *
 *  RETURNS BYTE POINTER OR NULL ON ERROR.
 *
 *  WILL ADJUST FILE NAME WHEN ATTEMPTING TO CREATE CACHED HEX FILE (BIN)
 */
uint8_t *
pic_pe_read_file(struct k8048 *k, char *filename, uint32_t *nbytes)
{
	int rc;
	struct stat st;
	uint8_t *pe;
	FILE *fp;
	char *s;

	if (strstr(filename, ".bin")) {
		rc = stat(filename, &st);
		if (rc < 0) {
			printf("%s: error: stat failed [%s]\n", __func__, filename);
			return NULL;
		}
		*nbytes = st.st_size;
		pe = (uint8_t *)calloc(*nbytes, sizeof(uint8_t));
		if (pe == NULL) {
			printf("%s: fatal error: calloc failed\n", __func__);
			io_exit(k, EX_OSERR); /* Panic */
		}
		fp = fopen(filename, "rb");
		if (fp == NULL) {
			printf("%s: error: fopen failed [%s]\n", __func__, filename);
			return NULL;
		}
		rc = fread((void *)pe, 1, *nbytes, fp);
		if (rc != *nbytes) {
			printf("%s: error: fread failed [%s]\n", __func__, filename);
			return NULL;
		}
		fclose(fp);
	} else { /* .hex */
		s = strstr(filename, ".hex");
		if (s == NULL) {
			return NULL;
		}
		*nbytes = inhx32_memory_create(k, &pe, filename);
		if (*nbytes == 0) {
			return NULL;
		}
		strcpy(s, ".bin");
		fp = fopen(filename, "wb");
		if (fp != NULL) {
			rc = fwrite((void *)pe, 1, *nbytes, fp);
			fclose(fp);
			if (rc != *nbytes)
				unlink(filename);
		}
	}
	return pe;
}

/*
 * LOOK UP PE FILE NAME
 */
int
pic_pe_lookup_file(struct k8048 *k, char *pathname, const char *filename)
{
	if (k->etc[0]) {
		snprintf(pathname, STRLEN, "%s/%s", k->etc, filename);
		if (access(pathname, R_OK) == 0)
			return 0;
	}

	char *dotdup = (char *)strdup(k->dotfile);
	if (dotdup == NULL) {
		printf("%s: fatal error: strdup failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}

	char *dname = dirname(dotdup);
	snprintf(pathname, STRLEN, "%s/%s", dname, filename);
	free(dotdup);
	if (access(pathname, R_OK) == 0)
		return 0;

	bzero(pathname, STRLEN);
	return -1;
}

/*
 * LOOK UP PE FILE .BIN OR .HEX
 *
 *  FILE IS PE FILENAME WITHOUT FILE NAME EXTENSION
 */
int
pic_pe_lookup(struct k8048 *k, char *pathname, const char *file)
{
	char filename[STRLEN];

	snprintf(filename, STRLEN, "%s.bin", file);
	if (pic_pe_lookup_file(k, pathname, filename) == 0)
		return 0;

	snprintf(filename, STRLEN, "%s.hex", file);
	return pic_pe_lookup_file(k, pathname, filename);
}

/*
 * READ CONFIG
 *
 *  Return -1 on error, 0 OK
 */
int
pic_read_config(struct k8048 *k)
{
	if (k->pic->read_config_memory)
		return k->pic->read_config_memory(k);

	printf("%s: information: unimplemented\n", __func__);
	return -1;
}

/*
 * DETERMINE PROGRAM FLASH SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_program_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	if (k->pic->get_program_size)
		size = k->pic->get_program_size(addr);
	else
		printf("%s: information: unimplemented\n", __func__);

	return size;
}

/*
 * DETERMINE DATA EEPROM/FLASH SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_data_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	if (k->pic->get_data_size)
		size = k->pic->get_data_size(addr);
	else
		printf("%s: information: unimplemented\n", __func__);

	return size;
}

/*
 * DETERMINE EXECUTIVE SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_executive_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	if (k->pic->get_executive_size)
		size = k->pic->get_executive_size(addr);
	else
		printf("%s: information: unimplemented\n", __func__);

	return size;
}

/*
 * DETERMINE BOOT FLASH SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_boot_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	if (k->pic->get_boot_size)
		size = k->pic->get_boot_size(addr);
	else
		printf("%s: information: unimplemented\n", __func__);

	return size;
}

/*
 * READ PROGRAM FLASH / DATA FLASH MEMORY BLOCK
 *
 *  INVOKE AFTER `pic_get_program_size'
 */
uint32_t
pic_read_program_memory_block(struct k8048 *k, uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t rc = UINT32_MAX;

	if (k->pic->read_program_memory_block)
		rc = k->pic->read_program_memory_block(k, data, addr, size);
	else
		printf("%s: information: unimplemented\n", __func__);

	return rc;
}

/*
 * READ DATA EEPROM/FLASH MEMORY BLOCK 
 *
 *  INVOKE AFTER `pic_get_data_size'
 */
uint32_t
pic_read_data_memory_block(struct k8048 *k, uint16_t *data, uint32_t addr, uint16_t size)
{
	uint32_t rc = UINT32_MAX;

	if (k->pic->read_data_memory_block)
		rc = k->pic->read_data_memory_block(k, data, addr, size);
	else
		printf("%s: information: unimplemented\n", __func__);

	return rc;
}
 
/*
 * BULK ERASE DEVICE
 *
 *  INVOKE AFTER `pic_read_config'
 */
void
pic_bulk_erase(struct k8048 *k)
{
	if (k->pic->bulk_erase) {
		k->pic->bulk_erase(k);
	} else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * PROGRAM FILE
 */
void
pic_program(struct k8048 *k, char *filename, int blank)
{
	uint32_t count = 0, nbytes;
	pic_data **pdata = NULL;
	uint32_t current_region = PIC_REGIONNOTSUP;

	if (!k->pic->program_data || !k->pic->program_end) {
		printf("%s: information: program unimplemented\n", __func__);
		return;
	}
	if (!k->pic->bulk_erase) {
		printf("%s: information: erase unimplemented\n", __func__);
		return;
	}
	if (pic_read_config(k) < 0)
		return;

	if (blank)
		k->pic->bulk_erase(k);

	nbytes = inhx32_array_create(k, &pdata, filename, &count);
	if (nbytes == 0)
		return;

	if (k->pic->program_begin)
		k->pic->program_begin(k);

	for (uint32_t i = 0; i < count; ++i)
		current_region = k->pic->program_data(k, current_region, pdata[i]);

	k->pic->program_end(k, blank);

	printf("Total: %u\n", nbytes);

	inhx32_array_free(k, pdata, count);
}

/*
 * VERIFY FILE
 *
 *  RETURN NUMBER OF VERIFY ERRORS
 */
uint32_t
pic_verify(struct k8048 *k, char *filename)
{
	uint32_t count = 0, nbytes;
	pic_data **pdata = NULL;
	uint16_t current_region = PIC_REGIONNOTSUP;
	uint32_t fail = 0;

	if (!k->pic->verify_data || !k->pic->verify_end) {
		printf("%s: information: verify unimplemented\n", __func__);
		return fail;
	}
	if (pic_read_config(k) < 0)
		return fail;

	nbytes = inhx32_array_create(k, &pdata, filename, &count);
	if (nbytes == 0)
		return fail;

	if (k->pic->verify_begin)
		k->pic->verify_begin(k);

	for (uint32_t i = 0; i < count; ++i)
		current_region = k->pic->verify_data(k, current_region, pdata[i], &fail);

	k->pic->verify_end(k);

	printf("Total: %u Fail: %u\n", nbytes, fail);

	inhx32_array_free(k, pdata, count);

	return fail;
}

/*
 * VIEW FILE
 */
void
pic_view(struct k8048 *k, char *filename)
{
	uint32_t count = 0, nbytes;
	pic_data **pdata = NULL;

	if (!k->pic->view_data) {
		printf("%s: information: view unimplemented\n", __func__);
		return;
	}

	nbytes = inhx32_array_create(k, &pdata, filename, &count);
	if (nbytes == 0)
		return;

	for (uint32_t i = 0; i < count; ++i)
		k->pic->view_data(k, pdata[i]);

	printf("Total: %u\n", nbytes);

	inhx32_array_free(k, pdata, count);
}

/*
 * WRITE BANDGAP CONFIG
 */
void
pic_writebandgap(struct k8048 *k, uint16_t bandgap)
{
	if (k->pic->write_bandgap) {
		if (!pic_read_config(k))
			k->pic->write_bandgap(k, bandgap);
	} else
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
}

/*
 * WRITE OSCILLATOR CALIBRATION
 */
void
pic_writeosccal(struct k8048 *k, uint16_t osccal)
{
	if (k->pic->write_osccal) {
		if (!pic_read_config(k))
			k->pic->write_osccal(k, osccal);
	} else
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
}

/*
 * BLANK DEVICE
 */
void
pic_blank(struct k8048 *k)
{
	if (k->pic->bulk_erase) {
		if (!pic_read_config(k))
			k->pic->bulk_erase(k);
	} else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * ERASE ROW(S)
 */
void
pic_erase(struct k8048 *k, uint32_t row, uint32_t nrows)
{
	if (k->pic->row_erase) {
		if (!pic_read_config(k))
			k->pic->row_erase(k, row, nrows);
	} else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP DEVICE ID
 */
void
pic_dumpdeviceid(struct k8048 *k)
{
	if (k->pic->dumpdeviceid) {
		if (!pic_read_config(k))
			k->pic->dumpdeviceid(k);
	} else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP CONFIGURATION
 */
void
pic_dumpconfig(struct k8048 *k)
{
	if (k->pic->dumpconfig) {
		if (!pic_read_config(k))
			k->pic->dumpconfig(k, PIC_VERBOSE);
	} else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP OSCILLATOR CALIBRATION
 */
void
pic_dumposccal(struct k8048 *k)
{
	if (k->pic->dumposccal) {
		if (!pic_read_config(k))
			k->pic->dumposccal(k);
	} else
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
}

/******************************************************************************
  INHX32 DATA DUMP
 *****************************************************************************/

/*
 * DUMP EXTENDED ADDRESS
 *
 * :BBAAAATTHHHHCC
 *  
 *  BB   02   = NUMBER OF BYTES
 *  AAAA 0000 = ADDRESS (0000)
 *  TT   04   = EXTENDED_LINEAR_ADDRESS
 *  HHHH      = EXTENDED ADDRESS << 16
 *  CC        = CHECKSUM
 *
 *  FLAG=1 UPDATE AND OUTPUT ALWAYS
 *  FLAG=0 UPDATE AND OUTPUT IF CHANGED
 */
void
pic_dumpaddr(uint32_t addr, int flag)
{
	static uint16_t oldaddr = 0;
	uint16_t newaddr = addr >> 16;

	if (flag || (newaddr != oldaddr)) {
		uint8_t hb = newaddr >> 8;
		uint8_t lb = newaddr & 0xFF;
		uint8_t cc = 0x100 - 0x06 - hb - lb;
		printf(":02000004%02X%02X%02X\n", hb, lb, cc);
	}
	oldaddr = newaddr;
}

/*
 * DUMP 8-BIT BYTE AS INHX32 DATA
 */
void
pic_dumpbyte(uint32_t addr, uint8_t byte)
{
	uint8_t cc, hb, lb;
	hb = addr >> 8;
	lb = addr;
	printf(":01%02X%02X00", hb, lb);
	cc = 0x01 + hb + lb + 0x00;
	lb = byte;
	cc = cc + lb;
	printf("%02X%02X\n", lb, (0x100 - cc) & 0xFF);
}

/*
 * DUMP 16-BIT WORD AS INHX32 DATA
 */
void
pic_dumpword16(uint32_t addr, uint16_t word)
{
	uint8_t cc, hb, lb;
	hb = addr >> 7;
	lb = addr << 1;
	printf(":02%02X%02X00", hb, lb);
	cc = 0x02 + hb + lb + 0x00;
	hb = word >> 8;
	lb = word;
	cc = cc + hb + lb;
	printf("%02X%02X%02X\n", lb, hb, (0x100 - cc) & 0xFF);
}

/*
 * DUMP 32-BIT WORD AS INHX32 DATA
 */
void
pic_dumpword32(uint32_t addr, uint32_t word)
{
	uint8_t cc, b3, b2, b1, b0;
	b1 = addr >> 7;
	b0 = addr << 1;
	printf(":04%02X%02X00", b1, b0);
	cc = 0x04 + b1 + b0 + 0x00;
	b3 = word >> 24;
	b2 = word >> 16;
	b1 = word >> 8;
	b0 = word;
	cc = cc + b3 + b2 + b1 + b0;
	printf("%02X%02X%02X%02X%02X\n", b0, b1, b2, b3, (0x0100 - cc) & 0xFF);
}

/*
 * DUMP DEVICE AS INHX32 DATA
 */
void
pic_dumpdevice(struct k8048 *k)
{
	uint32_t addr, size;

	/* Get userid/config */
	if (pic_read_config(k) < 0)
		return;

	/* Program flash */
	if (k->pic->get_program_size) {
		/* Get program flash size */
		size = pic_get_program_size(k, &addr);
		if (size == UINT32_MAX) {
			printf("%s: fatal error: program flash size invalid\n", __func__);
			io_exit(k, EX_SOFTWARE); /* Panic */
		}
		/* Dump program flash */
		if (size)
			pic_dump_program(k, addr, size, PIC_INHX32);
	}

	/* Boot flash */
	if (k->pic->get_boot_size) {
		/* Get boot flash size */
		size = pic_get_boot_size(k, &addr);
		if (size == UINT32_MAX) {
			printf("%s: fatal error: program flash size invalid\n", __func__);
			io_exit(k, EX_SOFTWARE); /* Panic */
		}
		/* Dump boot flash */
		if (size)
			pic_dump_program(k, addr, size, PIC_INHX32);
	}

	/* Data EEPROM/flash */
	if (k->pic->get_data_size) {
		/* Get data EEPROM/flash size */
		size = pic_get_data_size(k, &addr);
		if (size == UINT32_MAX) {
			printf("%s: fatal error: data EEPROM/flash size invalid\n", __func__);
			io_exit(k, EX_SOFTWARE); /* Panic */
		}
		/* Dump data EEPROM/flash */
		if (size)
			pic_dump_data(k, addr, size, PIC_INHX32);
	}

	/* Userid/Config */
	if (k->pic->dumpdevice) {
		k->pic->dumpdevice(k);
	}

	/* EOF */
	printf(":00000001FF\n");
}

/******************************************************************************
  HEXADECIMAL DATA DUMP
 *****************************************************************************/

/*
 * DUMP ADDRESS/SIZE ADJUST
 */
int
pic_dumpadj(struct k8048 *k, uint32_t *baddr, uint32_t *bsize, uint32_t naddr, uint32_t nwords)
{
	if (naddr != UINT32_MAX) {
		uint32_t laddr = *baddr + (*bsize * k->pic->dumpadj);
		if (naddr >= *baddr && naddr < laddr) {
			*bsize -= (naddr - *baddr) / k->pic->dumpadj;
			*baddr = naddr;
		} else if (naddr != *baddr) {
			printf("%s: information: address invalid\n", __func__);
			return -1;
		}
	}
	if (nwords != UINT32_MAX) {
		if (nwords < *bsize)
			*bsize = nwords;
	}
	return 0;
}

/*
 * DUMP PROGRAM FLASH IN HEX
 */
void
pic_dumpprogram(struct k8048 *k, uint32_t a, uint32_t n)
{
	uint32_t addr, size;

	if (!k->pic->get_program_size) {
		printf("%s: information: program flash is not supported on this architecture\n", __func__);
		return;
	}

	if (pic_read_config(k) < 0)
		return;

	/* Get program flash size */
	size = pic_get_program_size(k, &addr);
	if (size == UINT32_MAX) {
		printf("%s: fatal error: program flash size invalid\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (size == 0) {
		printf("%s: information: program flash is not supported on this device\n", __func__);
		return;
	}
	/* Adjust address and size */
	if (pic_dumpadj(k, &addr, &size, a, n))
		return;
	/* Dump program flash */
	pic_dump_program(k, addr, size, PIC_HEXDEC);
}

/*
 * DUMP DATA EEPROM/FLASH IN HEX
 */
void
pic_dumpdata(struct k8048 *k)
{
	uint32_t addr, size;

	if (!k->pic->get_data_size) {
		printf("%s: information: data EEPROM/flash is not supported on this architecture\n", __func__);
		return;
	}

	if (pic_read_config(k) < 0)
		return;

	/* Get data EEPROM/data flash size */
	size = pic_get_data_size(k, &addr);
	if (size == UINT32_MAX) {
		printf("%s: fatal error: data EEPROM/flash size invalid\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (size == 0) {
		printf("%s: information: data EEPROM/flash is not supported on this device\n", __func__);
		return;
	}
	/* Dump data EEPROM/flash */
	pic_dump_data(k, addr, size, PIC_HEXDEC);
}

/*
 * DUMP EXECUTIVE FLASH IN HEX
 */
void
pic_dumpexec(struct k8048 *k, uint32_t a, uint32_t n)
{
	uint32_t addr, size;

	if (!k->pic->get_executive_size) {
		printf("%s: information: EXECUTIVE flash is not supported on this architecture\n", __func__);
		return;
	}

	if (pic_read_config(k) < 0)
		return;

	/* Get program executive size */
	size = pic_get_executive_size(k, &addr);
	if (size == UINT32_MAX) {
		printf("%s: fatal error: EXECUTIVE flash size invalid\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (size == 0) {
		printf("%s: information: EXECUTIVE flash is not supported on this device\n", __func__);
		return;
	}
	/* Adjust address and size */
	if (pic_dumpadj(k, &addr, &size, a, n))
		return;
	/* Dump program executive */
	pic_dump_program(k, addr, size, PIC_HEXDEC);
}

/*
 * DUMP BOOT FLASH IN HEX
 */
void
pic_dumpboot(struct k8048 *k, uint32_t a, uint32_t n)
{
	uint32_t addr, size;

	if (!k->pic->get_boot_size) {
		printf("%s: information: BOOT flash is not supported on this architecture\n", __func__);
		return;
	}

	if (pic_read_config(k) < 0)
		return;

	/* Get boot flash size */
	size = pic_get_boot_size(k, &addr);
	if (size == UINT32_MAX) {
		printf("%s: fatal error: BOOT flash size invalid\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (size == 0) {
		printf("%s: information: BOOT flash is not supported on this device\n", __func__);
		return;
	}
	/* Adjust address and size */
	if (pic_dumpadj(k, &addr, &size, a, n))
		return;
	/* Dump boot flash */
	pic_dump_program(k, addr, size, PIC_HEXDEC);
}

/******************************************************************************
  DUMP HEX / INHX32 PROGRAM AND DATA
 *****************************************************************************/

/*
 * DUMP PROGRAM FLASH
 */
void
pic_dump_program(struct k8048 *k, uint32_t addr, uint32_t size, int mode)
{
	uint32_t data_size = sizeof(uint32_t) * (size + 16);

	/* Allocate program array */
	uint32_t *data = (uint32_t *)malloc(data_size);
	if (data == NULL) {
		printf("%s: fatal error: malloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}
	memset((void *)data, -1, data_size);
	/* Read program */
	if (pic_read_program_memory_block(k, data, addr, size) == UINT32_MAX) {
		printf("%s: fatal error: program flash read failed\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	/* Dump */
	if (mode == PIC_HEXDEC) {
		pic_dumphexcode(k, addr, size, data);
	} else if (mode == PIC_INHX32) {
		pic_dumpinhxcode(k, addr, size, data);
	}
	free(data);
}

/*
 * DETECT EMPTY PROGRAM FLASH ROW
 */
int
pic_mtcode(uint32_t compar, uint32_t size, uint32_t *data)
{
	uint32_t mtrow = 0;

	for (uint32_t j = 0; j < size; ++j) {
		if (data[j] == compar)
			mtrow++;
	}
	return (mtrow == size);
}

/*
 * DUMP HEX PROGRAM FLASH WORDS
 */
void
pic_dumphexcode(struct k8048 *k, uint32_t addr, uint32_t size, uint32_t *data)
{
	if (k->pic->dumphexcode)
		k->pic->dumphexcode(k, addr, size, data);
	else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP INHX32 PROGRAM FLASH WORDS
 */
void
pic_dumpinhxcode(struct k8048 *k, uint32_t addr, uint32_t size, uint32_t *data)
{
	if (k->pic->dumpinhxcode)
		k->pic->dumpinhxcode(k, addr, size, data);
	else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP DATA EEPROM/FLASH
 */
void
pic_dump_data(struct k8048 *k, uint32_t addr, uint32_t size, int mode)
{
	uint32_t data_size = sizeof(uint16_t) * (size + 16);

	/* Allocate data EEPROM/flash array */
	uint16_t *data = (uint16_t *)malloc(data_size);
	if (data == NULL) {
		printf("%s: fatal error: malloc failed\n", __func__);
		io_exit(k, EX_OSERR); /* Panic */
	}
	memset((void *)data, -1, data_size);
	/* Read data EEPROM/flash */
	if (pic_read_data_memory_block(k, data, addr, size) == UINT32_MAX) {
		printf("%s: fatal error: data EEPROM/flash read failed\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	/* Dump */
	if (mode == PIC_HEXDEC) {
		pic_dumphexdata(k, addr, size, data);
	} else if (mode == PIC_INHX32) {
		pic_dumpinhxdata(k, addr, size, data);
	}
	free(data);
}

/*
 * DETECT EMPTY DATA EEPROM/FLASH ROW
 */
int
pic_mtdata(uint16_t compar, uint32_t size, uint16_t *data)
{
	uint32_t mtrow = 0;

	for (uint32_t j = 0; j < size; ++j) {
		if (data[j] == compar)
			mtrow++;
	}
	return (mtrow == size);
}

/*
 * DUMP HEX DATA EEPROM/FLASH BYTES/WORDS
 */
void
pic_dumphexdata(struct k8048 *k, uint32_t addr, uint32_t size, uint16_t *data)
{
	if (k->pic->dumphexdata)
		k->pic->dumphexdata(k, addr, size, data);
	else
		printf("%s: information: unimplemented\n", __func__);
}

/*
 * DUMP INHX32 DATA EEPROM/FLASH BYTES/WORDS
 */
void
pic_dumpinhxdata(struct k8048 *k, uint32_t addr, uint32_t size, uint16_t *data)
{
	if (k->pic->dumpinhxdata)
		k->pic->dumpinhxdata(k, addr, size, data);
	else
		printf("%s: information: unimplemented\n", __func__);
}

/******************************************************************************
  PANEL WRITING
 *****************************************************************************/

/*
 * WRITE PANEL
 *
 *  mode:
 *  	PIC_PANEL_BEGIN
 *  		data1: 	memory region
 *  		data2:	memory size
 *
 *  	PIC_PANEL_UPDATE
 *  		data1:	memory address
 *  		data2:	memory data
 *
 *  	PIC_PANEL_END
 *  		data1:	void
 *  		data2:	void
 */
void
pic_write_panel(struct k8048 *k, int mode, uint32_t data1, uint32_t data2)
{
	static uint32_t write_pending = 0;
	static uint32_t panel_region = PIC_REGIONNOTSUP;
	static uint32_t panel_address = 0;
	static uint32_t *panel = NULL;
	static uint32_t panel_size = 0;

	if (k->pic->write_panel == NULL) {
		printf("%s: fatal error: write panel unimplemented\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
	if (mode == PIC_PANEL_BEGIN || mode == PIC_PANEL_END) {
		if (panel) {
			if (write_pending) {
				write_pending = 0;
				k->pic->write_panel(k, panel_region, panel_address, panel, panel_size);
			}
			free(panel);
			panel_region = PIC_REGIONNOTSUP;
			panel_address = 0;
			panel = NULL;
			panel_size = 0;
		}
	}
	if (mode == PIC_PANEL_BEGIN) {
		panel_region = data1;		/* MEMORY REGION */
		panel_size = data2;		/* MEMORY SIZE   */
	}
	if (mode == PIC_PANEL_BEGIN || mode == PIC_PANEL_UPDATE) {
		if (panel == NULL) {
			if (panel_size == 0) {
				printf("%s: fatal error: zero sized panel\n", __func__);
				io_exit(k, EX_SOFTWARE); /* Panic */
			}
			panel = malloc(sizeof(uint32_t) * panel_size);
			if (panel == NULL) {
				printf("%s: fatal error: malloc failed\n", __func__);
				io_exit(k, EX_SOFTWARE); /* Panic */
			}
			memset((void *)panel, -1, sizeof(uint32_t) * panel_size);
		}
	}
	if (mode == PIC_PANEL_UPDATE) {
		uint32_t address, new_address, boundary, mask;

		boundary = 0 - panel_size;
		mask = panel_size - 1;
		address = data1;		/* MEMORY ADDRESS */
		new_address = address & boundary;
		if (new_address != panel_address) {
			if (write_pending) {
				write_pending = 0;
				k->pic->write_panel(k, panel_region, panel_address, panel, panel_size);
				memset((void *)panel, -1, sizeof(uint32_t) * panel_size);
			}
			panel_address = new_address;
		}
		panel[address & mask] = data2;	/* MEMORY DATA */
		write_pending++;
	}
}
