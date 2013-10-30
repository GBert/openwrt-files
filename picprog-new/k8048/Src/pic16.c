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

#include "k8048.h"

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/
struct pic16_config pic16_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/
struct pic16_dsmap pic16_map[] =
{
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panel size P9 P9A P10 P11  Configuration Size        Erase*/
{"PIC18F2331",	PIC18F2331,	4096,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4331",	PIC18F4331,	4096,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2431",	PIC18F2431,	8192,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4431",	PIC18F4431,	8192,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  PIC16_CONFIG, PIC16_CSIZE,64},

{"PIC18F242",	PIC18F242,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F248",	PIC18F248,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F252",	PIC18F252,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F258",	PIC18F258,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F442",	PIC18F442,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F448",	PIC18F448,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F452",	PIC18F452,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F458",	PIC18F458,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},

{"PIC18F1220",	PIC18F1220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F1320",	PIC18F1320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2220",	PIC18F2220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2320",	PIC18F2320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4220",	PIC18F4220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4320",	PIC18F4320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, PIC16_CONFIG, PIC16_CSIZE,64},

{"PIC18F2221",	PIC18F2221,	2048,		256,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2321",	PIC18F2321,	4096,		256,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2410",	PIC18F2410,	8192,		0,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2420",	PIC18F2420,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2423",	PIC18F2423,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2450",	PIC18F2450,	8192,		0,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2455",	PIC18F2455,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2458",	PIC18F2458,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2480",	PIC18F2480,	8192,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2510",	PIC18F2510,	16384,		0,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2515",	PIC18F2515,	24576,		0,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2520",	PIC18F2520,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2523",	PIC18F2523,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2525",	PIC18F2525,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2550",	PIC18F2550,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2553",	PIC18F2553,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2580",	PIC18F2580,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2585",	PIC18F2585,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2610",	PIC18F2610,	32768,		0,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2620",	PIC18F2620,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2680",	PIC18F2680,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2682",	PIC18F2682,	40960,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F2685",	PIC18F2685,	49152,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4221",	PIC18F4221,	2048,		256,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4321",	PIC18F4321,	4096,		256,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4410",	PIC18F4410,	8192,		0,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4420",	PIC18F4420,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4423",	PIC18F4423,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4450",	PIC18F4450,	8192,		0,	DS39622K,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4455",	PIC18F4455,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4458",	PIC18F4458,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4480",	PIC18F4480,	8192,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4510",	PIC18F4510,	16384,		0,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4515",	PIC18F4515,	24576,		0,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4520",	PIC18F4520,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4523",	PIC18F4523,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4525",	PIC18F4525,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4550",	PIC18F4550,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4553",	PIC18F4553,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4580",	PIC18F4580,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4585",	PIC18F4585,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4610",	PIC18F4610,	32768,		0,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4620",	PIC18F4620,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4680",	PIC18F4680,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4682",	PIC18F4682,	40960,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F4685",	PIC18F4685,	49152,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},

{"PIC18F1230",	PIC18F1230,	2048,		128,	DS39752B,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F1330",	PIC18F1330,	4096,		128,	DS39752B,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F1330-ICD",
		PIC18F1330ICD,	4096,		128,	DS39752B,  0,		8,	   1000,0,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},

{"PIC18F45K22",	PIC18F45K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F25K22",	PIC18F25K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F23K22",	PIC18F23K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F24K22",	PIC18F24K22,	8192,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F26K22",	PIC18F26K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F43K22",	PIC18F43K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F46K22",	PIC18F46K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
#ifdef PICLF
{"PIC18LF45K22",PIC18LF45K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF25K22",PIC18LF25K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF23K22",PIC18LF23K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF24K22",PIC18LF24K22,	8192,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF26K22",PIC18LF26K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF43K22",PIC18LF43K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF46K22",PIC18LF46K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
#endif

{"PIC18F65K80",	PIC18F65K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F45K80",	PIC18F45K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F25K80",	PIC18F25K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F66K80",	PIC18F66K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F46K80",	PIC18F46K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F26K80",	PIC18F26K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
#ifdef PICLF
{"PIC18LF65K80",PIC18LF65K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF45K80",PIC18LF45K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF25K80",PIC18LF25K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF66K80",PIC18LF66K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF46K80",PIC18LF46K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF26K80",PIC18LF26K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
#endif

{"PIC18F24K50",	PIC18F24K50,	8192,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F25K50",	PIC18F25K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F45K50",	PIC18F45K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F26K50",	PIC18F26K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18F46K50",	PIC18F46K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
#ifdef PICLF
{"PIC18LF24K50",PIC18LF24K50,	8192,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF25K50",PIC18LF25K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF45K50",PIC18LF45K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF26K50",PIC18LF26K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
{"PIC18LF46K50",PIC18LF46K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,PIC16_CONFIG,PIC16_CSIZE,64},
#endif

{"PIC18F13K50",	PIC18F13K50,	4096,		256,	DS41342E,  0,		8,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18F14K50",	PIC18F14K50,	8192,		256,	DS41342E,  0,		16,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
#ifdef PICLF
{"PIC18LF13K50",PIC18LF13K50,	4096,		256,	DS41342E,  0,		8,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
{"PIC18LF14K50",PIC18LF14K50,	8192,		256,	DS41342E,  0,		16,	   1000,5000,100,5000,PIC16_CONFIG, PIC16_CSIZE,64},
#endif

{"PIC18F24J10",	PIC18F24J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,8,1024},
{"PIC18F44J10",	PIC18F44J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,8,1024},
{"PIC18F25J10",	PIC18F25J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,8,1024},
{"PIC18F45J10",	PIC18F45J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,8,1024},
{"PIC18F24J11",	PIC18F24J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18F44J11",	PIC18F44J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18F25J11",	PIC18F25J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18F45J11",	PIC18F45J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18F26J11",	PIC18F26J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18F46J11",	PIC18F46J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18F26J13",	PIC18F26J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18F46J13",	PIC18F46J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18F27J13",	PIC18F27J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18F47J13",	PIC18F47J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18F24J50",	PIC18F24J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18F44J50",	PIC18F44J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18F25J50",	PIC18F25J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18F45J50",	PIC18F45J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18F26J50",	PIC18F26J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18F46J50",	PIC18F46J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18F26J53",	PIC18F26J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18F46J53",	PIC18F46J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18F27J53",	PIC18F27J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18F47J53",	PIC18F47J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
#ifdef PICLF
{"PIC18LF24J10",PIC18LF24J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,8,1024},
{"PIC18LF44J10",PIC18LF44J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,8,1024},
{"PIC18LF25J10",PIC18LF25J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,8,1024},
{"PIC18LF45J10",PIC18LF45J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,8,1024},
{"PIC18LF24J11",PIC18LF24J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18LF44J11",PIC18LF44J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18LF25J11",PIC18LF25J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18LF45J11",PIC18LF45J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18LF26J11",PIC18LF26J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18LF46J11",PIC18LF46J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18LF26J13",PIC18LF26J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18LF46J13",PIC18LF46J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18LF27J13",PIC18LF27J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18LF47J13",PIC18LF47J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18LF24J50",PIC18LF24J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18LF44J50",PIC18LF44J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,8,1024},
{"PIC18LF25J50",PIC18LF25J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18LF45J50",PIC18LF45J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,8,1024},
{"PIC18LF26J50",PIC18LF26J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18LF46J50",PIC18LF46J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,8,1024},
{"PIC18LF26J53",PIC18LF26J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18LF46J53",PIC18LF46J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,8,1024},
{"PIC18LF27J53",PIC18LF27J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
{"PIC18LF47J53",PIC18LF47J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,8,1024},
#endif
{"(null)",	0,		0,		0,	0,	   0,		0,	   0, 0,  0,  0,   0,            0,          0}
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panel size P9 P9A P10 P11  Configuration Size        Erase*/
};
#define PIC16_SIZE ((sizeof(pic16_map) / sizeof(struct pic16_dsmap)) - 1)

/* Default device (null) */
unsigned int pic16_index = PIC16_SIZE;

void
pic16_selector(struct k8048 *k)
{
	int i;
	char *dnames[PIC16_SIZE];

	for (i = 0; i < PIC16_SIZE; ++i) {
		dnames[i] = pic16_map[i].devicename;
	}
	qsort(dnames, PIC16_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC16_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
        printf("Total: %d\n", (int)PIC16_SIZE);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*
 * 0000
 *
 * CORE INSTRUCTION (SHIFT IN 16 BIT INSTRUCTION)
 */
static inline void
pic16_core_instruction(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x00, word);
}

/*
 * 0000
 *
 * CORE INSTRUCTION (PROGRAMMING NOP)
 *
 * DS39752B-page 32 PIC18F1330   HIGH=P9(1ms)          LOW=P10(100us)
 * DS39576B-page 34 PIC18F252    HIGH=P9(1ms)          LOW=P10(5us)
 * DS41398B-page 38 PIC18F25K22  HIGH=P9(1ms)/P9A(5ms) LOW=P10(200us)
 * DS39972B-page 47 PIC18F26K80  HIGH=P9(1ms)/P9A(5ms) LOW=P10(100us)
 * DS39687E-page 30 PIC18LF27J53 HIGH=P9(3.4ms)	       LOW=NONE
 */
static inline void
pic16_core_instruction_nopp(struct k8048 *k, unsigned int h, unsigned int l)
{
	io_command_program(k, h, l);
}

/*
 * 0000
 *
 * CORE INSTRUCTION (ERASE NOP)
 *
 * DS39752B-page 32 PIC18F1330   P11(5ms)   + P10(100us)
 * DS39576C-page 34 PIC18F252    P11(10ms)  + P10(5us)
 * DS41398B-page 38 PIC18F25K22  P11(15ms)  + P10(200us)
 * DS39972B-page 48 PIC18F26K80  P11(5ms)   + P10(100us)
 * DS39687E-page 30 PIC18LF27J53 P11(524ms) + P10(54ms)
 */
static inline void
pic16_core_instruction_nope(struct k8048 *k)
{
	io_command_erase(k, pic16_map[pic16_index].p10, pic16_map[pic16_index].p11);
}

/*
 * 0010 = 0x02
 *
 * SHIFT OUT TABLAT REGISTER
 */
static inline unsigned char
pic16_shift_out_tablat_register(struct k8048 *k)
{
	return io_command_in8(k, 0x02);
}

/*
 * 1000 = 0x08
 *
 * TABLE READ
 */
static inline unsigned char
pic16_table_read(struct k8048 *k)
{
	return io_command_in8(k, 0x08);
}

/*
 * 1001 = 0x09
 *
 * TABLE READ POST INCREMENT
 */
static inline unsigned char
pic16_table_read_post_increment(struct k8048 *k)
{
	return io_command_in8(k, 0x09);
}

/*
 * 1010 = 0x0A
 *
 * TABLE READ POST DECREMENT
 */
static inline unsigned char
pic16_table_read_post_decrement(struct k8048 *k)
{
	return io_command_in8(k, 0x0A);
}

/*
 * 1011 = 0x0B
 *
 * TABLE READ PRE INCREMENT
 */
static inline unsigned char
pic16_table_read_pre_increment(struct k8048 *k)
{
	return io_command_in8(k, 0x0B);
}

/*
 * 1100 = 0x0C
 *
 * TABLE WRITE
 */
static inline void
pic16_table_write(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x0C, word);
}

/*
 * 1101 = 0x0D
 *
 * TABLE WRITE POST INCREMENT BY 2
 */
static inline void
pic16_table_write_post_increment_2(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x0D, word);
}

/*
 * 1110 = 0x0E
 *
 * TABLE WRITE POST DECREMENT BY 2
 */
static inline void
pic16_table_write_post_decrement_2(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x0E, word);
}

/*
 * 1110 = 0x0E
 *
 * TABLE WRITE START PROGRAMMING POST INCREMENT BY 2
 *
 * DS41398B-page 12
 */
static inline void
pic16_table_write_start_programming_2(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x0E, word);
}

/*
 * 1111 = 0x0F
 *
 * TABLE WRITE START PROGRAMMING
 */
static inline void
pic16_table_write_start_programming(struct k8048 *k, unsigned short word)
{
	io_command_out16(k, 0x0F, word);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * INIT DIRECT ACCESS TO CODE MEMORY (FLASH)
 */
void
pic16_init_code_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * INIT DIRECT ACCESS TO CONFIG MEMORY (FLASH)
 */
void
pic16_init_config_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x8C00 | eecon1);	/* BSF EECON1, CFGS	*/
}

/*
 * INIT DIRECT ACCESS TO DATA MEMORY/EEPROM
 */
void
pic16_init_data_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x9E00 | eecon1);	/* BCF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * WRITE ENABLE
 */
void
pic16_write_enable(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x847F);	/* BSF EECON1, WREN	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x84A6);	/* BSF EECON1, WREN	*/
		break;
	}
}

/*
 * WRITE DISABLE
 */
void
pic16_write_disable(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x947F);	/* BCF EECON1, WREN	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x94A6);	/* BCF EECON1, WREN	*/
		break;
	}
}

/*
 * FLASH ROW ERASE ENABLE
 */
void
pic16_free(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x887F);	/* BSF EECON1, FREE	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x88A6);	/* BSF EECON1, FREE	*/
		break;
	}
}

/*
 * WRITE
 */
void
pic16_write(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x827F);	/* BSF EECON1, WR	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x82A6);	/* BSF EECON1, WR	*/
		break;
	}
}

/*
 * SET TABLE POINTER
 */
void
pic16_set_table_pointer(struct k8048 *k, unsigned int address)
{
	unsigned char addrl =  (address & 0x000000ff);		/*  7:0	 */
	unsigned char addrh = ((address & 0x0000ff00) >> 8);	/* 15:8	 */
	unsigned char addru = ((address & 0x003f0000) >> 16);	/* 21:16 */

	/* TBLPTRU @ 0x0FF8 */
	pic16_core_instruction(k, 0x0E00 | addru);	/* MOVLW <Addr[21:16]>	*/
	pic16_core_instruction(k, 0x6EF8);		/* MOVWF TBLPTRU	*/
	/* TBLPTRH @ 0x0FF7 */
	pic16_core_instruction(k, 0x0E00 | addrh);	/* MOVLW <Addr[15:8]>	*/
	pic16_core_instruction(k, 0x6EF7);		/* MOVWF TBLPTRH	*/
	/* TBLPTRL @ 0x0FF6 */
	pic16_core_instruction(k, 0x0E00 | addrl);	/* MOVLW <Addr[7:0]>	*/
	pic16_core_instruction(k, 0x6EF6);		/* MOVWF TBLPTRL	*/
}

/*
 * BLOCK ERASE
 *
 * DS39972B-page 16
 */
void
pic16_erase_block(struct k8048 *k, unsigned int block)
{
	unsigned char datal =  (block & 0x000000ff);		/*  7:0  */
	unsigned char datah = ((block & 0x0000ff00) >> 8);	/* 15:8  */
	unsigned char datau = ((block & 0x00ff0000) >> 16);	/* 24:16 */

	pic16_set_table_pointer(k, 0x3C0004);
	pic16_table_write(k, (datal << 8) | datal);	/* Write datal to 3C0004h */
	pic16_core_instruction(k, 0x0E05);		/* MOVLW 05h              */
	pic16_core_instruction(k, 0x6EF6);		/* MOVWF TBLPTRL          */
	pic16_table_write(k, (datah << 8) | datah);	/* Write datah to 3C0005h */
	pic16_core_instruction(k, 0x0E06);		/* MOVLW 06h              */
	pic16_core_instruction(k, 0x6EF6);		/* MOVWF TBLPTRL          */
	pic16_table_write(k, (datau << 8) | datau);	/* Write datah to 3C0006h */
	pic16_core_instruction(k, 0x0000);		/* NOP                    */
	pic16_core_instruction_nope(k);			/* NOP ERASE P11 + P10    */
}

/*
 * ROW ERASE (PROGRAM FLASH OR IDLOCATIONS)
 *
 * DS39972B-page 19 PIC18F26K80  HIGH=P9(1ms)   LOW=P10(100us)
 * DS39622L-page 16 PIC18F2XXX   HIGH=P9(1ms)   LOW=P10(100us)
 * DS39687E-page 10 PIC18LF27J53 HIGH=P10(54ms) LOW=NDELAY
 */
void
pic16_erase_row(struct k8048 *k, unsigned int address, unsigned int nrows)
{
	int h, l;

	if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW) {
		/* PIC18J */
		h = pic16_map[pic16_index].p10;		/* minimum 49 or 54ms	*/
		l = 0;
	} else {
		h = pic16_map[pic16_index].p9;		/* normally 1ms		*/
		l = pic16_map[pic16_index].p10;		/* normally 100us	*/
	}
	pic16_init_code_memory_access(k);		/* BSF EECON1, EEPGD	*/
							/* BCF EECON1, CFGS	*/
	pic16_write_enable(k);				/* BSF EECON1, WREN	*/
	while (nrows--) {
		pic16_set_table_pointer(k, address);
		pic16_free(k);				/* BSF EECON1, FREE	*/
		pic16_write(k);				/* BSF EECON1, WR	*/
		switch (pic16_map[pic16_index].datasheet) {
		case DS30500A: /* PIC18F2431 DS30500A-page 11 */
		case DS39576B: /* PIC18F252  */
		case DS39592E: /* PIC18F1320 */
			pic16_table_write_start_programming(k, 0);
		default:break;
		}
		pic16_core_instruction_nopp(k, h, l);	/* ROW ERASE		*/
		address += pic16_map[pic16_index].erasesize;
	}
	pic16_write_disable(k);				/* BCF EECON1, WREN	*/
}

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic16_bulk_erase(struct k8048 *k)
{	
	io_init_program_verify(k);

	switch (pic16_map[pic16_index].datasheet) {	/* CHIP ERASE		*/
	case DS30500A: /* PIC18F2431 DS30500A-page 9  */
	case DS39576B: /* PIC18F252                   */
	case DS39592E: /* PIC18F1320 DS39592F-page 10 */
		       /* PIC18F2320 DS39592F-page 10 */
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0080);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE P11 + P10	*/
		break;

	case DS39622K: /* PIC18F2550 */
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x3f3f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE P11 + P10	*/
		break;

	case DS39752B: /* PIC18F1230/1330 */
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8787);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE P11 + P10	*/
		break;

	case DS41342E: /* PIC18F14K50 */
	case DS41398B: /* PIC18F25K22 */
	case DS41630B: /* PIC18F25K50 */
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE P11 + P10	*/
		break;

	case DS39972B: /* PIC18F26K80 */
		/*
		 * Description			Data (3C0006h:3C0004h)
	 	 * Erase Data EEPROM            800004h
		 * Erase Boot Block             800005h
		 * Erase Config Bits            800002h
		 * Erase Code EEPROM Block 0    800104h
		 * Erase Code EEPROM Block 1    800204h
		 * Erase Code EEPROM Block 2    800404h
		 * Erase Code EEPROM Block 3    800804h
		 */
		pic16_erase_block(k, 0x800104);		/* ERASE CODE BLOCK 0 TABLE 3-2 */
		pic16_erase_block(k, 0x800204);		/* ERASE CODE BLOCK 1 TABLE 3-3 */
		pic16_erase_block(k, 0x800404);		/* ERASE CODE BLOCK 2 TABLE 3-4 */
		pic16_erase_block(k, 0x800804);		/* ERASE CODE BLOCK 3 TABLE 3-5 */
		pic16_erase_block(k, 0x800005);		/* ERASE BOOT BLOCK   TABLE 3-6 */
		pic16_erase_block(k, 0x800002);		/* ERASE CONFIG BITS  TABLE 3-7 */
		pic16_erase_row(k, PIC16_ID_LOW, 1);	/* ERASE IDLOC                  */
		break;

	case DS39687E: /* PIC18LF27J53 */
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0101);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8080);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE P11 + P10	*/
		break;

	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	io_standby(k);
}

/*
 * ROW ERASE
 */
void
pic16_row_erase(struct k8048 *k, unsigned int row, unsigned int nrows)
{	
	if (row == PIC_ERASE_EEPROM) {
		if (pic16_map[pic16_index].eeprom == 0) {
			printf("%s: information: EEPROM is not supported on this device\n", __func__);
			return; /* NO EEPROM */
		}

		unsigned short address = PIC16_REGIONDATA;
		
		io_init_program_verify(k);

		/* ERASE EEPROM */
		for (int i = 0; i < pic16_map[pic16_index].eeprom; ++i) {
			pic16_set_data_pointer(k, address++);
			pic16_write_data_memory(k, 0xFF);
		}

		io_standby(k);
		return;
	}

	if (row == PIC_ERASE_ID) {
		if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW) {
			printf("%s: information: IDLOCATIONS are not supported on this device\n", __func__);
			return; /* PIC18J IDLOCS:N/A */
		}

		io_init_program_verify(k);

		/* ERASE IDLOCATIONS */
		pic16_erase_row(k, PIC16_ID_LOW, 1);

		io_standby(k);
		return;
	}
#if 0
	if (row == PIC_ERASE_CONFIG) {
		io_init_program_verify(k);

		/* ERASE CONFIG */
		if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW) {
			pic16_erase_row(k, pic16_map[pic16_index].configaddr, 1);
		} else {
			for (int i = 0; i < pic16_map[pic16_index].configsize; i++) {
				pic16_conf.index[i + PIC16_CONFIG_1L] = 0xFF;
			}
			pic16_write_config(k);
		}

		io_standby(k);
		return;
	}
#endif
	/*
	 * ERASE PROGRAM FLASH ROW(S)
	 */

	unsigned int maxrows = 2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize;
	if (row >= maxrows) {
		printf("%s: information: program flash row is out of range\n", __func__);
		return;
	}

	unsigned int numrows = maxrows - row;
	if (nrows > numrows) {
		nrows = numrows;
	}

	unsigned int address = row * pic16_map[pic16_index].erasesize;

	io_init_program_verify(k);

	/* ERASE PROGRAM FLASH */
	pic16_erase_row(k, address, nrows);

	io_standby(k);
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY
 *
 * IDLOC    200000 ... 200007 if supported
 * CONFIG   300000 ... 30000d or XXXXXX ... XXXXXX + 0xd
 * DEVICEID 3ffffe ... 3fffff
 */
void
pic16_read_config_memory(struct k8048 *k)
{
	unsigned short deviceid = 0, revision = 0, fault;
	int i;

	/* Device selected by user */
	if (k->devicename[0]) {
		printf("%s: fatal error: device select is not supported on this architecture.\n", __func__);
		exit(EX_SOFTWARE);
	}

	io_init_program_verify(k);

	/* Device id 1..2 */
	pic16_set_table_pointer(k, PIC16_DEVICEID_LOW);
	for (i = 0; i < 2; i++)
		pic16_conf.index[i + PIC16_CONFIG_DEVICEID1] = pic16_table_read_post_increment(k);

	i = 0;
	while (pic16_map[i].deviceid != 0) {
		if (pic16_map[i].devid1rev4 == 0) {
			deviceid = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
				(pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_IDMASK);
			revision = pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_REVMASK;
		} else {
			deviceid = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
				(pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_IDMASK_REV4);
			revision = pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_REVMASK_REV4;
		}
		if (pic16_map[i].deviceid == deviceid)
			break;
		i++;
	}

	if (pic16_map[i].deviceid == 0) {
		/*
		 * VELLEMAN K8048 SWITCH IN STANDBY [0000] 0000 rev  0
		 * VELLEMAN K8048 NO POWER          [FFFF] ffe0 rev 31
		 * VELLEMAN K0848 SWITCH IN RUN     [FFFF] ffe0 rev 31
		 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
		 */
		fault = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
			pic16_conf.index[PIC16_CONFIG_DEVICEID1];
		if (fault == 0x0000 || fault == 0xFFFF) {
			printf("%s: fatal error: %s.\n",
				__func__, io_fault(k, fault));
		} else {
			printf("%s: fatal error: device unknown: %04X rev %2d [%04X]\n",
				__func__, deviceid, revision, fault);
		}
		io_standby(k);
		exit(EX_SOFTWARE); /* Panic */
	}

	pic16_index = i; /* Device recognised */

	/* Id location 1..8 PIC18J IDLOC:N/A */
	if (pic16_map[pic16_index].configaddr == PIC16_CONFIG_LOW) {
		pic16_set_table_pointer(k, PIC16_ID_LOW);
		for (i = 0; i < 8; i++) {
			pic16_conf.index[i + PIC16_CONFIG_IDLOCATION1] = pic16_table_read_post_increment(k);
		}
	}

	/* Config 1..X */
	pic16_set_table_pointer(k, pic16_map[pic16_index].configaddr);
	for (i = 0; i < pic16_map[pic16_index].configsize; i++) {
		pic16_conf.index[i + PIC16_CONFIG_1L] = pic16_table_read_post_increment(k);
	}

	io_standby(k);
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
int
pic16_get_program_flash_size(void)
{
	return pic16_map[pic16_index].flash;
}

/*
 * GET DATA FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
int
pic16_get_data_flash_size(void)
{
	return 0;
}

/*
 * GET DATA EEPROM SIZE
 *
 *  RETURN SIZE IN BYTES
 */
int
pic16_get_data_eeprom_size(void)
{
	return pic16_map[pic16_index].eeprom;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK 0000 .. 0000 + SIZE
 *
 *  RETURN BASE ADDRESS
 */
unsigned long
pic16_read_flash_memory_block(struct k8048 *k, unsigned short *data, int max)
{
	int i;

	if (max) {
		io_init_program_verify(k);

		pic16_init_code_memory_access(k);
		pic16_set_table_pointer(k, PIC16_CODE_LOW);
		for (i = 0; i < max; i++) {
			data[i] = pic16_table_read_post_increment(k);
			data[i] |= pic16_table_read_post_increment(k) << 8;
		}

		io_standby(k);
	}
	return PIC16_BOOTBLOCK_LOW;
}

/*
 * READ DATA (EEPROM) MEMORY BLOCK 0000 .. 0000 + SIZE
 *
 *  RETURN BASE ADDRESS
 */
unsigned long
pic16_read_eeprom_memory_block(struct k8048 *k, unsigned char *data, int max)
{
	int i;

	io_init_program_verify(k);

	pic16_init_data_memory_access(k);
	for (i = 0; i < max; i++) {
		pic16_set_data_pointer(k, i);
		data[i] = pic16_read_data_memory(k);
	}

	io_standby(k);

	return PIC16_DATA_LOW;
}

/*****************************************************************************
 *
 * Program Data (EEPROM)
 *
 *****************************************************************************/

/*
 * SET DATA POINTER (EEPROM)
 */
void
pic16_set_data_pointer(struct k8048 *k, unsigned short address)
{
	unsigned char addrl =  (address & 0x00ff);		/*  7:0	*/
	unsigned char addrh = ((address & 0xff00) >> 8);	/* 15:8	*/

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EEADR  @ 0x0F74 */
		pic16_core_instruction(k, 0x0E00 | addrl);	/* MOVLW <Addr>	*/
		pic16_core_instruction(k, 0x6E74);		/* MOVWF EEADR	*/
		/* EEADRH @ 0x0F75 */
		pic16_core_instruction(k, 0x0E00 | addrh);	/* MOVLW <AddrH>*/
		pic16_core_instruction(k, 0x6E75);		/* MOVWF EEADRH	*/
		break;
	default:/* EEADR  @ 0x0FA9 */
		pic16_core_instruction(k, 0x0E00 | addrl);	/* MOVLW <Addr>	*/
		pic16_core_instruction(k, 0x6EA9);		/* MOVWF EEADR	*/
		/* EEADRH @ 0x0FAA */
		pic16_core_instruction(k, 0x0E00 | addrh);	/* MOVLW <AddrH>*/
		pic16_core_instruction(k, 0x6EAA);		/* MOVWF EEADRH	*/
		break;
	}
}

/*
 * READ DATA MEMORY (EEPROM)
 */
unsigned char
pic16_read_data_memory(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EECON1 @ 0x0F7F */
		/* EEDATA @ 0x0F73 */
		pic16_core_instruction(k, 0x807F);	/* BSF EECON1, RD	*/
		pic16_core_instruction(k, 0x5073);	/* MOVF EEDATA, W, 0	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		/* EEDATA @ 0x0FA8 */
		pic16_core_instruction(k, 0x80A6);	/* BSF EECON1, RD	*/
		pic16_core_instruction(k, 0x50A8);	/* MOVF EEDATA, W, 0	*/
		break;
	}

	/* TABLAT @ 0x0FF5 */
	pic16_core_instruction(k, 0x6EF5);		/* MOVWF TABLAT		*/
	pic16_core_instruction(k, 0x0000);		/* NOP			*/

	return pic16_shift_out_tablat_register(k);
}

/*
 * WRITE DATA MEMORY (EEPROM)
 *
 * DS39592F-page 17
 */
void
pic16_write_data_memory(struct k8048 *k, unsigned char data)
{
	struct timeval tv1, tv2, tv3;

	pic16_core_instruction(k, 0x0E00 | data);	/* MOVLW <Data>		*/
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B: /* PIC18F26K80 */
		/* EEDATA @ 0x0F73 */
		pic16_core_instruction(k, 0x6E73);	/* MOVWF EEDATA		*/
		break;
	default:/* EEDATA @ 0x0FA8 */
		pic16_core_instruction(k, 0x6EA8);	/* MOVWF EEDATA		*/
		break;
	}

	pic16_write_enable(k);				/* BSF EECON1, WREN     */

	switch (pic16_map[pic16_index].datasheet) {
	case DS30500A: /* PIC18F2431 */
	case DS39576B: /* PIC18F252  */
	case DS39592E: /* PIC18F1320 */
		       /* PIC18F2320 */
		/* EECON2 @ 0x0FA7 */
		pic16_core_instruction(k, 0x0E55);	/* MOVLW 0x55		*/
		pic16_core_instruction(k, 0x6EA7);	/* MOVWF EECON2		*/
		pic16_core_instruction(k, 0x0EAA);	/* MOVLW 0xAA		*/
		pic16_core_instruction(k, 0x6EA7);	/* MOVWF EECON2		*/
		break;
	}
	pic16_write(k);					/* BSF EECON1, WR       */

	gettimeofday(&tv1, NULL);
	do	/* Until write completes or times out */
	{
		switch (pic16_map[pic16_index].datasheet) {
		case DS39972B: /* PIC18F26K80 */
			/* EECON1 @ 0x0F7F */
			pic16_core_instruction(k, 0x507F);/* MOVF EECON1, W, 0	*/
			break;
		default:/* EECON1 @ 0x0FA6 */
			pic16_core_instruction(k, 0x50A6);/* MOVF EECON1, W, 0	*/
			break;
		}

		/* TABLAT @ 0x0FF5 */
		pic16_core_instruction(k, 0x6EF5);	/* MOVWF TABLAT		*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
	
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while ((pic16_shift_out_tablat_register(k) & 2) && (tv3.tv_sec < 1));

	pic16_write_disable(k);				/* BCF EECON1, WREN     */

	if (tv3.tv_sec)
		printf("%s: information: data write timed out.\n", __func__);
}

/*****************************************************************************
 *
 * Program Code/ID Panel
 *
 *****************************************************************************/

/*
 * WRITE BUFFER INIT
 */
void
pic16_write_buffer_init(struct k8048 *k)
{
	switch (pic16_map[pic16_index].datasheet) {
 		/* INIT SINGLE PANEL WRITES */
	case DS30500A: /* PIC18F2431 */
	case DS39576B: /* PIC18F252  */
	case DS39592E: /* PIC18F1320 */
		       /* PIC18F2320 */
		pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(k, 0x3C0006);	/* PANEL MODE CONFIG &	*/
		pic16_table_write(k, 0x0000);		/* SINGLE PANEL WRITES	*/
		break;
	default:/* MODERN DEVICES ARE ALL SINGLE PANEL */
		break;
	}
	pic16_init_code_memory_access(k);		/* SWITCH TO CODE	*/
}

/*
 * WRITE BUFFER (PANEL SIZE BYTES INTO CODE PANEL, 8 BYTES INTO ID PANEL)
 *
 * DS39622L-page 18 HIGH=P9(1ms)   LOW=P10(100us)
 * DS39687E-page 13 HIGH=P9(3.4ms) LOW=NDELAY
 */
void
pic16_write_buffer(struct k8048 *k, unsigned long address, unsigned char buffer[MAXPANELSIZE])
{
	int h, l, i, limit;

	switch (pic16_map[pic16_index].datasheet) {
	case DS39687E: /* PIC18LF27J53 */
		h = pic16_map[pic16_index].p9;		/* minimum 1.2 or 3.5ms */
		l = 0;
		break;
	default:h = pic16_map[pic16_index].p9;
		l = pic16_map[pic16_index].p10;
		break;
	}

	if (address == PIC16_ID_LOW)	/* The idloc panel size is 8 bytes	*/
		limit = 8 - 2;
	else				/* The code panel size varies		*/
		limit = pic16_map[pic16_index].panelsize - 2;
#if 0
	pic16_write_enable(k);				/* BSF EECON1, WREN	*/
#endif
	pic16_set_table_pointer(k, address);
	for (i = 0; i < limit; i += 2)
		pic16_table_write_post_increment_2(k, (buffer[i + 1] << 8) | buffer[i]);
	pic16_table_write_start_programming(k, (buffer[i + 1] << 8) | buffer[i]);
	pic16_core_instruction_nopp(k, h, l);		/* PANEL PROGRAM	*/
#if 0
	pic16_write_disable(k);				/* BCF EECON1, WREN	*/
#endif
}

/*
 * WRITE BUFFERED (SINGLE PANEL WRITES OF DEVICE PANEL SIZE)
 */
void
pic16_write_buffered(struct k8048 *k, unsigned long address, unsigned char data, int mode)
{
	static unsigned char buffer[MAXPANELSIZE];
	static unsigned long buffer_address = 0;
	static int write_pending = 0;
	unsigned long new_address, boundary, mask;
	int i;

	switch (mode) {
	case WB_FLUSH:
		if (write_pending)
			pic16_write_buffer(k, buffer_address, buffer);
		/* Fall-through */
	case WB_INIT:
		for (i = 0; i < pic16_map[pic16_index].panelsize; i++)
			buffer[i] = WB_SPACE;
		buffer_address = 0;
		write_pending = 0;
		break;
	case WB_ADD:
		boundary = 0 - pic16_map[pic16_index].panelsize;
		mask = pic16_map[pic16_index].panelsize - 1;

		new_address = address & boundary;
		if (new_address != buffer_address) {
			if (write_pending) {
				pic16_write_buffer(k, buffer_address, buffer);
				for (i = 0; i < pic16_map[pic16_index].panelsize; i++)
					buffer[i] = WB_SPACE;
			}
			buffer_address = new_address;
		}

		buffer[address & mask] = data;
		write_pending = 1;
		break;
	}
}

/*****************************************************************************
 *
 * Program Configuration
 *
 * NOT PIC18J
 *
 *****************************************************************************/

/*
 * GOTO 100000
 */
void
pic16_goto100000(struct k8048 *k)
{
	pic16_core_instruction(k, 0xEF00);		/* GOTO 0x100000	*/
	pic16_core_instruction(k, 0xF800);		/* GOTO 0x100000	*/
}

/*
 * WRITE CONFIG REGISTER
 */
void
pic16_write_configreg(struct k8048 *k, unsigned long address, int index, int h, int l)
{
	unsigned short data = pic16_conf.index[index] | pic16_conf.index[index + 1] << 8;

	pic16_set_table_pointer(k, address);	/* EVEN ADDRESS			 */
	pic16_table_write_start_programming(k, data);
	pic16_core_instruction_nopp(k, h, l);	/* CONFIG PROGRAM H=P9/P9A L=P10 */

	pic16_set_table_pointer(k, address + 1);
	pic16_table_write_start_programming(k, data);
	pic16_core_instruction_nopp(k, h, l);	/* CONFIG PROGRAM H=P9/P9A L=P10 */
}

/*
 * WRITE CONFIGURATION (REGISTER 6H MUST BE WRITTEN LAST)
 */
void
pic16_write_config(struct k8048 *k)
{
	int h, l;

	if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW)
		return; /* PIC18J CONFIG:FLASH */

	h = (pic16_map[pic16_index].p9a != 0)
		? (pic16_map[pic16_index].p9a) : (pic16_map[pic16_index].p9);
	l = pic16_map[pic16_index].p10;

	pic16_init_config_memory_access(k);
	pic16_goto100000(k);
#if 0
	pic16_write_enable(k);				/* BSF EECON1, WREN	*/
#endif
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 0, PIC16_CONFIG_1L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 2, PIC16_CONFIG_2L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 4, PIC16_CONFIG_3L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 6, PIC16_CONFIG_4L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 8, PIC16_CONFIG_5L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 12, PIC16_CONFIG_7L, h, l);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 10, PIC16_CONFIG_6L, h, l);
#if 0
	pic16_write_disable(k);				/* BCF EECON1, WREN	*/
#endif
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE, IDLOC, CONFIG or DATA
 */
int
pic16_getregion(unsigned int address)
{
	if (address >= PIC16_CODE_LOW && address <= PIC16_ID_HIGH)
		return PIC16_REGIONCODEID;

	if (address >= PIC16_CONFIG_LOW && address <= PIC16_CONFIG_HIGH)
		return PIC16_REGIONCONFIG;

	if (address >= PIC16_DATA_LOW && address <= PIC16_DATA_HIGH)
		return PIC16_REGIONDATA;

	printf("%s: warning: region unknown [addr=%04X]\n", __func__, address);
	return PIC16_REGIONUNKNOWN;
}

/*
 * INIT WRITE REGION FOR PROGRAM (CONFIG DELAYED)
 */
void
pic16_init_writeregion(struct k8048 *k, int region)
{
	switch (region) {
	case PIC16_REGIONCODEID:
		pic16_write_buffer_init(k);
		pic16_write_buffered(k, 0, 0, WB_INIT);
		break;
	case PIC16_REGIONDATA:
		pic16_init_data_memory_access(k);
		break;
	}
}

/*
 * INIT READ REGION FOR VERIFY
 */
void
pic16_init_readregion(struct k8048 *k, int region)
{
	switch (region) {
	case PIC16_REGIONCODEID:
	case PIC16_REGIONCONFIG:
		pic16_init_code_memory_access(k);
		break;
	case PIC16_REGIONDATA:
		pic16_init_data_memory_access(k);
		break;
	}
}

/*
 * WRITE REGION (CACHE CONFIG WORDS)
 */
void
pic16_writeregion(struct k8048 *k, unsigned long address, int region, unsigned char data)
{
	switch (region) {
	case PIC16_REGIONCODEID:
		pic16_write_buffered(k, address, data, WB_ADD);
		break;
	case PIC16_REGIONCONFIG:
		pic16_conf.index[PIC16_CONFIG_1L + (address & PIC16_CONFIG_MASK)] = data;
		break;
	case PIC16_REGIONDATA:
		pic16_set_data_pointer(k, address & PIC16_DATA_MASK);
		pic16_write_data_memory(k, data);
		break;
	}
}

/*
 * VERIFY REGION
 */
int
pic16_verifyregion(struct k8048 *k, unsigned long address, int region, int index, unsigned char data)
{
	unsigned char vdata = 0;

	switch (region) {
	case PIC16_REGIONCODEID:
		if (index == 0)
			pic16_set_table_pointer(k, address);
		vdata = pic16_table_read_post_increment(k);
		break;
	case PIC16_REGIONCONFIG:
		/* Can't verify the config area (assume okay) */
		vdata = data;
		break;
	case PIC16_REGIONDATA:
		pic16_set_data_pointer(k, address & PIC16_DATA_MASK);
		vdata = pic16_read_data_memory(k);
		break;
	}
	if (vdata != data) {
		printf("%s: error: read [%02X] expected [%02X] at [%04lX]\n",
			__func__, vdata, data, address);
		return FAIL;
	}
	return PASS;
}

/*
 * PROGRAM FILE
 */
void
pic16_program(struct k8048 *k, int blank)
{
	int i, j;
	unsigned long PC_address;
	int new_region, current_region = PIC16_REGIONUNKNOWN;
	int total = 0;

	/* Initialise device for programming */
	if (blank)
		pic16_bulk_erase(k);
	
	/* Program device */
	io_init_program_verify(k);

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		PC_address = inhx32_pdata[i]->address;
		new_region = pic16_getregion(PC_address);
		if (new_region == PIC16_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			if (current_region == PIC16_REGIONCODEID)
				pic16_write_buffered(k, 0, 0, WB_FLUSH);
			pic16_init_writeregion(k, new_region);
			current_region = new_region;
		}

		/* For each byte in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j++) {
			pic16_writeregion(k, PC_address++, current_region, inhx32_pdata[i]->bytes[j]);
			total++;
		}
	}
	if (current_region == PIC16_REGIONCODEID)
		pic16_write_buffered(k, 0, 0, WB_FLUSH);

	io_standby(k);

	/* Finalise device programming (write config words) */
	if (blank) {
		pic16_program_config(k);
		total += 14;
	}

	printf("Total: %d\n", total);
}

/*
 * VERIFY FILE (THIS WILL FAIL IF PROTECTION WAS ENABLED EARLIER)
 */
int
pic16_verify(struct k8048 *k)
{
	int i, j;
	unsigned long PC_address;
	int new_region, current_region = PIC16_REGIONUNKNOWN;
	int fail = 0, total = 0;

	/* Verify device */
	io_init_program_verify(k);

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		PC_address = inhx32_pdata[i]->address;
		new_region = pic16_getregion(PC_address);
		if (new_region == PIC16_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			pic16_init_readregion(k, new_region);
			current_region = new_region;
		}

		/* For each byte in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j += 1) {
			fail += pic16_verifyregion(k, PC_address++, current_region, j, inhx32_pdata[i]->bytes[j]);
			total++;
		}
	}

	io_standby(k);

	printf("Total: %d Pass: %d Fail: %d\n", total, total-fail, fail);
	return fail;
}
 
/*
 * PROGRAM CONFIGURATION (AFTER VERIFY)
 */
void
pic16_program_config(struct k8048 *k)
{
	io_init_program_verify(k);

	pic16_write_config(k);

	io_standby(k);
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
pic16_dumpdeviceid(struct k8048 *k)
{
	unsigned short deviceid, revision;

	if (pic16_map[pic16_index].devid1rev4 == 0) {
		deviceid = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
			(pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_IDMASK);
		revision = pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_REVMASK;
	} else {
		deviceid = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
			(pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_IDMASK_REV4);
		revision = pic16_conf.index[PIC16_CONFIG_DEVICEID1] & PIC16_DEVICEID1_REVMASK_REV4;
	}

	if (pic16_map[pic16_index].flash < 0x10000) {
		printf("[%06X] [PROGRAM]     %04X WORDS (%04X ROWS OF %04X WORDS)\n",
			PIC16_CODE_LOW, pic16_map[pic16_index].flash,
				2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize,
				pic16_map[pic16_index].erasesize / 2
				);
	} else {
		printf("[%06X] [PROGRAM]    %05X WORDS (%04X ROWS OF %04X WORDS)\n",
			PIC16_CODE_LOW, pic16_map[pic16_index].flash,
				2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize,
				pic16_map[pic16_index].erasesize / 2
				);
	}

	if (pic16_map[pic16_index].configaddr == PIC16_CONFIG_LOW) {
		/* Id location 1..8 PIC18J IDLOC:N/A */
		for (int i = 0; i < 8; ++i) {
			unsigned char c = pic16_conf.index[i + PIC16_CONFIG_IDLOCATION1];
			printf("[20000%d] [IDLOCATION%d] %02X %c\n", i, 1 + i,
				c, (c >= 32 && c < 127) ? (c) : ('.'));
		}
	}

	pic16_dumpconfig(k, BRIEF);

	printf("[3FFFFE] [DEVICEID]    %04X DEV:%04X (%X:%X) REV:%02X",
	/* Literal */	(pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) |
				pic16_conf.index[PIC16_CONFIG_DEVICEID1],
	/* Actual  */	deviceid,
	/* High    */	(deviceid >> 8),
	/* Low     */	(deviceid >> 5) & 7, revision);
	printf(" %s\n", pic16_map[pic16_index].devicename);

	if (pic16_map[pic16_index].eeprom > 0) {
		printf("[%06X] [DATA]        %04X BYTES\n",
			PIC16_DATA_LOW, pic16_map[pic16_index].eeprom);
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic16_dumpconfig(struct k8048 *k, int verbose)
{
	for (int i = 0; i < pic16_map[pic16_index].configsize; i += 2) {
		printf("[%06X] [CONFIG%d]     %04X\n", pic16_map[pic16_index].configaddr + i, i / 2 + 1,
			(pic16_conf.index[PIC16_CONFIG_1H + i] << 8) | pic16_conf.index[PIC16_CONFIG_1L + i]);
	}
	if (verbose)
		pic16_dumpconfig_verbose(k);
}

/*
 * DUMP VERBOSE CONFIG WORD DETAILS FOR DEVICE (INCOMPLETE)
 */
void
pic16_dumpconfig_verbose(struct k8048 *k)
{
	unsigned char w;

	if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW)
		return; /* PIC18J */

	printf("\n[CONFIG_1H] [%02X]\n", pic16_conf.index[PIC16_CONFIG_1H]);
	w = pic16_conf.index[PIC16_CONFIG_1H] & 0x80;
	if (w)
		printf("\t[IESO] Enabled\n");
	else
		printf("\t[IESO] Disabled\n");
	w = pic16_conf.index[PIC16_CONFIG_1H] & 0x40;
	if (w)
		printf("\t[FCMEN/FSCM] Enabled\n");
	else
		printf("\t[FCMEN/FSCM] Disabled\n");
	w = pic16_conf.index[PIC16_CONFIG_1H] & 0x0F;
	switch (w) {
	case 15:
	case 14:
	case 13:
	case 12:printf("\t[FOSC3:FOSC0] 11XX EXT RC CLKOUT\n");    break;
	case 11:printf("\t[FOSC3:FOSC0] 1011 UNKNOWN\n");          break;
	case 10:printf("\t[FOSC3:FOSC0] 1010 UNKNOWN\n");          break;
	case 9: printf("\t[FOSC3:FOSC0] 1001 INT RC CLKOUT\n");    break;
	case 8: printf("\t[FOSC3:FOSC0] 1000 INT RC NO CLKOUT\n"); break;
	case 7: printf("\t[FOSC3:FOSC0] 0111 EXT RC\n");           break;
	case 6: printf("\t[FOSC3:FOSC0] 0110 HS\n");               break;
	case 5: printf("\t[FOSC3:FOSC0] 0101 EC\n");               break;
	case 4: printf("\t[FOSC3:FOSC0] 0100 EC CLKOUT\n");        break;
	case 3: printf("\t[FOSC3:FOSC0] 0011 RC\n");               break;
	case 2: printf("\t[FOSC3:FOSC0] 0010 HS\n");               break;
	case 1: printf("\t[FOSC3:FOSC0] 0001 XT\n");               break;
	case 0: printf("\t[FOSC3:FOSC0] 0000 LP\n");               break;
	}

	printf("\n[CONFIG_2L] [%02X]\n", pic16_conf.index[PIC16_CONFIG_2L]);
	w = (pic16_conf.index[PIC16_CONFIG_2L] & 0x0C) >> 2;
	switch (w) {
	case 3: printf("\t[BORV1:BORV0] 2.0/2.5V\n"); break;
	case 2: printf("\t[BORV1:BORV0] 2.7V\n");     break;
	case 1: printf("\t[BORV1:BORV0] 4.2V\n");     break;
	case 0: printf("\t[BORV1:BORV0] 4.5V\n");     break;
	}
	w = pic16_conf.index[PIC16_CONFIG_2L] & 0x02;
	if (w)
		printf("\t[BOREN] Enabled\n");
	else
		printf("\t[BOREN] Disabled\n");

	w = pic16_conf.index[PIC16_CONFIG_2L] & 0x01;
	if (w)
		printf("\t[PWRTEN] Disabled\n");
	else
		printf("\t[PWRTEN] Enabled\n");

	/* XXX WDT o/p is wrong for the PIC18F25K22 */
	printf("\n[CONFIG_2H] [%02X]\n", pic16_conf.index[PIC16_CONFIG_2H]);
	w = (pic16_conf.index[PIC16_CONFIG_2H] & 0x1E) >> 1;
	switch (w) {
	case 15: printf("\t[WDTP3:WDTP0] 1:32768\n"); break;
	case 14: printf("\t[WDTP3:WDTP0] 1:16384\n"); break;
	case 13: printf("\t[WDTP3:WDTP0] 1:8192\n");  break;
	case 12: printf("\t[WDTP3:WDTP0] 1:4096\n");  break;
	case 11: printf("\t[WDTP3:WDTP0] 1:2048\n");  break;
	case 10: printf("\t[WDTP3:WDTP0] 1:1024\n");  break;
	case  9: printf("\t[WDTP3:WDTP0] 1:512\n");   break;
	case  8: printf("\t[WDTP3:WDTP0] 1:256\n");   break;
	case  7: printf("\t[WDTP3:WDTP0] 1:128\n");   break;
	case  6: printf("\t[WDTP3:WDTP0] 1:64\n");    break;
	case  5: printf("\t[WDTP3:WDTP0] 1:32\n");    break;
	case  4: printf("\t[WDTP3:WDTP0] 1:16\n");    break;
	case  3: printf("\t[WDTP3:WDTP0] 1:8\n");     break;
	case  2: printf("\t[WDTP3:WDTP0] 1:4\n");     break;
	case  1: printf("\t[WDTP3:WDTP0] 1:2\n");     break;
	case  0: printf("\t[WDTP3:WDTP0] 1:1\n");     break;
	}
	w = pic16_conf.index[PIC16_CONFIG_2H] & 0x01;
	if (w)
		printf("\t[WDTEN] Enabled\n");
	else
		printf("\t[WDTEN] Disabled\n");

	printf("\n[CONFIG_3H] [%02X]\n", pic16_conf.index[PIC16_CONFIG_3H]);
	w = pic16_conf.index[PIC16_CONFIG_3H] & 0x80;
	if (w)
		printf("\t[MCLRE] Enabled\n");
	else
		printf("\t[MCLRE] Disabled\n");
	w = pic16_conf.index[PIC16_CONFIG_3H] & 0x02;
	if (w)
		printf("\t[PBADEN/PBAD] Enabled (*)\n");
	else
		printf("\t[PBADEN/PBAD] Disabled (*)\n");
	w = pic16_conf.index[PIC16_CONFIG_3H] & 0x01;
	if (w)
		printf("\t[CCP2MX] RC1 (*)\n");
	else
		printf("\t[CCP2MX] RB3 (*)\n");

	printf("\n[CONFIG_4L] [%02X]\n", pic16_conf.index[PIC16_CONFIG_4L]);
	w = pic16_conf.index[PIC16_CONFIG_4L] & 0x80;
	if (w)
		printf("\t[DEBUG] Disabled\n");
	else
		printf("\t[DEBUG] Enabled\n");
	w = pic16_conf.index[PIC16_CONFIG_4L] & 0x04;
	if (w)
		printf("\t[LVP] Enabled\n");
	else
		printf("\t[LVP] Disabled\n");
	w = pic16_conf.index[PIC16_CONFIG_4L] & 0x01;
	if (w)
		printf("\t[STVREN] Enabled\n");
	else
		printf("\t[STVREN] Disabled\n");

	/*
	 * Protection bits
	 */
	printf("\n[CONFIG_5L] [%02X]", pic16_conf.index[PIC16_CONFIG_5L]);
	w = pic16_conf.index[PIC16_CONFIG_5L] & 0x08;
	if (w)
		printf("\t[CP3]   Off");
	else
		printf("\t[CP3]   On");
	w = pic16_conf.index[PIC16_CONFIG_5L] & 0x04;
	if (w)
		printf("\t[CP2]   Off");
	else
		printf("\t[CP2]   On");
	w = pic16_conf.index[PIC16_CONFIG_5L] & 0x02;
	if (w)
		printf("\t[CP1]   Off");
	else
		printf("\t[CP1]   On");
	w = pic16_conf.index[PIC16_CONFIG_5L] & 0x01;
	if (w)
		printf("\t[CP0]   Off");
	else
		printf("\t[CP0]   On");

	printf("\n[CONFIG_5H] [%02X]", pic16_conf.index[PIC16_CONFIG_5H]);
	w = pic16_conf.index[PIC16_CONFIG_5H] & 0x80;
	if (w)
		printf("\t[CPD]   Off");
	else
		printf("\t[CPD]   On");
	w = pic16_conf.index[PIC16_CONFIG_5H] & 0x40;
	if (w)
		printf("\t[CPB]   Off");
	else
		printf("\t[CPB]   On");

	printf("\n[CONFIG_6L] [%02X]", pic16_conf.index[PIC16_CONFIG_6L]);
	w = pic16_conf.index[PIC16_CONFIG_6L] & 0x08;
	if (w)
		printf("\t[WRT3]  Off");
	else
		printf("\t[WRT3]  On");
	w = pic16_conf.index[PIC16_CONFIG_6L] & 0x04;
	if (w)
		printf("\t[WRT2]  Off");
	else
		printf("\t[WRT2]  On");
	w = pic16_conf.index[PIC16_CONFIG_6L] & 0x02;
	if (w)
		printf("\t[WRT1]  Off");
	else
		printf("\t[WRT1]  On");
	w = pic16_conf.index[PIC16_CONFIG_6L] & 0x01;
	if (w)
		printf("\t[WRT0]  Off");
	else
		printf("\t[WRT0]  On");

	printf("\n[CONFIG_6H] [%02X]", pic16_conf.index[PIC16_CONFIG_6H]);
	w = pic16_conf.index[PIC16_CONFIG_6H] & 0x80;
	if (w)
		printf("\t[WRTD]  Off");
	else
		printf("\t[WRTD]  On");
	w = pic16_conf.index[PIC16_CONFIG_6H] & 0x40;
	if (w)
		printf("\t[WRTB]  Off");
	else
		printf("\t[WRTB]  On");
	w = pic16_conf.index[PIC16_CONFIG_6H] & 0x20;
	if (w)
		printf("\t[WRTC]  Off");
	else
		printf("\t[WRTC]  On");

	printf("\n[CONFIG_7L] [%02X]", pic16_conf.index[PIC16_CONFIG_7L]);
	w = pic16_conf.index[PIC16_CONFIG_7L] & 0x08;
	if (w)
		printf("\t[EBTR3] Off");
	else
		printf("\t[EBTR3] On");
	w = pic16_conf.index[PIC16_CONFIG_7L] & 0x04;
	if (w)
		printf("\t[EBTR2] Off");
	else
		printf("\t[EBTR2] On");
	w = pic16_conf.index[PIC16_CONFIG_7L] & 0x02;
	if (w)
		printf("\t[EBTR1] Off");
	else
		printf("\t[EBTR1] On");
	w = pic16_conf.index[PIC16_CONFIG_7L] & 0x01;
	if (w)
		printf("\t[EBTR0] Off");
	else
		printf("\t[EBTR0] On");

	printf("\n[CONFIG_7H] [%02X]", pic16_conf.index[PIC16_CONFIG_7H]);
	w = pic16_conf.index[PIC16_CONFIG_7H] & 0x40;
	if (w)
		printf("\t[EBTRB] Off\n");
	else
		printf("\t[EBTRB] On\n");

	printf("\n(*) Feature is device dependant.\n");
}

/*
 * DUMP DEVICE CONFIGURATION
 */
void
pic16_dumpdevice(struct k8048 *k)
{
	int i;

	if (pic16_map[pic16_index].configaddr != PIC16_CONFIG_LOW)
		return; /* PIC18J IDLOC:N/A CONFIG:FLASH */

	/* IDLOC:  Extended address = 0x0020 */
	printf(":020000040020%02X\n", 0x100 - 0x26);
	for (i = 0; i < 8; ++i) {
		pic_dumpbyte(k, i, pic16_conf.index[i + PIC16_CONFIG_IDLOCATION1]);
	}
	/* CONFIG: Extended address = 0x0030 */
	printf(":020000040030%02X\n", 0x100 - 0x36);
	for (i = 0; i < pic16_map[pic16_index].configsize; ++i) {
		pic_dumpbyte(k, i, pic16_conf.index[i + PIC16_CONFIG_1L]);
	}
}
