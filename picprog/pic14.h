/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.14 (14-bit word architecture)
 */

#ifndef _PIC14_H
#define _PIC14_H

/* 14-bit configuration */
#define PIC14_CONFIG_USERID0 (0)
#define PIC14_CONFIG_USERID1 (1)
#define PIC14_CONFIG_USERID2 (2)
#define PIC14_CONFIG_USERID3 (3)
#define PIC14_CONFIG_RESERVED4 (4)
#define PIC14_CONFIG_RESERVED5 (5)
#define PIC14_CONFIG_RESERVED6 (6) /* [2006] NO DEVICE ID (84) */
#define PIC14_CONFIG_DEVICEID (6)  /* [2006]    DEVICE ID (84A) */
#define PIC14_CONFIG_WORD1 (7)     /* [2007] CONFIG/CONFIG1 */
#define PIC14_CONFIG_WORD2 (8)     /* [2008] CONFIG2 (88, 726) OR CALIB (688) */
#define PIC14_CONFIG_WORD3 (9)     /* [2009] CALIB1 (726) */
#define PIC14_CONFIG_WORD4 (10)    /* [200A] CALIB2 (726) */
#define PIC14_CONFIG_SIZE (11)
struct pic14_config {
	unsigned short index[PIC14_CONFIG_SIZE];
};

/*
 * deviceid and revision mask
 *
 * nb. the actual deviceid is (0x2006)>>5
 */
#define PIC14_DEVICEID_MASK (0x3FE0)
#define PIC14_DEVICEREV_MASK (0x001F)

/*
 * DS30262E
 * PIC16F84A 00 0101 011X XXXX
 */
#define DS30262E (30262)
#define PIC16F84A (0x0560) /* [2006] */
#define PIC16F84  (0x0560) /* this device has no embedded device id */
#define PIC16C84  (0x0560) /* this device has no embedded device id */

/*
 * DS30034D (DS30277D PAGE 3-161)
 *  PIC16F627 00 0111 101X XXXX
 *  PIC16F627 00 0111 110X XXXX
 */
#define DS30034D (30034)
#define PIC16F627 (0x07a0)
#define PIC16F628 (0x07c0)

/*
 * DS39025F
 *  PIC16F870 00 1101 000X XXXX
 *  PIC16F871 00 1101 001X XXXX
 *  PIC16F872 00 1000 111X XXXX
 *  PIC16F873 00 1001 011X XXXX
 *  PIC16F874 00 1001 001X XXXX
 *  PIC16F876 00 1001 111X XXXX
 *  PIC16F877 00 1001 101X XXXX
 */
#define DS39025F (39025)
#define PIC16F870 (0x0d00)
#define PIC16F871 (0x0d20)
#define PIC16F872 (0x08e0)
#define PIC16F873 (0x0960)
#define PIC16F874 (0x0920)
#define PIC16F876 (0x09e0)
#define PIC16F877 (0x09a0)

/*
 * DS39589B
 *  PIC16F873A 00 1110 0100 XXXX
 *  PIC16F874A 00 1110 0110 XXXX
 *  PIC16F876A 00 1110 0000 XXXX
 *  PIC16F877A 00 1110 0010 XXXX
 */
#define DS39589B (39589)
#define PIC16F873A (0x0e40)
#define PIC16F874A (0x0e60)
#define PIC16F876A (0x0e00)
#define PIC16F877A (0x0e20)

/*
 * DS39607B
 *  PIC16F87 00 0111 0010 XXXX
 *  PIC16F88 00 0111 0110 XXXX
 */
#define DS39607B (39607)
#define PIC16F87 (0x0720)
#define PIC16F88 (0x0760)

/*
 * DS41191C
 *  PIC12F629 00 1111 100X XXXX
 *  PIC12F675 00 1111 110X XXXX
 *  PIC16F630 01 0000 110X XXXX
 *  PIC16F676 01 0000 111X XXXX
 */
#define DS41191C (41191)
#define PIC12F629 (0x0f80)
#define PIC12F675 (0x0fc0)
#define PIC16F630 (0x10c0)
#define PIC16F676 (0x10e0)
#define OSCCALADDR  (0x03ff) /* DS41191C ONLY */
#define BANDGAPADDR (0x2007) /* DS41191C ONLY */
#define BANDGAPMASK (0x3000) /* DS41191C ONLY */
#define CONFIGMASK  (0x01ff) /* DS41191C ONLY */

/*
 * DS41196F
 *  PIC16F627A 01 0000 010X XXXX
 *  PIC16F628A 01 0000 011X XXXX
 *  PIC16F648A 01 0001 000X XXXX
 */
#define DS41196F (41196)
#define PIC16F627A (0x1040)
#define PIC16F628A (0x1060)
#define PIC16F648A (0x1100)

/*
 * DS39603C
 *  PIC16F818 00 0100 1100 XXXX
 *  PIC16F819 00 0100 1110 XXXX
 */
#define DS39603C (39603)
#define PIC16F818 (0x04c0)
#define PIC16F819 (0x04e0)

/*
 * DS40245B
 *  PIC16F716 01 0001 010X XXXX
 */
#define DS40245B (40245)
#define PIC16F716 (0x1140)

/*
 * DS41204G
 *  PIC12F635 00 1111 101X XXXX
 *  PIC12F683 00 0100 011X XXXX
 *  PIC16F631 01 0100 001X XXXX
 *  PIC16F636 01 0000 101X XXXX The 16F636 and 16F639 share the same device ID
 *  PIC16F639 01 0000 101X XXXX The 16F636 and 16F639 share the same device ID
 *  PIC16F677 01 0100 010X XXXX
 *  PIC16F684 01 0000 100X XXXX
 *  PIC16F685 00 0100 101X XXXX
 *  PIC16F687 01 0011 001X XXXX
 *  PIC16F688 01 0001 100X XXXX
 *  PIC16F689 01 0011 010X XXXX
 *  PIC16F690 01 0100 000X XXXX
 */
#define DS41204G (41204)
#define PIC12F635 (0x0fa0)
#define PIC12F683 (0x0460)
#define PIC16F631 (0x1420)
#define PIC16F636 (0x10a0)
#define PIC16F639 (0x10a0)
#define PIC16F677 (0x1440)
#define PIC16F684 (0x1080)
#define PIC16F685 (0x04a0)
#define PIC16F687 (0x1320)
#define PIC16F688 (0x1180)
#define PIC16F689 (0x1340)
#define PIC16F690 (0x1400)

/*
 * DS41332D
 *  PIC16F722	PIC16LF722
 *  PIC16F722A  PIC16LF722A
 *  PIC16F723	PIC16LF723
 *  PIC16F723A	PIC16LF723A
 *  PIC16F724	PIC16LF724
 *  PIC16F726	PIC16LF726
 *  PIC16F727	PIC16LF727
 *
 * These devices may not be plugged into the k8048
 * without the possibility of causing damage to
 * the chip.
 *
 * They require a VPP voltage of 9V D.C. and not
 * 12V D.C as supplied by the k8048 and with the
 * L variant demand a lower VDD.
 *
 * A simple way to reduce VPP to 9V D.C. is to
 * connect VPP to ground via a 8K2 resistor.
 */
#define DS41332D (41332)
/* NB Low VPP */
#define PIC16F722   (0x1880)
#define PIC16F722A  (0x1B20)
#define PIC16F723   (0x1860)
#define PIC16F723A  (0x1B00)
#define PIC16F724   (0x1840)
#define PIC16F726   (0x1820)
#define PIC16F727   (0x1800)
/* NB Low VPP & VDD */
#define PIC16LF722  (0x1980)
#define PIC16LF722A (0x1B60)
#define PIC16LF723  (0x1960)
#define PIC16LF723A (0x1B40)
#define PIC16LF724  (0x1940)
#define PIC16LF726  (0x1920)
#define PIC16LF727  (0x1900)

/*
 * DS41284E
 *  PIC12F615	xx10 0001 100x xxxx
 *  PIC12HV615	xx10 0001 101x xxxx
 *  PIC12F617	xx01 0011 011x xxxx
 *  PIC16F616	xx01 0010 010x xxxx
 *  PIC16HV616	xx01 0010 011x xxxx
 *  PIC12F609	xx10 0010 010x xxxx
 *  PIC12HV609	xx10 0010 100x xxxx
 *  PIC16F610	xx10 0010 011x xxxx
 *  PIC16HV610	xx10 0010 101x xxxx
 */
#define DS41284E (41284)
#define PIC12F615  (0x2180)
#define PIC12HV615 (0x21a0)
#define PIC12F617  (0x1360)
#define PIC16F616  (0x1240)
#define PIC16HV616 (0x1260)
#define PIC12F609  (0x2240)
#define PIC12HV609 (0x2280)
#define PIC16F610  (0x2260)
#define PIC16HV610 (0x22a0)

/*
 * DS41287D
 *  PIC16F882 xx10 0000 000x xxxx
 *  PIC16F883 xx10 0000 001x xxxx
 *  PIC16F884 xx10 0000 010x xxxx
 *  PIC16F886 xx10 0000 011x xxxx
 *  PIC16F887 xx10 0000 100x xxxx
 */
#define DS41287D (41287)
#define PIC16F882 (0x2000)
#define PIC16F883 (0x2020)
#define PIC16F884 (0x2040)
#define PIC16F886 (0x2060)
#define PIC16F887 (0x2080)

/*
 * DS41572D
 *  PIC10F320  XX10 1001 101X XXXX
 *  PIC10F322  XX10 1001 100X XXXX
 *
 *  PIC10LF320 XX10 1001 111X XXXX
 *  PIC10LF322 XX10 1001 110X XXXX
 */
#define DS41572D (41572)
#define PIC10F320  (0x29A0)
#define PIC10F322  (0x2980)
#define PIC10LF320 (0x29E0)
#define PIC10LF322 (0x29C0)

/*
 * DS41573C
 *  PIC12F1501  0010 1100 110X XXXX
 *  PIC16F1503  0010 1100 111X XXXX
 *  PIC16F1507  0010 1101 000X XXXX
 *  PIC16F1508  0010 1101 001X XXXX
 *  PIC16F1509  0010 1101 010X XXXX
 *
 *  PIC12LF1501 0010 1100 100X XXXX
 *  PIC16LF1503 0010 1101 101X XXXX
 *  PIC16LF1507 0010 1101 110X XXXX
 *  PIC16LF1508 0010 1101 111X XXXX
 *  PIC16LF1509 0010 1110 000X XXXX
 */
#define DS41573C (41573)
#define PIC12F1501  (0x2CC0)
#define PIC16F1503  (0x2CE0)
#define PIC16F1507  (0x2D00)
#define PIC16F1508  (0x2D20)
#define PIC16F1509  (0x2D40)
#define PIC12LF1501 (0x2C80)
#define PIC16LF1503 (0x2DA0)
#define PIC16LF1507 (0x2DC0)
#define PIC16LF1508 (0x2DE0)
#define PIC16LF1509 (0x2E00)

/*
 * DS30324B
 *  PIC16F73 XX00 0110 000X XXXX
 *  PIC16F74 XX00 0110 001X XXXX
 *  PIC16F76 XX00 0110 010X XXXX
 *  PIC16F77 XX00 0110 011X XXXX
 */
#define DS30324B (30324)
#define PIC16F73 (0x0600)
#define PIC16F74 (0x0620)
#define PIC16F76 (0x0640)
#define PIC16F77 (0x0660)

/*
 * DS41439A
 *  PIC12F1840  XX01 1011 100X XXXX
 *  PIC16F1847  XX01 0100 100X XXXX
 *  PIC12LF1840 XX01 1011 110X XXXX
 *  PIC16LF1847 XX01 0100 101X XXXX
 */
#define DS41439A (41439)
#define PIC12F1840  (0x1D80)
#define PIC16F1847  (0x1480)
#define PIC12LF1840 (0x1DC0)
#define PIC16LF1847 (0x14A0)

/*
 * DS41397B
 *  PIC16F1933  xx10 0011 000x xxxx
 *  PIC16F1934  xx10 0011 010x xxxx
 *  PIC16F1936  xx10 0011 011x xxxx
 *  PIC16F1937  xx10 0011 100x xxxx
 *  PIC16F1938  xx10 0011 101x xxxx
 *  PIC16F1939  xx10 0011 110x xxxx
 *  PIC16F1946  xx10 0101 000x xxxx
 *  PIC16F1947  xx10 0101 001x xxxx
 *  PIC16LF1933 xx10 0100 000x xxxx
 *  PIC16LF1934 xx10 0100 010x xxxx
 *  PIC16LF1936 xx10 0100 011x xxxx
 *  PIC16LF1937 xx10 0100 100x xxxx
 *  PIC16LF1938 xx10 0100 101x xxxx
 *  PIC16LF1939 xx10 0100 110x xxxx
 *  PIC16LF1946 xx10 0101 100x xxxx
 *  PIC16LF1947 xx10 0101 101x xxxx
 *  PIC16LF1902 xx10 1100 001x xxxx
 *  PIC16LF1903 xx10 1100 000x xxxx
 *  PIC16LF1904 xx10 1100 100x xxxx
 *  PIC16LF1906 xx10 1100 011x xxxx
 *  PIC16LF1907 xx10 1100 010x xxxx
 */
#define DS41397B (41397)
#define PIC16F1933  (0x2300)
#define PIC16F1934  (0x2340)
#define PIC16F1936  (0x2360)
#define PIC16F1937  (0x2380)
#define PIC16F1938  (0x23A0)
#define PIC16F1939  (0x23C0)
#define PIC16F1946  (0x2500)
#define PIC16F1947  (0x2520)
#define PIC16LF1933 (0x2400)
#define PIC16LF1934 (0x2440)
#define PIC16LF1936 (0x2460)
#define PIC16LF1937 (0x2480)
#define PIC16LF1938 (0x24A0)
#define PIC16LF1939 (0x24C0)
#define PIC16LF1946 (0x2580)
#define PIC16LF1947 (0x25A0)
#define PIC16LF1902 (0x2C20)
#define PIC16LF1903 (0x2C00)
#define PIC16LF1904 (0x2C80)
#define PIC16LF1906 (0x2C60)
#define PIC16LF1907 (0x2C40)

/* KB to number of words */
#define WORD14(X) (((X) * 8192.0) / 14)

/* device to datasheet map */
struct pic14_dsmap
{
	char devicename[STRLEN];
	unsigned short deviceid;
	int flash;			/* program flash size in words */
	int eeprom;			/* data eeprom size in bytes 0=none */
	/*
	 * data flash on enhanced devices occupies the end of program flash and has no
	 * special treatment in this application.
	 */
	int data;			/* data flash size in bytes 0=none */
	unsigned int datasheet;
	unsigned int configaddr;	/* CONFIG BASE */
	unsigned char nconfig;		/* NUMBER OF CONFIG WORDS AT CONFIG BASE + 7 */
	unsigned char ncalib;		/* NUMBER OF CALIB. WORDS AT CONFIG BASE + 7 + nconfig */
	unsigned char latches;
	unsigned char idlatches;
};

/* regions (program/data/config) */
#define PIC14_REGIONUNKNOWN (0)
#define PIC14_REGIONCODE (1)
#define PIC14_REGIONCONFIG (2)
#define PIC14_REGIONDATA (3)

/* device memory map */
#define PIC14_CODE_LOW (0)		/* CODE ORIGIN */
#define PIC14_CONFIG_MIDRANGE (0x2000)	/* CONFIG ORIGIN FOR MID-RANGE */
#define PIC14_CONFIG_ENHANCED (0x8000)	/* CONFIG ORIGIN FOR ENHANCED MID-RANGE */
#define PIC14_CONFIG_MAX (256)		/* CONFIG MAX SIZE */
#define PIC14_DATA_MIDRANGE (0x2100)	/* DATA EEPROM ORIGIN FOR MID-RANGE */
#define PIC14_DATA_ENHANCED (0xF000)	/* DATA EEPROM ORIGIN FOR ENHANCED MID-RANGE */
#define PIC14_DATA_MAX (1024)		/* DATA EEPROM MAX SIZE ? */

/* default timings */
#define PIC14_TPROG_DEFAULT (10000)	/* 10ms  */
#define PIC14_TERASE_DEFAULT (10000)	/* 10ms  */
#define PIC14_TERASE_15MS (15000)	/* 15ms  */
#define PIC14_TERASE_30MS (30000)	/* 30ms  */
#define PIC14_TDISCHARGE_DEFAULT (100)	/* 100us */
#define PIC14_TDISCHARGE_300US (300)	/* 300us */

void pic14_selector(struct k8048 *);

/* hardware function prototypes */
void pic14_begin_programming_001000(struct k8048 *, int);
void pic14_begin_programming_011000(struct k8048 *, int);
void pic14_end_programming_001110(struct k8048 *, int);
void pic14_end_programming_010111(struct k8048 *, int);
void pic14_load_configuration(struct k8048 *, unsigned short);
void pic14_load_data_for_program_memory(struct k8048 *, unsigned short);
void pic14_load_data_for_data_memory(struct k8048 *, unsigned short);
void pic14_increment_address(struct k8048 *);
unsigned short pic14_read_data_from_program_memory(struct k8048 *);
unsigned char pic14_read_data_from_data_memory(struct k8048 *);
void pic14_begin_erase_programming(struct k8048 *, int t);
void pic14_chip_erase(struct k8048 *, int);
void pic14_bulk_erase_program_memory(struct k8048 *, int);
void pic14_bulk_erase_data_memory(struct k8048 *, int);
void pic14_bulk_erase_setup1(struct k8048 *);
void pic14_bulk_erase_setup2(struct k8048 *);

/* compound functions */
int pic14_get_program_flash_size(void);
int pic14_get_data_flash_size(void);
int pic14_get_data_eeprom_size(void);

unsigned short pic14_read_flash_memory_block(struct k8048 *, unsigned short *, int);
unsigned short pic14_read_flash_memory_increment(struct k8048 *);
unsigned short pic14_read_eeprom_memory_block(struct k8048 *, unsigned char *, int);
unsigned char  pic14_read_eeprom_memory_increment(struct k8048 *);

void pic14_read_config_memory(struct k8048 *);
unsigned short pic14_read_osccal(struct k8048 *);

void pic14_bulk_erase(struct k8048 *, unsigned short, unsigned short);

int pic14_write_osccal(struct k8048 *, unsigned short);
int pic14_write_calib1(struct k8048 *, unsigned short);
int pic14_write_config(struct k8048 *, unsigned short);
int pic14_write_word(struct k8048 *);

/* regions (code/config/data) */
int pic14_getregion(unsigned int);
unsigned int pic14_initregion(struct k8048 *, int);
void pic14_loadregion(struct k8048 *, int, unsigned short);
int pic14_verifyregion(struct k8048 *, unsigned short, int, unsigned short);

/* program/verify */
void pic14_programregion(struct k8048 *, unsigned short, int, unsigned short);
void pic14_program(struct k8048 *);
int pic14_verify(struct k8048 *);

/* diagnostics */
void pic14_dumpdeviceid(struct k8048 *);
void pic14_dumposccal(struct k8048 *);
void pic14_dumpconfig(struct k8048 *, int);
void pic14_dumpdevice(struct k8048 *);

void pic14_dumpconfig_16f627(unsigned int);
void pic14_dumpconfig_12f675(unsigned int);
void pic14_dumpconfig_16f877a(unsigned int);
void pic14_dumpconfig_16f88(unsigned int, unsigned int);
void pic14_dumpconfig_16f628a(unsigned int);
void pic14_dumpconfig_12f683(unsigned int);
void pic14_dumpconfig_16f886(unsigned int, unsigned int);

#endif
