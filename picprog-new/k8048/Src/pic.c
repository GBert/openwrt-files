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
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DETERMINE PROGRAM FLASH SIZE
 *
 * INVOKE AFTER `pic_read_config'
 */
int
pic_get_program_flash_size(struct k8048 *k)
{
	int size = -1;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_program_flash_size();
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_program_flash_size();
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_program_flash_size();
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
 * INVOKE AFTER `pic_read_config'
 */
int
pic_get_data_flash_size(struct k8048 *k)
{
	int size = -1;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_data_flash_size();
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_data_flash_size();
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_data_flash_size();
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
 * INVOKE AFTER `pic_read_config'
 */
int
pic_get_data_eeprom_size(struct k8048 *k)
{
	int size = -1;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		size = pic12_get_data_eeprom_size();
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		size = pic14_get_data_eeprom_size();
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		size = pic16_get_data_eeprom_size();
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
 * INVOKE AFTER `pic_get_program_size'
 *
 * RETURN PROGRAM BASE ADDRESS (>= 0) OR ERROR (-1)
 */
int
pic_read_flash_memory_block(struct k8048 *k, unsigned short *data, int max)
{
	int addr = -1;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		addr = pic12_read_flash_memory_block(k, data, max);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		addr = pic14_read_flash_memory_block(k, data, max);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		addr = pic16_read_flash_memory_block(k, data, max);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return addr;
}

/*
 * READ DATA EEPROM MEMORY BLOCK 
 *
 * INVOKE AFTER `pic_get_data_size'
 *
 * RETURN EEPROM BASE ADDRESS (>= 0) OR ERROR (-1)
 */
int
pic_read_eeprom_memory_block(struct k8048 *k, unsigned char *data, int max)
{
	int addr = -1;

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: EEPROM is not supported on this device\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		addr = pic14_read_eeprom_memory_block(k, data, max);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		addr = pic16_read_eeprom_memory_block(k, data, max);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
	return addr;
}
 
/*
 * PROGRAM
 */
void
pic_program(struct k8048 *k, const char *filename, int blank)
{
	inhx32(filename);
	if (inhx32_count <= 0) {
		return;
	}

	pic_read_config(k, CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_program(k, blank);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_program(k, blank);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_program(k, blank);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	inhx32_free();
}

/*
 * VERIFY DEVICE
 *
 * RETURN NUMBER OF VERIFY ERRORS
 */
int
pic_verify(struct k8048 *k, const char *filename)
{
	int fail = 0;
	
	inhx32(filename);
	if (inhx32_count <= 0) {
		return 0;
	}

	pic_read_config(k, CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		fail = pic12_verify(k);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		fail = pic14_verify(k);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		fail = pic16_verify(k);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	inhx32_free();

	return fail;
}

/*
 * BLANK A DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic_blank(struct k8048 *k)
{
	pic_read_config(k, CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_bulk_erase(k, INTERNAL);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_bulk_erase(k, INTERNAL, INTERNAL);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_bulk_erase(k);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * ERASE A ROW
 */
void
pic_erase(struct k8048 *k, int row)
{
	pic_read_config(k, CONFIG_ONLY);

	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		printf("%s: information: unsupported\n", __func__);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		printf("%s: information: unimplemented\n", __func__);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		pic16_row_erase(k, row);
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
	pic_read_config(k, CONFIG_ALL);

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
	int mode = VERBOSE;

	pic_read_config(k, CONFIG_ONLY);

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
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * WRITE BANDGAP CONFIG
 */
void
pic_writebandgap(struct k8048 *k, unsigned short bandgap)
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
		pic14_bulk_erase(k, INTERNAL, bandgap);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		printf("%s: information: BANDGAP is not supported on this architecture\n", __func__);
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
		pic12_read_config_memory(k, CONFIG_ALL);
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
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * WRITE OSCILLATOR CALIBRATION
 */
void
pic_writeosccal(struct k8048 *k, unsigned short osccal)
{
	switch (k->arch) {
#ifdef K12
	case ARCH12BIT:
		pic12_read_config_memory(k, CONFIG_ONLY);
		pic12_bulk_erase(k, osccal);
		break;
#endif
#ifdef K14
	case ARCH14BIT:
		pic14_read_config_memory(k);
		pic14_bulk_erase(k, osccal, INTERNAL);
		break;
#endif
#ifdef K16
	case ARCH16BIT:
		printf("%s: information: OSCCAL is not supported on this architecture\n", __func__);
		break;
#endif
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}
}

/*
 * DUMP BYTE AS INHX32 DATA
 */
void
pic_dumpbyte(struct k8048 *k, unsigned short address, unsigned char byte)
{
	unsigned char cc, hb, lb;

	if (k->debug >= 10) {
		printf("%s: information: address=%04X byte=%02X\n",
			__func__, address, byte);
	}
	hb = address >> 8;
	lb = address;
	printf(":01%02X%02X00", hb, lb);
	cc = 0x01 + hb + lb + 0x00;
	lb = byte;
	cc = cc + lb;
	printf("%02X%02X\n", lb, (0x100 - cc) & 0xFF);
}

/*
 * DUMP WORD AS INHX32 DATA
 */
void
pic_dumpword(struct k8048 *k, unsigned short address, unsigned short word)
{
	unsigned char cc, hb, lb;

	if (k->debug >= 10) {
		printf("%s: information: address=%04X word=%04X\n",
			__func__, address, word);
	}
	hb = address >> 7;
	lb = address << 1;
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
	int size;

	/* Get userid/config */
	pic_read_config(k, CONFIG_ALL);

	/* Extended address = 0x0000 */
	printf(":020000040000FA\n");

	/* Get program/data flash size */
	size = pic_get_data_flash_size(k);
	if (size > 0) {
		size += pic_get_program_flash_size(k);
		/* Dump program + data flash */
		pic_dump_program_flash(k, size, size, INHX32);
	} else {
		/* Dump program flash */
		size = pic_get_program_flash_size(k);
		pic_dump_program_flash(k, size, size, INHX32);
	}

	/* Get data EEPROM size */
	size = pic_get_data_eeprom_size(k);
	if (size > 0) {
		/* Dump data EEPROM */
		pic_dump_data_eeprom(k, size, INHX32);
	}

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
	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	/* EOF */
	printf(":00000001FF\n");
}

/*
 * DUMP PROGRAM AND DATA FLASH IN HEX
 */
void
pic_dumpflash(struct k8048 *k, int max)
{
	int size;

	pic_read_config(k, CONFIG_ONLY);

	/* Get program/data flash size */
	size = pic_get_data_flash_size(k);
	if (size > 0) {
		size += pic_get_program_flash_size(k);
	} else {
		size = pic_get_program_flash_size(k);
	}

	/* Adjust max size if necessary */
	if (max <= 0 || max > size) {
		max = size;
	}

	pic_dump_program_flash(k, max, size, HEXDEC);
}


/*
 * DUMP DATA EEPROM IN HEX
 */
void
pic_dumpeeprom(struct k8048 *k)
{
	int size;

	pic_read_config(k, CONFIG_ONLY);

	/* Get data EEPROM size */
	size = pic_get_data_eeprom_size(k);
	if (size <= 0) {
		printf("%s: information: EEPROM is not supported on this device\n",
			__func__);
		return;
	}

	pic_dump_data_eeprom(k, size, HEXDEC);
}

/*
 * DUMP MAX WORDS OF PROGRAM FLASH
 *
 * MODE MAY BE 'HEXDEC' OR 'INHX32'
 */
void
pic_dump_program_flash(struct k8048 *k, int max, int capacity, int mode)
{
	int i, lines = 0, addr = 0, baseaddr;
	unsigned short *code;

	/* Program code data */
	code = (unsigned short *)calloc(CODEWIDTH + max, sizeof(unsigned short));
	if (code == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	/* Read data */
	baseaddr = pic_read_flash_memory_block(k, code, max);
	if (baseaddr < 0) {
		free(code);
		return;
	}

	/* Dump data */
	while (addr < max) {
		/* Detect blank row */
		int skip = 0;
		for (i = 0; i < CODEWIDTH; i++) {
			/* arch = mask */
			if (code[addr + i] == k->arch)
				skip++;
		}
		/* Output row if not blank */
		if (skip < CODEWIDTH) {
			lines++;

			/* Intel hexadecimal output */
			if (mode == INHX32) {
				unsigned char cc, hb, lb;
				if (k->arch == ARCH16BIT) {
					hb = baseaddr >> 8;
					lb = baseaddr;
				} else {
					hb = baseaddr >> 7;
					lb = baseaddr << 1;
				}
				printf(":%02X%02X%02X00", CODEWIDTH * 2, hb, lb);
				cc = CODEWIDTH * 2 + hb + lb + 0x00;
				for (i = 0; i < CODEWIDTH; i++) {
					lb = code[addr + i];
					hb = code[addr + i] >> 8;
					printf("%02X%02X", lb, hb);
					cc = cc + lb + hb;
				}
				printf("%02X\n", (0x0100 - cc) & 0xFF);
			}

			/* Standard hexadecimal output */
			else {
				if (capacity < 0x10000)
					printf("[%04X] ", baseaddr);
				else
					printf("[%05X] ", baseaddr);
				for (i = 0; i < CODEWIDTH; i++)
					printf("%04X ", code[addr + i]);
				printf("\n");
			}

		}
		/* Move to next row */
		addr += CODEWIDTH;
		baseaddr += CODEWIDTH;
		if (k->arch == ARCH16BIT)
			baseaddr += CODEWIDTH;
	}
	/* Dump footer */
	if (mode == HEXDEC && lines == 0) {
		printf("%s: information: program flash empty\n", __func__);
	}
	free(code);
}

/*
 * DUMP MAX WORDS OF DATA MEMORY EEPROM
 *
 * MODE MAY BE 'HEXDEC' OR 'INHX32'
 */
void
pic_dump_data_eeprom(struct k8048 *k, int max, int mode)
{
	int i, lines = 0, header = 0, datawidth = DATAWIDTH, addr = 0, baseaddr;
	unsigned char ch, *data;

	if (mode == INHX32 && k->arch != ARCH16BIT) {
		datawidth /= 2;
	}
	unsigned char chars[datawidth + 1];

	/* Eeprom data */
	data = (unsigned char *)calloc(datawidth + max, sizeof(unsigned char));
	if (data == NULL) {
		printf("%s: fatal error: calloc failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	/* Data as chars [0 .. datawidth - 1] */
	chars[datawidth] = '\0';

	/* Read data */
	baseaddr = pic_read_eeprom_memory_block(k, data, max);
	if (baseaddr < 0) {
		free(data);
		return;
	}

	/* Dump data */
	while (addr < max) {
		/* Detect blank row */
		int skip = 0;
		for (i = 0; i < datawidth; i++) {
			/* arch = mask */
			if (data[addr + i] == 0xFF)
				skip++;
		}
		/* Output row if not blank */
		if (skip < datawidth) {
			lines++;

			/* Intel hexadecimal output */
			if (mode == INHX32) {
				unsigned char cc, hb, lb;
				if (k->arch == ARCH16BIT) {
					if (header == 0) {
						/* EEPROM: Extended address = 0x00f0 */
						printf(":0200000400F00A\n");
						header = 1;
					}
					hb = baseaddr >> 8;
					lb = baseaddr;
					printf(":%02X%02X%02X00", datawidth, hb, lb);
					cc = datawidth + hb + lb + 0x00;
					for (i = 0; i < datawidth; i++) {
						lb = data[addr + i];
						printf("%02X", lb);
						cc = cc + lb;
					}
				} else {
					hb = baseaddr >> 7;
					lb = baseaddr << 1;
					printf(":%02X%02X%02X00", datawidth * 2, hb, lb);
					cc = datawidth * 2 + hb + lb + 0x00;
					for (i = 0; i < datawidth; i++) {
						lb = data[addr + i];
						printf("%02X00", lb);
						cc = cc + lb + 0x00;
					}
				}
				printf("%02X\n", (0x0100 - cc) & 0xFF);
			}

			/* Standard hexadecimal output */
			else {
				/* Display data and store data as chars */
				printf("[%04X] ", baseaddr);
				for (i = 0; i < datawidth; i++) {
					ch = data[addr + i];
					if (ch >= ' ' && ch < 127)
						chars[i] = ch;
					else
						chars[i] = '.';
					printf("%02X ", ch);
				}
				/* Display data as chars */
				printf("%s\n", chars);
			}

		}
		/* Move to next row */
		addr += datawidth;
		baseaddr += datawidth;
	}
	/* Dump footer */
	if (mode == HEXDEC && lines == 0) {
		printf("%s: information: data eeprom empty\n", __func__);
	}
	free(data);
}
