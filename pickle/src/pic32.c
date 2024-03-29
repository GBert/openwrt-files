/*
 * Copyright (C) 2005-2020 Darron Broad
 * All rights reserved.
 *
 * This file is part of Pickle Microchip PIC ICSP.
 *
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#undef DEBUG

#include "pickle.h"

/******************************************************************************
 *
 * Session
 *
 *****************************************************************************/

extern struct pickle p;

/*****************************************************************************
 *
 * Hardware operations
 *
 *****************************************************************************/

struct pic_ops pic32_ops = {
	.arch				= ARCH32BIT,
	.align				= sizeof(uint32_t),
	.selector			= pic32_selector,
	.bootloader			= NULL,
	.program_begin			= pic32_program_begin,
	.program_data			= pic32_program_data,
	.program_end			= pic32_program_end,
	.verify_begin			= pic32_program_begin,
	.verify_data			= pic32_verify_data,
	.verify_end			= pic32_standby,
	.view_data			= pic32_view_data,
	.read_config_memory		= pic32_read_config_memory,
	.get_program_count		= pic32_get_program_count,
	.get_program_size		= pic32_get_program_size,
	.get_data_size			= NULL,
	.get_executive_size		= NULL,
	.get_boot_size			= pic32_get_boot_size,
	.read_program_memory_block	= pic32_read_program_memory_block,
	.read_data_memory_block 	= NULL,
	.write_panel			= pic32_write_panel,
	.bulk_erase			= pic32_bulk_erase,
	.write_osccal			= NULL,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= NULL,
	.dumpdeviceid			= pic32_dumpdeviceid,
	.dumpconfig			= pic32_dumpconfig,
	.dumposccal			= NULL,
	.dumpdevice			= NULL,
	.dumpadj			= 4,
	.dumphexcode			= pic32_dumphexcode,
	.dumpinhxcode			= pic32_dumpinhxcode,
	.dumphexdata			= NULL,
	.dumpinhxdata			= NULL,
	.debug				= NULL,
};

uint32_t
pic32_arch(void)
{
	p.pic = &pic32_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic32_config pic32_conf;

/*****************************************************************************
 *
 * Hardware algorithm map: Device
 *
 *****************************************************************************/

struct pic32_dsmap pic32_map[] =
{
/*Device name		Device id	 App. Data-Sheet	BOOT/PROG Size	Row/Page Size*/

/* PIC32MX1XX/2XX DS60001168F */
{"PIC32MX110F016B",	PIC32MX110F016B, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX110F016C",	PIC32MX110F016C, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX110F016D",	PIC32MX110F016D, DS60001168F,		768, 4096,	32, 256},

{"PIC32MX120F032B",	PIC32MX120F032B, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX120F032C",	PIC32MX120F032C, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX120F032D",	PIC32MX120F032D, DS60001168F,		768, 8192,	32, 256},

{"PIC32MX130F064B",	PIC32MX130F064B, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX130F064C",	PIC32MX130F064C, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX130F064D",	PIC32MX130F064D, DS60001168F,		768, 16384,	32, 256},

{"PIC32MX150F128B",	PIC32MX150F128B, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX150F128C",	PIC32MX150F128C, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX150F128D",	PIC32MX150F128D, DS60001168F,		768, 32768,	32, 256},

{"PIC32MX170F256B",	PIC32MX170F256B, DS60001168F,		768, 65536,	32, 256},
{"PIC32MX170F256D",	PIC32MX170F256D, DS60001168F,		768, 65536,	32, 256},

{"PIC32MX210F016B",	PIC32MX210F016B, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX210F016C",	PIC32MX210F016C, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX210F016D",	PIC32MX210F016D, DS60001168F,		768, 4096,	32, 256},

{"PIC32MX220F032B",	PIC32MX220F032B, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX220F032C",	PIC32MX220F032C, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX220F032D",	PIC32MX220F032D, DS60001168F,		768, 8192,	32, 256},

{"PIC32MX230F064B",	PIC32MX230F064B, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX230F064C",	PIC32MX230F064C, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX230F064D",	PIC32MX230F064D, DS60001168F,		768, 16384,	32, 256},

{"PIC32MX250F128B",	PIC32MX250F128B, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX250F128C",	PIC32MX250F128C, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX250F128D",	PIC32MX250F128D, DS60001168F,		768, 32768,	32, 256},

{"PIC32MX270F256B",	PIC32MX270F256B, DS60001168F,		768, 65536,	32, 256},
{"PIC32MX270F256D",	PIC32MX270F256D, DS60001168F,		768, 65536,	32, 256},

/* PIC32MX330/350/370/430/450/470 DS60001185C */
{"PIC32MX330F064H",	PIC32MX330F064H, DS60001185C,		3072, 16384,	128, 1024},
{"PIC32MX330F064L",	PIC32MX330F064H, DS60001185C,		3072, 16384,	128, 1024},

{"PIC32MX350F128H",	PIC32MX350F128H, DS60001185C,		3072, 32768,	128, 1024},
{"PIC32MX350F128L",	PIC32MX350F128H, DS60001185C,		3072, 32768,	128, 1024},

{"PIC32MX350F256H",	PIC32MX350F256H, DS60001185C,		3072, 65536,	128, 1024},
{"PIC32MX350F256L",	PIC32MX350F256H, DS60001185C,		3072, 65536,	128, 1024},

{"PIC32MX370F512H",	PIC32MX370F512H, DS60001185C,		3072, 131072,	128, 1024},
{"PIC32MX370F512L",	PIC32MX370F512H, DS60001185C,		3072, 131072,	128, 1024},

{"PIC32MX430F064H",	PIC32MX430F064H, DS60001185C,		3072, 16384,	128, 1024},
{"PIC32MX430F064L",	PIC32MX430F064H, DS60001185C,		3072, 16384,	128, 1024},

{"PIC32MX450F128H",	PIC32MX450F128H, DS60001185C,		3072, 32768,	128, 1024},
{"PIC32MX450F128L",	PIC32MX450F128H, DS60001185C,		3072, 32768,	128, 1024},

{"PIC32MX450F256H",	PIC32MX450F256H, DS60001185C,		3072, 65536,	128, 1024},
{"PIC32MX450F256L",	PIC32MX450F256H, DS60001185C,		3072, 65536,	128, 1024},

{"PIC32MX470F512H",	PIC32MX470F512H, DS60001185C,		3072, 131072,	128, 1024},
{"PIC32MX470F512L",	PIC32MX470F512H, DS60001185C,		3072, 131072,	128, 1024},

/* PIC32MX3XX/4XX DS61143H */
{"PIC32MX320F032H",	PIC32MX320F032H, DS61143H,		3072, 8192,	128, 1024},
{"PIC32MX320F064H",	PIC32MX320F064H, DS61143H,		3072, 16384,	128, 1024},
{"PIC32MX320F128H",	PIC32MX320F128H, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX340F128H",	PIC32MX340F128H, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX340F256H",	PIC32MX340F256H, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX340F512H",	PIC32MX340F512H, DS61143H,		3072, 131072,	128, 1024},

{"PIC32MX320F128L",	PIC32MX320F128L, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX340F128L",	PIC32MX340F128L, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX360F256L",	PIC32MX360F256L, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX360F512L",	PIC32MX360F512L, DS61143H,		3072, 131072,	128, 1024},

{"PIC32MX420F032H",	PIC32MX420F032H, DS61143H,		3072, 8192,	128, 1024},

{"PIC32MX440F128H",	PIC32MX440F128H, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX440F256H",	PIC32MX440F256H, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX440F512H",	PIC32MX440F512H, DS61143H,		3072, 131072,	128, 1024},
{"PIC32MX440F128L",	PIC32MX440F128L, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX460F256L",	PIC32MX460F256L, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX460F512L",	PIC32MX460F512L, DS61143H,		3072, 131072,	128, 1024},

/* PIC32MX5XX/6XX/7XX DS61156H */
{"PIC32MX534F064H",	PIC32MX534F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F064H",	PIC32MX564F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F128H",	PIC32MX564F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX575F256H",	PIC32MX575F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX575F512H",	PIC32MX575F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX534F064L",	PIC32MX534F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F064L",	PIC32MX564F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F128L",	PIC32MX564F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX575F256L",	PIC32MX575F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX575F512L",	PIC32MX575F512L, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX664F064H",	PIC32MX664F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX664F128H",	PIC32MX664F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX675F256H",	PIC32MX675F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX675F512H",	PIC32MX675F512H, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX695F512H",	PIC32MX695F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX664F064L",	PIC32MX664F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX664F128L",	PIC32MX664F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX675F256L",	PIC32MX675F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX675F512L",	PIC32MX675F512L, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX695F512L",	PIC32MX695F512L, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX764F128H",	PIC32MX764F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX775F256H",	PIC32MX775F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX775F512H",	PIC32MX775F512H, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX795F512H",	PIC32MX795F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX764F128L",	PIC32MX764F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX775F256L",	PIC32MX775F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX775F512L",	PIC32MX775F512L, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX795F512L",	PIC32MX795F512L, DS61156H,		3072, 131072,	128, 1024},

/* PIC32MZ EC DS60001191C */
{"PIC32MZ1024ECG064", PIC32MZ1024ECG064, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECG100", PIC32MZ1024ECG100, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECG124", PIC32MZ1024ECG124, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECG144", PIC32MZ1024ECG144, DS60001191C,		40960, 262144,	512, 4096},

{"PIC32MZ1024ECH064", PIC32MZ1024ECH064, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECH100", PIC32MZ1024ECH100, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECH124", PIC32MZ1024ECH124, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECH144", PIC32MZ1024ECH144, DS60001191C,		40960, 262144,	512, 4096},

{"PIC32MZ2048ECG064", PIC32MZ2048ECG064, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECG100", PIC32MZ2048ECG100, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECG124", PIC32MZ2048ECG124, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECG144", PIC32MZ2048ECG144, DS60001191C,		40960, 524288,	512, 4096},

{"PIC32MZ2048ECH064", PIC32MZ2048ECH064, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECH100", PIC32MZ2048ECH100, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECH124", PIC32MZ2048ECH124, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECH144", PIC32MZ2048ECH144, DS60001191C,		40960, 524288,	512, 4096},

{"PIC32MZ1024ECM064", PIC32MZ1024ECM064, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECM100", PIC32MZ1024ECM100, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECM124", PIC32MZ1024ECM124, DS60001191C,		40960, 262144,	512, 4096},
{"PIC32MZ1024ECM144", PIC32MZ1024ECM144, DS60001191C,		40960, 262144,	512, 4096},

{"PIC32MZ2048ECM064", PIC32MZ2048ECM064, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECM100", PIC32MZ2048ECM100, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECM124", PIC32MZ2048ECM124, DS60001191C,		40960, 524288,	512, 4096},
{"PIC32MZ2048ECM144", PIC32MZ2048ECM144, DS60001191C,		40960, 524288,	512, 4096},

/* PIC32MX1XX/2XX/5XX 64/100-PIN FAMILY */
{"PIC32MX120F064H",	PIC32MX120F064H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(64), 32,256},
{"PIC32MX130F128H",	PIC32MX130F128H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX130F128L",	PIC32MX130F128L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX230F128H",	PIC32MX230F128H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX230F128L",	PIC32MX230F128L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX530F128H",	PIC32MX530F128H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX530F128L",	PIC32MX530F128L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(128),32,256},
{"PIC32MX150F256H",	PIC32MX150F256H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX150F256L",	PIC32MX150F256L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX250F256H",	PIC32MX250F256H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX250F256L",	PIC32MX250F256L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX550F256H",	PIC32MX550F256H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX550F256L",	PIC32MX550F256L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(256),32,256},
{"PIC32MX170F512H",	PIC32MX170F512H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},
{"PIC32MX170F512L",	PIC32MX170F512L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},
{"PIC32MX270F512H",	PIC32MX270F512H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},
{"PIC32MX270F512L",	PIC32MX270F512L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},
{"PIC32MX570F512H",	PIC32MX570F512H, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},
{"PIC32MX570F512L",	PIC32MX570F512L, DS60001290D,	PIC32_WORD(3),PIC32_WORD(512),32,256},

/* PIC32MM0016/0032/0064/0128/0256GPL0XX */
{"PIC32MM0016GPL020",	PIC32MM0016GPL020, DS60001324B,	PIC32_WORD(6),PIC32_WORD(16), 64,512},
{"PIC32MM0032GPL020",	PIC32MM0032GPL020, DS60001324B,	PIC32_WORD(6),PIC32_WORD(32), 64,512},
{"PIC32MM0064GPL020",	PIC32MM0064GPL020, DS60001324B,	PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0016GPL028",	PIC32MM0016GPL028, DS60001324B,	PIC32_WORD(6),PIC32_WORD(16), 64,512},
{"PIC32MM0032GPL028",	PIC32MM0032GPL028, DS60001324B,	PIC32_WORD(6),PIC32_WORD(32), 64,512},
{"PIC32MM0064GPL028",	PIC32MM0064GPL028, DS60001324B,	PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0016GPL036",	PIC32MM0016GPL036, DS60001324B,	PIC32_WORD(6),PIC32_WORD(16), 64,512},
{"PIC32MM0032GPL036",	PIC32MM0032GPL036, DS60001324B,	PIC32_WORD(6),PIC32_WORD(32), 64,512},
{"PIC32MM0064GPL036",	PIC32MM0064GPL036, DS60001324B,	PIC32_WORD(6),PIC32_WORD(64), 64,512},

/* PIC32MM0064/0128/0245/GPM0XX */
{"PIC32MM0064GPM028",   PIC32MM0064GPM028, DS60001387D, PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0128GPM028",   PIC32MM0128GPM028, DS60001387D, PIC32_WORD(6),PIC32_WORD(128),64,512},
{"PIC32MM0256GPM028",   PIC32MM0256GPM028, DS60001387D, PIC32_WORD(6),PIC32_WORD(256),64,512},
{"PIC32MM0064GPM036",   PIC32MM0064GPM036, DS60001387D, PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0128GPM036",   PIC32MM0128GPM036, DS60001387D, PIC32_WORD(6),PIC32_WORD(128),64,512},
{"PIC32MM0256GPM036",   PIC32MM0256GPM036, DS60001387D, PIC32_WORD(6),PIC32_WORD(256),64,512},
{"PIC32MM0064GPM048",   PIC32MM0064GPM048, DS60001387D, PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0128GPM048",   PIC32MM0128GPM048, DS60001387D, PIC32_WORD(6),PIC32_WORD(128),64,512},
{"PIC32MM0256GPM048",   PIC32MM0256GPM048, DS60001387D, PIC32_WORD(6),PIC32_WORD(256),64,512},
{"PIC32MM0064GPM064",   PIC32MM0064GPM064, DS60001387D, PIC32_WORD(6),PIC32_WORD(64), 64,512},
{"PIC32MM0128GPM064",   PIC32MM0128GPM064, DS60001387D, PIC32_WORD(6),PIC32_WORD(128),64,512},
{"PIC32MM0256GPM064",   PIC32MM0256GPM064, DS60001387D, PIC32_WORD(6),PIC32_WORD(256),64,512},

{"(null)",		0,		 0,			0, 0,		0, 0},
/*Device name		Device id	 App. Data-Sheet	BOOT/PROG Size	Row/Page Size*/
};
#define PIC32_SIZE ((sizeof(pic32_map) / sizeof(struct pic32_dsmap)) - 1)

/* Default device (null) */
uint32_t pic32_index = PIC32_SIZE;

void
pic32_selector(void)
{
	uint32_t i;
	char *dnames[PIC32_SIZE];

	for (i = 0; i < PIC32_SIZE; ++i) {
		dnames[i] = pic32_map[i].devicename;
	}
	qsort(dnames, PIC32_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC32_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%-19s", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC32_SIZE);
}

/*****************************************************************************
 *
 * Hardware algorithm map: Device family
 *
 *****************************************************************************/

struct pic32_dstab pic32_tab[] =
{
	/* Extract PE: unzip PICKIT3.jar	      */

	/* PIC32MX1XX/2XX DS60001168F		      */
	{DS60001168F,	"RIPE_11_000301"},

	/* PIC32MX330/350/370/430/450/470 DS60001185C */
	{DS60001185C,	"RIPE_06_000201"},

	/* PIC32MX3XX/4XX DS61143H		      */
	{DS61143H,	"RIPE_06_000201"},

	/* PIC32MX5XX/6XX/7XX DS61156H		      */
	{DS61156H,	"RIPE_06_000201"},

	/* PIC32MZ EC DS60001191C		      */
	/* {DS60001191C, "RIPE_15_000504"},	      */
	{DS60001191C,	"RIPE_15_000502"},

	/* PIC32MX1XX/2XX/5XX DS60001290D	      */
	{DS60001290D,   "RIPE_11_000301"},

	/* PIC32MK 0512/1024			      */
	/* "RIPE_15a_000506"			      */

	/* PIC32MM0016/0032/0064/0128/0256GPL0XX      */
	{DS60001324B,	"RIPE_20b_000510"},

	/* PIC32MM0064/0128/0256GPM0XX		      */
	{DS60001387D,	"RIPE_20a_000510"},

	/* EOF					      */
	{0,		"(null)"},

	/* DS60001145P-page 9                         */
	/* PIC32MK0128/0256/0512/1024 RIPE_15_aabbcc  */

	/* DS60001145S-page 10                        */
	/* PIC32MX1XX/2XX/5XX         RIPE_11_aabbcc  */
	/* PIC32MK 0512/1024          RIPE_15a_aabbcc */

	/* DS60001364C-page 8                         */
	/* PIC32MM0016/0032/0064/0128/0256GPL0XX      */
	/*                            RIPE_20b_aabbcc */
};

/*****************************************************************************
 *
 * Program/Verify mode
 *
 *****************************************************************************/

/*
 * ENTER LVP PROGRAM/VERIFY MODE
 *
 * ENTRY - VDD FIRST
 */
void
pic32_program_verify(void)
{
	/* RESET & ACQUIRE GPIO */
	io_set_vpp(LOW);
	/* DS60001145M P6(100ns) */
	io_usleep(1000);

	/* PGD + PGC + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_pgm(LOW);
	io_usleep(1000);

	/* CONFIGURE I/O (GPIO-BB, FTDI-BB) */
	io_configure(TRUE, FALSE);

	/* VPP HIGH */
	io_set_vpp(HIGH);
	/* DS60001145M P20(500us) */
	io_usleep(500);

	/* VPP LOW */
	io_set_vpp(LOW);
	/* DS60001145M P18(40ns) */
	io_usleep(500);

	/* PROGRAM/VERIFY ENTRY CODE */
	io_program_out(PHCMKEY, 32);
	/* DS60001145M P19(40ns) */
	io_usleep(500);

	/* VPP HIGH */
	io_set_vpp(HIGH);
	/* DS60001145M P7(500ns) */
	io_usleep(500);

	/* ENTER RUN-TEST/IDLE STATE */
	pic32_setmode(PIC32_IDLE6);
}

/*
 * EXIT LVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic32_standby(void)
{
	/* ENTER TEST-LOGIC-RESET STATE */
	pic32_setmode(PIC32_RESET);
	/* DS60001145M P16(0s) */
	io_usleep(500);

	/* PGD + PGC + VPP + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_vpp(LOW);
	io_set_pgm(LOW);
}

/*****************************************************************************
 *
 * Memory Operations
 *
 *****************************************************************************/

/*
 * Kernel KSEG1 address to physical address
 *
 * DS61115F-page 3-15
 */
static inline uint32_t
pic32_phy(uint32_t addr)
{
	return addr & 0x1FFFFFFF;
}

/*
 * Physical address to kernel KSEG1 address
 *
 * DS61115F-page 3-15
 */
static inline uint32_t
pic32_kseg1(uint32_t addr)
{
	return addr | 0xA0000000;
}

/*****************************************************************************
 *
 * Pseudo Operations
 *
 *****************************************************************************/

/*
 * SetMode Pseudo Operation
 *
 *  To set the EJTAG state machine to a specific state
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 13
 */
void
pic32_setmode(uint8_t nbits, uint32_t tms)
{
	/* SET MODE */
	io_clock_bits_4phase(nbits, tms, /* tdi */ 0);
}

/*
 * SendCommand Pseudo Operation
 *
 *  To send a command to select a specific TAP register
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 14
 */
void
pic32_sendcommand(uint8_t nbits, uint32_t tdi)
{
	uint32_t tms = 1UL << (nbits - 1);

	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift IR state.
	 */

	/* ENTER SHIFT-IR STATE */
	io_clock_bit_4phase(1, 0);		/* SELECT-DR		*/
	io_clock_bit_4phase(1, 0);		/* SELECT-IR		*/
	io_clock_bit_4phase(0, 0);		/* CAPTURE-IR		*/
	io_clock_bit_4phase(0, 0);		/* SHIFT-IR		*/

	/*
	 * 2. The command is clocked into the device on TDI while
	 *    holding signal TMS low.
	 *
	 * 3. The last Most Significant bit (MSb) of the command is
	 *    clocked in while setting TMS high.
	 */

	/* SEND COMMAND */
	io_clock_bits_4phase(nbits,  		/* SHIFT-IR .. EXIT1-IR	*/
						tms, tdi);
	/*
	 * 4. The TMS Footer is clocked in on TMS to return the TAP
	 *    controller to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */
	io_clock_bit_4phase(1, 0);		/* UPDATE-IR		*/
	io_clock_bit_4phase(0, 0);		/* RUN-TEST/IDLE 	*/
}

/*
 * XferData Pseudo Operation
 *
 *  To clock data to and from the register selected by the command
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 15
 */
uint32_t
pic32_xferdata(uint8_t nbits, uint32_t tdi)
{
	uint32_t tms = 1UL << (nbits - 1);
	uint32_t tdo;

	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift DR state.
	 */

	/* ENTER SHIFT-DR STATE */
	io_clock_bit_4phase(1, 0);		/* SELECT-DR		*/
	io_clock_bit_4phase(0, 0);		/* CAPTURE-DR		*/
	tdo = io_clock_bit_4phase(0, 0);	/* SHIFT-DR		*/

	/*
	 * 2. The data is clocked in/out of the device on TDI/TDO while
	 *    holding signal TMS low.
	 *
	 * 3. The last MSb of the data is clocked in/out while setting
	 *    TMS high.
	 */

	/* TRANSFER DATA */
	tdo |= io_clock_bits_4phase(nbits,	/* SHIFT-DR .. EXIT1-DR */
						tms, tdi) << 1;
	/*
	 * 4. The TMS Footer is clocked in on TMS to return the TAP
	 *    controller to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */
	io_clock_bit_4phase(1, 0);		/* UPDATE-DR		*/
	io_clock_bit_4phase(0, 0);		/* RUN-TEST/IDLE	*/

	return tdo;
}

/*
 * XferFastData Pseudo Operation
 *
 *  To quickly send 32 bits of data in/out of the device
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 16
 */
uint32_t
pic32_xferfastdata(uint32_t tdi)
{
	uint8_t pracc;
	uint32_t tms = 0x80000000;
	uint32_t tdo;

	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift DR state.
	 */

	/* ENTER SHIFT-DR STATE */
	io_clock_bit_4phase(1, 0);		/* SELECT-DR 		*/
	io_clock_bit_4phase(0, 0);		/* CAPTURE-DR		*/

	pracc = io_clock_bit_4phase(0, 0);	/* SHIFT-DR             */
	if (!pracc) {
		printf("%s: fatal error: processor access invalid [%d]\n",
			__func__, pracc);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
	}

	/*
	 * 2. The input value of the PrAcc bit, which is "0", is clocked
	 *    in.
	 */

	/* PROCESSOR ACCESS */
	tdo = io_clock_bit_4phase(0, 0);	/* SHIFT-DR		*/

	/* TRANSFER DATA */
	tdo |= io_clock_bits_4phase(32,		/* SHIFT-DR .. EXIT1-DR */
						tms, tdi) << 1;
	/*
	 * 3. TMS Footer = 10 is clocked in to return the TAP controller
	 *    to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */
	io_clock_bit_4phase(1, 0);		/* UPDATE-DR		*/
	io_clock_bit_4phase(0, 0);		/* RUN-TEST/IDLE	*/

	return tdo;
}

/*
 * Wait until CPU is ready
 *
 *  Check if processor access bit is set
 *
 * DS60001145P-page 17
 */
void
pic32_wait_for_cpu(void)
{
	uint32_t controlVal;
	struct timeval tv1, tv2, tv3;

	/* Select control register */
	pic32_sendcommand(PIC32_ETAP_CONTROL);

	/* Wait until CPU is ready */
	gettimeofday(&tv1, NULL);
	do {
	 	/* Check if processor access bit is set */
		controlVal = pic32_xferdata(32,
			PIC32_EJTAG_CONTROL_PRACC | PIC32_EJTAG_CONTROL_PROBEN | PIC32_EJTAG_CONTROL_PROBTRAP);
		if (controlVal & PIC32_EJTAG_CONTROL_PRACC)
			return;

		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while (tv3.tv_sec < PIC_TIMEOUT);

	printf("%s: fatal error: cpu not ready\n", __func__);
	pic32_standby();
	io_exit(EX_SOFTWARE); /* Panic */
}

/*
 * XferInstruction Pseudo Operation
 *
 *  To send 32 bits of data for the device to execute
 *
 * DS60001145M-page 18
 */
uint32_t
pic32_xferinstruction(uint32_t instruction)
{
	uint32_t response;

	/* Wait until CPU is ready */
	pic32_wait_for_cpu();

	/* Select data register */
	pic32_sendcommand(PIC32_ETAP_DATA);

	/* Send the instruction */
	response = pic32_xferdata(32, instruction);

	/* Select control register */
	pic32_sendcommand(PIC32_ETAP_CONTROL);

	/* Tell CPU to execute instruction */
	pic32_xferdata(32, PIC32_EJTAG_CONTROL_PROBEN | PIC32_EJTAG_CONTROL_PROBTRAP);

	return response;
}

/*
 * ReadFromAddress Pseudo Operation for PIC32MX, PIC32MZ & PIC32MK devices (!PE)
 *
 *  To read 32 bits of data from the device memory
 *
 * DS60001145P-page 18
 */
uint32_t
pic32_readfromaddress_m4k(uint32_t addr)
{
	uint32_t data;

	/*
	 * 1. Load Fast Data register address to s3
	 */

	/* LUI S3, 0xFF20 	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0x3C13FF20);

	/*
	 * 2. Load memory address to be read into t0
	 */

	/* LUI T0, <Addr31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0x3C080000 | (addr >> 16));
	/* ORI T0,T0 <Addr15:0>	OR LOWER IMMEDIATE */
	pic32_xferinstruction(0x35080000 | (addr & 0xFFFF));

	/*
	 * 3. Read data
	 */

	/* LW T1, 0(T0)		LOAD WORD    */
	pic32_xferinstruction(0x8D090000);

	/*
	 * 4. Store data into Fast Data register
	 */

	/* SW T1, 0(S3)		STORE WORD   */
	pic32_xferinstruction(0xAE690000);
	/* NOP                               */
	pic32_xferinstruction(0x00000000);

	/*
	 * 5. Shift out the data
	 */

	/* Select the Fastdata Register */
	pic32_sendcommand(PIC32_ETAP_FASTDATA);

	/* Send/Receive 32-bit Data */
	data = pic32_xferfastdata(0);

	return data;
}

/*
 * ReadFromAddress Pseudo Operation for PIC32MM devices (!PE)
 *
 *  To read 32 bits of data from the device memory
 *
 * DS60001145P-page 19
 * DS60001364C-page 17
 */
uint32_t
pic32_readfromaddress_mm(uint32_t addr)
{
	uint32_t data;

	/*
	 * 1. Load Fast Data register address to s3
	 */

	/* LUI S3, 0xFF20 	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0xFF2041B3);

	/*
	 * 2. Load memory address to be read into t0
	 */

	/* LUI T0, <Addr31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0x000041A8 | (addr & 0xFFFF0000));
	/* ORI T0,T0 <Addr15:0>	OR LOWER IMMEDIATE */
	pic32_xferinstruction(0x00005108 | (addr << 16));

	/*
	 * 3. Read data
	 */

	/* LW T1, 0(T0)		LOAD WORD    */
	pic32_xferinstruction(0x0000FD28);

	/*
	 * 4. Store data into Fast Data register
	 */

	/* SW T1, 0(S3)		STORE WORD   */
	pic32_xferinstruction(0x0000F933);
	/* NOP32                             */
	pic32_xferinstruction(0x00000000);

	/*
	 * 5. Shift out the data
	 */

	/* Select the Fastdata Register */
	pic32_sendcommand(PIC32_ETAP_FASTDATA);

	/* Send/Receive 32-bit Data */
	data = pic32_xferfastdata(0);

	return data;
}

/*
 * Read From Address (!PE)
 */
static inline uint32_t
pic32_readfromaddress(uint32_t addr)
{
	switch (pic32_map[pic32_index].datasheet) {
	default:return pic32_readfromaddress_m4k(pic32_kseg1(addr));
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		return pic32_readfromaddress_mm(pic32_kseg1(addr));
	}
}

/*****************************************************************************
 *
 * Utility Operations
 *
 *****************************************************************************/

/*
 * Check device status
 *
 * DS60001145M-page 20
 */
uint32_t
pic32_check_device_status(void)
{
	pic32_sendcommand(PIC32_MTAP_SW_MTAP);
	pic32_sendcommand(PIC32_MTAP_COMMAND);

	return pic32_xferdata(PIC32_MCHP_STATUS) & PIC32_MCHP_STATUS_MASK;
}

/*
 * Erase Device
 *
 * DS60001145M-page 21
 * DS60001364C-page 20
 */
static inline void
pic32_erase_device(void)
{
	uint32_t statusVal;
	struct timeval tv1, tv2, tv3;

	/*
	 * SendCommand (MTAP_SW_MTAP).
	 *
	 * SendCommand (MTAP_COMMAND).
	 *
	 * XferData (MCHP_ERASE).
	 */

	pic32_sendcommand(PIC32_MTAP_SW_MTAP);
	pic32_sendcommand(PIC32_MTAP_COMMAND);
	pic32_xferdata(PIC32_MCHP_ERASE);

	/*
	 * XferData (MCHP_DE_ASSERT_RST), not PIC32MX.
	 */

	if ((pic32_map[pic32_index].datasheet == DS60001191C) ||	/* MZ EC */
		(pic32_map[pic32_index].datasheet == DS60001324B) ||
		(pic32_map[pic32_index].datasheet == DS60001387D)) {	/* MM */
		pic32_xferdata(PIC32_MCHP_DE_ASSERT_RST);
	}

	/*
	 * Delay > 10 ms.
	 *
	 * statusVal = XferData (MCHP_STATUS).
	 *
	 * If CFGRDY (statusVal<3>) is not "1" and FCBUSY (statusVal<2>) is not "0", GOTO to step 5.
	 */

	gettimeofday(&tv1, NULL);
	do {
		/*
		 * Wait for "Self-Timed Page Erase Time"
		 *
		 * Eg. DS60001324B-page 214
		 */
		io_usleep(18000); /* 18 ms */

		statusVal = pic32_xferdata(PIC32_MCHP_STATUS) & (PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FCBUSY);

		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while ((statusVal != PIC32_MCHP_STATUS_CFGRDY) && (tv3.tv_sec < PIC_TIMEOUT));

	if (statusVal != PIC32_MCHP_STATUS_CFGRDY) {
		printf("%s: fatal error: erase device failed\n", __func__);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
	}
}

/*
 * Enter Serial Execution Mode
 *
 * DS60001145M-page 22
 */
void
pic32_enter_serial_execution_mode(void)
{
	uint32_t statusVal;
	struct timeval tv1, tv2, tv3;

	statusVal = pic32_check_device_status();
	if (statusVal != (PIC32_MCHP_STATUS_CPS | PIC32_MCHP_STATUS_CFGRDY)) {
		printf("%s: fatal error: status invalid [0x%02X]\n", __func__, statusVal);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
	}

	pic32_xferdata(PIC32_MCHP_ASSERT_RST);		/* 2-Wire */

	pic32_sendcommand(PIC32_MTAP_SW_ETAP);
	pic32_sendcommand(PIC32_ETAP_EJTAGBOOT);

	pic32_sendcommand(PIC32_MTAP_SW_MTAP);		/* 2-Wire ... */
	pic32_sendcommand(PIC32_MTAP_COMMAND);

	pic32_xferdata(PIC32_MCHP_DE_ASSERT_RST);

	/*
	 * Flash Enable, PIC32MX / PIC32MM devices only
	 */

	if (pic32_map[pic32_index].datasheet != DS60001191C) { /* ! MZ EC */

		pic32_xferdata(PIC32_MCHP_FLASH_ENABLE);

		/* Wait until Flash is ready */
		gettimeofday(&tv1, NULL);
		do {
			statusVal = pic32_xferdata(PIC32_MCHP_STATUS) & PIC32_MCHP_STATUS_MASK;

			gettimeofday(&tv2, NULL);
			timersub(&tv2, &tv1, &tv3);
		}
		while ((statusVal != (PIC32_MCHP_STATUS_CPS | PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FAEN)) &&
			(tv3.tv_sec < PIC_TIMEOUT));

		if (statusVal != (PIC32_MCHP_STATUS_CPS | PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FAEN)) {
			printf("%s: fatal error: status invalid [0x%02X]\n", __func__, statusVal);
			pic32_standby();
			io_exit(EX_SOFTWARE); /* Panic */
		}
	}

	pic32_sendcommand(PIC32_MTAP_SW_ETAP);
}

/*****************************************************************************
 *
 * PE loader
 *
 *****************************************************************************/

/*
 * PIC32MX BMX
 */
static void
pic32_bmx(void)
{
	/*
	 * PIC32MX devices only: Initialize BMXCON to 0x1F0040.
	 *
	 * lui a0,0xbf88
	 * ori a0,a0,0x2000 // address of BMXCON
	 * lui a1,0x1f
	 * ori a1,a1,0x40   // $a1 has 0x1f0040
	 * sw a1,0(a0)      // BMXCON initialized
	 */

	pic32_xferinstruction(0x3c04bf88);
	pic32_xferinstruction(0x34842000);
	pic32_xferinstruction(0x3c05001f);
	pic32_xferinstruction(0x34a50040);
	pic32_xferinstruction(0xac850000);

	/*
	 * PIC32MX devices only: Initialize BMXDKPBA to 0x800.
	 *
	 * li a1,0x800
	 * sw a1,16(a0)
	 */

	pic32_xferinstruction(0x34050800);
	pic32_xferinstruction(0xac850010);

	/*
	 * PIC32MX devices only: Initialize BMXDUDBA and BMXDUPBA to the value of BMXDRMSZ.
	 *
	 * lw a1,64(a0) // load BMXDMSZ
	 * sw a1,32(a0)
	 * sw a1,48(a0)
	 */

	pic32_xferinstruction(0x8C850040);
	pic32_xferinstruction(0xac850020);
	pic32_xferinstruction(0xac850030);
}

#if 0
/*
 * Download the PE for PIC32MX, PIC32MZ & PIC32MK devices
 *
 * Custom PE loader.
 */
void
pic32_download_custom_pe_m4k(uint8_t *pe, uint32_t nbytes)
{
	/* PIC32MX BMX */
	if (pic32_map[pic32_index].datasheet != DS60001191C) /* ! MZ EC */
		pic32_bmx();

	/* LUI S0, <RAM_ADDR(31:16)>   */
	pic32_xferinstruction(0x3C1A0000);

	/* ORI S0,S0, <RAM_ADDR(15:0)> */
	pic32_xferinstruction(0x36100900);

	for (uint32_t i = 0; i < nbytes; i += 4) {
		/* Get low and high word data */
		uint16_t datal = pe[i]     | (pe[i + 1] << 8);
		uint16_t datah = pe[i + 2] | (pe[i + 3] << 8);

		/* LUI T0, <Data31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
		pic32_xferinstruction(0x3C080000 + datah);

		/* ORI T0,T0 <Data15:0>	OR LOWER IMMEDIATE */
		pic32_xferinstruction(0x35080000 + datal);

		/* SW T0, <OFFSET>(S0)	STORE WORD */
		pic32_xferinstruction(0xAE080000 + i);
	}

	/*
	 * Jump to the PE
	 *
	 * lui t9,0xa000
	 * ori t9,t9,0x900
	 * jr t9
	 * nop
	 */

	pic32_xferinstruction(0x3c19a000);
	pic32_xferinstruction(0x37390900);
	pic32_xferinstruction(0x03200008);
	pic32_xferinstruction(0x00000000);
}
#endif

/*
 * Download the PE for PIC32MX, PIC32MZ & PIC32MK devices
 *
 * DS60001145N-page 23
 */
void
pic32_download_pe_m4k(uint8_t *pe, uint32_t nbytes)
{
	/* PE LOADER OP CODES */
	static uint32_t peloader[] = {
		0x3c07dead, // lui a3, 0xdead
		0x3c06ff20, // lui a2, 0xff20
		0x3c05ff20, // lui al, 0xff20
			    // here1:
		0x8cc40000, // lw a0, 0 (a2)
		0x8cc30000, // lw v1, 0 (a2)
		0x1067000b, // beq v1, a3, <here3>
		0x00000000, // nop
		0x1060fffb, // beqz v1, <here1>
		0x00000000, // nop
			    // here2:
		0x8ca20000, // lw v0, 0 (a1)
		0x2463ffff, // addiu v1, v1, -1
		0xac820000, // sw v0, 0 (a0)
		0x24840004, // addiu a0, a0, 4
		0x1460fffb, // bnez v1, <here2>
		0x00000000, // nop
		0x1000fff3, // b <here1>
		0x00000000, // nop
			    // here3:
		0x3c02a000, // lui v0, 0xa000
		0x34420900, // ori v0, v0, 0x900
		0x00400008, // jr v0
		0x00000000, // nop
	};
	#define PESIZE (sizeof(peloader) / sizeof(uint32_t))

	/* PIC32MX BMX */
	if (pic32_map[pic32_index].datasheet != DS60001191C) /* ! MZ EC */
		pic32_bmx();

	/*
	 * Set up PIC32 RAM address for PE.
	 *
	 * lui a0,0xa000
	 * ori a0,a0,0x800
	 */

	pic32_xferinstruction(0x3c04a000);
	pic32_xferinstruction(0x34840800);

	/*
	 * Load the PE_Loader.
	 *
	 * lui a2, <PE_loader hi++>
	 * ori a2,a2, <PE_loader lo++>
	 * sw a2,0(a0)
	 * addiu a0,a0,4
	 */

	for (uint32_t i = 0; i < PESIZE; ++i) {
		uint32_t datah = peloader[i] >> 16;
		uint32_t datal = peloader[i] & 0x0000FFFF;

		pic32_xferinstruction(0x3c060000 + datah);
		pic32_xferinstruction(0x34c60000 + datal);
		pic32_xferinstruction(0xac860000);
		pic32_xferinstruction(0x24840004);
	}

	/*
	 * Jump to the PE_Loader.
	 *
	 * lui t9,0xa000
	 * ori t9,t9,0x800
	 * jr t9
	 * nop
	 */

	pic32_xferinstruction(0x3c19a000);
	pic32_xferinstruction(0x37390800);
	pic32_xferinstruction(0x03200008);
	pic32_xferinstruction(0x00000000);

	/*
	 * Load the PE using the PE_Loader.
	 */

	pic32_sendcommand(PIC32_ETAP_FASTDATA);
	pic32_xferfastdata(pic32_kseg1(0x0900));
	pic32_xferfastdata(nbytes >> 2);
	for (uint32_t i = 0; i < nbytes; i += 4) {
		uint32_t wdata = (uint32_t)pe[i] |
			((uint32_t)pe[i + 1] << 8) |
			((uint32_t)pe[i + 2] << 16) |
			((uint32_t)pe[i + 3] << 24);
		pic32_xferfastdata(wdata);
	}

	/*
	 * Jump to the PE.
	 */

	pic32_xferfastdata(0x00000000);
	pic32_xferfastdata(0xDEAD0000);
	io_usleep(10000); /* 10ms */
}

#if 0
/*
 * PIC32MM CFGCON
 */
static void
pic32_cfgcon(void)
{
	pic32_xferinstruction(0xBF8041A4); /* lui a0,0xbf80    */
	pic32_xferinstruction(0x3B005084); /* ori a0,a0,0x3b00 */
	pic32_xferinstruction(0x000041A5); /* lui a1,0x0000    */
	pic32_xferinstruction(0x000050A5); /* ori a1,a1,0x0000 */
	pic32_xferinstruction(0x0000F8A4); /* sw a1,0(a0)      */
}
#endif

#if 0
/*
 * Download the PE for PIC32MM GPL devices
 *
 * Custom PE loader.
 */
void
pic32_download_custom_pe_mm(uint8_t *pe, uint32_t nbytes)
{
	uint32_t address = 0xA0000300;

	/* LUI A0, <RAM_ADDR(31:16)> */
	pic32_xferinstruction(0x41A4 | (address & 0xFFFF0000));

	/* ORI A0,A0, <RAM_ADDR(15:0)> */
	pic32_xferinstruction(0x5084 | (address << 16));

	for (uint32_t i = 0; i < nbytes; i += 4) {
		/* Get low and high word data */
		uint16_t datal = pe[i]     | (pe[i + 1] << 8);
		uint16_t datah = pe[i + 2] | (pe[i + 3] << 8);

		/* LUI A1, <Data31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
		pic32_xferinstruction(0x41A5 | (datah << 16));

		/* ORI A1,A1 <Data15:0>	OR LOWER IMMEDIATE */
		pic32_xferinstruction(0x50A5 | (datal << 16));

		/* SW A1, <OFFSET>(A0)	STORE WORD */
		pic32_xferinstruction(0xF8A4 | (i << 16));
	}
#if 1
	for (uint32_t i = 0; i < nbytes; i += 4) {
		uint32_t *ptr = (uint32_t *)&pe[i];
		uint32_t wrote = *ptr;
		uint32_t read = pic32_readfromaddress(address + i);
		printf("ADDRESS=0x%08X WROTE=0x%08X READ=0x%08X\n",
			address + i, wrote, read);
		if (wrote != read) {
			printf("FAULT\n");
			pic32_standby();
			io_exit(EX_SOFTWARE); /* Panic */
		}
	}
#endif
	/*
	 * Jump to the PE.
	 *
	 * lui t9,0xXXXX
	 * ori t9,t9,0xXXXX
	 * jr t9; nop16
	 */

	pic32_xferinstruction(0x41B9 | (address & 0xFFFF0000));
	pic32_xferinstruction(0x5339 | (address << 16));
	pic32_xferinstruction(0x0C004599);
}
#endif

/*
 * Download the PE for PIC32MM devices
 *
 * DS60001145P-page 25
 * DS60001364C-page 23
 * DS60001364D-pages 22 and 23
 */
void
pic32_download_pe_mm(uint8_t *pe, uint32_t nbytes)
{
	/* PE LOADER OP CODES */
	static uint32_t peloader[] = {
	/*
	 * TABLE 11-2
	 *
	 * NOT WORKING
	 *
	 *	0xDEAD41A7, // lui a3, 0xdead
	 *	0xFF2041A6, // lui a2, 0xff20
	 *	0xFF2041A5, // lui al, 0xff20
	 *		    // here1:
	 *	0x69E06A60, // lw a0, 0 (a2)
	 *		    // lw v1, 0 (a2)
	 *	0x000C94E3, // beq v1, a3, <here3>
	 *	0x8DFA0C00, // nop16
	 *		    // beqz v1, <here1>
	 *		    // here2:
	 *		    // lw v0, 0 (a1)
	 *	0xE9406DBE, // addiu v1, v1, -1
	 *		    // sw v0, 0 (a0)
	 *	0xADFB6E42, // addiu a0, a0, 4
	 *		    // bnez v1, <here2>
	 *	0xCFF20C00, // nop16
	 *		    // b <here1>
	 *	0x0C000C00, // nop16; nop16
	 *		    // here3:
	 *	0xA00041A2, // lui v0, 0xa000
	 *	0x03005042, // ori v0, v0, 0x300
	 *	0x0C004582, // jr v0
	 *		    // nop16
	 *
	 * SOLUTION
	 *
	 * See: https://www.microchip.com/forums/m973762.aspx
	 */
		0xFF2041A3,
		0xDEAD41A5,
		0x69306A30,
		0x000994A2,
		0xA00041B9,
		0xFFF840E2,
		0xEB406B30,
		0xCFFA6E42,
		0x33396D2E,
		0x45990301,
		0x0C000C00
	};
	#define PESIZE (sizeof(peloader) / sizeof(uint32_t))

	/*
	 * Set up the PIC32MM RAM address for the PE.
	 *
	 * lui a0,0xa000
	 * ori a0,a0,0x200
	 */

	pic32_xferinstruction(0xA00041A4);
	pic32_xferinstruction(0x02005084);

	/*
	 * Load the PE_loader.
	 *
	 * lui a2, <PE_loader hi++>
	 * ori a2,a2, <PE_loader lo++>
	 * sw a2,0(a0)
	 * addiu a0,a0,4
	 */

	for (uint32_t i = 0; i < PESIZE; ++i) {
		pic32_xferinstruction(0x41A6 | (peloader[i] & 0xFFFF0000));
		pic32_xferinstruction(0x50C6 | (peloader[i] << 16));
		pic32_xferinstruction(0x6E42EB40);
	}

	/*
	 * Jump to the PE_loader.
	 *
	 * lui t9,0xa000
	 * ori t9,t9,0x201
	 * jr t9; nop16
	 * nop16; nop16
	 * nop16; nop16
	 */

	pic32_xferinstruction(0xA00041B9);
	pic32_xferinstruction(0x02015339);
	pic32_xferinstruction(0x0C004599);
	pic32_xferinstruction(0x0C000C00);
	pic32_xferinstruction(0x0C000C00);

	/*
	 * Load the PE using the PE_loader.
	 */

	pic32_sendcommand(PIC32_ETAP_FASTDATA);
	pic32_xferfastdata(pic32_kseg1(0x0300));
	pic32_xferfastdata(nbytes >> 2);
	for (uint32_t i = 0; i < nbytes; i += 4) {
		uint32_t wdata = (uint32_t)pe[i] |
			((uint32_t)pe[i + 1] << 8) |
			((uint32_t)pe[i + 2] << 16) |
			((uint32_t)pe[i + 3] << 24);
		pic32_xferfastdata(wdata);
	}

	/*
	 * Jump to the PE.
	 */

	pic32_xferfastdata(0x00000000);
	pic32_xferfastdata(0xDEAD0000);
	io_usleep(10000); /* 10ms */
}

/*****************************************************************************
 *
 * Download data
 *
 *****************************************************************************/

/*
 * Download Data Block for PIC32MX, PIC32MZ & PIC32MK devices (!PE)
 *
 * DS60001145M-page 25
 */
void
pic32_download_data_block_m4k(uint32_t *panel, uint32_t panel_size)
{
	/*
	 * Initialize SRAM Base Address to 0xA0000000.
	 */

	/* LUI S0, 0xA000	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0x3C10A000);

	/*
	 * Write the entire row of data to be programmed into system SRAM.
	 *
	 * Repeat until one row of data has been loaded.
	 */

	uint32_t offset = 0;
	uint16_t datal, datah;
	do {
		/* Get low and high word data */
		datal = (uint16_t)panel[offset + 0] | (uint16_t)panel[offset + 1] << 8;
		datah = (uint16_t)panel[offset + 2] | (uint16_t)panel[offset + 3] << 8;

		/* LUI T0, <Data31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
		pic32_xferinstruction(0x3C080000 | datah);

		/* ORI T0,T0 <Data15:0>	OR LOWER IMMEDIATE */
		pic32_xferinstruction(0x35080000 | datal);

		/* SW T0, <OFFSET>(S0)	STORE WORD */
		pic32_xferinstruction(0xAE080000 | offset);

		/* OFFSET increments by 4 */
		offset += 4;
	}
	while (offset < panel_size);
}

/*
 * Download Data Block for PIC32MM devices (!PE)
 *
 * DS60001364C-page 24
 */
void
pic32_download_data_block_mm(uint32_t *panel, uint32_t panel_size)
{
	/*
	 * Initialize SRAM Base Address to 0xA0000000.
	 */

	/* LUI A0, 0xA000	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(0xA00041A4);

	/*
	 * Write the entire row of data to be programmed into system SRAM.
	 *
	 * Repeat until one row of data has been loaded.
	 */

	uint32_t offset = 0;
	uint16_t datal, datah;
	do {
		/* Get low and high word data */
		datal = (uint16_t)panel[offset + 0] | (uint16_t)panel[offset + 1] << 8;
		datah = (uint16_t)panel[offset + 2] | (uint16_t)panel[offset + 3] << 8;

		/* LUI A1, <Data31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
		pic32_xferinstruction(0x41A5 | (datah << 16));

		/* ORI A1,A1 <Data15:0>	OR LOWER IMMEDIATE */
		pic32_xferinstruction(0x50A5 | (datal << 16));

		/* SW A1, <OFFSET>(A0)	STORE WORD */
		pic32_xferinstruction(0xF8A4 | (offset << 16));

		/* OFFSET increments by 4 */
		offset += 4;
	}
	while (offset < panel_size);
}

/*
 * Download Data Block (!PE)
 */
static inline void
pic32_download_data_block(uint32_t *panel, uint32_t panel_size)
{
	switch (pic32_map[pic32_index].datasheet) {
	default:pic32_download_data_block_m4k(panel, panel_size);
		break;
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		pic32_download_data_block_mm(panel, panel_size);
		break;
	}
}

/*****************************************************************************
 *
 * Flash write
 *
 *****************************************************************************/

/*
 * Flash Row Write for PIC32MX, PIC32MZ & PIC32MK devices (!PE)
 *
 * DS60001145M-page 26
 */
void
pic32_flash_row_write_m4k(uint32_t address)
{
	/*
	 * Registers a1, a2, and a3 are set for WREN = 1 or NVMOP<3:0> = 0011,
	 * WR = 1 and WREN = 1, respectively.
	 *
	 * Registers s1 and s2 are set for the unlock data values and S0 is
	 * initialized to "0".
	 */

	pic32_xferinstruction(0x34054003); /* ori a1,$0,0x4003 */
	pic32_xferinstruction(0x34068000); /* ori a2,$0,0x8000 */
	pic32_xferinstruction(0x34074000); /* ori a3,$0,0x4000 */

	pic32_xferinstruction(0x3c11aa99); /* lui s1,0xaa99    */
	pic32_xferinstruction(0x36316655); /* ori s1,s1,0x6655 */
	pic32_xferinstruction(0x3c125566); /* lui s2,0x5566    */
	pic32_xferinstruction(0x365299aa); /* ori s2,s2,0x99aa */

	/*
	 * Set the physical source SRAM address
	 */

	pic32_xferinstruction(0x3c100000); /* lui s0,0x0000 <RAM_ADDR(31:16)> */

	/*
	 * PIC32MX devices only: Set register a0 to the base address of the NVM
	 * register (0xBF80_F400).
	 */

	if (pic32_map[pic32_index].datasheet != DS60001191C) { /* ! MZ EC */
		pic32_xferinstruction(0x3c04bf80); /* lui a0,0xbf80	  */
		pic32_xferinstruction(0x3484f400); /* ori a0,a0,0xf400 */
	}

	/*
	 * PIC32MZ EC devices only: Set register a0 to the base address of the
	 * NVM register (0xBF80_0600).
	 *
	 * Register s3 is set for the value used to disable write protection in
	 * NVMBPB.
	 *
	 * Unlock and disable Boot Flash write protection.
	 */

	else {
		pic32_xferinstruction(0x3c04bf80); /* lui a0,0xbf80    */
		pic32_xferinstruction(0x34840600); /* ori a0,a0,0x0600 */

		pic32_xferinstruction(0x34138080); /* ori s3,$0,0x8080 */

		pic32_xferinstruction(0xac910010); /* sw s1,16(a0)     */
		pic32_xferinstruction(0xac920010); /* sw s2,16(a0)     */
		pic32_xferinstruction(0xac930090); /* sw s3,144(a0)    */

		pic32_xferinstruction(0x00000000); /* nop              */
	}

	/*
	 * Set the NVMADDR register with the address of the Flash row to be programmed.
	 */

	uint32_t addrh = pic32_phy(address) >> 16, addrl = pic32_phy(address) & 0x0000FFFF;

	pic32_xferinstruction(0x3c080000 + addrh); /* lui t0,<FLASH_ROW_ADDR(31:16)>   */
	pic32_xferinstruction(0x35080000 + addrl); /* ori t0,t0,<FLASH_ROW_ADDR(15:0)> */
	pic32_xferinstruction(0xac880020);         /* sw t0,32(a0)                     */

	/*
	 * PIC32MX devices only: Set the NVMSRCADDR register with the physical
	 * source SRAM address (offset is 64).
	 */

	if (pic32_map[pic32_index].datasheet != DS60001191C) /* ! MZ EC */
		pic32_xferinstruction(0xac900040);           /* sw s0,64(a0) */

	/*
	 * PIC32MZ EC devices only: Set the NVMSRCADDR register with the
	 * physical source SRAM address (offset is 112).
	 */

	else
		pic32_xferinstruction(0xac900070);           /* sw s0,112(a0) */

	/*
	 * Set up the NVMCON register for write operation.
	 */

	pic32_xferinstruction(0xac850000); /* sw a1,0(a0) */
#if 0
	io_usleep(6); /* Min. delay 6 us */
#endif
	/*
	 * Unlock the NVMCON register and start the write operation.
	 */

	pic32_xferinstruction(0xac910010); /* sw s1,16(a0) */
	pic32_xferinstruction(0xac920010); /* sw s2,16(a0) */
	pic32_xferinstruction(0xac860008); /* sw a2,8(a0)  */

	/*
	 * Wait for "Self-Timed Row Write Cycle Time"
	 *
	 * Eg. DS60001324B-page 214
	 */
#if 0
	io_usleep(1500); /* 1.5 ms */
#endif
	/*
	 * Clear the WREN bit (NVMCONM<14>).
	 */

	pic32_xferinstruction(0xac870004); /* sw a3,4(a0) */
}

/*
 * Flash Row Write for PIC32MM devices (!PE)
 *
 * DS60001145P-page 29
 * DS60001364C-page 26
 * DS60001364E-page 27
 */
void
pic32_flash_row_write_mm(uint32_t address)
{
	/*
	 * Initialize constants. Registers a1 and a2 are set for WREN = 1 or NVMOP<3:0> = 0011
	 * and WR = 1, respectively. Registers s1 and s2 are set for the unlock data values and
	 * s0 is initialized to "0".
	 */

	pic32_xferinstruction(0x400350A0); /* li a1,0x4003     */
	pic32_xferinstruction(0x800050C0); /* li a2,0x8000     */

	pic32_xferinstruction(0xAA9941B1); /* lui s1,0xaa99    */
	pic32_xferinstruction(0x66555231); /* ori s1,s1,0x6655 */
	pic32_xferinstruction(0x556641B2); /* lui s2,0x5566    */
	pic32_xferinstruction(0x99AA5252); /* ori s2,s2,0x99aa */

	/*
	 * Set the physical source SRAM address
	 */

	pic32_xferinstruction(0x000041B0); /* lui s0,0x0000 <RAM_ADDR(31:16)> */

	/*
	 * Set register a0 to the base address of the NVM controller. Register s3
	 * is set for the value used to disable write protection in NVMBPB.
	 */
	
	uint32_t nvmcon = 0;
	switch (pic32_map[pic32_index].datasheet) {
	default:printf("%s: fatal error: NVMCON not defined for PIC32 family\n", __func__);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
		break;
	case DS60001324B:
		nvmcon = PIC32MM_GPL_NVMCON; /* 0xBF802380 */
		break;
	case DS60001387D:
		nvmcon = PIC32MM_GPM_NVMCON; /* 0xBF802930 */
		break;
	}
	pic32_xferinstruction(0x41A4 | (nvmcon & 0xFFFF0000)); /* lui a0,<NVMCON(31:16)> */
	pic32_xferinstruction(0x5084 | (nvmcon << 16));        /* ori a0,a0,<NVMCON(15:0)> */
	pic32_xferinstruction(0x80005260);                     /* li s3,0x8000 // BWPAUNLK bit mask */

	/*
	 * Unlock and disable Boot Flash write protection.
	 */

	pic32_xferinstruction(0x0010FA24); /* sw s1,16(a0)  // NVMKEY = 0xaa996655 */
	pic32_xferinstruction(0x0010FA44); /* sw s2,16(a0)  // NVMKEY = 0x556699aa */
	pic32_xferinstruction(0x0070FA64); /* sw s3,112(a0) // BWPAUNLK bit (NVMBPB register) = 1 */

	/*
	 * Set the NVMADDR register with the address of the Flash row to be programmed.
	 */

	pic32_xferinstruction(0x41A8 | (address & 0xFFFF0000)); /* lui t0,<FLASH_ROW_ADDR(31:16)>   */
	pic32_xferinstruction(0x5108 | (address << 16));        /* ori t0,t0,<FLASH_ROW_ADDR(15:0)> */
	pic32_xferinstruction(0x0020F904);                      /* sw t0,32(a0) */

	/*
	 * Set the NVMSRCADDR register with the physical source SRAM address.
	 */

	pic32_xferinstruction(0x0050FA04); /* sw s0,80(a0) */

	/*
	 * Set up the NVMCON register for write operation.
	 */

	pic32_xferinstruction(0x0C00EAC0); /* sw a1,0(a0); nop16 // NVMCON = 0x4003 */
#if 0
	io_usleep(6); /* Min. delay 6 us */
#endif
	/*
	 * Unlock the NVMCON register and start the write operation (WR bit = 1).
	 */

	pic32_xferinstruction(0xFA44E8C4); /* sw s1,16(a0) // NVMKEY = 0xaa996655 */
	pic32_xferinstruction(0xEB420010); /* sw s2,16(a0) // NVMKEY = 0x556699aa */
					   /* sw a2,8(a0)  // NVMCONSET = 0x8000  */
	/*
	 * Wait for "Self-Timed Row Write Cycle Time"
	 *
	 * Eg. DS60001324B-page 214
	 */
#if 0
	io_usleep(1500); /* 1.5 ms */
#else
	/*
	 * Wait at least 500 ns after seeing a "0" in the WR bit (NVMCON[15]) before
	 * writing to any of the NVM registers. This requires inserting a NOP in the
	 * execution.
	 *
	 * DS60001364E-page 27
	 */

	pic32_xferinstruction(0x0C000C00); /* nop16; nop16 */
	pic32_xferinstruction(0x0C000C00); /* nop16; nop16 */
#endif
	/*
	 * Clear the WREN bit (NVMCONM<14>).
	 */

	pic32_xferinstruction(0x400050E0); /* li a3,0x4000       */
	pic32_xferinstruction(0x0C00EBC1); /* sw a3,4(a0); nop16 */
}

/*
 * Flash Row Write (!PE)
 */
static inline void
pic32_flash_row_write(uint32_t address)
{
	switch (pic32_map[pic32_index].datasheet) {
	default:pic32_flash_row_write_m4k(pic32_phy(address));
		break;
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		pic32_flash_row_write_mm(pic32_phy(address));
		break;
	}
}

/*
 * Get PE Resonse
 *
 * DS60001145N-page 30
 */
static inline uint32_t
pic32_getperesponse(void)
{
	return pic32_xferinstruction(0x00000000);
}

/*****************************************************************************
 *
 * PE Commands
 *
 *****************************************************************************/

/*
 * PE ROW PROGRAM
 *
 *  Op code 0x0
 *
 * DS60001145N-page 32
 * DS60001364E-page 30
 */
void
pic32_pe_row_program(uint32_t addr, uint32_t *panel, uint32_t panel_size)
{
	/* Select the Fastdata Register */
	pic32_sendcommand(PIC32_ETAP_FASTDATA);

	/* Op code + Operand */
	pic32_xferfastdata(panel_size >> 2);

	/* PHYSICAL Address */
	pic32_xferfastdata(pic32_phy(addr));

	/* Data */
	for (uint32_t i = 0; i < panel_size; i += 4) {
		uint32_t wdata = (uint32_t)panel[i] |
			((uint32_t)panel[i + 1] << 8) |
			((uint32_t)panel[i + 2] << 16) |
			((uint32_t)panel[i + 3] << 24);
		pic32_xferfastdata(wdata);
	}
	
	/*
	 * PIC32MM GPL: Allow JTAGEN to be cleared, per section 17
	 * of DS60001364E. The specification states that you only need
	 * to do this for four-wire JTAG, but PIC32MM0064GPL028
	 * silicon rev 4 also needs this on ICSP.
	 */
	if (pic32_kseg1(addr) == PIC32MM_CONFIG_PANEL &&
		pic32_map[pic32_index].datasheet == DS60001324B) /* PIC32MM GPL */ {
		pic32_sendcommand(PIC32_MTAP_SW_MTAP);
		/* PE delay (DS60001364E) + Self-Timed Row Write Cycle Time (DS60001324B) */
		io_usleep(250 + 1500);
		pic32_sendcommand(PIC32_MTAP_SW_ETAP);
	}

	/* Check response */
	uint32_t rc = pic32_getperesponse();
	if (rc & 0xFFFF) {
		printf("%s: fatal error: PE ROW PROGRAM failed [0x%08X]\n",
			__func__, rc);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
	}
}

/*
 * PE READ
 *
 *  Op code 0x1
 *
 * DS60001145N-page 33
 */
void
pic32_pe_read(uint32_t addr, uint16_t nwords, uint32_t *data)
{
	/* Select the Fastdata Register */
	pic32_sendcommand(PIC32_ETAP_FASTDATA);

	/* Op code + Operand */
	pic32_xferfastdata(0x00010000 + nwords);

	/* KSEG1 Address */
	pic32_xferfastdata(pic32_kseg1(addr));

	/* Check response */
	uint32_t rc = pic32_getperesponse();
	if (rc != 0x00010000) {
		printf("%s: fatal error: PE READ failed [0x%08X]\n",
			__func__, rc);
		pic32_standby();
		io_exit(EX_SOFTWARE); /* Panic */
	}

	/* Select the Fastdata Register */
	pic32_sendcommand(PIC32_ETAP_FASTDATA);

	/* Get Data */
	for (uint32_t i = 0; i < nwords; ++i)
		data[i]	= pic32_xferfastdata(0);
}

/*****************************************************************************
 *
 * PE functions
 *
 *****************************************************************************/

/*
 * PE LOAD
 */
void
pic32_pe_load(void)
{
	uint8_t *pe = NULL;
	uint32_t nbytes = 0;

	/* Validate PE */
	if (pic32_conf.pepath[0] == '\0') {
		return;
	}

	/* Get PE */
	pe = pic_pe_read_file(pic32_conf.pepath, &nbytes);
	if (pe == NULL) {
		bzero(pic32_conf.pepath, STRLEN);
		return;
	}

	/* Download PE (upload to chip) */
	switch (pic32_map[pic32_index].datasheet) {
	default:pic32_download_pe_m4k(pe, nbytes);
		break;
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		pic32_download_pe_mm(pe, nbytes);
		break;
	}

	/* Free PE */
	free(pe);
}

/*
 * PE READ WITH CACHE
 */
uint32_t
pic32_pe_readword(uint32_t address)
{
	static uint32_t cache_addr = UINT32_MAX;
	static uint32_t cache_data[PIC32_PE_READWORDS];

	uint32_t new_address = address & PIC32_PE_READPAGE;

	if (new_address != cache_addr) {
		pic32_pe_read(new_address, PIC32_PE_READWORDS, cache_data);
		cache_addr = new_address;
	}
	return cache_data[(address >> 2) & PIC32_PE_READMASK];
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic32_bulk_erase(void)
{
	pic32_program_verify();

	pic32_erase_device();

	pic32_standby();
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * GET CONFIGURATION
 */
int32_t
pic32_read_config_memory(void)
{
	/* NULL device */
	pic32_index = PIC32_SIZE;

	/* Reset configuration */
	memset(&pic32_conf, 0, sizeof(pic32_conf));

	pic32_program_verify();

	/* Device id/rev */
	pic32_conf.deviceid = pic32_xferdata(32, 0);

	uint32_t dev = 0;
	while (pic32_map[dev].deviceid) {
		if (pic32_map[dev].deviceid == (pic32_conf.deviceid & PIC32_IDMASK))
			break;
		++dev;
	}
	if (!pic32_map[dev].deviceid) {
		if (pic32_conf.deviceid == 0x00000000 || pic32_conf.deviceid == 0xFFFFFFFF) {
			printf("%s: information: %s\n",
				__func__, io_fault(pic32_conf.deviceid));
		} else {
			printf("%s: information: device unknown: [%08X]\n",
				__func__, pic32_conf.deviceid);
		}
		pic32_standby();
		return -1;
	}

	/* Device recognised */
	pic32_index = dev;

	/* Device status */
	pic32_conf.status = pic32_check_device_status();
	if (PIC32_MCHP_STATUS_CFGRDY != (pic32_conf.status & (PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FCBUSY))) {
		printf("%s: information: status invalid [0x%02X]\n", __func__, pic32_conf.status);
		pic32_standby();
		return -1;
	}

	pic32_enter_serial_execution_mode();

	/* Device id/rev */
	switch (pic32_map[pic32_index].datasheet) {
	default:pic32_conf.devidaddr = PIC32_DEVID;
		break;
	case DS60001324B: /* PIC32MM GPL */
		pic32_conf.devidaddr = PIC32MM_GPL_DEVID;
		break;
	case DS60001387D: /* PIC32MM GPM */
		pic32_conf.devidaddr = PIC32MM_GPM_DEVID;
		break;
	}

	/* Device id/rev validation */
	uint32_t deviceid = pic32_readfromaddress(pic32_conf.devidaddr);
	if (deviceid != pic32_conf.deviceid) {
		printf("%s: information: device id invalid [0x%08X] != [0x%08X]\n",
			__func__, deviceid, pic32_conf.deviceid);
		pic32_standby();
		return -1;
	}

	/* Device family */
	dev = 0;
	while (pic32_tab[dev].datasheet) {
		if (pic32_tab[dev].datasheet == pic32_map[pic32_index].datasheet) {
			pic_pe_lookup(pic32_conf.pepath, pic32_tab[dev].filename);
			break;
		}
		++dev;
	}

	/* Config */
	switch (pic32_map[pic32_index].datasheet) {
	default:pic32_conf.configaddr = PIC32_BOOT + (pic32_map[pic32_index].boot << 2) - (4 << 2);
		pic32_conf.configsize = PIC32_CONFIG_SIZE;
		for (uint32_t i = 0; i < pic32_conf.configsize; ++i)
			pic32_conf.config[pic32_conf.configsize - 1 - i] = pic32_readfromaddress(pic32_conf.configaddr + 4 * i);
		break;
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		pic32_conf.configaddr = PIC32MM_PRI_CONFIG;
		pic32_conf.configsize = PIC32MM_CONFIG_SIZE;
		for (uint32_t i = 0; i < pic32_conf.configsize; ++i)
			pic32_conf.config[i] = pic32_readfromaddress(pic32_conf.configaddr + 4 * i);
		break;
	}

	pic32_standby();

	return 0;
}

/*
 * GET PROGRAM COUNT
 *
 *  RETURN NUMBER OF PARTITIONS
 */
uint32_t
pic32_get_program_count(void)
{
	return 1;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic32_get_program_size(uint32_t *addr, uint32_t partition)
{
	*addr = pic32_phy(PIC32_CODE);

	return pic32_map[pic32_index].prog;
}

/*
 * GET BOOT FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic32_get_boot_size(uint32_t *addr)
{
	*addr = pic32_phy(PIC32_BOOT);

	return pic32_map[pic32_index].boot;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic32_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	pic32_program_verify();
	pic32_enter_serial_execution_mode();
	pic32_pe_load();

	if (pic32_conf.pepath[0]) {
		/* PE */
		for (uint32_t i = 0; i < size; ++i) {
			data[i] = pic32_pe_readword(addr);
			addr += 4;
		}
	} else {
		/* !PE */
		for (uint32_t i = 0; i < size; ++i) {
			data[i] = pic32_readfromaddress(addr);
			addr += 4;
		}
	}

	pic32_standby();

	return addr;
}

/*****************************************************************************
 *
 * Program Code
 *
 *****************************************************************************/

/*
 * WRITE PANEL TO 32-BIT PROGRAM FLASH
 */
void
pic32_write_panel(uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		if (pic32_conf.pepath[0]) {
			/* PE */
			pic32_pe_row_program(address, panel, panel_size);
		} else {
			/* !PE */
			pic32_download_data_block(panel, panel_size);
			pic32_flash_row_write(address);
		}
		break;
	default:printf("%s: warning: region unsupported [%d]\n", __func__, region);
		break;
	}
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE OR BOOT
 *
 *  RETURN PIC_REGIONCODE:
 *	0x1D000000 .. 0x1D1FFFFF (2048KB)
 *
 *  RETURN PIC_REGIONBOOT:
 *  	0x1FC00000 .. 0x1FC1XXXX
 *
 * DS60001145N-page 10
 */
uint32_t
pic32_getregion(uint32_t address)
{
	uint32_t vaddr = pic32_kseg1(address);

	if (vaddr >= PIC32_CODE && vaddr < (PIC32_CODE + (pic32_map[pic32_index].prog << 2))) {
		return PIC_REGIONCODE;
	}
	if (vaddr >= PIC32_BOOT && vaddr < (PIC32_BOOT + (pic32_map[pic32_index].boot << 2))) {
		return PIC_REGIONBOOT;
	}
	if (p.f) fprintf(p.f, "%s: warning: address unsupported [%08X]\n",
		__func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
static inline uint32_t
pic32_init_writeregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		pic_write_panel(PIC_PANEL_BEGIN, region, pic32_map[pic32_index].row << 2);
		return region;
	}
	if (p.f) fprintf(p.f, "%s: warning: region unsupported [%d]\n",
		__func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * WRITE REGION
 */
static inline void
pic32_writeregion(uint32_t address, uint32_t region, uint32_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		pic_write_panel(PIC_PANEL_UPDATE, address, data);
		return;
	}
	if (p.f) fprintf(p.f, "%s: warning: region unsupported [%d]\n",
		__func__, region);
}

/*
 * INIT REGION FOR VERIFY
 *
 *  RETURN REGION IF VERIFY SUPPORTED
 */
static inline uint32_t
pic32_init_verifyregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		/* Nothing to do on PIC32 */
		return region;
	}
	if (p.f) fprintf(p.f, "%s: warning: region unsupported [%d]\n",
		__func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * GET VERIFY DATA FOR REGION
 */
static inline uint32_t
pic32_verifyregion(uint32_t address, uint32_t region, uint32_t wdata)
{
	uint32_t vdata = 0;

	if (region == PIC_REGIONNOTSUP) {
		if (p.f) fprintf(p.f, "%s: warning: region unsupported [%d]\n",
			__func__, region);
		return wdata;
	}
	if (pic32_conf.pepath[0]) {
		/* PE */
		vdata = pic32_pe_readword(address);
	} else {
		/* !PE */
		vdata = pic32_readfromaddress(address);
	}
	return vdata;
}

/*****************************************************************************
 *
 * Program & verify
 *
 *****************************************************************************/

/*
 * BEGIN PROGRAMMING
 */
void
pic32_program_begin(void)
{
	pic32_program_verify();
	pic32_enter_serial_execution_mode();
	pic32_pe_load();
}

/*
 * PROGRAM DATA
 */
uint32_t
pic32_program_data(uint32_t current_region, pic_data *pdata)
{
	uint32_t address, new_region;

	for (uint32_t i = 0; i < pdata->nbytes; ++i) {
		address = pdata->address + i;
		new_region = pic32_getregion(address);
		if (new_region != current_region) {
			pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic32_init_writeregion(new_region);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		pic32_writeregion(address, current_region, pdata->bytes[i]);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic32_program_end(__attribute__((unused)) int config)
{
	pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
	pic32_standby();
}

/*
 * VERIFY DATA
 */
uint32_t
pic32_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	uint32_t address, new_region, wdata, vdata;

	for (uint32_t i = 0; i < pdata->nbytes; i += 4) {
		address = pdata->address + i;
		new_region = pic32_getregion(address);
		if (new_region != current_region)
			current_region = pic32_init_verifyregion(new_region);
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		wdata = (uint32_t)pdata->bytes[i] |
			((uint32_t)pdata->bytes[i + 1] << 8) |
			((uint32_t)pdata->bytes[i + 2] << 16) |
			((uint32_t)pdata->bytes[i + 3] << 24);
		vdata = pic32_verifyregion(address, current_region, wdata);
		if (vdata != wdata) {
			if (p.f) printf("%s: error: read [%08X] expected [%08X] at [%08X]\n",
				__func__, vdata, wdata, pic32_phy(address));
			pdata->bytes[i] = vdata;
			pdata->bytes[i + 1] = vdata >> 8;
			pdata->bytes[i + 2] = vdata >> 16;
			pdata->bytes[i + 3] = vdata >> 24;
			(*fail) += 4;
		}
	}
	return current_region;
}

/*
 * VIEW DATA
 */
void
pic32_view_data(pic_data *pdata)
{
	uint32_t wdata;

	printf("[%08X] ", pdata->address);
	for (uint32_t i = 0; i < pdata->nbytes; i += 4) {
		wdata = (uint32_t)pdata->bytes[i] |
			((uint32_t)pdata->bytes[i + 1] << 8) |
			((uint32_t)pdata->bytes[i + 2] << 16) |
			((uint32_t)pdata->bytes[i + 3] << 24);
		printf("%08X ", wdata);
	}
	putchar('\n');
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
pic32_dumpdeviceid(void)
{
	printf("[%08X] [PROGRAM]  %08X WORDS\n", pic32_phy(PIC32_CODE), pic32_map[pic32_index].prog);
	printf("[%08X] [DEVICEID] %08X VER:%1X DEV:%04X MAN:%03X %s\n",
		pic32_phy(pic32_conf.devidaddr),
		pic32_conf.deviceid,
		(pic32_conf.deviceid & 0xF0000000) >> 28,
		(pic32_conf.deviceid & 0x0FFFF000) >> 12,
		(pic32_conf.deviceid & 0x00000FFF),
		pic32_map[pic32_index].devicename);
	printf("[%08X] [BOOT]     %08X WORDS\n", pic32_phy(PIC32_BOOT), pic32_map[pic32_index].boot);
	pic32_dumpconfig(PIC_BRIEF, 0);
	if (pic32_conf.pepath[0]) {
		char *pedup = (char *)strdup(pic32_conf.pepath);
		if (pedup == NULL) {
			printf("%s: fatal error: strdup failed\n", __func__);
			io_exit(EX_OSERR); /* Panic */
		}
		printf("           [PE]       %s\n", basename(pedup));
		free(pedup);
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic32_dumpconfig(uint32_t mode, uint32_t partition)
{
	switch (pic32_map[pic32_index].datasheet) {
	default:for (uint32_t i = 0; i < pic32_conf.configsize; ++i) {
			printf("[%08X] [DEVCFG%d]  %08X\n", pic32_phy(pic32_conf.configaddr + 4 * i),
				pic32_conf.configsize - 1 - i, pic32_conf.config[pic32_conf.configsize - 1 - i]);
		}
		break;
	case DS60001324B: /* PIC32MM GPL */
	case DS60001387D: /* PIC32MM GPM */
		printf("[%08X] [RESERVED] %08X\n", pic32_phy(pic32_conf.configaddr),      pic32_conf.config[0]);
		printf("[%08X] [FDEVOPT]  %08X\n", pic32_phy(pic32_conf.configaddr) + 4,  pic32_conf.config[1]);
		printf("[%08X] [FICD]     %08X\n", pic32_phy(pic32_conf.configaddr) + 8,  pic32_conf.config[2]);
		printf("[%08X] [FPOR]     %08X\n", pic32_phy(pic32_conf.configaddr) + 12, pic32_conf.config[3]);
		printf("[%08X] [FWDT]     %08X\n", pic32_phy(pic32_conf.configaddr) + 16, pic32_conf.config[4]);
		printf("[%08X] [FOCSEL]   %08X\n", pic32_phy(pic32_conf.configaddr) + 20, pic32_conf.config[5]);
		printf("[%08X] [FSEC]     %08X\n", pic32_phy(pic32_conf.configaddr) + 24, pic32_conf.config[6]);
		printf("[%08X] [RESERVED] %08X\n", pic32_phy(pic32_conf.configaddr) + 28, pic32_conf.config[7]);
		printf("[%08X] [FSIGN]    %08X\n", pic32_phy(pic32_conf.configaddr) + 32, pic32_conf.config[8]);
		printf("[%08X] [RESERVED] %08X\n", pic32_phy(pic32_conf.configaddr) + 36, pic32_conf.config[9]);
	}
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic32_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 4) {
#ifndef DEBUG
		if (pic_mtcode(PIC32_MASK, 4, &data[i]))
			continue;
#endif
		printf("[%08X] ", pic32_phy(address));
		for (j = 0; j < 4; ++j) {
			printf("%08X ", data[i + j]);
		}
		for (j = 0; j < 4; ++j) {
			putchar(PIC_CHAR(0xFF & data[i + j]));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 8)));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 16)));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 24)));
		}
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: flash empty\n", __func__);
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic32_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* PIC32: Physical addressing */
	address = pic32_phy(address);

	/* PIC32: Extended address */
	pic_dumpaddr(address, 1);

	for (i = 0; i < size; address += 16, i += 4) {
		if (pic_mtcode(PIC32_MASK, 4, &data[i]))
			continue;

		/* PIC32: Extended address */
		pic_dumpaddr(address, 0);

		uint8_t cc, b0, b1, b2, b3;
		b0 = address >> 8;
		b1 = address;
		printf(":%02X%02X%02X00", 16, b0, b1);

		cc = 16 + b0 + b1 + 0x00;
		for (j = 0; j < 4; ++j) {
			b0 = data[i + j];
			b1 = data[i + j] >> 8;
			b2 = data[i + j] >> 16;
			b3 = data[i + j] >> 24;
			printf("%02X%02X%02X%02X", b0, b1, b2, b3);
			cc = cc + b0 + b1 + b2 + b3;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}
