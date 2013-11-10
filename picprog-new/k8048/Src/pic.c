/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "k8048.h"

#ifdef DEBUG
void
pic_x(struct k8048 *k)
{
	pic_dump_program_flash(k, PIC24_EXEC_LOW, 1024, PIC_INHX32);
}
#endif

/*
 * COMPARE DEVICE NAMES FOR QSORT
 */
#if 0
int
pic_cmpval(char *s)
{
	int t = (s[3] - '0') * 10 + (s[4] - '0'), u = 0;

	for (int i = 5; s[i]; ++i) {
		if (isdigit(s[i])) {
			u = u * 10 + (s[i] - '0');
		}
	}
	return (t << 16) + u;
}
int
pic_cmp(const void *p1, const void *p2)
{
	char *s1 = *(char **)p1, *s2 = *(char **)p2;
	int n1 = pic_cmpval(s1), n2 = pic_cmpval(s2);

	if (n1 == n2)
		return 0;
	if (n1 < n2)
		return -1;
	return 1;
}
#else
int
pic_cmp(const void *p1, const void *p2)
{
	char *s1 = *(char **)p1, *s2 = *(char **)p2;

	return strcmp(s1, s2);
}
#endif

/*
 * DUMP DEVICE SELECTION
 */
void
pic_selector(struct k8048 *k)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_selector(k);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_selector(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_selector(k);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_selector(k);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * READ CONFIG
 */
void
pic_read_config(struct k8048 *k, int flag)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_read_config_memory(k, flag);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_read_config_memory(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_read_config_memory(k);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_read_config_memory(k);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DETERMINE PROGRAM FLASH SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_program_flash_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_program_flash_size(addr);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_program_flash_size(addr);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_program_flash_size(addr);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		size = pic24_get_program_flash_size(addr);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return size;
}

/*
 * DETERMINE DATA FLASH SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_data_flash_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_data_flash_size(addr);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_data_flash_size(addr);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_data_flash_size(addr);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		size = pic24_get_data_flash_size(addr);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return size;
}

/*
 * DETERMINE DATA EEPROM SIZE
 *
 *  INVOKE AFTER `pic_read_config'
 */
uint32_t
pic_get_data_eeprom_size(struct k8048 *k, uint32_t *addr)
{
	uint32_t size = UINT32_MAX;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_data_eeprom_size(addr);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_data_eeprom_size(addr);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_data_eeprom_size(addr);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		size = pic24_get_data_eeprom_size(addr);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
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

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_executive_size(addr);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_executive_size(addr);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_executive_size(addr);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		size = pic24_get_executive_size(addr);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return size;
}

/*
 * READ PROGRAM FLASH / DATA FLASH MEMORY BLOCK
 *
 *  INVOKE AFTER `pic_get_program_size'
 */
uint32_t
pic_read_flash_memory_block(struct k8048 *k, uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t rc = UINT32_MAX;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		rc = pic12_read_flash_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		rc = pic14_read_flash_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		rc = pic16_read_flash_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		rc = pic24_read_flash_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return rc;
}

/*
 * READ DATA EEPROM MEMORY BLOCK 
 *
 *  INVOKE AFTER `pic_get_data_size'
 */
uint32_t
pic_read_eeprom_memory_block(struct k8048 *k, uint8_t *data, uint32_t addr, uint16_t size)
{
	uint32_t rc = UINT32_MAX;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: EEPROM is not supported on this device\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		rc = pic14_read_eeprom_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		rc = pic16_read_eeprom_memory_block(k, data, addr, size);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return rc;
}
 
/*
 * PROGRAM
 */
void
pic_program(struct k8048 *k, char *filename, int blank)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_program(k, filename, blank);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_program(k, filename, blank);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_program(k, filename, blank);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_program(k, filename, blank);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * VERIFY DEVICE
 *
 *  RETURN NUMBER OF VERIFY ERRORS
 */
uint32_t
pic_verify(struct k8048 *k, char *filename)
{
	uint32_t fail = UINT32_MAX;
	
	pic_read_config(k, PIC_CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		fail = pic12_verify(k, filename);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		fail = pic14_verify(k, filename);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		fail = pic16_verify(k, filename);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		fail = pic24_verify(k, filename);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	return fail;
}

/*
 * WRITE BANDGAP CONFIG
 */
void
pic_writebandgap(struct k8048 *k, uint16_t bandgap)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_read_config_memory(k);
		pic14_bulk_erase(k, PIC_INTERNAL, bandgap);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * WRITE OSCILLATOR CALIBRATION
 */
void
pic_writeosccal(struct k8048 *k, uint16_t osccal)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_read_config_memory(k, PIC_CONFIG_ONLY);
		pic12_bulk_erase(k, osccal);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_read_config_memory(k);
		pic14_bulk_erase(k, osccal, PIC_INTERNAL);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * BLANK A DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic_blank(struct k8048 *k)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_bulk_erase(k, PIC_INTERNAL);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_bulk_erase(k, PIC_INTERNAL, PIC_INTERNAL);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_bulk_erase(k);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_bulk_erase(k);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * ERASE ROW(S)
 */
void
pic_erase(struct k8048 *k, uint32_t row, uint32_t nrows)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: unsupported\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_row_erase(k, row, nrows);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_row_erase(k, row, nrows);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP DEVICE ID
 */
void
pic_dumpdeviceid(struct k8048 *k)
{
	pic_read_config(k, PIC_CONFIG_ALL);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_dumpdeviceid(k);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumpdeviceid(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumpdeviceid(k);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_dumpdeviceid(k);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP CONFIGURATION
 */
void
pic_dumpconfig(struct k8048 *k)
{
	int mode = PIC_VERBOSE;

	pic_read_config(k, PIC_CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_dumpconfig(k, mode);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumpconfig(k, mode);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumpconfig(k, mode);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_dumpconfig(k, mode);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP OSCILLATOR CALIBRATION
 */
void
pic_dumposccal(struct k8048 *k)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_read_config_memory(k, PIC_CONFIG_ALL);
		pic12_dumposccal(k);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_read_config_memory(k);
		pic14_dumposccal(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
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
pic_dumpword(uint32_t addr, uint16_t word)
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
 * DUMP DEVICE AS INHX32 DATA
 */
void
pic_dumpdevice(struct k8048 *k)
{
	/* Get userid/config */
	pic_read_config(k, PIC_CONFIG_ALL);

	/* Get program flash size */
	uint32_t faddr, fsize = pic_get_program_flash_size(k, &faddr);
	if (fsize == 0 || fsize == UINT32_MAX) {
		printf("%s: fatal error: program flash size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Get data flash size */
	uint32_t daddr, dsize = pic_get_data_flash_size(k, &daddr);
	if (dsize == UINT32_MAX) {
		printf("%s: fatal error: data flash size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Dump program + data flash */
	fsize += dsize;
	pic_dump_program_flash(k, faddr, fsize, PIC_INHX32);

	/* Get data EEPROM size */
	dsize = pic_get_data_eeprom_size(k, &daddr);
	if (dsize == UINT32_MAX) {
		printf("%s: fatal error: data EEPROM size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Dump data EEPROM */
	if (dsize)
		pic_dump_data_eeprom(k, daddr, dsize, PIC_INHX32);

	/* Dump userid/config */
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_dumpdevice(k);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumpdevice(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumpdevice(k);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_dumpdevice(k);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		break;
#endif
	default:break;
	}

	/* EOF */
	printf(":00000001FF\n");
}

/******************************************************************************
  HEXADECIMAL DATA DUMP
 *****************************************************************************/

/*
 * DUMP PROGRAM AND DATA FLASH IN HEX
 */
void
pic_dumpflash(struct k8048 *k, uint32_t size)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	/* Get program flash size */
	uint32_t faddr, fsize = pic_get_program_flash_size(k, &faddr);
	if (fsize == 0 || fsize == UINT32_MAX) {
		printf("%s: fatal error: program flash size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Get data flash size */
	uint32_t daddr, dsize = pic_get_data_flash_size(k, &daddr);
	if (dsize == UINT32_MAX) {
		printf("%s: fatal error: data flash size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Dump program + data flash */
	fsize += dsize;
	if (size == 0 || size > fsize)
		size = fsize;
	pic_dump_program_flash(k, faddr, size, PIC_HEXDEC);
}

/*
 * DUMP DATA EEPROM IN HEX
 */
void
pic_dumpeeprom(struct k8048 *k)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	/* Get data EEPROM size */
	uint32_t daddr, dsize = pic_get_data_eeprom_size(k, &daddr);
	if (dsize == UINT32_MAX) {
		printf("%s: fatal error: data EEPROM size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	if (dsize == 0) {
		printf("%s: information: EEPROM is not supported on this device\n", __func__);
		return;
	}
	/* Dump data EEPROM */
	pic_dump_data_eeprom(k, daddr, dsize, PIC_HEXDEC);
}

/*
 * DUMP EXECUTIVE IN HEX
 */
void
pic_dumpexec(struct k8048 *k)
{
	pic_read_config(k, PIC_CONFIG_ONLY);

	/* Get EXECUTIVE size */
	uint32_t eaddr, esize = pic_get_executive_size(k, &eaddr);
	if (esize == UINT32_MAX) {
		printf("%s: fatal error: EXECUTIVE size invalid\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	if (esize == 0) {
		printf("%s: information: EXECUTIVE is not supported on this device\n", __func__);
		return;
	}
	/* Dump program executive */
	pic_dump_program_flash(k, eaddr, esize, PIC_HEXDEC);
}

/******************************************************************************
  DUMP HEX / INHX32 PROGRAM AND DATA
 *****************************************************************************/

/*
 * DUMP PROGRAM FLASH
 */
void
pic_dump_program_flash(struct k8048 *k, uint32_t addr, uint32_t size, int mode)
{
	/* Allocate program array */
	uint32_t *data = (uint32_t *)calloc(size + 16, sizeof(uint32_t));
	if (data == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	/* Read program */
	if (pic_read_flash_memory_block(k, data, addr, size) == UINT32_MAX) {
		printf("%s: fatal error: program flash read failed\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Dump */
	if (mode == PIC_HEXDEC) {
		pic_dumphexwords(k, addr, size, data);
	} else if (mode == PIC_INHX32) {
		pic_dumpinhxwords(k, addr, size, data);
	}
	free(data);
}

/*
 * DETECT EMPTY FLASH ROW
 */
int
pic_mtwords(uint32_t compar, uint32_t size, uint32_t *data)
{
	uint32_t mtrow = 0;

	for (uint32_t j = 0; j < size; ++j) {
		if (data[j] == compar)
			mtrow++;
	}
	return (mtrow == size);
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic_dumphexwords(struct k8048 *k, uint32_t addr, uint32_t size, uint32_t *data)
{
	/* Dump words */
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_dumphexwords(k, addr, size, data);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumphexwords(k, addr, size, data);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumphexwords(k, addr, size, data);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_dumphexwords(k, addr, size, data);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic_dumpinhxwords(struct k8048 *k, uint32_t addr, uint32_t size, uint32_t *data)
{
	/* Dump words */
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_dumpinhxwords(k, addr, size, data);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumpinhxwords(k, addr, size, data);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumpinhxwords(k, addr, size, data);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		pic24_dumpinhxwords(k, addr, size, data);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		break;
#endif
	default:break;
	}
}

/*
 * DUMP DATA EEPROM
 */
void
pic_dump_data_eeprom(struct k8048 *k, uint32_t addr, uint32_t size, int mode)
{
	/* Allocate EEPROM array */
	uint8_t *data = (uint8_t *)calloc(size + 16, sizeof(uint8_t));
	if (data == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	/* Read EEPROM */
	if (pic_read_eeprom_memory_block(k, data, addr, size) == UINT32_MAX) {
		printf("%s: fatal error: data EEPROM read failed\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	/* Dump */
	if (mode == PIC_HEXDEC) {
		pic_dumphexbytes(k, addr, size, data);
	} else if (mode == PIC_INHX32) {
		pic_dumpinhxbytes(k, addr, size, data);
	}
	free(data);
}

/*
 * DETECT EMPTY EEPROM ROW
 */
int
pic_mtbytes(uint32_t compar, uint32_t size, uint8_t *data)
{
	uint32_t mtrow = 0;

	for (uint32_t j = 0; j < size; ++j) {
		if (data[j] == compar)
			mtrow++;
	}
	return (mtrow == size);
}

/*
 * DUMP HEX EEPROM BYTES
 */
void
pic_dumphexbytes(struct k8048 *k, uint32_t addr, uint32_t size, uint8_t *data)
{
	/* Dump bytes */
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumphexbytes(k, addr, size, data);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumphexbytes(k, addr, size, data);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP INHX32 EEPROM BYTES
 */
void
pic_dumpinhxbytes(struct k8048 *k, uint32_t addr, uint32_t size, uint8_t *data)
{
	/* Dump bytes */
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_dumpinhxbytes(k, addr, size, data);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_dumpinhxbytes(k, addr, size, data);
		break;
#endif
#ifdef K24
	case ARCH24BIT:
		break;
#endif
#ifdef K32
	case ARCH32BIT:
		break;
#endif
	default:break;
	}
}
