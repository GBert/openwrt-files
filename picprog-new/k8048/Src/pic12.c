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

#include "k8048.h"

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/
struct pic12_config pic12_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/
struct pic12_dsmap pic12_map[] =
{
/*Device name	Device id 	Flash	Data	Data-sheet Backup OSC	Config 	Latches */
{"PIC12F508",	PIC12F508,	512,	0,	DS41227E,  0x0204,	0x03FF,	1},
{"PIC12F509",	PIC12F509,	1024,	0,	DS41227E,  0x0404,	0x07FF,	1},
{"PIC16F505",	PIC16F505,	1024,	0,	DS41226G,  0x0404,	0x07FF,	1},

{"PIC12F510",	PIC12F510,	1024,	0,	DS41257B,  0x0404,	0x07FF,	1},

{"PIC16F506",	PIC16F506,	1024,	0,	DS41258C,  0x0404,	0x07FF,	1},

{"PIC10F200",	PIC10F200,	256,	0,	DS41228F,  0x0104,	0x01FF,	1},
{"PIC10F202",	PIC10F202,	512,	0,	DS41228F,  0x0204,	0x03FF,	1},
{"PIC10F204",	PIC10F204,	256,	0,	DS41228F,  0x0104,	0x01FF,	1},
{"PIC10F206",	PIC10F206,	512,	0,	DS41228F,  0x0204,	0x03FF,	1},

{"PIC16F54",	PIC16F54,	512,	0,	DS41207D,  0,		0x03FF,	1},

{"PIC16F57",	PIC16F57,	2048,	0,	DS41208C,  0,		0x0FFF,	4},

{"PIC16F59",	PIC16F59,	2048,	0,	DS41243B,  0,		0x0FFF,	4},

{"PIC10F220",	PIC10F220,	256,	0,	DS41266C,  0x0104,	0x01FF,	1},
{"PIC10F222",	PIC10F222,	512,	0,	DS41266C,  0x0204,	0x03FF,	1},

{"PIC12F519",	PIC12F519,	1024,	+64,	DS41316C,  0x0444,	0x07FF,	1},

{"(null)",	0,		0,	0,	0,	   0,		0,	0}
/*Device name	Device id 	Flash	Data	Data-sheet Backup OSC	Config 	Latches */
};
#define PIC12_SIZE ((sizeof(pic12_map) / sizeof(struct pic12_dsmap)) - 1)

/* Default device (null) */
int pic12_index = PIC12_SIZE;

void
pic12_selector(struct k8048 *k)
{
	int i;
	char *dnames[PIC12_SIZE];

	for (i = 0; i < PIC12_SIZE; ++i) {
		dnames[i] = pic12_map[i].devicename;
	}
	qsort(dnames, PIC12_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC12_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %d\n", (int)PIC12_SIZE);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*I/O*************************************************************************/

/*
 * LOAD DATA FOR PROGRAM MEMORY
 *  PC <= 0x07FF (PIC16F505)
 *  WR <= word
 *
 * XX0010 = 0x02
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 */
static inline void
pic12_load_data_for_program_memory(struct k8048 *k, unsigned short word)
{
	io_command_out(k, 0x02);
	io_word_out14(k, word);
}

/*
 * READ DATA FROM PROGRAM MEMORY
 *  RETURN (PC)
 *
 * XX0100 = 0x04
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 */
static inline unsigned short
pic12_read_data_from_program_memory(struct k8048 *k)
{
	io_command_out(k, 0x04);
	return io_word_in14(k) & MASK12BIT;
}

/*
 * INCREMENT ADDRESS
 *  PC <= 1 + PC
 *
 * XX0110 = 0x06
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 */
static inline void
pic12_increment_address(struct k8048 *k)
{
	io_command_out(k, 0x06);
}

/*PROG************************************************************************/

/*
 * BEGIN PROGRAMMING
 *  (PC) <= WR
 *
 * XX1000 = 0x08
 *
 * DS41226G-page 4 16F505 TPROG(2ms)
 * DS41227E-page 3 12F508 TPROG(2ms)
 * DS41258C-page 4 16F506 TPROG(2ms)
 * DS41228F-page 6 10F200 TPROG(2ms)
 * DS41207D-page 3 16F54  TPROG(2ms)
 * DS41208C-page 3 16F57  TPROG(2ms)
 * DS41266C-page 3 10F220 TPROG(2ms)
 * DS41316C-page 3 12F519 TPROG(2ms)
 */
static inline void
pic12_begin_programming(struct k8048 *k)
{
	io_command_out(k, 0x08);
	io_usleep(k, PIC12_TPROG_DEFAULT);
}

/*
 * END PROGRAMMING
 *
 * XX1110 = 0x0E
 *
 * DS41226G-page 4 16F505 TDIS(100us)
 * DS41227E-page 3 12F508 TDIS(100us)
 * DS41258C-page 4 16F506 TDIS(100us)
 * DS41228F-page 6 10F200 TDIS(100us)
 * DS41207D-page 3 16F54  TDIS(100us)
 * DS41208C-page 3 16F57  TDIS(100us)
 * DS41266C-page 3 10F220 TDIS(100us)
 * DS41316C-page 3 12F519 TDIS(100us)
 */
static inline void
pic12_end_programming(struct k8048 *k)
{
	io_command_out(k, 0x0E);
	io_usleep(k, PIC12_TDISCHARGE_DEFAULT);
}

/*ERASE***********************************************************************/

/*
 * BULK ERASE PROGRAM MEMORY
 *
 * XX1001 = 0x09
 *
 * DS41226G-page 4 16F505 TERA(10ms)
 * DS41227E-page 3 12F508 TERA(10ms)
 * DS41258C-page 4 16F506 TERA(10ms)
 * DS41228F-page 6 10F200 TERA(10ms)
 * DS41207D-page 3 16F54  TERA(10ms)
 * DS41208C-page 3 16F57  TERA(10ms)
 * DS41266C-page 3 10F220 TERA(10ms)
 * DS41316C-page 3 12F519 TERA(10ms)
 */
static inline void
pic12_bulk_erase_program_memory(struct k8048 *k)
{
	io_command_out(k, 0x09);
	io_usleep(k, PIC12_TERASE_DEFAULT);
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
pic12_read_program_memory_increment(struct k8048 *k)
{
	unsigned short data = pic12_read_data_from_program_memory(k);

	pic12_increment_address(k);
	
	return data;
}

/*
 * BLANK DEVICE
 *
 * SAVE OSCCAL (DEPENDANT UPON DEVICE AND OPTION)
 * DISABLE PROTECTION AND BULK ERASE
 * RESTORE OSCCAL (DEPENDANT UPON DEVICE AND OPTION)
 */
void
pic12_bulk_erase(struct k8048 *k, unsigned short osccal)
{
	int i;

	if (pic12_map[pic12_index].backupaddr == 0) {
		if (osccal != INTERNAL && osccal != NOINTERNAL) {
			printf("%s: error: OSCCAL is not supported on this device\n",
				__func__);
			return;
		}
	} else {
		if (osccal == INTERNAL) {
			osccal = pic12_read_osccal(k);
		}
	}

	io_init_program_verify(k);

	if (pic12_map[pic12_index].datasheet == DS41316C) { /* PIC12F519 */
		pic12_bulk_erase_program_memory(k); /* Erase config/program flash */

		pic12_increment_address(k); /* Skip config word */

		for (i = 0; i < pic12_map[pic12_index].flash; i++)
			pic12_increment_address(k); /* Skip program flash */

		pic12_bulk_erase_program_memory(k); /* Erase data flash */

		for (i = 0; i < pic12_map[pic12_index].data; i++)
			pic12_read_program_memory_increment(k); /* Skip data flash */

		pic12_bulk_erase_program_memory(k); /* Erase id/osccal */
	} else {
		pic12_increment_address(k); /* Skip config word */

		for (i = 0; i < pic12_map[pic12_index].flash; i++)
			pic12_increment_address(k); /* Skip program flash */

		for (i = 0; i < pic12_map[pic12_index].data; i++)
			pic12_read_program_memory_increment(k); /* Skip data flash */

		pic12_bulk_erase_program_memory(k); /* Erase device */
	}

	io_standby(k);

	if (pic12_map[pic12_index].backupaddr != 0) {
		if (osccal != NOINTERNAL) {
			pic12_write_osccal(k, osccal);
		}
	}
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ AND TEST CONFIGURATION WORD
 */
void
pic12_read_config_word(struct k8048 *k)
{
	unsigned short fault;
	int i;

	if (!k->devicename[0]) {
		printf("%s: fatal error: device must be selected on this architecture.\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}

	for (i = 0; pic12_map[i].deviceid; i++) {
		if (strcasecmp(pic12_map[i].devicename, k->devicename) == 0) {
			break;
		}
	}
	if (pic12_map[i].deviceid == 0) {
		printf("%s: fatal error: unknown device: [%s]\n", __func__, k->devicename);
		exit(EX_SOFTWARE); /* Panic */
	}

	pic12_conf.index[PIC12_CONFIG_WORD] = pic12_read_program_memory_increment(k);

	/*
	 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
	 * VELLEMAN K8048 NO POWER          [3FFF]
	 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
	 */
	fault = pic12_conf.index[PIC12_CONFIG_WORD];
	if (fault == 0x0000 || fault == 0x3FFF) {
		printf("%s: fatal error: %s.\n",
			__func__, io_fault(k, fault));
		exit(EX_SOFTWARE); /* Panic */
	}

	pic12_index = i; /* Device recognised */
}

/*
 * READ CONFIGURATION MEMORY
 *
 *  READ CONFIG WORD AND USERID
 */
void
pic12_read_config_memory(struct k8048 *k, int f)
{
	int i;

	io_init_program_verify(k);

	pic12_read_config_word(k);				/* Get config word */
	if (f == CONFIG_ALL) {
		for (i = 0; i < (pic12_map[pic12_index].flash - 1); i++) {
			pic12_increment_address(k);		/* Skip program flash */
		}
		/* Get OSCCAL RESET */
		pic12_conf.index[PIC12_CONFIG_OSCCAL_RESET] = pic12_read_program_memory_increment(k);
		for (i = 0; i < pic12_map[pic12_index].data; i++)
			pic12_read_program_memory_increment(k); /* Skip data flash */
		/* Get USERID 0..3 */
		pic12_conf.index[PIC12_CONFIG_USERID0] = pic12_read_program_memory_increment(k);
		pic12_conf.index[PIC12_CONFIG_USERID1] = pic12_read_program_memory_increment(k);
		pic12_conf.index[PIC12_CONFIG_USERID2] = pic12_read_program_memory_increment(k);
		pic12_conf.index[PIC12_CONFIG_USERID3] = pic12_read_program_memory_increment(k);
		/* Get OSCCAL BACKUP */
		pic12_conf.index[PIC12_CONFIG_OSCCAL_BACKUP] = pic12_read_program_memory_increment(k);
	}

	io_standby(k);
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
int
pic12_get_program_flash_size(void)
{
	return pic12_map[pic12_index].flash;    
}

/*
 * GET DATA FLASH SIZE
 *
 *  RETURN SIZE IN WORDS (BYTES)
 */
int
pic12_get_data_flash_size(void)
{
	return pic12_map[pic12_index].data;
}

/*
 * GET DATA EEPROM SIZE
 *
 *  RETURN SIZE IN BYTES
 */
int
pic12_get_data_eeprom_size(void)
{
	return 0; /* Never supported? */
}

/*
 * READ PROGRAM FLASH OR DATA FLASH MEMORY BLOCK 0000 .. 0000 + SIZE
 *
 *  RETURN BASE ADDRESS
 */
unsigned short
pic12_read_flash_memory_block(struct k8048 *k, unsigned short *block, int size)
{
	int i;

	io_init_program_verify(k);

	pic12_read_config_word(k);
	for (i = 0; i < size; i++)
		block[i] = pic12_read_program_memory_increment(k);

	io_standby(k);

	return PIC12_CODE_LOW;
}

/*
 * READ DATA EEPROM MEMORY BLOCK
 *
 *  RETURN BASE ADDRESS
 */
unsigned short
pic12_read_eeprom_memory_block(struct k8048 *k, unsigned short *block, int size)
{
	return 0x2100; /* Never supported? */
}

/*****************************************************************************
 *
 * Read/Write Oscillator Calibration 
 *
 *****************************************************************************/

/*
 * READ OSCILLATOR CALIBRATION WORD
 *
 *  RETURN DATA WORD
 */
unsigned short
pic12_read_osccal(struct k8048 *k)
{
	if (pic12_map[pic12_index].backupaddr == 0) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
		return 0;
	}

	io_init_program_verify(k);

	pic12_read_config_memory(k, CONFIG_ALL);

	io_standby(k);

	return pic12_conf.index[PIC12_CONFIG_OSCCAL_RESET];
}

/*
 * WRITE OSCILLATOR CALIBRATION WORD
 */
int
pic12_write_osccal(struct k8048 *k, unsigned short osccal)
{
	int i;
	unsigned short vdata;
	
	if (pic12_map[pic12_index].backupaddr == 0) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
		return FAIL;
	}

	io_init_program_verify(k);
	
	pic12_increment_address(k);			/* Skip config word */
	for (i = 0; i < (pic12_map[pic12_index].flash - 1); i++)
		pic12_increment_address(k);		/* Skip program flash memory */

	/*
	 * Write OSCCAL RESET
	 */
	pic12_load_data_for_program_memory(k, osccal);
	pic12_begin_programming(k);
	pic12_end_programming(k);
	vdata = pic12_read_data_from_program_memory(k);
	if (vdata != osccal) {
		io_standby(k);
		printf("%s: error: OSCCAL RESET write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, osccal);
		return FAIL;
	}
	pic12_increment_address(k);			/* Skip OSCCAL RESET */

	for (i = 0; i < pic12_map[pic12_index].data; i++)
		pic12_read_program_memory_increment(k);	/* Skip data flash memory */

	pic12_increment_address(k);			/* Skip USERID0 */
	pic12_increment_address(k);			/* Skip USERID1 */
	pic12_increment_address(k);			/* Skip USERID2 */
	pic12_increment_address(k);			/* Skip USERID3 */

	/*
	 * Write OSCCAL BACKUP
	 */
	pic12_load_data_for_program_memory(k, osccal);
	pic12_begin_programming(k);
	pic12_end_programming(k);
	vdata = pic12_read_data_from_program_memory(k);
	if (vdata != osccal) {
		io_standby(k);
		printf("%s: error: OSCCAL BACKUP write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, osccal);
		return FAIL;
	}
#if 0
	pic12_increment_address(k);			/* Skip OSCCAL BACKUP */
#endif

	io_standby(k);

	return PASS;
}

/*****************************************************************************
 *
 * Program Config
 *
 *****************************************************************************/

/*
 * WRITE CONFIG
 */
int
pic12_write_config(struct k8048 *k, unsigned short config)
{
	io_init_program_verify(k);

	/* Store config in working register */
	pic12_load_data_for_program_memory(k, config);

	/* Program working register */
	pic12_begin_programming(k);
	pic12_end_programming(k);

	io_standby(k);

	return 1;
}

/*****************************************************************************
 *
 * Region control
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE or CONFIG
 */
int
pic12_getregion(unsigned int address)
{
	unsigned short user3 = 3 +
		pic12_map[pic12_index].flash + pic12_map[pic12_index].data;

	if (address >= PIC12_CODE_LOW && address <= user3) {
		return PIC12_REGIONCODE;
	}
	if (address == PIC12_CONFIG) {
		return PIC12_REGIONCONFIG;
	}
	printf("%s: warning: region unknown [addr=%04X]\n", __func__, address);
	return PIC12_REGIONUNKNOWN;
}

/*
 * INITIALISE FOR REGION
 */
unsigned int
pic12_initregion(struct k8048 *k, int region)
{
	switch (region) {
	case PIC12_REGIONCODE:
		/* Skip config */
		pic12_increment_address(k);
		return PIC12_CODE_LOW;
		break;
	case PIC12_REGIONCONFIG:
		return PIC12_CONFIG;
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
pic12_loadregion(struct k8048 *k, int region, unsigned short word)
{
	switch (region) {
	case PIC12_REGIONCODE:
	case PIC12_REGIONCONFIG:
		pic12_load_data_for_program_memory(k, word);
		break;
	default:printf("%s: warning: region unknown\n", __func__);
		break;
	}
}

/*****************************************************************************
 *
 * Program & verify region (one word)
 *
 *****************************************************************************/

/*
 * PROGRAM DATA FOR REGION (CACHE CONFIG WORD FOR LATER)
 */
void
pic12_programregion(struct k8048 *k, unsigned short address, int region, unsigned short data)
{
	/*
	 * Cache config word
	*/
	if (region == PIC12_REGIONCONFIG) {
		pic12_conf.index[PIC12_CONFIG_WORD] = data;
		return;
	}

	/*
	 * Ignore OSCCAL
	 *
	 * OSCCAL address is on a multi-word boundary but the devices are not multi-word
	 */
	if (pic12_map[pic12_index].backupaddr != 0) {
		if (address == (pic12_map[pic12_index].flash - 1)) {
			printf("%s: warning: OSCCAL RESET word ignored: word [%04X] address [%04X]\n",
				__func__, data, address);
			return;
		}
		if (address == pic12_map[pic12_index].backupaddr) {
			printf("%s: warning: OSCCAL BACKUP word ignored: word [%04X] address [%04X]\n",
				__func__, data, address);
			return;
		}
	}

	/*
	 * Write multi-word code
	 */
	static int write_pending = 0;
	int multiword = (pic12_map[pic12_index].latches > 1);
	if (multiword && region == PIC12_REGIONCODE) {
		if (data != MW_NODATA) {
			pic12_loadregion(k, region, data);
			write_pending = 1;
		}
		unsigned int mask = pic12_map[pic12_index].latches - 1;
		if ((address & mask) == mask) {
			if (write_pending) {
				pic12_begin_programming(k);
				pic12_end_programming(k);
				write_pending = 0;
			}
		}
		return;
	}

	/*
	 * Write single word code
	 */

	/* Store data in working register */
	pic12_loadregion(k, region, data);

	/* Program working register */
	pic12_begin_programming(k);
	pic12_end_programming(k);
}

/*
 * VERIFY DATA FOR REGION
 */
int
pic12_verifyregion(struct k8048 *k, unsigned short address, int region, unsigned short data)
{
	unsigned short vdata;

	switch (region) {
	case PIC12_REGIONCODE:
	case PIC12_REGIONCONFIG:
		vdata = pic12_read_data_from_program_memory(k);
		break;
	default:printf("%s: warning: region unknown\n", __func__);
		return FAIL;
	}
	if (vdata != data) {
		printf("%s: error: read [%04x] expected [%04x] at [%04x]\n",
			__func__, vdata, data, address);
		return FAIL;
	}
	return PASS;
}

/*****************************************************************************
 *
 * Region programming
 *
 *****************************************************************************/
	
/*
 * PROGRAM DEVICE USING GLOBAL INHX32 DATA
 */
void
pic12_program(struct k8048 *k, int blank)
{
	int i, j;
	unsigned short hex_address, PC_address = 0, wdata;
	int new_region, current_region = PIC12_REGIONUNKNOWN;
	int total = 0;
	int multiword = (pic12_map[pic12_index].latches > 1);

	/* Initialise device for programming */
	if (blank)
		pic12_bulk_erase(k, INTERNAL);

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		hex_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic12_getregion(hex_address);
		if (new_region == PIC12_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			if (multiword && current_region == PIC12_REGIONCODE)
				pic12_programregion(k, MW_FLUSH, PIC12_REGIONCODE, MW_NODATA);
			io_init_program_verify(k); /* Reset P.C. */
			PC_address = pic12_initregion(k, new_region);
			current_region = new_region;
		}

		/* Skip over unused P.C. locations */
		while (hex_address > PC_address) {
			if (multiword && current_region == PIC12_REGIONCODE)
				pic12_programregion(k, PC_address, PIC12_REGIONCODE, MW_NODATA);
			PC_address++;
			pic12_increment_address(k);
		}

		/* For each word in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j += 2) {
			wdata = inhx32_pdata[i]->bytes[j] | (inhx32_pdata[i]->bytes[j + 1] << 8);
			wdata &= MASK12BIT;
			pic12_programregion(k, PC_address, current_region, wdata);
			PC_address++;
			pic12_increment_address(k);
			total++;
		}
	}
	if (multiword && current_region == PIC12_REGIONCODE)
		pic12_programregion(k, MW_FLUSH, PIC12_REGIONCODE, MW_NODATA);

	io_standby(k);
	
	/* Finalise device programming (write config word) */
	if (blank)
		pic12_write_config(k, pic12_conf.index[PIC12_CONFIG_WORD]);

	printf("Total: %d\n", total);
}

/*
 * VERIFY DEVICE USING GLOBAL INHX32 DATA
 */
int
pic12_verify(struct k8048 *k)
{
	int i, j;
	unsigned short hex_address, PC_address = 0, wdata;
	int new_region, current_region = PIC12_REGIONUNKNOWN;
	int fail = 0, total = 0;

	/* For each line */
	for (i = 0; i < inhx32_count; i++) {
		hex_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic12_getregion(hex_address);
		if (new_region == PIC12_REGIONUNKNOWN)
			continue;

		if (new_region != current_region) {
			io_init_program_verify(k); /* Reset P.C. */
			PC_address = pic12_initregion(k, new_region);
			current_region = new_region;
		}

		/* Skip over unused P.C. locations */
		while (hex_address > PC_address) {
			PC_address++;
			pic12_increment_address(k);
		}

		/* For each word in line */
		for (j = 0; j < inhx32_pdata[i]->nbytes; j += 2) {
			wdata = inhx32_pdata[i]->bytes[j] | (inhx32_pdata[i]->bytes[j + 1] << 8);
			wdata &= MASK12BIT;
			fail += pic12_verifyregion(k, PC_address++, current_region, wdata);
			pic12_increment_address(k);
			total++;
		}
	}
	io_standby(k);

	printf("Total: %d Pass: %d Fail: %d\n", total, total - fail, fail);
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
pic12_dumpdeviceid(struct k8048 *k)
{
	int i;
	unsigned char u;

	printf("[%04X] [PROGRAM]  %04X WORDS\n",
		PIC12_CODE_LOW, pic12_map[pic12_index].flash);
	if (pic12_map[pic12_index].backupaddr) {
	printf("[%04X] [OSCCAL]   %04X\n",
		pic12_map[pic12_index].flash - 1,
		pic12_conf.index[PIC12_CONFIG_OSCCAL_RESET]);
	}
	if (pic12_map[pic12_index].data > 0) {
	printf("[%04X] [DATA]     %04X BYTES\n",
		pic12_map[pic12_index].flash,
		pic12_map[pic12_index].data);
	}
	for (i = 0; i < 4; i++) {
	u = pic12_conf.index[i] & 0xff;
	printf("[%04X] [USERID%d]  %04X %c\n",
		pic12_map[pic12_index].flash + 
		pic12_map[pic12_index].data  + i,
		i,
		pic12_conf.index[i],
		(u >= 32 && u < 127) ? u : '.');
	}
	if (pic12_map[pic12_index].backupaddr) {
	printf("[%04X] [BACKUP]   %04X\n",
		pic12_map[pic12_index].backupaddr,
		pic12_conf.index[PIC12_CONFIG_OSCCAL_BACKUP]);
	}
	pic12_dumpconfig(k, BRIEF);
	printf("       [DEVICEID] %s\n", pic12_map[pic12_index].devicename);
}

/*
 * DUMP OSCCAL DETAILS
 */
void
pic12_dumposccal(struct k8048 *k)
{
	if (pic12_map[pic12_index].backupaddr) {
		printf("[%04X] [OSCCAL]   %04X\n",
			pic12_map[pic12_index].flash - 1,
			pic12_conf.index[PIC12_CONFIG_OSCCAL_RESET]);
		printf("[%04X] [BACKUP]   %04X\n",
			pic12_map[pic12_index].backupaddr,
			pic12_conf.index[PIC12_CONFIG_OSCCAL_BACKUP]);
	} else {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
	}
}

/*
 * DUMP CONFIG WORD DETAILS
 */
void
pic12_dumpconfig(struct k8048 *k, int verbose)
{
	printf("[%04X] [CONFIG]   %04X\n",
		PIC12_CONFIG, pic12_conf.index[PIC12_CONFIG_WORD]);
}

/*
 * DUMP DEVICE CONFIGURATION
 */
void
pic12_dumpdevice(struct k8048 *k)
{
	int i;
	unsigned short addr;

	/* IDLOC */
	addr = pic12_map[pic12_index].flash + pic12_map[pic12_index].data;
	for (i = 0; i < 4; ++i) {
		pic_dumpword(k, addr + i, pic12_conf.index[i + PIC12_CONFIG_USERID0]);
	}
	/* CONFIG */
	pic_dumpword(k, PIC12_CONFIG, pic12_conf.index[PIC12_CONFIG_WORD]);
}
