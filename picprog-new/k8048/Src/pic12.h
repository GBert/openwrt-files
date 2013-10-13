/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.12 (12-bit word architecture)
 */

#ifndef _PIC12_H
#define _PIC12_H

/* 12-bit configuration */
#define PIC12_CONFIG_USERID0  (0) 	/* PIC16F505: 0x0400 */
#define PIC12_CONFIG_USERID1  (1) 	/* PIC16F505: 0x0401 */
#define PIC12_CONFIG_USERID2  (2)	/* PIC16F505: 0x0402 */
#define PIC12_CONFIG_USERID3  (3) 	/* PIC16F505: 0x0403 */
#define PIC12_CONFIG_OSCCAL_BACKUP (4) 	/* PIC16F505: 0x0404 */
/* OSCCAL BACKUP 1..3 are documented but not used */
#define PIC12_CONFIG_OSCCAL_BACKUP0 (4)	/* PIC12F519: 0x0444 */
#define PIC12_CONFIG_OSCCAL_BACKUP1 (5)	/* PIC12F519: 0x0445 */
#define PIC12_CONFIG_OSCCAL_BACKUP2 (6)	/* PIC12F519: 0x0446 */
#define PIC12_CONFIG_OSCCAL_BACKUP3 (7)	/* PIC12F519: 0x0447 */
/* */
#define PIC12_CONFIG_OSCCAL_RESET (8)	/* PIC16F505: 0x03FF */
#define PIC12_CONFIG_WORD (9)  		/* PIC16F505: 0x07FF */
#define PIC12_CONFIG_SIZE (10) 
struct pic12_config {
	unsigned short index[PIC12_CONFIG_SIZE];
};

#define PIC12_TPROG_DEFAULT (2000)     /*   2ms DS41226G-page 18 */
#define PIC12_TERASE_DEFAULT (10000)   /*  10ms DS41226G-page 18 */
#define PIC12_TDISCHARGE_DEFAULT (100) /* 100us DS41226G-page 18 */
 
/* 12-bit word devices have no embedded device id */

/* DS41228F PIC10F200/202/204/206 */
#define DS41228F (41228)
#define PIC10F200 (10200)
#define PIC10F202 (10202)
#define PIC10F204 (10204)
#define PIC10F206 (10206)

/* DS41227E PIC12F508/509 */
#define DS41227E (41227)
#define PIC12F508 (12508)
#define PIC12F509 (12509)

/* DS41207D PIC16F54 */
#define DS41207D (41207)
#define PIC16F54 (1654)

/* DS41208C PIC16F57 */
#define DS41208C (41208)
#define PIC16F57 (1657)

/* DS41243B PIC16F59 */
#define DS41243B (41243)
#define PIC16F59 (1659)

/* DS41226G PIC16F505 */
#define DS41226G (41226)
#define PIC16F505 (16505)

/* DS41258C PIC16F506 */
#define DS41258C (41258)
#define PIC16F506 (16506)

/* DS41257B PIC12F510 */
#define DS41257B (41257)
#define PIC12F510 (12510)

/* DS41266C PIC12F220/222 */
#define DS41266C (41266)
#define PIC10F220 (10220)
#define PIC10F222 (10222)

/* DS41316C PIC12F519 */
#define DS41316C (41316)
#define PIC12F519 (12519)

/* KB to number of words */
#define WORD12(X) (((X) * 8192.0) / 12)

/* device to datasheet map */
struct pic12_dsmap
{
	char devicename[STRLEN];
	unsigned short deviceid;	/* derived from part number */
	int flash;                      /* program flash size in words */
	int data;                       /* data flash size in bytes 0=none */
	unsigned int datasheet;
	unsigned short backupaddr;	/* osccal backup address. unsupported = 0 */
	unsigned short configaddr;
	unsigned char latches;
};

/* regions (program/config) */
#define PIC12_REGIONUNKNOWN (0)
#define PIC12_REGIONCODE (1)
#define PIC12_REGIONCONFIG (2)

/* device memory map */
#define PIC12_CODE_LOW (0x0000)
#define PIC12_CODE_HIGH (0x07FF) /* 2K words (3KB) is the largest flash size available */
#define PIC12_CONFIG (0x0FFF) /* default config address */

void pic12_selector(struct k8048 *);

unsigned short pic12_read_program_memory_increment(struct k8048 *);

void pic12_read_config_word(struct k8048 *);
#define CONFIG_ONLY (0)
#define CONFIG_ALL (1)
void pic12_read_config_memory(struct k8048 *, int);

void pic12_bulk_erase(struct k8048 *, unsigned short);

int pic12_get_program_flash_size(void);
int pic12_get_data_flash_size(void);
int pic12_get_data_eeprom_size(void);
unsigned short pic12_read_flash_memory_block(struct k8048 *, unsigned short *, int);
unsigned short pic12_read_eeprom_memory_block(struct k8048 *, unsigned short *, int);

unsigned short pic12_read_osccal(struct k8048 *);
int pic12_write_osccal(struct k8048 *, unsigned short);
int pic12_write_config(struct k8048 *, unsigned short);

void pic12_dumpdeviceid(struct k8048 *);
void pic12_dumposccal(struct k8048 *);
void pic12_dumpconfig(struct k8048 *, int);
void pic12_dumpdevice(struct k8048 *);

void pic12_programregion(struct k8048 *, unsigned short, int, unsigned short);

void pic12_program(struct k8048 *, int);
int pic12_verify(struct k8048 *);

#endif
