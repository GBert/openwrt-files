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

#include "k8048.h"

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/
struct pic14_config pic14_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/
struct pic14_dsmap pic14_map[] =
{
/*Device name	Device id	Flash	EEPROM	Data	Data-sheet Configuration Latches*/
#if 0
/* No test device available */
{"PIC16C84",	PIC16C84,	1024,	64,	0,	DS30189D,  0x2000, 1, 0, 1,	1},
#endif
{"PIC16F83",	PIC16F83,	512,	64,	0,	DS30262E,  0x2000, 1, 0, 1,	1},
{"PIC16F84",	PIC16F84,	1024,	64,	0,	DS30262E,  0x2000, 1, 0, 1,	1},
{"PIC16F84A",	PIC16F84A,	1024,	64,	0,	DS30262E,  0x2000, 1, 0, 1,	1},

{"PIC16F627",	PIC16F627,	1024,	128,	0,	DS30034D,  0x2000, 1, 0, 1,	1},
{"PIC16F628",	PIC16F628,	2048,	128,	0,	DS30034D,  0x2000, 1, 0, 1,	1},

{"PIC16F870",	PIC16F870,	2048,	64,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F871",	PIC16F871,	2048,	64,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F872",	PIC16F872,	2048,	64,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F873",	PIC16F873,	4096,	128,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F874",	PIC16F874,	4096,	128,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F876",	PIC16F876,	8192,	256,	0,	DS39025F,  0x2000, 1, 0, 1,	1},
{"PIC16F877",	PIC16F877,	8192,	256,	0,	DS39025F,  0x2000, 1, 0, 1,	1},

{"PIC16F873A",	PIC16F873A,	4096,	128,	0,	DS39589B,  0x2000, 1, 0, 8,	4},
{"PIC16F874A",	PIC16F874A,	4096,	128,	0,	DS39589B,  0x2000, 1, 0, 8,	4},
{"PIC16F876A",	PIC16F876A,	8192,	256,	0,	DS39589B,  0x2000, 1, 0, 8,	4},
{"PIC16F877A",	PIC16F877A,	8192,	256,	0,	DS39589B,  0x2000, 1, 0, 8,	4},

/* These devices have two config words at 2007 & 2008 */
{"PIC16F87",	PIC16F87,	4096,	256,	0,	DS39607B,  0x2000, 2, 0, 4,	4},
{"PIC16F88",	PIC16F88,	4096,	256,	0,	DS39607B,  0x2000, 2, 0, 4,	4},

{"PIC12F629",	PIC12F629,	1024,	128,	0,	DS41191C,  0x2000, 1, 0, 1,	1},
{"PIC12F675",	PIC12F675,	1024,	128,	0,	DS41191C,  0x2000, 1, 0, 1,	1},

{"PIC16F630",	PIC16F630,	1024,	128,	0,	DS41191C,  0x2000, 1, 0, 1,	1},
{"PIC16F676",	PIC16F676,	1024,	128,	0,	DS41191C,  0x2000, 1, 0, 1,	1},

{"PIC16F627A",	PIC16F627A,	1024,	128,	0,	DS41196F,  0x2000, 1, 0, 1,	1},
{"PIC16F628A",	PIC16F628A,	2048,	128,	0,	DS41196F,  0x2000, 1, 0, 1,	1},
{"PIC16F648A",	PIC16F648A,	4096,	256,	0,	DS41196F,  0x2000, 1, 0, 1,	1},

{"PIC16F818",	PIC16F818,	1024,	128,	0,	DS39603C,  0x2000, 1, 0, 4,	4},
{"PIC16F819",	PIC16F819,	2048,	256,	0,	DS39603C,  0x2000, 1, 0, 4,	4},

{"PIC16F716",	PIC16F716,	2048,	0,	0,	DS40245B,  0x2000, 1, 0, 4,	1},

/* These devices have one config word at 2007 and up to two calibration words at 2008 & 2009 */
{"PIC16F631",	PIC16F631,	1024,	128,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F677",	PIC16F677,	2048,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F685",	PIC16F685,	4096,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F687",	PIC16F687,	2048,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F689",	PIC16F689,	4096,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F690",	PIC16F690,	4096,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC12F635",	PIC12F635,	1024,	128,	0,	DS41204G,  0x2000, 1, 2, 4,	1},
{"PIC16F636",	PIC16F636,	2048,	256,	0,	DS41204G,  0x2000, 1, 2, 4,	1},
{"PIC16F639",	PIC16F639,	2048,	256,	0,	DS41204G,  0x2000, 1, 2, 4,	1},
{"PIC12F683",	PIC12F683,	2048,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F684",	PIC16F684,	2048,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},
{"PIC16F688",	PIC16F688,	4096,	256,	0,	DS41204G,  0x2000, 1, 1, 4,	1},

/* These devices have two config words at 2007 & 2008 and two calibration words at 2009 & 200A */
{"PIC16F722",	PIC16F722,	2048,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F722A",	PIC16F722A,	2048,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F723",	PIC16F723,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F723A",	PIC16F723A,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F724",	PIC16F724,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F726",	PIC16F726,	8192,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16F727",	PIC16F727,	8192,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
#ifdef PICLF
{"PIC16LF722",	PIC16LF722,	2048,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF722A",	PIC16LF722A,	2048,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF723",	PIC16LF723,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF723A",	PIC16LF723A,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF724",	PIC16LF724,	4096,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF726",	PIC16LF726,	8192,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
{"PIC16LF727",	PIC16LF727,	8192,	0,	0,	DS41332D,  0x2000, 2, 2, 8,	1},
#endif

/* These devices have one config word at 2007 and one calibration word at 2008 */
{"PIC12F609",	PIC12F609,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
{"PIC12HV609",	PIC12HV609,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
{"PIC12F615",	PIC12F615,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
{"PIC12HV615",	PIC12HV615,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
/* */
{"PIC16F610",	PIC16F610,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
{"PIC16HV610",	PIC16HV610,	1024,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
{"PIC16F616",	PIC16F616,	2048,	0,	0,	DS41284E,  0x2000, 1, 1, 4,	1},
{"PIC16HV616",	PIC16HV616,	2048,	0,	0,	DS41284E,  0x2000, 1, 1, 1,	1},
/* */
{"PIC12F617",	PIC12F617,	2048,	0,	0,	DS41284E,  0x2000, 1, 1, 4,	1},

/* These devices have two config words at 2007 & 2008 and one calibration word at 2009 */
{"PIC16F882",	PIC16F882,	2048,	128,	0,	DS41287D,  0x2000, 2, 1, 8,	1},
{"PIC16F883",	PIC16F883,	4096,	256,	0,	DS41287D,  0x2000, 2, 1, 8,	1},
{"PIC16F884",	PIC16F884,	4096,	256,	0,	DS41287D,  0x2000, 2, 1, 8,	1},
{"PIC16F886",	PIC16F886,	8192,	256,	0,	DS41287D,  0x2000, 2, 1, 8,	1},
{"PIC16F887",	PIC16F887,	8192,	256,	0,	DS41287D,  0x2000, 2, 1, 8,	1},

{"PIC16F73",	PIC16F73,	4096,	0,	0,	DS30324B,  0x2000, 1, 0, 2,	1},
{"PIC16F74",	PIC16F74,	4096,	0,	0,	DS30324B,  0x2000, 1, 0, 2,	1},
{"PIC16F76",	PIC16F76,	8192,	0,	0,	DS30324B,  0x2000, 1, 0, 2,	1},
{"PIC16F77",	PIC16F77,	8192,	0,	0,	DS30324B,  0x2000, 1, 0, 2,	1},

/* These devices have one config word at 2007 and two calibration words at 2008 & 2009 */
{"PIC10F320",	PIC10F320,	256,	0,	0,	DS41572D,  0x2000, 1, 2, 16,	1},
{"PIC10F322",	PIC10F322,	512,	0,	0,	DS41572D,  0x2000, 1, 2, 16,	1},
#ifdef PICLF
{"PIC10LF320",	PIC10LF320,	256,	0,	0,	DS41572D,  0x2000, 1, 2, 16,	1},
{"PIC10LF322",	PIC10LF322,	512,	0,	0,	DS41572D,  0x2000, 1, 2, 16,	1},
#endif

/* These devices have two config words at 8007 & 8008 and two calibration words at 8009 & 800A */
{"PIC12F1501",	PIC12F1501,	1024,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
{"PIC16F1503",	PIC16F1503,	2048,	0,	-128,	DS41573C,  0x8000, 2, 2, 16,	1},
{"PIC16F1507",	PIC16F1507,	2048,	0,	-128,	DS41573C,  0x8000, 2, 2, 16,	1},
{"PIC16F1508",	PIC16F1508,	4096,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
{"PIC16F1509",	PIC16F1509,	8192,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
#ifdef PICLF
{"PIC12LF1501",	PIC12LF1501,	1024,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
{"PIC16LF1503",	PIC16LF1503,	2048,	0,	-128,	DS41573C,  0x8000, 2, 2, 16,	1},
{"PIC16LF1507",	PIC16LF1507,	2048,	0,	-128,	DS41573C,  0x8000, 2, 2, 16,	1},
{"PIC16LF1508",	PIC16LF1508,	4096,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
{"PIC16LF1509",	PIC16LF1509,	8192,	0,	-128,	DS41573C,  0x8000, 2, 2, 32,	1},
#endif

/* These devices have two config words at 8007 & 8008 and two calibration words at 8009 & 800A */
{"PIC12F1840",	PIC12F1840,	4096,	256,	0,	DS41439A,  0x8000, 2, 2, 32,	1},
{"PIC16F1847",	PIC16F1847,	8192,	256,	0,	DS41439A,  0x8000, 2, 2, 32,	1},
#ifdef PICLF
{"PIC12LF1840",	PIC12LF1840,	4096,	256,	0,	DS41439A,  0x8000, 2, 2, 32,	1},
{"PIC16LF1847",	PIC16LF1847,	8192,	256,	0,	DS41439A,  0x8000, 2, 2, 32,	1},
#endif

/* These devices have two config words at 8007 & 8008 and two calibration words at 8009 & 800A */
{"PIC16F1933",	PIC16F1933,	4096,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1934",	PIC16F1934,	4096,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1936",	PIC16F1936,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1937",	PIC16F1937,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1938",	PIC16F1938,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1939",	PIC16F1939,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1946",	PIC16F1946,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16F1947",	PIC16F1947,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
#ifdef PICLF
{"PIC16LF1933",	PIC16LF1933,	4096,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1934",	PIC16LF1934,	4096,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1936",	PIC16LF1936,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1937",	PIC16LF1937,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1938",	PIC16LF1938,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1939",	PIC16LF1939,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1946",	PIC16LF1946,	8192,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1947",	PIC16LF1947,	16384,	256,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
/* */
{"PIC16LF1902",	PIC16LF1902,	2048,	0,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1903",	PIC16LF1903,	4096,	0,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1904",	PIC16LF1904,	4096,	0,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1906",	PIC16LF1906,	8192,	0,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
{"PIC16LF1907",	PIC16LF1907,	8192,	0,	0,	DS41397B,  0x8000, 2, 2, 8,	1},
#endif

{"PIC16F917",	PIC16F917,	8192,	256,	0,	DS41244F,  0x2000, 1, 2, 8,	1},
{"PIC16F916",	PIC16F916,	8192,	256,	0,	DS41244F,  0x2000, 1, 2, 8,	1},
{"PIC16F914",	PIC16F914,	4096,	256,	0,	DS41244F,  0x2000, 1, 2, 8,	1},
{"PIC16F913",	PIC16F913,	4096,	256,	0,	DS41244F,  0x2000, 1, 2, 8,	1},
{"PIC16F946",	PIC16F946,	16384,	256,	0,	DS41244F,  0x2000, 1, 2, 8,	1},

/* These devices have two config words at 8007 & 8008 and two calibration words at 8009 & 800A */
{"PIC12F1822",	PIC12F1822,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 16,	1},
{"PIC16F1823",	PIC16F1823,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 16,	1},
{"PIC16F1826",	PIC16F1826,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 8,	1},
{"PIC16F1827",	PIC16F1827,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 8,	1},
{"PIC16F1824",	PIC16F1824,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16F1828",	PIC16F1828,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16F1825",	PIC16F1825,	8192,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16F1829",	PIC16F1829,	8192,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
#ifdef PICLF
{"PIC12LF1822",	PIC12LF1822,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 16,	1},
{"PIC16LF1823",	PIC16LF1823,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 16,	1},
{"PIC16LF1826",	PIC16LF1826,	2048,	256,	0,	DS41390D,  0x8000, 2, 2, 8,	1},
{"PIC16LF1827",	PIC16LF1827,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 8,	1},
{"PIC16LF1824",	PIC16LF1824,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16LF1828",	PIC16LF1828,	4096,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16LF1825",	PIC16LF1825,	8192,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
{"PIC16LF1829",	PIC16LF1829,	8192,	256,	0,	DS41390D,  0x8000, 2, 2, 32,	1},
#endif
{"(null)",	0,		0,	0,	0,	0,	   0,      0, 0, 0,	0}
/*Device name	Device id	Flash	EEPROM	Data	Data-sheet Configuration Latches*/
};
#define PIC14_SIZE ((sizeof(pic14_map) / sizeof(struct pic14_dsmap)) - 1)

/* Default device (null) */
int pic14_index = PIC14_SIZE;

void
pic14_selector(struct k8048 *k)
{
	int i;
	char *dnames[PIC14_SIZE];

	for (i = 0; i < PIC14_SIZE; ++i) {
		dnames[i] = pic14_map[i].devicename;
	}
	qsort(dnames, PIC14_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC14_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
        printf("Total: %d\n", (int)PIC14_SIZE);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*I/O*************************************************************************/

/*
 * LOAD CONFIGURATION
 *  PC <= CONFIG_LOW
 *
 * X00000 = 0x00
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS40245B-page 5  16F716
 * DS41204H-page 14 12F683
 * DS41332D-page 13 16F726
 * DS41284E-page 8  12F615
 * DS41287D-page 13 16F886
 * DS30324B-page 4  16F73
 * DS41572D-page 11 10F320
 * DS41573C-page 15 16F1507
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline void
pic14_load_configuration(struct k8048 *k, unsigned short word)
{
	io_command_out(k, 0x00);
	io_word_out14(k, word);
}

/*
 * LOAD DATA FOR PROGRAM MEMORY
 *  PC <= CODE_LOW
 *  WR <= word
 *
 * X00010 = 0x02
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS40245B-page 5  16F716
 * DS41204H-page 14 12F683
 * DS41332D-page 13 16F726
 * DS41284E-page 8  12F615
 * DS41287D-page 13 16F886
 * DS30324B-page 4  16F73
 * DS41572D-page 11 10F320
 * DS41573C-page 15 16F1507
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline void
pic14_load_data_for_program_memory(struct k8048 *k, unsigned short word)
{
	io_command_out(k, 0x02);
	io_word_out14(k, word);
}

/*
 * LOAD DATA FOR DATA MEMORY
 *  PC <= DATA_LOW
 *  WR <= word
 *
 * X00011 = 0x03
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS41204H-page 14 12F683
 * DS41287D-page 13 16F886
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline void
pic14_load_data_for_data_memory(struct k8048 *k, unsigned short word)
{
	io_command_out(k, 0x03);
	io_word_out14(k, word);
}

/*
 * INCREMENT ADDRESS
 *  PC <= 1 + PC
 *
 * X00110 = 0x06
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS40245B-page 5  16F716
 * DS41204H-page 14 12F683
 * DS41332D-page 13 16F726
 * DS41284E-page 8  12F615
 * DS41287D-page 13 16F886
 * DS30324B-page 4  16F73
 * DS41572D-page 11 10F320
 * DS41573C-page 15 16F1507
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline void
pic14_increment_address(struct k8048 *k)
{
	io_command_out(k, 0x06);
}

/*
 * READ DATA FROM PROGRAM MEMORY
 *  RETURN (PC)
 *
 * X00100 = 0x04
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS40245B-page 5  16F716
 * DS41204H-page 14 12F683
 * DS41332D-page 13 16F726
 * DS41284E-page 8  12F615
 * DS41287D-page 13 16F886
 * DS30324B-page 4  16F73
 * DS41572D-page 11 10F320
 * DS41573C-page 15 16F1507
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline unsigned short
pic14_read_data_from_program_memory(struct k8048 *k)
{
	io_command_out(k, 0x04);
	return io_word_in14(k);
}

/*
 * READ DATA FROM DATA MEMORY
 *  RETURN (PC)
 *
 * X00101 = 0x05
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88
 * DS41191B-page 4  16F676
 * DS41196G-page 5  16F628A
 * DS39603C-page 7  16F819
 * DS41204H-page 14 12F683
 * DS41287D-page 13 16F886
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825
 */
static inline unsigned char
pic14_read_data_from_data_memory(struct k8048 *k)
{
	io_command_out(k, 0x05);
	return (unsigned char)(io_word_in14(k));
}

/*
 * RESET ADDRESS
 *  PC <= 0
 *
 * X10110 = 0x16
 *
 * DS41332D-page 13 16F726
 * DS41572D-page 11 10F320
 * DS41573C-page 15 16F1507
 * DS41439A-page 11 16F1847
 * DS41397B-page 19 16F1936
 * DS41390D-page 13 16F1825
 */
static inline void
pic14_reset_address(struct k8048 *k)
{
	io_command_out(k, 0x16);
}

/*PROG************************************************************************/

/*
 * BEGIN ERASE / BEGIN ERASE PROGRAMMING
 *  (PC) <= WR
 *
 * 001000 = 0x08
 *
 * DS30189D-page 4 16C84
 * DS30262E-page 4 16F84A  TERA+TPROG(4+4ms) OR 10ms INTERNAL/NO END
 * DS30034D-page 4 16F627  TERA+TPROG(5+8ms)         INTERNAL/NO END
 * DS39025F-page 6 16F872  TERA+TPROG(4+4ms)         INTERNAL/NO END
 * DS39589C-page 7 16F877A TPROG2(10ms)              INTERNAL/NO END
 * DS39607C-page 9 16F88   TPROG2(1ms)               EXTERNAL
 * DS39603C-page 7 16F819  TPROG2(1ms)		     EXTERNAL
 */
static inline void
pic14_begin_erase_programming(struct k8048 *k, int t)
{
	io_command_out(k, 0x08);
	io_usleep(k, t);
}

/*
 * BEGIN PROGRAMMING
 *  (PC) <= WR
 *
 * 001000 = 0x08
 *
 * DS41196G-page 5  16F628A TPROG(4ms)   OR TDPROG(6ms DATA)         INTERNAL/NO END
 * DS41191B-page 4  16F676  TPROG1(3ms)  OR TPROG1(6ms DATA + ERASE) INTERNAL/NO END
 * DS41204H-page 14 12F683  TPROG1(3ms)  OR TPROG1(6ms DATA)         INTERNAL/NO END
 * DS41332D-page 13 16F726  TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 * DS41287D-page 13 16F886  TPROG1(3ms)  OR TPROG1(6ms DATA)         INTERNAL/NO END
 * DS30324B-page 4  16F73   TPROG(1ms)                               EXTERNAL
 * DS41572D-page 11 10F320  TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 * DS41573C-page 15 16F1507 TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 * DS41439A-page 11 16F1847 TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 * DS41397B-page 19 16F1936 TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 * DS41244F-page 12 16F917                                           INTERNAL/NO END
 * DS41390D-page 13 16F1825 TPINT(2.5ms) OR TPINT(5ms CONFIG)        INTERNAL/NO END
 */
static inline void
pic14_begin_programming_001000(struct k8048 *k, int t)
{
	io_command_out(k, 0x08);
	io_usleep(k, t);
}

/*
 * BEGIN PROGRAMMING
 *  (PC) <= WR
 *
 * 011000 = 0x18
 *
 * DS30262E-page 4  16F84A  TPROG(8ms)    INTERNAL/NO END
 * DS30034D-page 4  16F627  TPROG(8ms)    INTERNAL/NO END
 * DS39025F-page 6  16F872  TPROG(4ms)    INTERNAL/NO END
 * DS39589C-page 7  16F877A TPROG1(1ms)   EXTERNAL
 * DS39607C-page 9  16F88   TPROG1(1ms)   EXTERNAL
 * DS41191B-page 4  16F676  TPROG2(2ms)   EXTERNAL
 * DS39603C-page 7  16F819  TPROG1(1ms)   EXTERNAL
 * DS40245B-page 5  16F716  TPROG(2ms)    EXTERNAL
 * DS41204H-page 14 12F683  TPROG2(3ms)   EXTERNAL
 * DS41332D-page 13 16F726  TPEXT(2.1ms)  EXTERNAL
 * DS41284E-page 8  12F615  TPROG(3ms)    EXTERNAL
 * DS41287D-page 13 16F886  TPROG2(2.5ms) EXTERNAL
 * DS41572D-page 11 10F320  TPEXT(2.1ms)  EXTERNAL
 * DS41573C-page 15 16F1507 TPEXT(2.1ms)  EXTERNAL
 * DS41439A-page 11 16F1847 TPEXT(2.1ms)  EXTERNAL
 * DS41397B-page 19 16F1936 TPEXT(2.1ms)  EXTERNAL (NOT CONFIG)
 * DS41244F-page 12 16F917                EXTERNAL
 * DS41390D-page 13 16F1825 TPEXT(2.1ms)  EXTERNAL
 */
static inline void
pic14_begin_programming_011000(struct k8048 *k, int t)
{
	io_command_out(k, 0x18);
	io_usleep(k, t);
}

/*
 * END PROGRAMMING
 *
 * 010111 = 0x17
 *
 * DS39589C-page 7 16F877A NDELAY
 * DS39607C-page 9 16F88   NDELAY
 * DS39603C-page 7 16F819  NDELAY
 */
static inline void
pic14_end_programming_010111(struct k8048 *k, int t)
{
	io_command_out(k, 0x17);
}

/*
 * END PROGRAMMING
 *
 * 001110 = 0x0E
 *
 * DS40245B-page 5 16F716 TDIS(100us)
 * DS30324B-page 4 16F73  NDELAY
 */
static inline void
pic14_end_programming_001110(struct k8048 *k, int t)
{
	io_command_out(k, 0x0E);
	io_usleep(k, t);
}

/*
 * END PROGRAMMING
 *
 * 001010 = 0x0A
 *
 * DS41191B-page 4  16F676  TDIS(0.5us)
 * DS41204H-page 14 12F683  TDIS(100us)
 * DS41332D-page 13 16F726  TDIS(100us)
 * DS41284E-page 8  12F615  TDIS(100us)
 * DS41287D-page 13 16F886  TDIS(100us)
 * DS41572D-page 11 10F320  TDIS(100us)
 * DS41573C-page 15 16F1507 TDIS(300us)
 * DS41439A-page 11 16F1847 TDIS(100us)
 * DS41397B-page 19 16F1936 TDIS(300us)
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825 TDIS(300us)
 */
static inline void
pic14_end_programming_001010(struct k8048 *k, int t)
{
	io_command_out(k, 0x0A);
	io_usleep(k, t);
}

/*ERASE***********************************************************************/

/*
 * BULK ERASE PROGRAM MEMORY (PRELOAD 0x3FFF)
 *
 * X01001 = 0x09
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627  TERA(5ms)
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88   TPROG3(10ms)
 * DS41191B-page 4  16F676  TERA(6ms)
 * DS41196G-page 5  16F628A TERA(6ms)
 * DS39603C-page 7  16F819  TPROG3(2ms)
 * DS40245B-page 5  16F716  TERA(6ms)
 * DS41204H-page 14 12F683  TERA(6ms)
 * DS41332D-page 13 16F726  TERAB(5ms)
 * DS41284E-page 8  12F615  TERA(6ms)
 * DS41287D-page 13 16F886  TERA(6ms)
 * DS30324B-page 4  16F73   TERA(30ms)
 * DS41572D-page 11 10F320  TERAB(5ms)
 * DS41573C-page 15 16F1507 TERAB(5ms)
 * DS41439A-page 11 16F1847 TERAB(5ms)
 * DS41397B-page 19 16F1936 TERAB(5ms)
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825 TERAB(5ms)
 */
static inline void
pic14_bulk_erase_program_memory(struct k8048 *k, int t)
{
	io_command_out(k, 0x09);
	io_usleep(k, t);
}

/*
 * BULK ERASE DATA MEMORY (PRELOAD 0x3FFF)
 *
 * X01011 = 0x0B
 *
 * DS30189D-page 4  16C84
 * DS30262E-page 4  16F84A
 * DS30034D-page 4  16F627  TERA(5ms)
 * DS39589C-page 7  16F877A
 * DS39607C-page 9  16F88   TPROG3(10ms)
 * DS41191B-page 4  16F676  TERA(6ms)
 * DS41196G-page 5  16F628A TERA(6ms)
 * DS39603C-page 7  16F819  TPROG3(2ms)
 * DS41204H-page 14 12F683  TERA(6ms)
 * DS41287D-page 13 16F886  TERA(6ms)
 * DS41439A-page 11 16F1847 TERAB(5ms)
 * DS41397B-page 19 16F1936 TERAB(5ms)
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825 TERAB(5ms)
 */
static inline void
pic14_bulk_erase_data_memory(struct k8048 *k, int t)
{
	io_command_out(k, 0x0B);
	io_usleep(k, t);
}

/*
 * BULK ERASE SETUP 1 (TOGGLE SELECT EVEN ROWS)
 *
 * 000001 = 0x01
 *
 * DS30262E-page 10 16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 */
static inline void
pic14_bulk_erase_setup1(struct k8048 *k)
{
	io_command_out(k, 0x01);
}

/*
 * BULK ERASE SETUP 2 (TOGGLE SELECT EVEN ROWS)
 *
 * 000111 = 0x07
 *
 * DS30262E-page 10 16F84A
 * DS30034D-page 4  16F627
 * DS39025F-page 6  16F872
 */
static inline void
pic14_bulk_erase_setup2(struct k8048 *k)
{
	io_command_out(k, 0x07);
}

/*
 * CHIP ERASE (PRELOAD 0x3FFF)
 *
 * X11111 = 0x1F
 *
 * DS39589C-page 7 16F877A TPROG3(10ms)
 * DS39607C-page 9 16F88   TPROG4(10ms)
 * DS39603C-page 7 16F819  TPROG4(8ms)
 */
static inline void
pic14_chip_erase(struct k8048 *k, int t)
{
	io_command_out(k, 0x1F);
	io_usleep(k, t);
}

/*
 * ROW ERASE PROGRAM MEMORY
 *
 * X10001 = 0x11
 *
 * DS41204H-page 14 12F683  TERA(6ms)
 * DS41332D-page 13 16F726  TERAR(2.5ms)
 * DS41284E-page 8  12F615  TERA(6ms)
 * DS41287D-page 13 16F886  TERA(6ms)
 * DS41572D-page 11 10F320  TERAR(2.5ms)
 * DS41573C-page 15 16F1507 TERAR(2.5ms)
 * DS41439A-page 11 16F1847 TERAR(2.5ms)
 * DS41397B-page 19 16F1936 TERAR(2.5ms)
 * DS41244F-page 12 16F917
 * DS41390D-page 13 16F1825 TERAR(2.5ms)
 */
static inline void
pic14_row_erase_program_memory(struct k8048 *k, int t)
{
#if 0
	switch (pic14_map[pic14_index].datasheet) {
	case DS41204G:  /* PIC12F6XX/16F6XX     */
	case DS41332D:	/* PIC16F726            */
	case DS41284E:	/* PIC12F615            */
	case DS41287D:  /* PIC16F887            */
	case DS41572D:	/* PIC10F320            */
	case DS41573C:  /* PIC16F1507           */
	case DS41439A:	/* PIC16F1847           */
	case DS41397B:	/* PIC16F1936           */
	case DS41244F:	/* PIC16F917            */
 	case DS41390D:	/* PIC16F1825           */
	}
#endif
	io_command_out(k, 0x11);
	io_usleep(k, t);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * READ PROGRAM MEMORY
 *
 *  RETURN CODE WORD, INCREMENT PC
 */
unsigned short
pic14_read_flash_memory_increment(struct k8048 *k)
{
	unsigned short data = pic14_read_data_from_program_memory(k);

	pic14_increment_address(k);

	return data;
}

/*
 * READ DATA EEPROM MEMORY
 *
 *  RETURN DATA BYTE, INCREMENT PC
 */
unsigned char
pic14_read_eeprom_memory_increment(struct k8048 *k)
{
	unsigned char data = pic14_read_data_from_data_memory(k);

	pic14_increment_address(k);

	return data;
}

/*
 * BULK ERASE (DISABLE PROTECTION AND ERASE DEVICE)
 *
 * SAVE OSCCAL (DEPENDANT UPON DEVICE AND OPTION)
 * DISABLE PROTECTION AND ERASE DEVICE
 * RESTORE OSCCAL (DEPENDANT UPON DEVICE AND OPTION)
 */
void
pic14_bulk_erase(struct k8048 *k, unsigned short osccal, unsigned short bandgap)
{
	int i;

	if (pic14_map[pic14_index].datasheet != DS41191C) {
		if (osccal != INTERNAL && osccal != NOINTERNAL) {
			printf("%s: error: OSCCAL is not supported on this device\n",
				__func__);
			return;
		}
		if (bandgap != INTERNAL && bandgap != NOINTERNAL) {
			printf("%s: error: BANDGAP is not supported on this device\n",
				__func__);
			return;
		}
	} else {
		if (osccal == INTERNAL) {
			osccal = pic14_read_osccal(k);
		}
		if (bandgap == INTERNAL) {
			bandgap = pic14_conf.index[PIC14_CONFIG_WORD1];
		}
	}

	io_init_program_verify(k);

	switch (pic14_map[pic14_index].datasheet) {
	case DS30262E:  /* PIC16F83/84/84A */
	case DS30034D:  /* PIC16F62X       */
	case DS39025F:  /* PIC16F87X       */
			/* DISABLE PROTECTION & ERASE PROGRAM FLASH */
			pic14_load_configuration(k, 0x3fff);
			for (i = 0; i < 7; i++)
				pic14_increment_address(k);
			pic14_bulk_erase_setup1(k);
			pic14_bulk_erase_setup2(k);
			pic14_begin_erase_programming(k, PIC14_TERASE_DEFAULT);
			pic14_bulk_erase_setup1(k);
			pic14_bulk_erase_setup2(k);
			/* ERASE DATA EEPROM */
			pic14_load_data_for_data_memory(k, 0x3fff);
			if (pic14_conf.index[PIC14_CONFIG_DEVICEID] == PIC16F83 ||
				pic14_conf.index[PIC14_CONFIG_DEVICEID] == PIC16F84) {
				pic14_bulk_erase_setup1(k);
				pic14_bulk_erase_setup2(k);
				pic14_begin_erase_programming(k, PIC14_TERASE_DEFAULT);
				pic14_bulk_erase_setup1(k);
				pic14_bulk_erase_setup2(k);
			} else {
				pic14_bulk_erase_data_memory(k, PIC14_TERASE_DEFAULT);
				pic14_begin_programming_011000(k, PIC14_TPROG_DEFAULT);
			}
			break;

	case DS39589B:  /* PIC16F87XA    */
	case DS39603C:  /* PIC16F818/819 */
	case DS39607B:  /* PIC16F87/88   */
			pic14_load_configuration(k, 0x3fff);
			pic14_chip_erase(k, PIC14_TERASE_DEFAULT);
			break;

			/* HAS DATA EEPROM      */
	case DS41196F:  /* PIC16F627A/628A/648A */
	case DS41287D:  /* PIC16F887            */
	case DS41204G:  /* PIC12F6XX/16F6XX     */
	case DS41191C:  /* PIC12F629/675        */
			/* PIC16F630/676        */
	case DS41439A:	/* PIC16F1847           */
	case DS41397B:	/* PIC16F1936           */
	case DS41244F:	/* PIC16F917            */
 	case DS41390D:	/* PIC16F1825           */
			pic14_load_configuration(k, 0x3fff);
			/* DISABLE PROTECTION & ERASE PROGRAM FLASH */
			pic14_bulk_erase_program_memory(k, PIC14_TERASE_DEFAULT);
			/* ERASE DATA EEPROM */
			pic14_bulk_erase_data_memory(k, PIC14_TERASE_DEFAULT);
			break;

			/* HAS NO DATA EEPROM            */
	case DS40245B:  /* PIC16F716  DEFAULT ERASE TIME */
	case DS41332D:	/* PIC16F726                     */
	case DS41284E:	/* PIC12F615                     */
	case DS41572D:	/* PIC10F320                     */
	case DS41573C:  /* PIC16F1507                    */
			pic14_load_configuration(k, 0x3fff);
			/* ERASE PROGRAM FLASH */
			pic14_bulk_erase_program_memory(k, PIC14_TERASE_DEFAULT);
			break;
	case DS30324B:  /* PIC16F73   30MS ERASE TIME    */
			pic14_load_configuration(k, 0x3fff);
			/* DISABLE PROTECTION & ERASE PROGRAM FLASH */
			pic14_bulk_erase_program_memory(k, PIC14_TERASE_30MS);
			break;

	default:	printf("%s: information: unimplemented\n", __func__);
			break;
	}

	io_standby(k);

	if (pic14_map[pic14_index].datasheet == DS41191C) {
		if (osccal != NOINTERNAL) {
			pic14_write_osccal(k, osccal);
		}
		if (bandgap != NOINTERNAL) {
			bandgap = (bandgap & BANDGAPMASK) | CONFIGMASK;
			pic14_write_config(k, bandgap);
		}
	}
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY 2000/8000 .. 200X/800X AND STORE DEVICE MAP INDEX
 */
void
pic14_read_config_memory(struct k8048 *k)
{
	unsigned short deviceid, fault;
	int i;

	io_init_program_verify(k);

	pic14_load_configuration(k, 0);
	for (i = 0; i < PIC14_CONFIG_SIZE; i++) {
		pic14_conf.index[i] = pic14_read_flash_memory_increment(k);
	}

	io_standby(k);

	if (k->devicename[0]) {
		for (i = 0; pic14_map[i].deviceid; i++) {
			if (strcasecmp(pic14_map[i].devicename, k->devicename) == 0) {
				pic14_conf.index[PIC14_CONFIG_DEVICEID] = pic14_map[i].deviceid;
				break;
			}
		}
		if (pic14_map[i].deviceid == 0) {
			printf("%s: fatal error: unknown device: [%s]\n", __func__, k->devicename);
			exit(EX_SOFTWARE); /* Panic */
		}
	} else {
		deviceid = pic14_conf.index[PIC14_CONFIG_DEVICEID] & PIC14_DEVICEID_MASK;
		for (i = 0; pic14_map[i].deviceid; i++) {
			if (pic14_map[i].deviceid == deviceid) {
				break;
			}
		}
		if (pic14_map[i].deviceid == 0) {
			/*
			 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
			 * VELLEMAN K8048 NO POWER          [3FFF]
			 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
			 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
			 */
			fault = pic14_conf.index[PIC14_CONFIG_DEVICEID];
			if (fault == 0x0000 || fault == 0x3FFF) {
				printf("%s: fatal error: %s.\n",
					__func__, io_fault(k, fault));
			} else {
				printf("%s: fatal error: device unknown: %04X [%04X]\n",
					__func__, deviceid, fault);
			}
			exit(EX_SOFTWARE); /* Panic */
		}
	}

	pic14_index = i; /* Device recognised */
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
int
pic14_get_program_flash_size(void)
{
	return pic14_map[pic14_index].flash;
}

/*
 * GET DATA FLASH SIZE
 *
 *  RETURN SIZE IN WORDS (BYTES)
 */
int
pic14_get_data_flash_size(void)
{
	return pic14_map[pic14_index].data;
}

/*
 * GET DATA EEPROM SIZE
 *
 *  RETURN SIZE IN BYTES
 */
int
pic14_get_data_eeprom_size(void)
{
	return pic14_map[pic14_index].eeprom;
}

/*
 * READ PROGRAM FLASH OR DATA FLASH MEMORY BLOCK 0000 .. 0000 + SIZE
 *
 *  RETURN BASE ADDRESS
 */
unsigned short
pic14_read_flash_memory_block(struct k8048 *k, unsigned short *block, int size)
{
	int i;

	io_init_program_verify(k);

	pic14_load_data_for_program_memory(k, 0);
	for (i = 0; i < size; i++)
		block[i] = pic14_read_flash_memory_increment(k);

	io_standby(k);

	return PIC14_CODE_LOW;
}

/*
 * READ DATA EEPROM MEMORY BLOCK 2100 .. 2100 + SIZE or F000 .. F000 + SIZE
 *
 *  RETURN BASE ADDRESS
 */
unsigned short
pic14_read_eeprom_memory_block(struct k8048 *k, unsigned char *block, int size)
{
	int i;
	unsigned short dataaddr = (pic14_map[pic14_index].configaddr == PIC14_CONFIG_MIDRANGE) ?
		PIC14_DATA_MIDRANGE : PIC14_DATA_ENHANCED;

	io_init_program_verify(k);

	pic14_load_data_for_data_memory(k, 0);
	for (i = 0; i < size; i++)
		block[i] = pic14_read_eeprom_memory_increment(k);

	io_standby(k);

	return dataaddr;
}

/*
 * WRITE LATCH OR WRITE LATCHES
 *
 *  RETURN 1
 */
int
pic14_write_word(struct k8048 *k)
{
	switch (pic14_map[pic14_index].datasheet) {
	case DS30262E:  /* PIC16F84A		BEGIN PROGRAMMING ONLY CYCLE */
	case DS30034D:  /* PIC16F62X */
	case DS39025F:  /* PIC16F87X */
			pic14_begin_programming_011000(k, PIC14_TPROG_DEFAULT);
			break;

	case DS39607B:  /* PIC16F87/88		BEGIN PROGRAMMING ONLY CYCLE */
	case DS39589B:  /* PIC16F87XA           BEGIN PROGRAMMING ONLY CYCLE */
	case DS39603C:  /* PIC16F818/819        BEGIN PROGRAMMING ONLY CYCLE */
			pic14_begin_programming_011000(k, PIC14_TPROG_DEFAULT);
			pic14_end_programming_010111(k, PIC14_TDISCHARGE_DEFAULT);
			break;

	case DS40245B:  /* PIC16F716 		BEGIN PROGRAMMING */
			pic14_begin_programming_011000(k, PIC14_TPROG_DEFAULT);
			pic14_end_programming_001110(k, PIC14_TDISCHARGE_DEFAULT);
			break;

	case DS41196F:  /* PIC16F627A/628A/648A BEGIN PROGRAMMING ONLY CYLCE */
	case DS41204G:  /* PIC12F6XX/16F6XX */
	case DS41572D:  /* PIC10F320  */
	case DS41287D:  /* PIC16F887  */
	case DS41332D:  /* PIC16F726  */
	case DS41573C:  /* PIC16F1507 */
	case DS41439A:  /* PIC16F1847 */
	case DS41397B:	/* PIC16F1936 */
	case DS41244F:  /* PIC16F917  */
 	case DS41390D:	/* PIC16F1825           BEGIN PROGRAMMING INTERNAL */
			pic14_begin_programming_001000(k, PIC14_TPROG_DEFAULT);
			break;

	case DS30324B:  /* PIC16F73             BEGIN PROGRAMMING */
			pic14_begin_programming_001000(k, PIC14_TPROG_DEFAULT);
			pic14_end_programming_001110(k, PIC14_TDISCHARGE_DEFAULT);
			break;
			
	case DS41284E:	/* PIC12F615 		BEGIN PROGRAMMING */
	case DS41191C:  /* PIC12F629/675        BEGIN PROGRAMMING */
			/* PIC16F630/676	BEGIN PROGRAMMING */
			pic14_begin_programming_011000(k, PIC14_TPROG_DEFAULT);
			pic14_end_programming_001010(k, PIC14_TDISCHARGE_DEFAULT);
			break;

	default:	printf("%s: information: unimplemented\n", __func__);
			return 0;
			break;
	}
	return 1;
}

/*****************************************************************************
 *
 * Read/Write Oscillator Calibration 
 *
 * DS41191C
 *
 *****************************************************************************/

/*
 * READ OSCILLATOR CALIBRATION WORD
 *
 *  RETURN OSCCAL WORD
 */
unsigned short
pic14_read_osccal(struct k8048 *k)
{
	int i;
	unsigned short osccal;

	if (pic14_map[pic14_index].datasheet != DS41191C) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
		return 0;
	}

	io_init_program_verify(k);

	pic14_load_data_for_program_memory(k, 0);
	for (i = 0; i < OSCCALADDR; i++)
		pic14_increment_address(k);
	osccal = pic14_read_flash_memory_increment(k);

	io_standby(k);

	if (k->debug >= 10) {
		printf("%s: information: OSCCAL read: word [%04X]\n", __func__, osccal);
	}
	return osccal;
}

/*
 * WRITE OSCILLATOR CALIBRATION WORD
 */
int
pic14_write_osccal(struct k8048 *k, unsigned short osccal)
{
	int i;
	unsigned short vdata;

	if (pic14_map[pic14_index].datasheet != DS41191C) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
		return FAIL;
	}

	io_init_program_verify(k);

	if (k->debug >= 10) {
		printf("%s: information: OSCCAL write: word [%04X]\n", __func__, osccal);
	}

	pic14_load_data_for_program_memory(k, osccal);
	for (i = 0; i < OSCCALADDR; i++)
		pic14_increment_address(k);
	pic14_write_word(k);
	vdata = pic14_read_data_from_program_memory(k);

	io_standby(k);

	if (vdata != osccal) {
		printf("%s: error: OSCCAL write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, osccal);
		return FAIL;
	}
	return PASS;
}

/*
 * WRITE CALIBRATION WORD AT X008
 */
int
pic14_write_calib1(struct k8048 *k, unsigned short calib1)
{
	int i;
	unsigned short vdata;

	if (pic14_map[pic14_index].datasheet != DS41204G) {
		printf("%s: error: CALIB1 is not supported on this device\n", __func__);
		return FAIL;
	}

	io_init_program_verify(k);
	
	if (k->debug >= 10) {
		printf("%s: information: CALIB1 write: word [%04X]\n", __func__, calib1);
	}

	pic14_load_configuration(k, calib1);
	for (i = 0; i < 8; i++)
		pic14_increment_address(k);
	pic14_write_word(k);
	vdata = pic14_read_data_from_program_memory(k);

	io_standby(k);
	if (vdata != calib1) {
		printf("%s: error: CALIB1 write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, calib1);
		return FAIL;
	}
	return PASS;
}

/*****************************************************************************
 *
 * Program Config
 *
 *****************************************************************************/

/*
 * WRITE CONFIG
 *
 *  RETURN PROGRAM COUNT WHEN NO ERROR ELSE 0
 */
int
pic14_write_config(struct k8048 *k, unsigned short config1)
{
	int i;
	unsigned short config2 = pic14_conf.index[PIC14_CONFIG_WORD2];
	unsigned short vdata1 = 0, vdata2 = 0;

	io_init_program_verify(k);

	if (k->debug >= 10) {
		printf("%s: information: CONFIG1 write: word [%04X]\n",
			__func__, config1);
	}

	pic14_load_configuration(k, config1);
	for (i = 0; i < 7; i++) {
		pic14_increment_address(k);
	}
	switch (pic14_map[pic14_index].datasheet) {
	case DS39589B:	/* PIC16F87XA		BEGIN ERASE/PROGRAMMING CYCLE */
			pic14_begin_erase_programming(k, PIC14_TERASE_DEFAULT);
			vdata1 = pic14_read_data_from_program_memory(k);
			break;

	case DS30324B:	/* PIC16F73		LOAD DATA FOR PROGRAM MEMORY */
	case DS41196F:	/* PIC16F627A/628A/648A */
			pic14_load_data_for_program_memory(k, config1);
			pic14_write_word(k);
			vdata1 = pic14_read_data_from_program_memory(k);
			break;

			/*
			 * TWO CONFIG WORDS
			 */
	case DS39607B:	/* PIC16F87/88 */
	case DS41287D:	/* PIC16F887   */
	case DS41332D:	/* PIC16F726   */
	case DS41573C:	/* PIC16F1507  */
	case DS41439A:  /* PIC16F1847  */
	case DS41397B:	/* PIC16F1936  */
	case DS41390D:	/* PIC18F1825  */
			pic14_load_data_for_program_memory(k, config1);
			pic14_write_word(k);
			vdata1 = pic14_read_data_from_program_memory(k);
			pic14_increment_address(k);
			if (k->debug >= 10) {
				printf("%s: information: CONFIG2 write: word [%04X]\n",
					__func__, config2);
			}
			pic14_load_data_for_program_memory(k, config2);
			pic14_write_word(k);
			vdata2 = pic14_read_data_from_program_memory(k);
			break;

			/*
			 * ONE CONFIG WORD
			 */
	default:	pic14_write_word(k);
			vdata1 = pic14_read_data_from_program_memory(k);
			break;
	}

	io_standby(k);

	if (pic14_map[pic14_index].datasheet == DS41191C) {
		vdata1 &= CONFIGMASK;
		config1 &= CONFIGMASK;
	}
	if (vdata1 != config1) {
		printf("%s: error: CONFIG1 write failed: read [%04X] expected [%04X]\n",
			__func__, vdata1, config1);
		return 0;
	}
	if (pic14_map[pic14_index].nconfig == 2) {
		if (vdata2 != config2) {
			printf("%s: error: CONFIG2 write failed: read [%04X] expected [%04X]\n",
				__func__, vdata2, config2);
			return 0;
		}
		return 2;
	}
	return 1;
}

/*****************************************************************************
 *
 * Region control
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE, CONFIG or DATA
 */
int
pic14_getregion(unsigned int address)
{
	unsigned short dataaddr = (pic14_map[pic14_index].configaddr == PIC14_CONFIG_MIDRANGE) ?
		PIC14_DATA_MIDRANGE : PIC14_DATA_ENHANCED;

	if (address >= PIC14_CODE_LOW &&
		(address < (PIC14_CODE_LOW + pic14_map[pic14_index].flash)))
		return PIC14_REGIONCODE;

	if (address >= pic14_map[pic14_index].configaddr &&
		(address < (pic14_map[pic14_index].configaddr + PIC14_CONFIG_MAX)))
		return PIC14_REGIONCONFIG;

	if (address >= dataaddr && (address < (dataaddr + PIC14_DATA_MAX)))
		return PIC14_REGIONDATA;

	printf("%s: warning: region unknown [addr=%04X]\n", __func__, address);
	return PIC14_REGIONUNKNOWN;
}

/*
 * INITIALISE FOR REGION
 *
 *  RETURN ADDRESS
 */
unsigned int
pic14_initregion(struct k8048 *k, int region)
{
	unsigned short dataaddr = (pic14_map[pic14_index].configaddr == PIC14_CONFIG_MIDRANGE) ?
		PIC14_DATA_MIDRANGE : PIC14_DATA_ENHANCED;

	switch (region) {
	case PIC14_REGIONCODE:
		pic14_load_data_for_program_memory(k, 0);
		return PIC14_CODE_LOW;
		break;
	case PIC14_REGIONCONFIG:
		pic14_load_configuration(k, 0);
		return pic14_map[pic14_index].configaddr;
		break;
	case PIC14_REGIONDATA:
		pic14_load_data_for_data_memory(k, 0);
		return dataaddr;
		break;
	default:printf("%s: warning: region unknown\n", __func__);
		break;
	}
	return 0;
}

/*
 * LOAD DATA FOR REGION (STORE WORD IN WORKING REGISTER)
 */
void
pic14_loadregion(struct k8048 *k, int region, unsigned short word)
{
	switch (region) {
	case PIC14_REGIONCODE:
	case PIC14_REGIONCONFIG:
		pic14_load_data_for_program_memory(k, word);
		break;
	case PIC14_REGIONDATA:
		pic14_load_data_for_data_memory(k, word);
		break;
	default:printf("%s: warning: region unknown\n", __func__);
		break;
	}
}

/*****************************************************************************
 *
 * Program & verify region
 *
 *****************************************************************************/

/*
 * PROGRAM DATA FOR REGION (CACHE CONFIG WORDS FOR LATER)
 */
void
pic14_programregion(struct k8048 *k, unsigned short address, int region, unsigned short data)
{ 
	/*
	 * Cache config word(s)
	 */
	if (region == PIC14_REGIONCONFIG &&
		(address >= (pic14_map[pic14_index].configaddr + PIC14_CONFIG_DEVICEID))) {
		int ofs = address - pic14_map[pic14_index].configaddr;
		if (ofs == PIC14_CONFIG_WORD1) {
			if (pic14_map[pic14_index].datasheet == DS41191C) {
				data = (data & CONFIGMASK) | (pic14_conf.index[PIC14_CONFIG_WORD1] & BANDGAPMASK);
			}
			if (k->debug >= 10) {
				printf("%s: information: CONFIG1 cache: word [%04X]\n", __func__, data);
			}
			pic14_conf.index[PIC14_CONFIG_WORD1] = data;
			return;
		}
		if (ofs == PIC14_CONFIG_WORD2 && pic14_map[pic14_index].nconfig == 2) {
			if (k->debug >= 10) {
				printf("%s: information: CONFIG2 cache: word [%04X]\n", __func__, data);
			}
			pic14_conf.index[PIC14_CONFIG_WORD2] = data;
			return;
		}
		printf("%s: warning: config word ignored: word [%04X] address [%04X]\n",
			__func__, data, address);
		return;
	}
	
	/*
	 * Ignore OSCCAL
	 *
	 * OSCCAL address is on a multi-word boundary but the devices are not multi-word
	 */
	if (pic14_map[pic14_index].datasheet == DS41191C) {
		if (address == OSCCALADDR) {
			printf("%s: warning: OSCCAL word ignored: word [%04X] address [%04X]\n",
				__func__, data, address);
			return;
		}
	}

	/*
	 * Write multi-word code
	 */
	static int write_pending = 0;
	int multiword = (pic14_map[pic14_index].latches > 1);
	if (multiword && region == PIC14_REGIONCODE) {
		if (data != MW_NODATA) {
			pic14_loadregion(k, region, data);
			write_pending = 1;
		}
		unsigned int mask = pic14_map[pic14_index].latches - 1;
		if ((address & mask) == mask) {
			if (write_pending) {
				pic14_write_word(k);
				write_pending = 0;
			}
		}
		return;
	}
	
	/*
	 * Write single word code, data or userid
	 */

	/* Store data in working register */
	pic14_loadregion(k, region, data);

	/* Program working register */
	pic14_write_word(k);
}

/*
 * VERIFY DATA FOR REGION
 */
int
pic14_verifyregion(struct k8048 *k, unsigned short address, int region, unsigned short data)
{
	unsigned short vdata;

	switch (region) {
	case PIC14_REGIONCODE:
	case PIC14_REGIONCONFIG:
		vdata = pic14_read_data_from_program_memory(k);
		break;
	case PIC14_REGIONDATA:
		vdata = pic14_read_data_from_data_memory(k);
		break;
	default:printf("%s: warning: region unknown\n", __func__);
		return FAIL;
	}
	if (pic14_map[pic14_index].datasheet == DS41191C) {
		if (address == BANDGAPADDR) {
			vdata &= CONFIGMASK;
			data &= CONFIGMASK;
		}
	}
	if (vdata != data) {
		printf("%s: error: read [%04X] expected [%04X] at [%04X]\n",
			__func__, vdata, data, address);
		return FAIL;
	}
	return PASS;
}

/*****************************************************************************
 *
 * Program & verify file
 *
 *****************************************************************************/

/*
 * PROGRAM DEVICE USING GLOBAL INHX32 DATA
 */
void
pic14_program(struct k8048 *k, int blank)
{
	int i, j;
	unsigned short hex_address, PC_address = 0, wdata;
	int new_region, current_region = PIC14_REGIONUNKNOWN;
	int total = 0;
	int multiword = (pic14_map[pic14_index].latches > 1);

	/* Initialise device for programming */
	if (blank)
		pic14_bulk_erase(k, INTERNAL, NOINTERNAL);

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		hex_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic14_getregion(hex_address);
		if (new_region == PIC14_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			if (multiword && current_region == PIC14_REGIONCODE)
				pic14_programregion(k, MW_FLUSH, PIC14_REGIONCODE, MW_NODATA);
			io_init_program_verify(k);  /* Reset P.C. */
			PC_address = pic14_initregion(k, new_region);
			current_region = new_region;
		}

		/* Skip over unused P.C. locations */
		while (hex_address > PC_address) {
			if (multiword && current_region == PIC14_REGIONCODE)
				pic14_programregion(k, PC_address, PIC14_REGIONCODE, MW_NODATA);
			PC_address++;
			pic14_increment_address(k);
		}

		/* For each word in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j += 2) {
			wdata = inhx32_pdata[i]->bytes[j] | (inhx32_pdata[i]->bytes[j + 1] << 8);
			wdata &= MASK14BIT;
			pic14_programregion(k, PC_address, current_region, wdata);
			PC_address++;
			pic14_increment_address(k);
			total++;
		}
	}
	if (multiword && current_region == PIC14_REGIONCODE)
		pic14_programregion(k, MW_FLUSH, PIC14_REGIONCODE, MW_NODATA);

	io_standby(k);

	/* Finalise device programming (write config word(s)) */
	if (blank)
		pic14_write_config(k, pic14_conf.index[PIC14_CONFIG_WORD1]);

	printf("Total: %d\n", total);

	if (pic14_map[pic14_index].datasheet == DS40245B) {
		printf("%s: information: The %s must be power cycled before verification.\n",
			__func__, pic14_map[pic14_index].devicename);
	}
}

/*
 * VERIFY DEVICE USING GLOBAL INHX32 DATA
 */
int
pic14_verify(struct k8048 *k)
{
	int i, j;
	unsigned short hex_address, PC_address = 0, wdata;
	int new_region, current_region = PIC14_REGIONUNKNOWN;
	int fail = 0, total = 0;

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		hex_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic14_getregion(hex_address);
		if (new_region == PIC14_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			io_init_program_verify(k); /* Reset P.C. */
			PC_address = pic14_initregion(k, new_region);
			current_region = new_region;
		}

		/* Skip over unused P.C. locations */
		while (hex_address > PC_address) {
			PC_address++;
			pic14_increment_address(k);
		}

		/* For each word in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j += 2) {
			wdata = inhx32_pdata[i]->bytes[j] | (inhx32_pdata[i]->bytes[j + 1] << 8);
			wdata &= MASK14BIT;
			fail += pic14_verifyregion(k, PC_address++, current_region, wdata);
			pic14_increment_address(k);
			total++;
		}
	}
	io_standby(k);

	printf("Total: %d Pass: %d Fail: %d\n", total, total-fail, fail);
	return fail;
}

/*****************************************************************************
 *
 * Diagnostic functions
 *
 *****************************************************************************/

/*
 * DUMP DEVICE ID DETAILS
 */
void
pic14_dumpdeviceid(struct k8048 *k)
{
	int i;
	unsigned char u;
	unsigned short deviceid = pic14_conf.index[PIC14_CONFIG_DEVICEID] & PIC14_DEVICEID_MASK;
	unsigned short dataaddr = (pic14_map[pic14_index].configaddr == PIC14_CONFIG_MIDRANGE) ?
		PIC14_DATA_MIDRANGE : PIC14_DATA_ENHANCED;
	
	printf("[%04X] [PROGRAM]  %04X WORDS\n",
		PIC14_CODE_LOW, pic14_map[pic14_index].flash);
	if (pic14_map[pic14_index].datasheet == DS41191C)
		pic14_dumposccal(k);
	for (i = 0; i < 6; i++) {
		if (i < 4) {
			printf("[%04X] [USERID%d]  ",
				pic14_map[pic14_index].configaddr + i, i);
		} else {
			printf("[%04X] [RESERVED] ",
				pic14_map[pic14_index].configaddr + i);
		}
		u = pic14_conf.index[i] & 0xff;
		printf("%04X %c\n", pic14_conf.index[i],
			(u >= 32 && u < 127) ? u : '.');
	}
	printf("[%04X] [DEVICEID] %04X DEV:%X (%X) REV:%X",
		pic14_map[pic14_index].configaddr + PIC14_CONFIG_DEVICEID,
		pic14_conf.index[PIC14_CONFIG_DEVICEID],
		deviceid,
		deviceid >> 5,
		pic14_conf.index[PIC14_CONFIG_DEVICEID] & PIC14_DEVICEREV_MASK);
	printf(" %s\n", pic14_map[pic14_index].devicename);
	pic14_dumpconfig(k, BRIEF);
	if (pic14_map[pic14_index].eeprom > 0) {
		printf("[%04X] [DATA]     %04X BYTES\n", dataaddr,
			pic14_map[pic14_index].eeprom);
	}
}

/*
 * DUMP OSCCAL DETAILS
 */
void
pic14_dumposccal(struct k8048 *k)
{
	if (pic14_map[pic14_index].datasheet != DS41191C) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
	} else {
		printf("[%04X] [OSCCAL]   %04X\n", OSCCALADDR, pic14_read_osccal(k));
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic14_dumpconfig(struct k8048 *k, int mode)
{
	int i, ofs = PIC14_CONFIG_WORD1;
	unsigned short addr = pic14_map[pic14_index].configaddr + ofs;

	/* BRIEF OR VERBOSE */
	if (pic14_map[pic14_index].nconfig == 1) {
		printf("[%04X] [CONFIG]   %04X\n",
			addr++, pic14_conf.index[ofs++]);
	} else for (i = 0; i < pic14_map[pic14_index].nconfig; ++i) {
		printf("[%04X] [CONFIG%d]  %04X\n",
			addr++, i + 1, pic14_conf.index[ofs++]);
	}
	if (pic14_map[pic14_index].ncalib == 1) {
		printf("[%04X] [CALIB]    %04X\n",
			addr++, pic14_conf.index[ofs++]);
	} else for (i = 0; i < pic14_map[pic14_index].ncalib; ++i) {
		printf("[%04X] [CALIB%d]   %04X\n",
			addr++, i + 1, pic14_conf.index[ofs++]);
	}
	/* VERBOSE */
	if (mode == VERBOSE) switch (pic14_map[pic14_index].datasheet) {
	case DS30034D:	pic14_dumpconfig_16f627(pic14_conf.index[PIC14_CONFIG_WORD1]);
			break;
	case DS39589B:	pic14_dumpconfig_16f877a(pic14_conf.index[PIC14_CONFIG_WORD1]);
			break;
	case DS41191C:	pic14_dumpconfig_12f675(pic14_conf.index[PIC14_CONFIG_WORD1]);
			break;
	case DS39607B:	pic14_dumpconfig_16f88(pic14_conf.index[PIC14_CONFIG_WORD1],
				pic14_conf.index[PIC14_CONFIG_WORD2]);
			break;
	case DS41196F:	pic14_dumpconfig_16f628a(pic14_conf.index[PIC14_CONFIG_WORD1]);
			break;
	case DS41204G:	pic14_dumpconfig_12f683(pic14_conf.index[PIC14_CONFIG_WORD1]);
			break;
	case DS41287D:	pic14_dumpconfig_16f886(pic14_conf.index[PIC14_CONFIG_WORD1],
				pic14_conf.index[PIC14_CONFIG_WORD2]);
			break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC16F627
 */
void
pic14_dumpconfig_16f627(unsigned int config)
{
	unsigned short w;

	w = (config & 0x3000) >> 12;
	printf("\nCode protection for 2K program memory:\n");
	switch (w) {
	case 0: printf("\t[CP] 0000h -> 07FFh code protected\n"); break;
	case 1: printf("\t[CP] 0200h -> 07FFh code protected\n"); break;
	case 2: printf("\t[CP] 0400h -> 07FFh code protected\n"); break;
	case 3: printf("\t[CP] Program memory code protection off\n"); break;
	}

	w = (config & 0x0c00) >> 10;
	printf("\nCode protection for 1K program memory:\n");
	switch (w) {
	case 0: printf("\t[CP] 0000h -> 03FFh code protected\n"); break;
	case 1: printf("\t[CP] 0200h -> 03FFh code protected\n"); break;
	case 2: printf("\t[CP] Program memory code protection off\n"); break;
	case 3: printf("\t[CP] Program memory code protection off\n"); break;
	}

	w = config & 0x100;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");

	w = config & 0x80;
	if (w)
		printf("\n[LVP] Low voltage programming enabled\n");
	else
		printf("\n[LVP] Low voltage programming disabled\n");

	w = config & 0x40;
	if (w)
		printf("\n[BODEN] Brown-out detect reset enabled\n");
	else
		printf("\n[BODEN] Brown-out detect reset disabled\n");

	w = config & 0x20;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config & 0x08;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config & 0x04;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = ((config & 0x10) >> 2) | (config & 0x03);
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[ER] 111\n"); break;
	case 6: printf("\t[ER] 110\n"); break;
	case 5: printf("\t[INTRC] 101\n"); break;
	case 4: printf("\t[INTRC] 100\n"); break;
	case 3: printf("\t[EC] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC12F675
 */
void
pic14_dumpconfig_12f675(unsigned int config)
{
	unsigned short w;

	w = (config & 0x3000) >> 12;
	printf("\nBand-gap calibration bits:\n");
	switch (w) {
	case 0: printf("\t[BG1:BG0] 00\n"); break;
	case 1: printf("\t[BG1:BG0] 01\n"); break;
	case 2: printf("\t[BG1:BG0] 10\n"); break;
	case 3: printf("\t[BG1:BG0] 11\n"); break;
	}
	
	w = config & 0x100;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");
	
	w = config & 0x80;
	if (w)
		printf("\n[CP] Program memory code protection off\n");
	else
		printf("\n[CP] Program memory code protection on\n");
	
	w = config & 0x40;
	if (w)
		printf("\n[BODEN] Brown-out detect reset enabled\n");
	else
		printf("\n[BODEN] Brown-out detect reset disabled\n");

	w = config & 0x20;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config & 0x10;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config & 0x08;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = config & 0x07;
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[RC] 111\n"); break;
	case 6: printf("\t[RC] 110\n"); break;
	case 5: printf("\t[INTOSC] 101 CLKOUT\n"); break;
	case 4: printf("\t[INTOSC] 100 NO CLKOUT\n"); break;
	case 3: printf("\t[EC] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC16F877A
 */
void
pic14_dumpconfig_16f877a(unsigned int config)
{
	unsigned short w;

	w = config & 0x2000;
	if (w)
		printf("\n[CP] Code memory code protection off\n");
	else
		printf("\n[CP] Code memory code protection on\n");

	w = config & 0x0800;
	if (w)
		printf("\n[DEBUG] In circuit debugger off\n");
	else
		printf("\n[DEBUG] In circuit debugger on\n");
	
	w = (config & 0x0600) >> 9;
	printf("\nWrite protection:\n");
	switch (w) {
	case 3: printf("\t[WRT1:WRT0] 11 off\n"); break;
	case 2: printf("\t[WRT1:WRT0] 10 0000 => 00ff Protected\n"); break;
	case 1: printf("\t[WRT1:WRT0] 01 0000 => 07ff Protected\n"); break;
	case 0: printf("\t[WRT1:WRT0] 00 0000 => 0fff Protected\n"); break;
	}

	w = config & 0x0100;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");

	w = config & 0x80;
	if (w)
		printf("\n[LVP] Low voltage programming enabled\n");
	else
		printf("\n[LVP] Low voltage programming disabled\n");

	w = config & 0x40;
	if (w)
		printf("\n[BOREN] Brown-out reset enabled\n");
	else
		printf("\n[BOREN] Brown-out reset disabled\n");

	w = config & 0x20;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config & 0x08;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config & 0x04;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = config & 0x03;
	printf("\nOscillator selection:\n");
	switch (w) {
	case 3: printf("\t[RC] 11\n"); break;
	case 2: printf("\t[HS] 10\n"); break;
	case 1: printf("\t[XT] 01\n"); break;
	case 0: printf("\t[LP] 00\n"); break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC16F87/8
 */
void
pic14_dumpconfig_16f88(unsigned int config1, unsigned int config2)
{
	unsigned short w;

	w = config1 & 0x2000;
	if (w)
		printf("\n[CP] Code memory code protection off\n");
	else
		printf("\n[CP] Code memory code protection on\n");

	w = config1 & 0x0800;
	if (w)
		printf("\n[DEBUG] In circuit debugger off\n");
	else
		printf("\n[DEBUG] In circuit debugger on\n");

	w = (config1 & 0x0600) >> 9;
	printf("\n[WRT1:WRT0] Write protection: 0x%02X\n", w);

	w = config1 & 0x0100;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");

	w = config1 & 0x80;
	if (w)
		printf("\n[LVP] Low voltage programming enabled\n");
	else
		printf("\n[LVP] Low voltage programming disabled\n");

	w = config1 & 0x40;
	if (w)
		printf("\n[BOREN] Brown-out reset enabled\n");
	else
		printf("\n[BOREN] Brown-out reset disabled\n");

	w = config1 & 0x20;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config1 & 0x08;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config1 & 0x04;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = ((config1 & 0x10) >> 2) | (config1 & 0x03);
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[EXTRC] 111 CLKOUT\n"); break;
	case 6: printf("\t[EXTRC] 110 NO CLKOUT\n"); break;
	case 5: printf("\t[INTRC] 101 CLKOUT\n"); break;
	case 4: printf("\t[INTRC] 100 NO CLKOUT\n"); break;
	case 3: printf("\t[ECIO] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}

	w = config2 & 0x01;
	if (w)
		printf("\n[FCMEN] Fail-safe clock monitor enabled\n");
	else
		printf("\n[FCMEN] Fail-safe clock monitor disabled\n");

	w = config2 & 0x02;
	if (w)
		printf("\n[IESO] Internal/external switch over mode enabled\n");
	else
		printf("\n[IESO] Internal/external switch over mode disabled\n");
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC16F628A
 */
void
pic14_dumpconfig_16f628a(unsigned int config)
{
	unsigned short w;

	w = config & 0x2000;
	if (w)
		printf("\n[CP] Code memory code protection off\n");
	else
		printf("\n[CP] Code memory code protection on\n");

	w = config & 0x0100;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");

	w = config & 0x80;
	if (w)
		printf("\n[LVP] Low voltage programming enabled\n");
	else
		printf("\n[LVP] Low voltage programming disabled\n");

	w = config & 0x40;
	if (w)
		printf("\n[BOREN] Brown-out reset enabled\n");
	else
		printf("\n[BOREN] Brown-out reset disabled\n");

	w = config & 0x20;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config & 0x08;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config & 0x04;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = ((config & 0x10) >> 2) | (config & 0x03);
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[RC] 111 CLKOUT\n"); break;
	case 6: printf("\t[RC] 110 NO CLKOUT\n"); break;
	case 5: printf("\t[INTOSC] 101 CLKOUT\n"); break;
	case 4: printf("\t[INTOSC] 100 NO CLKOUT\n"); break;
	case 3: printf("\t[ECIO] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC12F683
 */
void
pic14_dumpconfig_12f683(unsigned int config)
{
	unsigned short w;

	w = config & 0x0800;
	if (w)
		printf("\n[FCMEN] Fail-safe clock monitor enabled\n");
	else
		printf("\n[FCMEN] Fail-safe clock monitor disabled\n");
	
	w = config & 0x0300;
	printf("\nBrown-out reset selection:\n");
	switch (w) {
	case 3:	printf("\t[BOR] Enabled\n"); break;
	case 2:	printf("\t[BOR] Enabled during operation and disabled in Sleep\n"); break;
	case 1:	printf("\t[BOR] Controlled by SBOREN bit of the PCON register\n"); break;
	case 0: printf("\t[BOR] Disabled\n"); break;
	}

	w = config & 0x0080;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");
	
	w = config & 0x0040;
	if (w)
		printf("\n[CP] Program memory code protection off\n");
	else
		printf("\n[CP] Program memory code protection on\n");
	
	w = config & 0x0020;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config & 0x0010;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config & 0x0008;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = config & 0x07;
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[RC] 111 CLKOUT\n"); break;
	case 6: printf("\t[RCIO] 110 NO CLKOUT\n"); break;
	case 5: printf("\t[INTOSC] 101 CLKOUT\n"); break;
	case 4: printf("\t[INTOSCIO] 100 NO CLKOUT\n"); break;
	case 3: printf("\t[EC] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR PIC16F886
 */
void
pic14_dumpconfig_16f886(unsigned int config1, unsigned int config2)
{
	unsigned short w;
	
	w = config1 & 0x2000;
	if (w)
		printf("\n[DEBUG] In circuit debugger off\n");
	else
		printf("\n[DEBUG] In circuit debugger on\n");

	w = config1 & 0x1000;
	if (w)
		printf("\n[LVP] Low voltage programming enabled\n");
	else
		printf("\n[LVP] Low voltage programming disabled\n");

	w = config1 & 0x0800;
	if (w)
		printf("\n[FCMEN] Fail-safe clock monitor enabled\n");
	else
		printf("\n[FCMEN] Fail-safe clock monitor disabled\n");
	
	w = config1 & 0x0400;
	if (w)
		printf("\n[IESO] Internal/external switch over mode enabled\n");
	else
		printf("\n[IESO] Internal/external switch over mode disabled\n");
	
	w = config1 & 0x0300;
	printf("\nBrown-out reset selection:\n");
	switch (w) {
	case 3:	printf("\t[BOR] Enabled\n"); break;
	case 2:	printf("\t[BOR] Enabled during operation and disabled in Sleep\n"); break;
	case 1:	printf("\t[BOR] Controlled by SBOREN bit of the PCON register\n"); break;
	case 0: printf("\t[BOR] Disabled\n"); break;
	}

	w = config1 & 0x0080;
	if (w)
		printf("\n[CPD] Data memory code protection off\n");
	else
		printf("\n[CPD] Data memory code protection on\n");

	w = config1 & 0x0040;
	if (w)
		printf("\n[CP] Code memory code protection off\n");
	else
		printf("\n[CP] Code memory code protection on\n");

	w = config1 & 0x0020;
	if (w)
		printf("\n[MCLRE] MCLR enabled\n");
	else
		printf("\n[MCLRE] MCLR disabled\n");

	w = config1 & 0x0010;
	if (w)
		printf("\n[PWRTE] Power-up timer disabled\n");
	else
		printf("\n[PWRTE] Power-up timer enabled\n");

	w = config1 & 0x0008;
	if (w)
		printf("\n[WDT] Watchdog timer enabled\n");
	else
		printf("\n[WDT] Watchdog timer disabled\n");

	w = config1 & 0x0007;
	printf("\nOscillator selection:\n");
	switch (w) {
	case 7: printf("\t[RC] 111 CLKOUT\n"); break;
	case 6: printf("\t[RCIO] 110 NO CLKOUT\n"); break;
	case 5: printf("\t[INTOSC] 101 CLKOUT\n"); break;
	case 4: printf("\t[INTOSCIO] 100 NO CLKOUT\n"); break;
	case 3: printf("\t[ECIO] 011\n"); break;
	case 2: printf("\t[HS] 010\n"); break;
	case 1: printf("\t[XT] 001\n"); break;
	case 0: printf("\t[LP] 000\n"); break;
	}

	w = (config2 & 0x0006) >> 1;
	printf("\n[WRT1:WRT0] Write protection: 0x%02X\n", w);
	
	w = config2 & 0x0001;
	if (w)
		printf("\n[BOR4V] Brown-out reset set to 4.0V\n");
	else
		printf("\n[BOR4V] Brown-out reset set to 2.1V\n");
}

/*
 * DUMP DEVICE CONFIGURATION
 */
void
pic14_dumpdevice(struct k8048 *k)
{
	int i;

	/* Enhanced 14-bit: Extended address = 0x0001 */
	if (pic14_map[pic14_index].configaddr >= 0x8000) {
		printf(":020000040001F9\n");
	}
	/* IDLOC  */
	for (i = 0; i < 4; ++i) {
		pic_dumpword(k, pic14_map[pic14_index].configaddr + i,
			pic14_conf.index[i + PIC14_CONFIG_USERID0]);
	}
	/* CONFIG */
	for (i = 0; i < pic14_map[pic14_index].nconfig; ++i) {
		pic_dumpword(k, pic14_map[pic14_index].configaddr + PIC14_CONFIG_WORD1 + i,
			pic14_conf.index[PIC14_CONFIG_WORD1 + i]);
	}
}
