/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.24 (24-bit word architecture)
 */

#include "k8048.h"

static inline void pic24_goto200(struct k8048 *);

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic24_config pic24_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic24_dsmap pic24_map[] =
{
/*Device name       Device id        Flash  EEPROM Data-sheet Config/n     Panelsize*/
{"PIC24FJ16GA002",  PIC24FJ16GA002,  5632,  0,     DS39786D,  0x002BFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ16GA004",  PIC24FJ16GA004,  5632,  0,     DS39786D,  0x002BFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ32GA002",  PIC24FJ32GA002,  11264, 0,     DS39786D,  0x0057FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ32GA004",  PIC24FJ32GA004,  11264, 0,     DS39786D,  0x0057FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ48GA002",  PIC24FJ48GA002,  16896, 0,     DS39786D,  0x0083FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ48GA004",  PIC24FJ48GA004,  16896, 0,     DS39786D,  0x0083FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ64GA002",  PIC24FJ64GA002,  22016, 0,     DS39786D,  0x00ABFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ64GA004",  PIC24FJ64GA004,  22016, 0,     DS39786D,  0x00ABFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ64GA006",  PIC24FJ64GA006,  22016, 0,     DS39786D,  0x00ABFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ64GA008",  PIC24FJ64GA008,  22016, 0,     DS39786D,  0x00ABFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ64GA010",  PIC24FJ64GA010,  22016, 0,     DS39786D,  0x00ABFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ96GA006",  PIC24FJ96GA006,  32768, 0,     DS39786D,  0x00FFFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ96GA008",  PIC24FJ96GA008,  32768, 0,     DS39786D,  0x00FFFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ96GA010",  PIC24FJ96GA010,  32768, 0,     DS39786D,  0x00FFFC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ128GA006", PIC24FJ128GA006, 44032, 0,     DS39786D,  0x0157FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ128GA008", PIC24FJ128GA008, 44032, 0,     DS39786D,  0x0157FC, 2, PIC24FJ_WRITE_ROW_SIZE},
{"PIC24FJ128GA010", PIC24FJ128GA010, 44032, 0,     DS39786D,  0x0157FC, 2, PIC24FJ_WRITE_ROW_SIZE},

{"(null)",	    0,		     0,     0,     0,         0,        0, 0},
/*Device name       Device id        Flash  EEPROM Data-sheet Config/n     Panelsize*/
};
#define PIC24_SIZE ((sizeof(pic24_map) / sizeof(struct pic24_dsmap)) - 1)

/* Default device (null) */
uint32_t pic24_index = PIC24_SIZE;

void
pic24_selector(struct k8048 *k)
{
	uint32_t i;
	char *dnames[PIC24_SIZE];

	for (i = 0; i < PIC24_SIZE; ++i) {
		dnames[i] = pic24_map[i].devicename;
	}
	qsort(dnames, PIC24_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC24_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC24_SIZE);
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
 *
 * DS39768D-page 15 PIC24FJ64GA002
 * DS70102K-page 36 dsPIC30F4013
 */
void
pic24_program_verify(struct k8048 *k)
{
	/* PGD + PGC LOW */
	io_set_pgd_pgc(k, LOW, LOW);
	/* VPP + PGM LOW */
	io_set_vpp_pgm(k, LOW, LOW);
	/* DS39768D PIC24FJ64GA002 P6(100ns) */
	/* DS70102K dsPIC30F4013   P6(?)     */
	io_usleep(k, 1000); /* 1ms */

	/* VPP HIGH */
	io_set_vpp_pgm(k, HIGH, LOW);
	/* DS39768D PIC24FJ64GA002 UNSPECIFIED  */
	/* DS70102K dsPIC30F4013   UNNAMED(4ms) */
	io_usleep(k, 4000);  /* 4ms */

	/* VPP LOW */
	io_set_vpp_pgm(k, LOW, LOW);
	/* DS39768D PIC24FJ64GA002 P18(40ns)     */
	/* DS70102K dsPIC30F4013   UNNAMED(10us) */
	io_usleep(k, 1000);  /* 1ms */

	/* LVP KEY ENTRY */
	if (k->key == LVPKEY) {
		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(k, QHCMKEY, 32);
		/* DS39768D PIC24FJ64GA002 P19(1ms) */
		io_usleep(k, 1000);  /* 1ms */

		/* VPP HIGH */
		io_set_vpp_pgm(k, HIGH, LOW);
		/* DS39768D PIC24FJ64GA002 P7(25ms) */
		io_usleep(k, 25000); /* 25ms */
	}
	/* HVP */
	else {
		/* VPP HIGH */
		io_set_vpp_pgm(k, HIGH, LOW);
		/* DS70102K dsPIC30F4013 UNNAMED(4ms) */
		io_usleep(k, 4000);  /* 4ms */
	}

	/*
	 * STEP 1: EXIT THE RESET VECTOR
	 *	NOP
	 *	GOTO 0x200
	 *	NOP
	 */

	/* DS39768D-page 13 PIC24FJ64GA002 */
	/* DS70102K-page 34 dsPIC30F4013 */
	io_program_out(k, 0, 33);
	pic24_goto200(k);
}

/*
 * EXIT HVP/LVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic24_standby(struct k8048 *k)
{
	/* PGD + PGC LOW */
	io_set_pgd_pgc(k, LOW, LOW);
	/* VPP + PGM LOW */
	io_set_vpp_pgm(k, LOW, LOW);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*
 * 0000
 *
 * SIX (SHIFT IN 24-BIT INSTRUCTION AND EXECUTE)
 */
static inline void
pic24_six(struct k8048 *k, uint32_t data, uint8_t nops)
{
	io_program_out(k, 0x0, 4);
	io_program_out(k, data, 24);
	if (nops)
		io_program_out(k, 0, 28 * nops);
}

/*
 * 0001
 *
 * REGOUT (SHIFT OUT THE VISI (0x784) REGISTER)
 */
static inline uint16_t
pic24_regout(struct k8048 *k)
{
	uint16_t word;

	io_program_out(k, 0x1, 4);
	word = io_program_in(k, 24) >> 8;
	if (1)
		io_program_out(k, 0, 28 * 1);

	return word;
}

/*
 * 0010 .. 1111
 *
 * RESERVED
 */

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * GOTO 200
 */
static inline void
pic24_goto200(struct k8048 *k)
{
	pic24_six(k, 0x040200, 1);	/* GOTO 0x200 */
}

/*
 * INITIALISE TBLPAG, THE READ POINTER (W6) AND THE WRITE POINTER (W7) FOR TBLRD INSTRUCTION
 *
 *  AFFECTS
 *   TBLPAG = ADDRESS HIGH
 *   W6 = ADDRESS LOW
 *   W7 = VISI(0x784)
 */
void
pic24_set_read_pointer(struct k8048 *k, uint32_t addr)
{
	uint32_t high = 0x200000 | ((addr & 0x00FF0000) >> 12);
	uint32_t low  = 0x200006 | ((addr & 0x0000FFFF) <<  4);

	pic24_six(k, high,     0);	/* MOV #<SourceAddress23:16>, W0 */
	pic24_six(k, 0x880190, 0);	/* MOV W0, TPLPAG                */
	pic24_six(k, low,      0);	/* MOV #<SourceAddress15:0>, W6  */
	pic24_six(k, 0x207847, 1);	/* MOV #VISI, W7                 */

	pic24_goto200(k);
}

/*
 * INITIALISE TBLPAG AND THE WRITE POINTER (W7) FOR TBLWT INSTRUCTION
 *
 *  AFFECTS
 *   TBLPAG = ADDRESS HIGH
 *   W7 = ADDRESS LOW
 */
void
pic24_set_write_pointer(struct k8048 *k, uint32_t addr)
{
	uint32_t high = 0x200000 | ((addr & 0x00FF0000) >> 12);
	uint32_t low  = 0x200007 | ((addr & 0x0000FFFF) <<  4);

	pic24_six(k, high,     0);	/* MOV #<DestinationAddress23:16>, W0 */
	pic24_six(k, 0x880190, 0);	/* MOV W0, TPLPAG                     */
	pic24_six(k, low,      0);	/* MOV #<DestinationAddress15:0>, W7  */
}

/*
 * TABLE READ 16-BIT WITH POST INCREMENT
 *
 *  REQUIRES
 *   TBLPAG = ADDRESS HIGH
 *   W6 = ADDRESS LOW
 *   W7 = VISI(0x784)
 *
 *   96 0B BA TBLRDL [W6], [W7]
 *   B6 0B BA TBLRDL [W6++], [W7]
 */
static inline uint16_t
pic24_table_read16_post_increment(struct k8048 *k)
{
	/* TABLE READ LOW (WORD) [TBLPAG]:[W6] => [W7] (VISI), INCREMENT W6    */
	pic24_six(k, 0xBA0BB6, 2);	/* TBLRDL [W6++], [W7]                 */
	return pic24_regout(k);		/* Clock out contents of VISI register */
}

/*
 * TABLE READ 24-BIT WITH POST INCREMENT
 *
 *  REQUIRES
 *   TBLPAG = ADDRESS HIGH
 *   W6 = ADDRESS LOW
 *   W7 = VISI(0x784)
 *
 *   96 0B BA TBLRDL [W6], [W7]
 *   B6 0B BA TBLRDL [W6++], [W7]
 *
 *   96 8B BA TBLRDH [W6], [W7]
 *   B6 8B BA TBLRDH [W6++], [W7]
 */
uint32_t
pic24_table_read24_post_increment(struct k8048 *k)
{
	uint32_t word;
	uint16_t low, high;

	/* TABLE READ LOW  WORD [TBLPAG]:[W6] => [W7] (VISI)                   */
	pic24_six(k, 0xBA0B96, 2);	/* TBLRDL [W6], [W7]	               */
	low = pic24_regout(k);		/* Clock out contents of VISI register */

	/* TABLE READ HIGH WORD [TBLPAG]:[W6++] => [W7] (VISI)                 */
	pic24_six(k, 0xBA8BB6, 2);	/* TBLRDH [W6++], [W7]                 */
	high = pic24_regout(k);		/* Clock out contents of VISI register */

	word = (high << 16) | low;

	return word & PIC24_MASK;
}

/*
 * TABLE READ 2 x 24-BIT WITH POST INCREMENT
 *
 *  REQUIRES
 *   TBLPAG = ADDRESS HIGH
 *   W6 = ADDRESS LOW
 *   W7 = VISI(0x784)
 */
void
pic24_table_read48_post_increment(struct k8048 *k, uint32_t *word0, uint32_t *word1)
{
	uint16_t msb;

	pic24_six(k, 0xBA0B96, 2);	/* TBLRDL [W6], [W7]	               */
	*word0 = pic24_regout(k);	/* Clock out contents of VISI register */

	pic24_six(k, 0xBADBB6, 2);	/* TBLRDH.B [W6++], [W7++]             */
	pic24_six(k, 0xBAD3D6, 2);	/* TBLRDH.B [++W6], [W7--]             */
	msb = pic24_regout(k);		/* Clock out contents of VISI register */

	pic24_six(k, 0xBA0BB6, 2);	/* TBLRDL [W6++], [W7]	               */
	*word1 = pic24_regout(k);	/* Clock out contents of VISI register */

	*word0 |= (msb & 0x00FF) << 16;
	*word1 |= (msb & 0xFF00) << 8;
}

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 *
 *  DS39768D-page 17 PIC24FJ64GA002
 */
void
pic24_bulk_erase(struct k8048 *k)
{       
	struct timeval tv1 = {0}, tv2 = {0}, tv3 = {0};

	pic24_program_verify(k);

	/* NVMCON = 0x760 (0x3B0) */
	pic24_six(k, 0x2404FA, 0);	/* MOV #0x404F, W10   */
	pic24_six(k, 0x883B0A, 0);	/* MOV W10, NVMCON    */
	pic24_six(k, 0x200000, 0);	/* MOV #<PAGEVAL>, W0 */
	pic24_six(k, 0x880190, 0);	/* MOV W0, TBLPAG     */
	pic24_six(k, 0x200000, 0);	/* MOV #0x0000, W0    */
	pic24_six(k, 0xBB0800, 2);	/* TBLWTL W0, [W0]    */

	pic24_six(k, 0xA8E761, 2);	/* BSET NVMCON, #WR   */

	gettimeofday(&tv1, NULL);
	do	/* Until write completes or times out */
	{
		pic24_goto200(k);

		pic24_six(k, 0x803B02, 0);/* MOV NVMCON, W2   */
		pic24_six(k, 0x883C22, 1);/* MOV W2, VISI     */

		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while ((pic24_regout(k) & 0x8000) && (tv3.tv_sec < PIC24_TIMEOUT));

	pic24_standby(k);

	if (tv3.tv_sec >= PIC24_TIMEOUT)
		printf("%s: information: bulk erase timed out.\n", __func__);
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

void
pic24_read_config_memory(struct k8048 *k)
{
	/* Device selected by user */
	if (k->devicename[0]) {
		printf("%s: fatal error: device select is not supported on this architecture.\n", __func__);
		exit(EX_SOFTWARE);
	}

	pic24_program_verify(k);

	/* Device id/rev */
	pic24_set_read_pointer(k, PIC24_DEVID);
	pic24_conf.deviceid = pic24_table_read24_post_increment(k);
	pic24_conf.revision = pic24_table_read24_post_increment(k);

	uint32_t i = 0;
	while (pic24_map[i].deviceid) {
		if (pic24_map[i].deviceid == pic24_conf.deviceid)
			break;
		i++;
	}
	if (!pic24_map[i].deviceid) {
		if (pic24_conf.deviceid == 0x000000 || pic24_conf.deviceid == 0xFFFFFF) {
			printf("%s: fatal error: %s.\n",
				__func__, io_fault(k, pic24_conf.deviceid));
		} else {
			printf("%s: fatal error: device unknown: [%04X] [%04X]\n",
				__func__, pic24_conf.deviceid, pic24_conf.revision);
		}
		pic24_standby(k);
		exit(EX_SOFTWARE); /* Panic */
	}

	pic24_index = i; /* Device recognised */

	/* App-id */
	pic24_set_read_pointer(k, PIC24FJ_APPID);
	pic24_conf.appid = pic24_table_read24_post_increment(k);

	/* Calibration */
	pic24_set_read_pointer(k, PIC24FJ_CALIB);
	pic24_conf.calib = pic24_table_read24_post_increment(k);

	/* Config */
	pic24_set_read_pointer(k, pic24_map[pic24_index].configaddr);
	for (i = 0; i < pic24_map[pic24_index].nconfig; ++i)
		pic24_conf.config[i] = pic24_table_read16_post_increment(k);

	pic24_standby(k);
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic24_get_program_flash_size(uint32_t *addr)
{
	*addr = PIC24_CODE_LOW;

	return pic24_map[pic24_index].flash;
}

/*
 * GET DATA FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic24_get_data_flash_size(uint32_t *addr)
{
	*addr = 0;

	return 0;
}

/*
 * GET DATA EEPROM SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic24_get_data_eeprom_size(uint32_t *addr)
{
	*addr = PIC24_DATA_LOW;

	return pic24_map[pic24_index].eeprom;
}

/*
 * GET EXECUTIVE SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic24_get_executive_size(uint32_t *addr)
{
	*addr = PIC24_EXEC_LOW;

	return (PIC24_EXEC_HIGH - PIC24_EXEC_LOW + 2) >> 1;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic24_read_flash_memory_block(struct k8048 *k, uint32_t *data, uint32_t addr, uint32_t size)
{
	if (size & 1) {
		printf("%s: fatal error: block size uneven\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}

	pic24_program_verify(k);

	pic24_set_read_pointer(k, addr);
	uint32_t pc = 0;
	for (uint32_t i = 0; i < size;) {
		uint32_t word0, word1;
		pic24_table_read48_post_increment(k, &word0, &word1);
		data[i++] = (uint32_t)word0;
		data[i++] = (uint32_t)word1;
		if ((++pc % 32) == 0)
			pic24_goto200(k);
	}

	pic24_standby(k);

	return addr;
}

/*****************************************************************************
 *
 * Program Code
 *
 *****************************************************************************/

/*
 * WRITE BUFFER INIT
 */
void
pic24_write_buffer_init(struct k8048 *k)
{
	/* NVMCON = 0x760 (0x3B0) PIC24FJ_WRITE_ROW = 0x4001 */
	pic24_six(k, 0x24001A, 0); 	/* MOV #0x4001, W10   */
	pic24_six(k, 0x883B0A, 0);	/* MOV W10, NVMCON    */
}

/*
 * WRITE BUFFER
 */
void
pic24_write_buffer(struct k8048 *k, uint32_t address, uint32_t *buffer)
{
	struct timeval tv1 = {0}, tv2 = {0}, tv3 = {0};

	pic24_set_write_pointer(k, address);

	for (uint32_t i = 0; i < (pic24_map[pic24_index].panelsize / 4); i += 1) {
		uint16_t lsw0 = buffer[i * 4 + 0];
		uint16_t lsw1 = buffer[i * 4 + 1];
		uint16_t lsw2 = buffer[i * 4 + 2];
		uint16_t lsw3 = buffer[i * 4 + 3];
		uint8_t  msb0 = buffer[i * 4 + 0] >> 16;
		uint8_t  msb1 = buffer[i * 4 + 1] >> 16;
		uint8_t  msb2 = buffer[i * 4 + 2] >> 16;
		uint8_t  msb3 = buffer[i * 4 + 3] >> 16;

		uint32_t w0 = 0x200000 | (lsw0 << 4);
		uint32_t w1 = 0x200001 | (msb1 << 12) | (msb0 << 4);
		uint32_t w2 = 0x200002 | (lsw1 << 4);
		uint32_t w3 = 0x200003 | (lsw2 << 4);
		uint32_t w4 = 0x200004 | (msb3 << 12) | (msb2 << 4);
		uint32_t w5 = 0x200005 | (lsw3 << 4);

		/* LOAD W0:W5 WITH 4 INSTRUCTIONS */
		pic24_six(k, w0, 0);	/* MOV #<LSW0>, W0      */
		pic24_six(k, w1, 0);	/* MOV #<MSB1:MSB0>, W1 */
		pic24_six(k, w2, 0);	/* MOV #<LSW1>, W2      */
		pic24_six(k, w3, 0); 	/* MOV #<LSW2>, W3      */
		pic24_six(k, w4, 0); 	/* MOV #<MSB3:MSB2>, W4 */
		pic24_six(k, w5, 0);	/* MOV #<LSW3>, W5      */

		/* SET THE READ POINTER AND LOAD LATCHES */
		pic24_six(k, 0xEB0300, 1);/* CLR W6                  */
		pic24_six(k, 0xBB0BB6, 2);/* TBLWTL   [W6++], [W7]   */
		pic24_six(k, 0xBBDBB6, 2);/* TBLWTH.B [W6++], [W7++] */
		pic24_six(k, 0xBBEBB6, 2);/* TBLWTH.B [W6++], [++W7] */
		pic24_six(k, 0xBB1BB6, 2);/* TBLWTL   [W6++], [W7++] */
		pic24_six(k, 0xBB0BB6, 2);/* TBLWTL   [W6++], [W7]   */
		pic24_six(k, 0xBBDBB6, 2);/* TBLWTH.B [W6++], [W7++] */
		pic24_six(k, 0xBBEBB6, 2);/* TBLWTH.B [W6++], [++W7] */
		pic24_six(k, 0xBB1BB6, 2);/* TBLWTL   [W6++], [W7++] */
	}	
	pic24_six(k, 0xA8E761, 2);	/* BSET NVMCON, #WR   */

	gettimeofday(&tv1, NULL);
	do	/* Until write completes or times out */
	{
		pic24_goto200(k);

		pic24_six(k, 0x803B02, 0);/* MOV NVMCON, W2   */
		pic24_six(k, 0x883C22, 1);/* MOV W2, VISI     */

		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while ((pic24_regout(k) & 0x8000) && (tv3.tv_sec < PIC24_TIMEOUT));

	pic24_goto200(k);

	if (tv3.tv_sec >= PIC24_TIMEOUT)
		printf("%s: information: write panel timed out.\n", __func__);
}

/*
 * WRITE BUFFERED
 */
void
pic24_write_buffered(struct k8048 *k, uint32_t address, uint32_t data, int mode)
{
	static uint32_t *buffer = NULL;
	static uint32_t buffer_address = 0;
	static int write_pending = 0;

	if (mode == PIC24_PANEL_BEGIN || mode == PIC24_PANEL_END) {
		if (buffer) {
			if (write_pending)
				pic24_write_buffer(k, buffer_address << 1, buffer);
			free(buffer);
			buffer = NULL;
			buffer_address = 0;
			write_pending = 0;
		}
	}
	if (mode == PIC24_PANEL_BEGIN || mode == PIC24_PANEL_UPDATE) {
		if (buffer == NULL) {
			buffer = malloc(sizeof(uint32_t) * pic24_map[pic24_index].panelsize);
			if (buffer == NULL) {
				printf("%s: fatal error: malloc failed\n", __func__);
				exit(EX_SOFTWARE); /* Panic */
			}
			memset((void *)buffer, -1, sizeof(uint32_t) * pic24_map[pic24_index].panelsize);
		}
	}
	if (mode == PIC24_PANEL_UPDATE) {
		uint32_t new_address, boundary, mask;

		boundary = 0 - pic24_map[pic24_index].panelsize;
		mask = pic24_map[pic24_index].panelsize - 1;
		address >>= 1;
		new_address = address & boundary;
		if (new_address != buffer_address) {
			if (write_pending) {
				pic24_write_buffer(k, buffer_address << 1, buffer);
				memset((void *)buffer, -1, sizeof(uint32_t) * pic24_map[pic24_index].panelsize);
			}
			buffer_address = new_address;
		}
		buffer[address & mask] = data;
		write_pending = 1;
	}
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE, DATA, CONFIG or ?
 *
 *  TODO FIXME
 */
uint32_t
pic24_getregion(uint32_t address)
{
	uint32_t code_high = PIC24_CODE_LOW +
		(pic24_map[pic24_index].flash << 1) - 2;

	if (address >= PIC24_CODE_LOW   && address <= code_high) {
		return PIC24_REGIONCODE;
	}
	if (address >= PIC24_DATA_LOW   && address <= PIC24_DATA_HIGH) {
		return PIC24_REGIONDATA;
	}
	if (address >= PIC24_CONFIG_LOW && address <= PIC24_CONFIG_HIGH) {
		return PIC24_REGIONCONFIG;
	}
	if (address >= PIC24_EXEC_LOW   && address <= PIC24_EXEC_HIGH) {
		return PIC24_REGIONEXEC;
	}
	if (address >= PIC24_UNIT_LOW   && address <= PIC24_UNIT_HIGH) {
		return PIC24_REGIONUNIT;
	}
	if (address >= PIC24_DIAG_LOW   && address <= PIC24_DIAG_HIGH) {
		return PIC24_REGIONDIAG;
	}
	printf("%s: warning: address unsupported [%06X]\n",
		__func__, address);
	return PIC24_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
uint32_t
pic24_init_writeregion(struct k8048 *k, uint32_t region)
{
	pic24_write_buffered(k, 0, 0, PIC24_PANEL_END);
	if (region == PIC24_REGIONCODE) {
		pic24_write_buffered(k, 0, 0, PIC24_PANEL_BEGIN);
		return region;
	}
	printf("%s: warning: region unsupported [%d]\n",
		__func__, region);
	return PIC24_REGIONNOTSUP;
}

/*
 * WRITE REGION
 */
void
pic24_writeregion(struct k8048 *k, uint32_t address, uint32_t region, uint32_t data)
{
	switch (region) {
	case PIC24_REGIONCODE:
		pic24_write_buffered(k, address, data, PIC24_PANEL_UPDATE);
		break;
	default:printf("%s: warning: region unsupported [%d]\n",
		__func__, region);
		break;
	}
}

/*
 * INIT REGION FOR VERIFY
 *
 *  RETURN REGION IF VERIFY SUPPORTED
 */
uint32_t
pic24_init_verifyregion(struct k8048 *k, uint32_t region)
{
	if (region != PIC24_REGIONNOTSUP) {
		return region;
	}
	printf("%s: warning: region unsupported [%d]\n",
		__func__, region);
	return PIC24_REGIONNOTSUP;
}

/*
 * VERIFY REGION
 *
 *  RETURN FAILURE COUNT
 */
uint32_t
pic24_verifyregion(struct k8048 *k, uint32_t address, uint32_t region, uint16_t index, uint32_t data)
{
	uint32_t vdata = 0;

	if (region == PIC24_REGIONNOTSUP) {
		printf("%s: warning: region unsupported [%d]\n",
                	__func__, region);
		return 1;
	}
	if (index == 0)
		pic24_set_read_pointer(k, address);
	vdata = pic24_table_read24_post_increment(k);
	if (vdata != data) {
		printf("%s: error: read [%06X] expected [%06X] at [%06X]\n",
			__func__, vdata, data, address);
		return 1;
	}
	return 0;
}

/*
 * PROGRAM FILE
 */
void
pic24_program(struct k8048 *k, char *filename, int blank)
{
	uint32_t PC_address, wdata;
	uint32_t new_region, current_region = PIC24_REGIONNOTSUP;
	uint32_t total = 0;

	/* Get HEX */
	if (!inhx32(filename, 4))
		return;

	/* Initialise device for programming */
	if (blank)
		pic24_bulk_erase(k);

	/* Program device */
	pic24_program_verify(k);
	pic24_write_buffer_init(k);

	/* For each line */
	for (uint32_t i = 0; i < inhx32_count; i++) {
		PC_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic24_getregion(PC_address);
		if (new_region == PIC24_REGIONNOTSUP)
			continue;
		if (new_region != current_region) {
			current_region = pic24_init_writeregion(k, new_region);
			if (current_region == PIC24_REGIONNOTSUP)
				continue;
		}

		/* For each word in line */
		for (uint32_t j = 0; j < inhx32_pdata[i]->nbytes; j += 4) {
			wdata = inhx32_pdata[i]->bytes[j] |
				(inhx32_pdata[i]->bytes[j + 1] << 8) | (inhx32_pdata[i]->bytes[j + 2] << 16);
			pic24_writeregion(k, PC_address, current_region, wdata);
			PC_address += 2;
			total++;
		}
	}
	pic24_write_buffered(k, 0, 0, PIC24_PANEL_END);

	pic24_standby(k);
#if 0
	/* Finalise device programming (write config words) */
	if (blank) {
		total += pic24_program_config(k);
	}
#endif
	printf("Total: %u\n", total);

	inhx32_free();
}

/*
 * VERIFY FILE
 */
uint32_t
pic24_verify(struct k8048 *k, char *filename)
{
	uint32_t PC_address, fail = 0, total = 0, wdata;
	uint32_t new_region, current_region = PIC24_REGIONNOTSUP;

	/* Get HEX */
	if (!inhx32(filename, 4))
		return 1;

	/* Verify device */
	pic24_program_verify(k);

	/* For each line */
	for (uint32_t i = 0; i < inhx32_count; i++) {
		PC_address = (inhx32_pdata[i]->address >> 1);
		new_region = pic24_getregion(PC_address);
		if (new_region == PIC24_REGIONNOTSUP)
			continue;
		if (new_region != current_region) {
			current_region = pic24_init_verifyregion(k, new_region);
			if (current_region == PIC24_REGIONNOTSUP)
				continue;
		}

		/* For each word in line */
		for (uint32_t j = 0; j < inhx32_pdata[i]->nbytes; j += 4) {
			wdata = inhx32_pdata[i]->bytes[j] |
				(inhx32_pdata[i]->bytes[j + 1] << 8) | (inhx32_pdata[i]->bytes[j + 2] << 16);
			fail += pic24_verifyregion(k, PC_address, current_region, j, wdata);
			PC_address += 2;
			total++;
		}
	}
	pic24_standby(k);

	printf("Total: %u Pass: %u Fail: %u\n", total, total-fail, fail);

	inhx32_free();

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
pic24_dumpdeviceid(struct k8048 *k)
{
	printf("[%06X] [PROGRAM] %X WORDS\n",
		PIC24_CODE_LOW,
		pic24_map[pic24_index].flash);

	pic24_dumpconfig(k, PIC_BRIEF);

	printf("[%06X] [APPID]   %06X\n", PIC24FJ_APPID, pic24_conf.appid);
	printf("[%06X] [CALIB]   %06X\n", PIC24FJ_CALIB,  pic24_conf.calib);

	int16_t family = (pic24_conf.deviceid & PIC24_FAMILY_MASK) >> PIC24_DEV_SHIFT;
	int16_t device = (pic24_conf.deviceid & PIC24_DEV_MASK);
	printf("[%06X] [DEVID]   %04X FAM:%02X DEV:%02X %s\n",
		PIC24_DEVID,
		pic24_conf.deviceid,
		family,
		device,
		pic24_map[pic24_index].devicename);

	int16_t majorrev = (pic24_conf.revision & PIC24_MAJRV_MASK) >> PIC24_DEV_SHIFT;
	int16_t dotrev = (pic24_conf.revision & PIC24_DOT_MASK);
	printf("[%06X] [DEVREV]  %04X MAJ:%1X  DOT:%1X\n",
		PIC24_DEVREV,
		pic24_conf.revision,
		majorrev,
		dotrev);
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic24_dumpconfig(struct k8048 *k, int mode)
{
	printf("[%06X] [CONFIG2] %04X\n", pic24_map[pic24_index].configaddr + 0, pic24_conf.config[0]);
	printf("[%06X] [CONFIG1] %04X\n", pic24_map[pic24_index].configaddr + 2, pic24_conf.config[1]);
#ifdef VERBOSE
	if (mode == PIC_VERBOSE)
		pic24_dumpconfig_verbose(k);
#endif
}

/*
 * DUMP VERBOSE CONFIG WORD DETAILS FOR DEVICE
 */
#ifdef VERBOSE
void
pic24_dumpconfig_verbose(struct k8048 *k)
{
	/* UNIMPLEMENTED */
}
#endif /* VERBOSE */

/*
 * DUMP HEX FLASH WORDS
 */
void
pic24_dumphexwords(struct k8048 *k, uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 8) {
		if (pic_mtwords(PIC24_MASK, 8, &data[i]))
			continue;
		printf("[%06X] ", address);
		for (j = 0; j < 8; ++j)
			printf("%06X ", data[i + j] & PIC24_MASK);
		for (j = 0; j < 8; ++j) {
			putchar(PIC_CHAR(data[i + j] & 0xFF));
			putchar(PIC_CHAR(data[i + j] >> 8));
			putchar(PIC_CHAR(data[i + j] >> 16));
		}
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: program flash empty\n", __func__);
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic24_dumpinhxwords(struct k8048 *k, uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* 24-bit: Extended address */
	pic_dumpaddr(address << 1, 1);

	for (i = 0; i < size; address += 8, i += 4) {
		if (pic_mtwords(PIC24_MASK, 4, &data[i]))
			continue;
		/* 24-bit: Extended address */
		pic_dumpaddr(address << 1, 0);

		uint8_t cc, hb, mb, lb;
		mb = address >> 7;
		lb = address << 1;
		printf(":%02X%02X%02X00", 16, mb, lb);
		cc = 16 + mb + lb + 0x00;
		for (j = 0; j < 4; ++j) {
			lb = data[i + j];
			mb = data[i + j] >> 8;
			hb = data[i + j] >> 16;
			printf("%02X%02X%02X00", lb, mb, hb);
			cc = cc + lb + mb + hb + 0x00;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}

/*
 * DUMP HEX EEPROM BYTES
 */
void
pic24_dumphexbytes(struct k8048 *k, uint32_t address, uint32_t size, uint8_t *data)
{
	/* UNIMPLEMENTED */
}

/*
 * DUMP INHX32 EEPROM BYTES
 */
void
pic24_dumpinhxbytes(struct k8048 *k, uint32_t address, uint32_t size, uint8_t *data)
{
	/* UNIMPLEMENTED */
}

/*
 * DUMP DEVICE CONFIGURATION
 */
void
pic24_dumpdevice(struct k8048 *k)
{
	/* UNIMPLEMENTED */
}
