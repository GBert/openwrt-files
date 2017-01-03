/*
 * Copyright (C) 2005-2017 Darron Broad
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

struct pic_ops pic16n_ops = {
	.arch				= ARCH16BIT,
	.align				= sizeof(uint8_t),
	.selector			= pic16n_selector,
	.program_begin			= pic16n_program_verify,
	.program_data			= pic16n_program_data,
	.program_end			= pic16n_program_end,
	.verify_begin			= pic16n_program_verify,
	.verify_data			= pic16n_verify_data,
	.verify_end			= pic16n_standby,
	.view_data			= pic16n_view_data,
	.read_config_memory		= pic16n_read_config_memory,
	.get_program_size		= pic16n_get_program_size,
	.get_data_size			= pic16n_get_data_size,
	.get_executive_size		= NULL,
	.get_boot_size			= NULL,
	.read_program_memory_block	= pic16n_read_program_memory_block,
	.read_data_memory_block		= pic16n_read_data_memory_block,
	.write_panel			= pic16n_write_panel,
	.bulk_erase			= pic16n_bulk_erase,
	.write_osccal			= NULL,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= NULL,
	.dumpdeviceid			= pic16n_dumpdeviceid,
	.dumpconfig			= pic16n_dumpconfig,
	.dumposccal			= NULL,
	.dumpdevice			= pic16n_dumpdevice,
	.dumpadj			= 2,
	.dumphexcode			= pic16n_dumphexcode,
	.dumpinhxcode			= pic16n_dumpinhxcode,
	.dumphexdata			= pic16n_dumphexdata,
	.dumpinhxdata			= pic16n_dumpinhxdata,
};

uint32_t
pic16n_arch(void)
{
	p.pic = &pic16n_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic16n_config pic16n_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic16n_dsmap pic16n_map[] =
{
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches*/
{"PIC18F24K40", PIC18F24K40,    DS40001772B,	PIC16N_WORD(16), 6,      	256,	64},
{"PIC18F25K40", PIC18F25K40,    DS40001772B,	PIC16N_WORD(32), 6,      	256,	64},
{"PIC18F45K40", PIC18F45K40,    DS40001772B,	PIC16N_WORD(32), 6,      	256,	64},
{"PIC18F26K40", PIC18F26K40,    DS40001772B,	PIC16N_WORD(64), 6,      	1024,	64},
{"PIC18F46K40", PIC18F46K40,    DS40001772B,	PIC16N_WORD(64), 6,      	1024,	64},
{"PIC18F27K40", PIC18F27K40,    DS40001772B,	PIC16N_WORD(128),6,      	1024,	128},
{"PIC18F47K40", PIC18F47K40,    DS40001772B,	PIC16N_WORD(128),6,      	1024,	128},
{"PIC18LF24K40",PIC18LF24K40,   DS40001772B,	PIC16N_WORD(16), 6,      	256,	64},
{"PIC18LF25K40",PIC18LF25K40,   DS40001772B,	PIC16N_WORD(32), 6,      	256,	64},
{"PIC18LF45K40",PIC18LF45K40,   DS40001772B,	PIC16N_WORD(32), 6,      	256,	64},
{"PIC18LF26K40",PIC18LF26K40,   DS40001772B,	PIC16N_WORD(64), 6,      	1024,	64},
{"PIC18LF46K40",PIC18LF46K40,   DS40001772B,	PIC16N_WORD(64), 6,      	1024,	64},
{"PIC18LF27K40",PIC18LF27K40,   DS40001772B,	PIC16N_WORD(128),6,      	1024,	128},
{"PIC18LF47K40",PIC18LF47K40,   DS40001772B,	PIC16N_WORD(128),6,      	1024,	128},

{"(null)",      0,              0,              0,               0,             0,      0},
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches*/
};
#define PIC16N_SIZE ((sizeof(pic16n_map) / sizeof(struct pic16n_dsmap)) - 1)

/* Default device (null) */
uint32_t pic16n_index = PIC16N_SIZE;

void
pic16n_selector(void)
{
	uint32_t i;
	char *dnames[PIC16N_SIZE];

	for (i = 0; i < PIC16N_SIZE; ++i) {
		dnames[i] = pic16n_map[i].devicename;
	}
	qsort(dnames, PIC16N_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC16N_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC16N_SIZE);
}

/*****************************************************************************
 *
 * Program/Verify mode
 *
 *****************************************************************************/

/*
 * ENTER HVP/LVP PROGRAM/VERIFY MODE
 *
 * ENTRY - VDD FIRST
 */
void
pic16n_program_verify(void)
{
	printf("%s()\n", __func__);

	/* RESET & ACQUIRE GPIO */
	io_set_vpp(LOW);
	/* DS40001753B TENTS(100ns) */
	io_usleep(1000);

	/* PGD + PGC + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_pgm(LOW);
	io_usleep(1000);

	/* INPUT DATA ON CLOCK RISING EDGE, MSB FIRST */
	io_configure(FALSE, TRUE);

	/* LVP */
	if (p.key == LVPKEY) {

		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* TENTH(250ns) */
		io_usleep(250);

		/* VPP LOW */
		io_set_vpp(LOW);
		/* TENTH(250ns) */
		io_usleep(250);

		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(MCHPKEY, 32);
		/* UNSPECIFIED */
		io_usleep(250);
	}
	/* HVP */
	else {
		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* DS40001753B TENTS(100ns) */
		io_usleep(250);
	}
}

/*
 * EXIT HVP/LVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic16n_standby(void)
{
	printf("%s()\n", __func__);

	/* PGD + PGC + VPP + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_vpp(LOW);
	io_set_pgm(LOW);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*I/O*************************************************************************/

/*
 * LOAD DATA FOR NVM
 *  0x00 PC = PC
 *  0x02 PC = PC + 2
 *
 * DS40001772B-page 12
 */
static inline void
pic16n_load_data_for_nvm(uint32_t word, uint8_t j /* 0 || 1 */)
{
	io_program_out(j << 1, 8);
	/* TDLY 1us */
	io_program_out(word << 1, 24);
	/* TDLY 1us */
}

/*
 * READ DATA FROM NVM
 *  0xFC PC = PC
 *  0xFE PC = PC + 2
 *
 * DS40001772B-page 10
 */
static inline uint32_t
pic16n_read_data_from_nvm(uint8_t j /* 0 || 1 */)
{
	uint32_t word;

	io_program_out(0xFC | (j << 1), 8);
	/* TDLY 1us */
	word = io_program_in(24);
	/* TDLY 1us */

	word = (word >> 1) & PIC16N_WORD_MASK;

	return word;
}

/*
 * INCREMENT ADDRESS
 *  0xF8 PC = PC + 2
 *
 * DS40001772B-page 10
 */
static inline void
pic16n_increment_address(void)
{
	io_program_out(0xF8, 8);
	/* TDLY 1us */
}

/*
 * LOAD PC ADDRESS
 *  0x80 PC = address
 *
 * DS40001772B-page 10
 */
static inline void
pic16n_load_pc_address(uint32_t address)
{
	io_program_out(0x80, 8);
	/* TDLY 1us */
	io_program_out(address << 1, 24);
	/* TDLY 1us */
}

/*PROG************************************************************************/

/*
 * BEGIN INTERNALLY TIMED PROGRAMMING
 *  0xE0
 *
 * DS40001772B-page 12
 *
 * DS40001772B TPINT(2.8ms PROGRAM) TPINT(5.6ms CONFIG/EEPROM)
 */
static inline void
pic16n_begin_internally_timed_programming(uint32_t t)
{
	io_program_out(0xE0, 8);
	/* TPINT */
	io_usleep(t);
}

/*
 * BEGIN EXTERNALLY TIMED PROGRAMMING
 *  0xC0
 *
 * DS40001772B-page 12
 *
 * DS40001772B TPEXT(2.1ms PROGRAM) (NOT CONFIG)
 */
static inline void
pic16n_begin_externally_timed_programming(uint32_t t)
{
	io_program_out(0xC0, 8);
	/* TPEXT */
	io_usleep(t);
}

/*
 * END EXTERNALLY TIMED PROGRAMMING
 *  0x82
 *
 * DS40001772B-page 12
 *
 * DS40001772B TDIS(300us PROGRAM)
 */
static inline void
pic16n_end_externally_timed_programming(uint32_t t)
{
	io_program_out(0x82, 8);
	/* TDIS */
	io_usleep(t);
}

/*ERASE***********************************************************************/

/*
 * BULK ERASE MEMORY (PRELOAD PC)
 *  0x18
 *
 * PC = 0x000000 ERASE PROGRAM AND CONFIG
 * PC = 0x300000 ERASE PROGRAM AND CONFIG AND USERID
 * PC = 0x310000 ERASE EEPROM
 *
 * DS40001772B-page 12
 *
 * DS40001772B TERAB(25.2ms PIC18F26K40)
 */
static inline void
pic16n_bulk_erase_memory(uint32_t t)
{
	io_program_out(0x18, 8);
	io_usleep(t);
}

/*
 * ROW ERASE MEMORY (PRELOAD PC)
 *  0xF0
 *
 * DS40001772B-page 12
 *
 * DS40001772B TERAR(2.8ms)
 */
static inline void
pic16n_row_erase_memory(uint32_t t)
{
	io_program_out(0xF0, 8);
	io_usleep(t);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

void
pic16n_bulk_erase(void)
{
	pic16n_program_verify();

	pic16n_load_pc_address(PIC16N_USERID_ADDR);
	pic16n_bulk_erase_memory(PIC16N_TERAB);

	if (pic16n_map[pic16n_index].eeprom) {
		pic16n_load_pc_address(PIC16N_EEPROM_ADDR);
		pic16n_bulk_erase_memory(PIC16N_TERAB);
	}

	pic16n_standby();
}

#if 0
void
pic16n_row_erase(uint32_t row, uint32_t nrows)
{
	fprintf(stderr, "%s()\n", __func__);
}
#endif

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY AND STORE DEVICE MAP INDEX
 */
int
pic16n_read_config_memory(void)
{
	uint32_t dev;
	uint16_t word;

	/* NULL device */
	pic16n_index = PIC16N_SIZE;

	/* Reset configuraton */
	memset(&pic16n_conf, 0, sizeof(pic16n_conf));

	pic16n_program_verify();
	pic16n_load_pc_address(PIC16N_REVID_ADDR);

	pic16n_conf.revisionid = pic16n_read_data_from_nvm(1);	/* Revisionid */
	pic16n_conf.deviceid = pic16n_read_data_from_nvm(1);	/* Deviceid   */

	/* Device detect */
	for (dev = 0; pic16n_map[dev].deviceid; ++dev) {
		if (pic16n_map[dev].deviceid == pic16n_conf.deviceid) {
			break;
		}
	}
	if (pic16n_map[dev].deviceid == 0) {
		/*
		 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
		 * VELLEMAN K8048 NO POWER          [3FFF]
		 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
		 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
		 */
		if (pic16n_conf.deviceid == 0x0000 || pic16n_conf.deviceid == 0x3FFF) {
			printf("%s: information: %s.\n",
				__func__, io_fault(pic16n_conf.deviceid));
		} else {
			printf("%s: information: device unknown: [%04X]\n",
				__func__, pic16n_conf.deviceid);
		}
		pic16n_standby();
		return -1;
	}

	/* Device recognised */
	pic16n_index = dev;

	/* User ID */
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; i += 2) {
		word = pic16n_read_data_from_nvm(1);
		pic16n_conf.userid[i] = word;
		pic16n_conf.userid[i + 1] = word >> 8;
	}

	/* Config word(s) */
	for (uint32_t i = 0; i < PIC16N_CONFIG_MAX; i += 2) {
		word = pic16n_read_data_from_nvm(1);
		pic16n_conf.config[i] = word;
		pic16n_conf.config[i + 1] = word >> 8;
	}

	pic16n_standby();

	return 0;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic16n_get_program_size(uint32_t *addr)
{
	*addr = 0;

	return pic16n_map[pic16n_index].flash;
}

/*
 * GET DATA EEPROM/FLASH SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic16n_get_data_size(uint32_t *addr)
{
	*addr = PIC16N_EEPROM_ADDR;

	return pic16n_map[pic16n_index].eeprom;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16n_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t i;
	uint16_t word;

	pic16n_program_verify();
	pic16n_load_pc_address(addr);

	size &= PIC16N_EVEN_MASK;
	for (i = 0; i < size; i += 2) {
		word = pic16n_read_data_from_nvm(1);
		data[i] = word & 0xFF;
		data[i + 1] = word >> 8;
	}

	pic16n_standby();

	return addr;
}

/*
 * READ DATA EEPROM BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16n_read_data_memory_block(uint16_t *data, uint32_t addr, uint16_t size)
{
	uint32_t i;
	uint16_t word;

	pic16n_program_verify();
	pic16n_load_pc_address(addr);

	size &= PIC16N_EVEN_MASK;
	for (i = 0; i < size; i++) {
		word = pic16n_read_data_from_nvm(1);
		data[i] = word & 0xFF;
		data[i + 1] = word >> 8;
	}

	pic16n_standby();

	return addr;
}

/*****************************************************************************
 *
 * Program Code/Data Panel
 *
 *****************************************************************************/

/*
 * WRITE PANEL
 */
void
pic16n_write_panel(uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	uint16_t word;

	for (uint32_t i = 0; i < panel_size; i += 2) {
		word = (panel[i] & 0xFF) | (panel[i + 1] << 8);
		pic16n_load_data_for_nvm(word, 1);
	}

	pic16n_load_pc_address(address);
	pic16n_begin_internally_timed_programming(PIC16N_TPINT_CODE);
}

/*****************************************************************************
 *
 * Program UserID/Config
 *
 *****************************************************************************/

/*
 * WRITE USERID/CONFIG WORD
 *  
 *  RETURN WORD
 */
static inline
uint16_t
pic16n_write_config_word(uint16_t word)
{
	pic16n_load_data_for_nvm(word, 0);
	pic16n_begin_internally_timed_programming(PIC16N_TPINT_CONFIG);

	return pic16n_read_data_from_nvm(1);
}

/*
 * WRITE CONFIG
 *
 *  RETURN 0 ON ERROR ELSE PROGRAM COUNT
 */
uint32_t
pic16n_write_config(void)
{
#if 0
	uint16_t vdata;

	pic16n_program_verify();

	pic16n_load_pc_address(PIC16N_USERID_ADDR);
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; ++i) {
		vdata = pic16n_write_config_word(pic16n_conf.userid[i]);
		if (vdata != pic16n_conf.userid[i]) {
			printf("%s: error: USERID%d write failed: read [%04X] expected [%04X]\n",
				__func__, i, vdata, pic16n_conf.userid[i]);
			pic16n_standby();
			return 0;
		}
	}

	pic16n_load_pc_address(PIC16N_CONFIG_ADDR);
	for (uint32_t i = 0; i < PIC16N_CONFIG_MAX; ++i) {
		vdata = pic16n_write_config_word(pic16n_conf.config[i]);
		if (vdata != pic16n_conf.config[i]) {
			printf("%s: error: CONFIG%d write failed: read [%04X] expected [%04X]\n",
				__func__, i + 1, vdata, pic16n_conf.config[i]);
			pic16n_standby();
			return 0;
		}
	}

	pic16n_standby();
#endif
	return (PIC16N_USERID_MAX + PIC16N_CONFIG_MAX);
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE, ID, CONFIG or DATA
 *
 *  RETURN PIC_REGIONCODE:
 *      0 .. FLASH SIZE - 1
 *
 *  RETURN PIC_REGIONID:
 *      0x200000 .. 0x200007
 *
 *  RETURN PIC_REGIONCONFIG:
 *      0x300000 .. 0x30000B
 *
 *  RETURN PIC_REGIONDATA:
 *      0xF00000 .. 0xFXXXXX
 */
uint16_t
pic16n_getregion(uint32_t address)
{
	/* CODE */
	if (address < pic16n_map[pic16n_index].flash) {
		return PIC_REGIONCODE;
	}
	/* ID */
	if (address >= PIC16N_USERID_ADDR && address < (PIC16N_USERID_ADDR + PIC16N_USERID_MAX)) {
		return PIC_REGIONID;
	}
	/* CONFIG */
	uint16_t config_high = PIC16N_CONFIG_ADDR + pic16n_map[pic16n_index].config;
	if (address >= PIC16N_CONFIG_ADDR && address < config_high) {
		return PIC_REGIONCONFIG;
	}
	/* EEPROM */
	if (pic16n_map[pic16n_index].eeprom) {
		uint16_t data_high = PIC16N_EEPROM_ADDR + pic16n_map[pic16n_index].eeprom;
		if (address >= PIC16N_EEPROM_ADDR && address < data_high)
			return PIC_REGIONDATA;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: address unsupported [%04X]\n", __func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING (ID & CONFIG DELAYED)
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
static inline uint32_t
pic16n_init_writeregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic16n_load_pc_address(0); /* Reset latch address */
		pic_write_panel(PIC_PANEL_BEGIN, region, pic16n_map[pic16n_index].latches);
		/* Follow through */
	case PIC_REGIONID:
	case PIC_REGIONCONFIG:
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * WRITE REGION (CACHE ID & CONFIG)
 */
static inline void
pic16n_writeregion(uint32_t address, uint32_t region, uint16_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONDATA:
		pic_write_panel(PIC_PANEL_UPDATE, address, data);
		return;
	case PIC_REGIONID:
		pic16n_conf.userid[address - PIC16N_USERID_ADDR] = data;
		return;
	case PIC_REGIONCONFIG:
		pic16n_conf.config[address - PIC16N_CONFIG_ADDR] = data;
		return;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
}

/*
 * INIT REGION FOR VERIFY
 *
 *  RETURN REGION IF VERIFY SUPPORTED
 */
static inline uint32_t
pic16n_init_verifyregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
	case PIC_REGIONDATA:
	case PIC_REGIONCONFIG:
		/* Nothing to do on PIC16 new */
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * VERIFY REGION
 *
 *  RETURN DATA
 */
static inline uint32_t
pic16n_verifyregion(uint32_t address, uint32_t region, uint16_t index, uint16_t wdata)
{
	uint16_t vdata = 0;

	if (region == PIC_REGIONNOTSUP) {
		if (p.f)
			fprintf(p.f, "%s: warning: region unsupported [%d]\n",
				__func__, region);
		return wdata;
	}
	if (index == 0)
		pic16n_load_pc_address(address);
	vdata = pic16n_read_data_from_nvm(1);
	if (vdata != wdata && p.f) {
		printf("%s: error: read [%04X] expected [%04X] at [%06X]\n",
			__func__, vdata, wdata, address);
	}
	return vdata;
}

/*****************************************************************************
 *
 * Program & verify
 *
 *****************************************************************************/

/*
 * PROGRAM DATA
 */
uint32_t
pic16n_program_data(uint32_t current_region, pic_data *pdata)
{
	uint32_t address, new_region;

	for (uint32_t i = 0; i < pdata->nbytes; ++i) {
		address = pdata->address + i;
		new_region = pic16n_getregion(address);
		if (new_region != current_region) {
			pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic16n_init_writeregion(new_region);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		pic16n_writeregion(address, current_region, pdata->bytes[i]);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic16n_program_end(int config)
{
	pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
	pic16n_standby();
	if (config)
		pic16n_write_config();
}

/*
 * VERIFY DATA
 */
uint32_t
pic16n_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	return 0;
}

/*
 * VIEW DATA
 */
void
pic16n_view_data(pic_data *pdata)
{
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
pic16n_dumpdeviceid(void)
{
	printf("[000000] [PROGRAM]     %04X WORDS (%04X ROWS OF %04X WORDS)\n",
		pic16n_map[pic16n_index].flash,
		2 * pic16n_map[pic16n_index].flash / pic16n_map[pic16n_index].latches,
		pic16n_map[pic16n_index].latches / 2);
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; ++i) {
		printf("[%06X] [IDLOCATION%d] %02X %c\n",
			PIC16N_USERID_ADDR + i,
			i + 1,
			pic16n_conf.userid[i], PIC_CHAR(0xFF & pic16n_conf.userid[i]));
	}

	pic16n_dumpconfig(PIC_BRIEF);

	printf("[3FFFFC] [REVISION]    %04X MAJ:%02X MIN:%02X\n",
		pic16n_conf.revisionid,
		(pic16n_conf.revisionid >> PIC16N_MAJOR_SHIFT) & PIC16N_REV_MASK,
		pic16n_conf.revisionid & PIC16N_REV_MASK);
	printf("[3FFFFE] [DEVICEID]    %04X %s\n", pic16n_conf.deviceid,	
		pic16n_map[pic16n_index].devicename);
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic16n_dumpconfig(int mode)
{
	for (uint32_t i = 0; i < PIC16N_CONFIG_MAX; i += 2) {
		printf("[%06X] [CONFIG%d]     %04X\n",
			PIC16N_CONFIG_ADDR + i,
			i / 2 + 1,
			(pic16n_conf.config[i + 1] << 8) | pic16n_conf.config[i]);
	}
#ifdef VERBOSE
	if (mode == PIC_VERBOSE) switch (pic16n_map[pic16n_index].datasheet) {
	default:break;
#endif
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic16n_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic16n_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
}

/*
 * DUMP HEX DATA EEPROM BYTES
 */
void
pic16n_dumphexdata(uint32_t address, uint32_t size, uint16_t *data)
{
}

/*
 * DUMP INHX32 DATA EEPROM BYTES
 */
void
pic16n_dumpinhxdata(uint32_t address, uint32_t size, uint16_t *data)
{
}

/*
 * DUMP INHX32 DEVICE CONFIGURATION
 */
void
pic16n_dumpdevice(void)
{
}
