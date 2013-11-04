/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.16 (16-bit word architecture)
 */

#ifndef _PIC16_H
#define _PIC16_H

/* 16-bit configuration */
#define PIC16_CONFIG_IDLOCATION1 (0)
#define PIC16_CONFIG_IDLOCATION2 (1)
#define PIC16_CONFIG_IDLOCATION3 (2)
#define PIC16_CONFIG_IDLOCATION4 (3)
#define PIC16_CONFIG_IDLOCATION5 (4)
#define PIC16_CONFIG_IDLOCATION6 (5)
#define PIC16_CONFIG_IDLOCATION7 (6)
#define PIC16_CONFIG_IDLOCATION8 (7)

#define PIC16_CONFIG_1L (8)
#define PIC16_CONFIG_1H (9)
#define PIC16_CONFIG_2L (10)
#define PIC16_CONFIG_2H (11)
#define PIC16_CONFIG_3L (12)
#define PIC16_CONFIG_3H (13)
#define PIC16_CONFIG_4L (14)
#define PIC16_CONFIG_4H (15)
#define PIC16_CONFIG_5L (16)
#define PIC16_CONFIG_5H (17)
#define PIC16_CONFIG_6L (18)
#define PIC16_CONFIG_6H (19)
#define PIC16_CONFIG_7L (20)
#define PIC16_CONFIG_7H (21)
#define PIC16_CONFIG_8L (22)	/* DS30677B */
#define PIC16_CONFIG_8H (23)	/* DS30677B */

#define PIC16_CONFIG_DEVICEID1 (24)    /*  DEV2 DEV1 DEV0 REV4 DEV3 REV2 REV1 REV0 */
#define PIC16_CONFIG_DEVICEID2 (25)    /* DEV10 DEV9 DEV8 DEV7 DEV6 DEV5 DEV4 DEV3 */

#define PIC16_CONFIG_SIZE (26)
struct pic16_config {
	unsigned char index[PIC16_CONFIG_SIZE];
};

#define PIC16_DEVICEID1_IDMASK (0xE0)		/* DEVID1 REV4 not used */
#define PIC16_DEVICEID1_REVMASK (0x1F)
#define PIC16_DEVICEID1_IDMASK_REV4 (0xF0)	/* DEVID1 REV4 used */
#define PIC16_DEVICEID1_REVMASK_REV4 (0x0F)

/*
 * DS39576B
 *  ONLY SINGLE PANEL WRITES IMPLEMENTED
 */
#define DS39576B (39576)
#define PIC18F242 (0x0480)
#define PIC18F248 (0x0800)
#define PIC18F252 (0x0400)
#define PIC18F258 (0x0840)
#define PIC18F442 (0x04a0)
#define PIC18F448 (0x0820)
#define PIC18F452 (0x0420)
#define PIC18F458 (0x0860)

/*
 * DS39592E == DS39576B
 */
#define DS39592E (39592)
#define PIC18F1220 (0x07e0)
#define PIC18F1320 (0x07c0)
#define PIC18F2220 (0x0580)
#define PIC18F2320 (0x0500)
#define PIC18F4220 (0x05a0)
#define PIC18F4320 (0x0520)

/*
 * DS30500A == DS39567B
 */
#define DS30500A (30500)
#define PIC18F2331 (0x08e0)
#define PIC18F2431 (0x08c0)
#define PIC18F4331 (0x08a0)
#define PIC18F4431 (0x0880)

/*
 * DS39622J/K != DS39576B
 *  1. NEW BULK ERASE
 *  2. VARIOUS PANEL SIZES
 *  3. EEPROM WRITE OMITS EECON2 0x55/0xaa WRITE
 *  4. EEPROM TABLAT SHIFT NEEDS PRE NOP
 *  5. SUPPORTS ONLY SINGLE PANEL WRITES
 */
#define DS39622K (39622)
#define PIC18F2221 (0x2160)
#define PIC18F2321 (0x2120)
#define PIC18F2410 (0x1160)
#define PIC18F2420 (0x1140)    /* DEVID1 REV4=0 */
#define PIC18F2423 (0x1150)    /* DEVID1 REV4=1 */
#define PIC18F2450 (0x2420)
#define PIC18F2455 (0x1260)
#define PIC18F2458 (0x2a60)
#define PIC18F2480 (0x1ae0)
#define PIC18F2510 (0x1120)
#define PIC18F2515 (0x0ce0)
#define PIC18F2520 (0x1100)    /* DEVID1 REV4=0 */
#define PIC18F2523 (0x1110)    /* DEVID1 REV4=1 */
#define PIC18F2525 (0x0cc0)
#define PIC18F2550 (0x1240)
#define PIC18F2553 (0x2a40)
#define PIC18F2580 (0x1ac0)
#define PIC18F2585 (0x0ee0)
#define PIC18F2610 (0x0ca0)
#define PIC18F2620 (0x0c80)
#define PIC18F2680 (0x0ec0)
#define PIC18F2682 (0x2700)
#define PIC18F2685 (0x2720)
#define PIC18F4221 (0x2140)
#define PIC18F4321 (0x2100)
#define PIC18F4410 (0x10e0)
#define PIC18F4420 (0x10c0)    /* DEVID1 REV4=0 */
#define PIC18F4423 (0x10d0)    /* DEVID1 REV4=1 */
#define PIC18F4450 (0x2400)
#define PIC18F4455 (0x1220)
#define PIC18F4458 (0x2a20)
#define PIC18F4480 (0x1aa0)
#define PIC18F4510 (0x10a0)
#define PIC18F4515 (0x0c60)
#define PIC18F4520 (0x1080)    /* DEVID1 REV4=0 */
#define PIC18F4523 (0x1090)    /* DEVID1 REV4=1 */
#define PIC18F4525 (0x0c40)
#define PIC18F4550 (0x1200)
#define PIC18F4553 (0x2a00)
#define PIC18F4580 (0x1a80)
#define PIC18F4585 (0x0ea0)
#define PIC18F4610 (0x0c20)
#define PIC18F4620 (0x0c00)
#define PIC18F4680 (0x0e80)
#define PIC18F4682 (0x2740)
#define PIC18F4685 (0x2760)

/*
 * DS39752A ~= DS39622J (new chip erase)
 */
#define DS39752B (39752)
#define PIC18F1230 (0x1e00)
#define PIC18F1330 (0x1e20)
#define PIC18F1330ICD (0x1fe0)

/*
 * DS41398B
 */
#define DS41398B (41398)
#define PIC18F45K22  (0x5500)
#define PIC18LF45K22 (0x5520)
#define PIC18F25K22  (0x5540)
#define PIC18LF25K22 (0x5560)
#define PIC18F23K22  (0x5740)
#define PIC18LF23K22 (0x5760)
#define PIC18F24K22  (0x5640)
#define PIC18LF24K22 (0x5660)
#define PIC18F26K22  (0x5440)
#define PIC18LF26K22 (0x5460)
#define PIC18F43K22  (0x5700)
#define PIC18LF43K22 (0x5720)
#define PIC18F44K22  (0x5600)
#define PIC18LF44K22 (0x5620)
#define PIC18F46K22  (0x5400)
#define PIC18LF46K22 (0x5420)

/*
 * DS39972B
 */
#define DS39972B (39972)
#define PIC18F66K80  (0x60e0)
#define PIC18F46K80  (0x6100)
#define PIC18F26K80  (0x6120)
#define PIC18F65K80  (0x6140)
#define PIC18F45K80  (0x6160)
#define PIC18F25K80  (0x6180)
#define PIC18LF66K80 (0x61c0)
#define PIC18LF46K80 (0x61e0)
#define PIC18LF26K80 (0x6200)
#define PIC18LF65K80 (0x6220)
#define PIC18LF45K80 (0x6240)
#define PIC18LF25K80 (0x6260)

/*
 * DS41630B
 */
#define DS41630B (41630)
#define PIC18F45K50  (0x5c00)
#define PIC18LF45K50 (0x5c80)
#define PIC18F25K50  (0x5c20)
#define PIC18LF25K50 (0x5ca0)
#define PIC18F24K50  (0x5c60)
#define PIC18LF24K50 (0x5ce0)
#define PIC18F26K50  (0x5d20)
#define PIC18LF26K50 (0x5d60)
#define PIC18F46K50  (0x5d00)
#define PIC18LF46K50 (0x5d40)

/*
 * DS41342E
 */
#define DS41342E (41342)
#define PIC18LF13K50 (0x4700)
#define PIC18LF14K50 (0x4720)
#define PIC18F13K50  (0x4740)
#define PIC18F14K50  (0x4760)

/*
 * DS39687E
 */
#define DS39687E (39687)
#define PIC18F24J10  (0x1D00)
#define PIC18F25J10  (0x1C00)
#define PIC18F44J10  (0x1D20)
#define PIC18F45J10  (0x1C20)
#define PIC18F25J11  (0x4DA0)
#define PIC18F24J11  (0x4D80)
#define PIC18F26J11  (0x4DC0)
#define PIC18F45J11  (0x4E00)
#define PIC18F44J11  (0x4DE0)
#define PIC18F46J11  (0x4E20)
#define PIC18F24J50  (0x4C00)
#define PIC18F25J50  (0x4C20)
#define PIC18F26J50  (0x4C40)
#define PIC18F44J50  (0x4C60)
#define PIC18F45J50  (0x4C80)
#define PIC18F46J50  (0x4CA0)
#define PIC18F26J13  (0x5920)
#define PIC18F27J13  (0x5960)
#define PIC18F46J13  (0x59A0)
#define PIC18F47J13  (0x59E0)
#define PIC18F26J53  (0x5820)
#define PIC18F27J53  (0x5860)
#define PIC18F46J53  (0x58A0)
#define PIC18F47J53  (0x58E0)
#define PIC18LF24J10 (0x1D40)
#define PIC18LF25J10 (0x1C40)
#define PIC18LF44J10 (0x1D60)
#define PIC18LF45J10 (0x1C60)
#define PIC18LF24J50 (0x4CC0)
#define PIC18LF25J50 (0x4CE0)
#define PIC18LF26J50 (0x4D00)
#define PIC18LF44J50 (0x4D20)
#define PIC18LF45J50 (0x4D40)
#define PIC18LF46J50 (0x4D60)
#define PIC18LF24J11 (0x4E40)
#define PIC18LF25J11 (0x4E60)
#define PIC18LF26J11 (0x4E80)
#define PIC18LF44J11 (0x4EA0)
#define PIC18LF45J11 (0x4EC0)
#define PIC18LF46J11 (0x4EE0)
#define PIC18LF26J13 (0x5B20)
#define PIC18LF27J13 (0x5B60)
#define PIC18LF46J13 (0x5BA0)
#define PIC18LF47J13 (0x5BE0)
#define PIC18LF26J53 (0x5A20)
#define PIC18LF27J53 (0x5A60)
#define PIC18LF46J53 (0x5AA0)
#define PIC18LF47J53 (0x5AE0)

/* KB to number of words */
#define WORD16(X) ((X) * 512)

/* device to datasheet map */
struct pic16_dsmap
{
	char devicename[STRLEN];
	unsigned short deviceid;
	int flash;			/* flash  size in words */
	int eeprom;			/* eeprom size in bytes 0=none */
	unsigned int datasheet;
	unsigned char devid1rev4;	/* DEVID1 REV4 used 0/1 */
	unsigned char panelsize;
	unsigned int p9, p9a;		/* Program delay times P9 and P9A in us */
	unsigned int p10, p11;		/* Erase delay times P10 and P11 in us */
	unsigned int configaddr;        /* CONFIG ADDRESS */
	unsigned int configsize;        /* CONFIG SIZE */
	unsigned int erasesize;         /* ERASE BLOCK SIZE 64 or 1024 for PIC18J */
};
#define MAXPANELSIZE (64)

/* programming regions to determine the correct programming algorithm */
#define PIC16_REGIONUNKNOWN (0)	/*1000000 + */
#define PIC16_REGIONCODEID (1)	/* 000000 -> 1fffff 200000 -> 200007 PANEL WRITING  */
#define PIC16_REGIONCONFIG (2)	/* 300000 -> 30000d                  BYTE WRITING   */
#define PIC16_REGIONDATA (3)	/* f00000 -> ffffff                  EEPROM WRITING */

/* some of the following high values are theorhetical limits */
#define PIC16_CODE_LOW    (0x000000)	/* 128KB MAX  */
#define PIC16_CODE_HIGH   (0x1fffff)
#define PIC16_ID_LOW      (0x200000)	/* not PIC18J */
#define PIC16_ID_HIGH     (0x200007)
#define PIC16_CONFIG_LOW  (0x300000)	/* not PIC18J */
#define PIC16_CONFIG_HIGH (0x30000d)
#define PIC16_CONFIG_MASK (0x00000f)
#define PIC16_DATA_LOW    (0xf00000)	/* pseudo address range */
#define PIC16_DATA_HIGH   (0xffffff)
#define PIC16_DATA_MASK   (0x0fffff)

/* config address and size for non pic18j */
#define PIC16_CONFIG (PIC16_CONFIG_LOW)
#define PIC16_CSIZE (14) /* 0 -> D */

/* the following is not programmable */
#define PIC16_DEVICEID_LOW (0x3ffffe)
#define PIC16_DEVICEID_HIGH (0x3fffff)

/* the following are not used at present and later devices have many more */
#define PIC16_BOOTBLOCK_LOW (0x000000)
#define PIC16_BOOTBLOCK_HIGH (0x0001FF)
#define PIC16_BLOCK0_LOW (0x000200)
#define PIC16_BLOCK0_HIGH (0x001FFF)
#define PIC16_BLOCK1_LOW (0x002000)
#define PIC16_BLOCK1_HIGH (0x003FFF)
#define PIC16_BLOCK2_LOW (0x004000)
#define PIC16_BLOCK2_HIGH (0x005FFF)
#define PIC16_BLOCK3_LOW (0x006000)
#define PIC16_BLOCK3_HIGH (0x007FFF)

/* buffered write modes */
#define WB_INIT (0)
#define WB_FLUSH (1)
#define WB_ADD (2)

/* buffer space filler */
#define WB_SPACE (0xff)

void pic16_selector(struct k8048 *);

void pic16_init_code_memory_access(struct k8048 *);

void pic16_write_enable(struct k8048 *);
void pic16_write_disable(struct k8048 *);
void pic16_free(struct k8048 *);
void pic16_write(struct k8048 *);

/* machine code block utilities */
void pic16_set_table_pointer(struct k8048 *, unsigned int);

void pic16_erase_block(struct k8048 *, unsigned int);
void pic16_erase_row(struct k8048 *, unsigned int, unsigned int);
void pic16_bulk_erase(struct k8048 *);
void pic16_row_erase(struct k8048 *, unsigned int, unsigned int);

/* data (EEPROM) */
void pic16_init_data_memory_access(struct k8048 *);
void pic16_set_data_pointer(struct k8048 *, unsigned short);
unsigned char pic16_read_data_memory(struct k8048 *);
void pic16_write_data_memory(struct k8048 *, unsigned char);

void pic16_read_config_memory(struct k8048 *);

void pic16_goto100000(struct k8048 *);
void pic16_write_configreg(struct k8048 *, unsigned long, int, int, int);
void pic16_write_config(struct k8048 *);

/* multi-byte read */
int pic16_get_program_flash_size(void);
int pic16_get_data_flash_size(void);
int pic16_get_data_eeprom_size(void);

unsigned long pic16_read_flash_memory_block(struct k8048 *, unsigned short *, int);
unsigned long pic16_read_eeprom_memory_block(struct k8048 *, unsigned char *, int);

/* diagnostics */
void pic16_dumpdeviceid(struct k8048 *);
void pic16_dumpconfig(struct k8048 *, int);
void pic16_dumpconfig_verbose(struct k8048 *);
void pic16_dumpdevice(struct k8048 *);

/* regions */
int pic16_getregion(unsigned int);
unsigned char pic16_programregion(struct k8048 *, int, unsigned long, unsigned char);

/* program/write/verify */
void pic16_write_buffer(struct k8048 *, unsigned long, unsigned char *);
void pic16_write_buffered(struct k8048 *, unsigned long, unsigned char, int);

void pic16_program(struct k8048 *, int);
int pic16_verify(struct k8048 *);
void pic16_program_config(struct k8048 *);

#endif
