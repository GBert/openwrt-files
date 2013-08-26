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
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panel size*/
{"PIC18F242",	PIC18F242,	8192,		256,	DS39576B,  0,		8},
{"PIC18F248",	PIC18F248,	8192,		256,	DS39576B,  0,		8},
{"PIC18F252",	PIC18F252,	16384,		256,	DS39576B,  0,		8},
{"PIC18F258",	PIC18F258,	16384,		256,	DS39576B,  0,		8},
{"PIC18F442",	PIC18F442,	8192,		256,	DS39576B,  0,		8},
{"PIC18F448",	PIC18F448,	8192,		256,	DS39576B,  0,		8},
{"PIC18F452",	PIC18F452,	16384,		256,	DS39576B,  0,		8},
{"PIC18F458",	PIC18F458,	16384,		256,	DS39576B,  0,		8},

{"PIC18F1220",	PIC18F1220,	2048,		256,	DS39592E,  0,		8},
{"PIC18F1320",	PIC18F1320,	4096,		256,	DS39592E,  0,		8},
{"PIC18F2220",	PIC18F2220,	2048,		256,	DS39592E,  0,		8},
{"PIC18F2320",	PIC18F2320,	4096,		256,	DS39592E,  0,		8},
{"PIC18F4220",	PIC18F4220,	2048,		256,	DS39592E,  0,		8},
{"PIC18F4320",	PIC18F4320,	4096,		256,	DS39592E,  0,		8},

{"PIC18F2221",	PIC18F2221,	2048,		256,	DS39622K,  0,		8},
{"PIC18F2321",	PIC18F2321,	4096,		256,	DS39622K,  0,		8},

{"PIC18F2410",	PIC18F2410,	8192,		0,	DS39622K,  0,		32},

{"PIC18F2420",	PIC18F2420,	8192,		256,	DS39622K,  1,		32},
{"PIC18F2423",	PIC18F2423,	8192,		256,	DS39622K,  1,		32},

{"PIC18F2450",	PIC18F2450,	8192,		0,	DS39622K,  0,		8},

{"PIC18F2455",	PIC18F2455,	12288,		256,	DS39622K,  0,		32},
{"PIC18F2458",	PIC18F2458,	12288,		256,	DS39622K,  0,		32},
{"PIC18F2480",	PIC18F2480,	8192,		256,	DS39622K,  0,		32},
{"PIC18F2510",	PIC18F2510,	16384,		0,	DS39622K,  0,		32},

{"PIC18F2515",	PIC18F2515,	24576,		0,	DS39622K,  0,		64},

{"PIC18F2520",	PIC18F2520,	16384,		256,	DS39622K,  1,		32},
{"PIC18F2523",	PIC18F2523,	16384,		256,	DS39622K,  1,		32},

{"PIC18F2525",	PIC18F2525,	24576,		1024,	DS39622K,  0,		64},

{"PIC18F2550",	PIC18F2550,	16384,		256,	DS39622K,  0,		32},
{"PIC18F2553",	PIC18F2553,	16384,		256,	DS39622K,  0,		32},
{"PIC18F2580",	PIC18F2580,	16384,		256,	DS39622K,  0,		32},

{"PIC18F2585",	PIC18F2585,	24576,		1024,	DS39622K,  0,		64},
{"PIC18F2610",	PIC18F2610,	32768,		0,	DS39622K,  0,		64},
{"PIC18F2620",	PIC18F2620,	32768,		1024,	DS39622K,  0,		64},
{"PIC18F2680",	PIC18F2680,	32768,		1024,	DS39622K,  0,		64},
{"PIC18F2682",	PIC18F2682,	40960,		1024,	DS39622K,  0,		64},
{"PIC18F2685",	PIC18F2685,	49152,		1024,	DS39622K,  0,		64},

{"PIC18F4221",	PIC18F4221,	2048,		256,	DS39622K,  0,		8},
{"PIC18F4321",	PIC18F4321,	4096,		256,	DS39622K,  0,		8},

{"PIC18F4410",	PIC18F4410,	8192,		0,	DS39622K,  0,		32},

{"PIC18F4420",	PIC18F4420,	8192,		256,	DS39622K,  1,		32},
{"PIC18F4423",	PIC18F4423,	8192,		256,	DS39622K,  1,		32},

{"PIC18F4450",	PIC18F4450,	8192,		0,	DS39622K,  0,		8},

{"PIC18F4455",	PIC18F4455,	12288,		256,	DS39622K,  0,		32},
{"PIC18F4458",	PIC18F4458,	12288,		256,	DS39622K,  0,		32},
{"PIC18F4480",	PIC18F4480,	8192,		256,	DS39622K,  0,		32},
{"PIC18F4510",	PIC18F4510,	16384,		0,	DS39622K,  0,		32},

{"PIC18F4515",	PIC18F4515,	24576,		0,	DS39622K,  0,		64},

{"PIC18F4520",	PIC18F4520,	16384,		256,	DS39622K,  1,		32},
{"PIC18F4523",	PIC18F4523,	16384,		256,	DS39622K,  1,		32},

{"PIC18F4525",	PIC18F4525,	24576,		1024,	DS39622K,  0,		64},

{"PIC18F4550",	PIC18F4550,	16384,		256,	DS39622K,  0,		32},
{"PIC18F4553",	PIC18F4553,	16384,		256,	DS39622K,  0,		32},
{"PIC18F4580",	PIC18F4580,	16384,		256,	DS39622K,  0,		32},

{"PIC18F4585",	PIC18F4585,	24576,		1024,	DS39622K,  0,		64},
{"PIC18F4610",	PIC18F4610,	32768,		0,	DS39622K,  0,		64},
{"PIC18F4620",	PIC18F4620,	32768,		1024,	DS39622K,  0,		64},
{"PIC18F4680",	PIC18F4680,	32768,		1024,	DS39622K,  0,		64},
{"PIC18F4682",	PIC18F4682,	40960,		1024,	DS39622K,  0,		64},
{"PIC18F4685",	PIC18F4685,	49152,		1024,	DS39622K,  0,		64},

{"PIC18F1230",	PIC18F1230,	2048,		128,	DS39752A,  0,		8},
{"PIC18F1330",	PIC18F1330,	4096,		128,	DS39752A,  0,		8},
{"PIC18F1330-ICD",
		PIC18F1330ICD,	4096,		128,	DS39752A,  0,		8},

{"PIC18F45K22",	PIC18F45K22,	16384,		256,	DS41398B,  0,		64},
{"PIC18F25K22",	PIC18F25K22,	16384,		256,	DS41398B,  0,		64},
{"PIC18F23K22",	PIC18F23K22,	4096,		256,	DS41398B,  0,		64},
{"PIC18F24K22",	PIC18F24K22,	8192,		256,	DS41398B,  0,		64},
{"PIC18F26K22",	PIC18F26K22,	32768,		1024,	DS41398B,  0,		64},
{"PIC18F43K22",	PIC18F43K22,	4096,		256,	DS41398B,  0,		64},
{"PIC18F46K22",	PIC18F46K22,	32768,		1024,	DS41398B,  0,		64},
#ifdef PICLF
{"PIC18LF45K22",PIC18LF45K22,	16384,		256,	DS41398B,  0,		64},
{"PIC18LF25K22",PIC18LF25K22,	16384,		256,	DS41398B,  0,		64},
{"PIC18LF23K22",PIC18LF23K22,	4096,		256,	DS41398B,  0,		64},
{"PIC18LF24K22",PIC18LF24K22,	8192,		256,	DS41398B,  0,		64},
{"PIC18LF26K22",PIC18LF26K22,	32768,		1024,	DS41398B,  0,		64},
{"PIC18LF43K22",PIC18LF43K22,	4096,		256,	DS41398B,  0,		64},
{"PIC18LF46K22",PIC18LF46K22,	32768,		1024,	DS41398B,  0,		64},
#endif

{"PIC18F65K80",	PIC18F65K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18F45K80",	PIC18F45K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18F25K80",	PIC18F25K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18F66K80",	PIC18F66K80,	32768,		1024,	DS39972B,  0,		64},
{"PIC18F46K80",	PIC18F46K80,	32768,		1024,	DS39972B,  0,		64},
{"PIC18F26K80",	PIC18F26K80,	32768,		1024,	DS39972B,  0,		64},
#ifdef PICLF
{"PIC18LF65K80",PIC18LF65K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18LF45K80",PIC18LF45K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18LF25K80",PIC18LF25K80,	16384,		1024,	DS39972B,  0,		64},
{"PIC18LF66K80",PIC18LF66K80,	32768,		1024,	DS39972B,  0,		64},
{"PIC18LF46K80",PIC18LF46K80,	32768,		1024,	DS39972B,  0,		64},
{"PIC18LF26K80",PIC18LF26K80,	32768,		1024,	DS39972B,  0,		64},
#endif

{"PIC18F24K50",	PIC18F24K50,	8192,		256,	DS41630B,  0,		64},
{"PIC18F25K50",	PIC18F25K50,	16384,		256,	DS41630B,  0,		64},
{"PIC18F45K50",	PIC18F45K50,	16384,		256,	DS41630B,  0,		64},
{"PIC18F26K50",	PIC18F26K50,	32768,		256,	DS41630B,  0,		64},
{"PIC18F46K50",	PIC18F46K50,	32768,		256,	DS41630B,  0,		64},
#ifdef PICLF
{"PIC18LF24K50",PIC18LF24K50,	8192,		256,	DS41630B,  0,		64},
{"PIC18LF25K50",PIC18LF25K50,	16384,		256,	DS41630B,  0,		64},
{"PIC18LF45K50",PIC18LF45K50,	16384,		256,	DS41630B,  0,		64},
{"PIC18LF26K50",PIC18LF26K50,	32768,		256,	DS41630B,  0,		64},
{"PIC18LF46K50",PIC18LF46K50,	32768,		256,	DS41630B,  0,		64},
#endif

{"(null)",	0,		0,		0,	0,	   0,		0}
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panel size*/
};

/* Selected device (defaults to (null) entry) */
unsigned int pic16_index = (sizeof(pic16_map) / sizeof(struct pic16_dsmap)) - 1;

/* Selected device key for programming mode entry */
unsigned long pic16_key = NOKEY;

void
pic16_selector(struct k8048 *k)
{
	int i;
	for (i = 0; pic16_map[i].deviceid; ++i) {
		if (i % 4 == 3)
			printf("%s\n", pic16_map[i].devicename);
		else
			printf("%s\t", pic16_map[i].devicename);
	}
	if (i % 4)
		printf("\n");
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
void
pic16_core_instruction(struct k8048 *k, unsigned short word)
{
	lpp_icsp_write_16(k->lpp_context,0, word);
	/* io_command_out(k, "0000"); */
	/* io_word_out(k, word); */
}

/*
 * 0000
 *
 * CORE INSTRUCTION (PROGRAMMING NOP)
 *
 * DS39576B-page 13
 */
void
pic16_core_instruction_nopp(struct k8048 *k)
{
	struct mc_icsp_cmd_only_t cmd_config = {
		.command = 0x00,
		.pgc_value_after_cmd = 1,
		.pgd_value_after_cmd = 0,
		.mdelay = 1,
		.udelay = 0
	};
	lpp_icsp_command_only(k->lpp_context,&cmd_config);
	/* io_command_out(k, "000");	 000 */
	/* io_set_pgd_pgc(k, LOW, HIGH);   0   */
	lpp_icsp_delay_us(k->lpp_context,5000);
	/* io_usleep(k, 5000);		 P9  */
	/* io_set_pgc(k, LOW);		 */
	/* io_usleep(k, 1);		 P10 */
	lpp_icsp_data_only_16(k->lpp_context,0);
	/* io_word_out(k, 0); */
}

/*
 * 0000
 *
 * CORE INSTRUCTION (ERASE NOP)
 *
 * DS39576B-page 7
 */
void
pic16_core_instruction_nope(struct k8048 *k)
{
	lpp_icsp_command_only(k->lpp_context,0);
	/* io_command_out(k, "0000"); */
	lpp_icsp_delay_us(k->lpp_context,5001);
	/* io_usleep(k, 5001);		 P10 + P11  */
	lpp_icsp_data_only_16(k->lpp_context,0);
	/* io_word_out(k, 0); */
}

/*
 * 0010
 *
 * SHIFT OUT TABLAT REGISTER
 */
unsigned char
pic16_shift_out_tablat_register(struct k8048 *k)
{
	unsigned char data;
	/* io_command_out(k, "0010"); */
	/* return io_byte_in(k); */
	lpp_icsp_read_8(k->lpp_context, 2, &data);
	return data;
}

/*
 * 1000
 *
 * TABLE READ
 */
unsigned char
pic16_table_read(struct k8048 *k)
{
	unsigned char data;
	/* io_command_out(k, "1000"); */
	/* return io_byte_in(k); */
	lpp_icsp_read_8(k->lpp_context, 8, &data);
	return data;
}

/*
 * 1001
 *
 * TABLE READ POST INCREMENT
 */
unsigned char
pic16_table_read_post_increment(struct k8048 *k)
{
	unsigned char data;
	/* io_command_out(k, "1001"); */
	/* return io_byte_in(k); */
	lpp_icsp_read_8(k->lpp_context, 9, &data);
	return data;
}

/*
 * 1010
 *
 * TABLE READ POST DECREMENT
 */
unsigned char
pic16_table_read_post_decrement(struct k8048 *k)
{
	unsigned char data;
	/* io_command_out(k, "1010"); */
	/* return io_byte_in(k); */
	lpp_icsp_read_8(k->lpp_context, 10, &data);
	return data;
}

/*
 * 1011
 *
 * TABLE READ PRE INCREMENT
 */
unsigned char
pic16_table_read_pre_increment(struct k8048 *k)
{
	unsigned char data;
	/* io_command_out(k, "1011"); */
	/* return io_byte_in(k); */
	lpp_icsp_read_8(k->lpp_context, 11, &data);
	return data;
}

/*
 * 1100
 *
 * TABLE WRITE
 */
void
pic16_table_write(struct k8048 *k, unsigned short word)
{
	/* io_command_out(k, "1100"); */
	/* io_word_out(k, word); */
	lpp_icsp_write_16(k->lpp_context,12, word);
}

/*
 * 1101
 *
 * TABLE WRITE POST INCREMENT BY 2
 */
void
pic16_table_write_post_increment_2(struct k8048 *k, unsigned short word)
{
	/* io_command_out(k, "1101"); */
	/* io_word_out(k, word); */
	lpp_icsp_write_16(k->lpp_context,13, word);
}

/*
 * 1110
 *
 * TABLE WRITE POST DECREMENT BY 2
 */
void
pic16_table_write_post_decrement_2(struct k8048 *k, unsigned short word)
{
	/* io_command_out(k, "1110"); */
	/* io_word_out(k, word); */
	lpp_icsp_write_16(k->lpp_context,14, word);
}

/*
 * 1110
 *
 * TABLE WRITE START PROGRAMMING POST INCREMENT BY 2
 *
 * DS41398B-page 12
 */
void
pic16_table_write_start_programming_2(struct k8048 *k, unsigned short word)
{
	/* io_command_out(k, "1110"); */
	/* io_word_out(k, word); */
	lpp_icsp_write_16(k->lpp_context,14, word);
}

/*
 * 1111
 *
 * TABLE WRITE START PROGRAMMING
 */
void
pic16_table_write_start_programming(struct k8048 *k, unsigned short word)
{
	/* io_command_out(k, "1111"); */
	/* io_word_out(k, word); */
	lpp_icsp_write_16(k->lpp_context,15, word);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * INIT CODE MEMORY ACCESS
 */
void
pic16_init_code_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * INIT CONFIG MEMORY ACCESS
 */
void
pic16_init_config_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x8C00 | eecon1);	/* BSF EECON1, CFGS	*/
}

/*
 * INIT DATA MEMORY ACCESS (EEPROM)
 */
void
pic16_init_data_memory_access(struct k8048 *k)
{
	unsigned char eecon1 = 0xA6; /* @ 0x0FA6 */

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		eecon1 = 0x7F; /* @ 0x0F7F */
	default:break;
	}
	pic16_core_instruction(k, 0x9E00 | eecon1);	/* BCF EECON1, EEPGD	*/
	pic16_core_instruction(k, 0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * SET TABLE POINTER
 */
void
pic16_set_table_pointer(struct k8048 *k, unsigned long address)
{
	unsigned char addrl =  (address & 0x000000ff);		/*  7:0	 */
	unsigned char addrh = ((address & 0x0000ff00) >> 8);	/* 15:8	 */
	unsigned char addru = ((address & 0x003f0000) >> 16);	/* 21:16 */

	/* TBLPTRU @ 0x0FF8 */
	pic16_core_instruction(k, 0x0E00 | addru);	/* MOVLW <Addr[21:16]>  */
	pic16_core_instruction(k, 0x6EF8);		/* MOVWF TBLPTRU	*/
	/* TBLPTRH @ 0x0FF7 */
	pic16_core_instruction(k, 0x0E00 | addrh);	/* MOVLW <Addr[15:8]>   */
	pic16_core_instruction(k, 0x6EF7);		/* MOVWF TBLPTRH	*/
	/* TBLPTRL @ 0x0FF6 */
	pic16_core_instruction(k, 0x0E00 | addrl);	/* MOVLW <Addr[7:0]>	*/
	pic16_core_instruction(k, 0x6EF6);		/* MOVWF TBLPTRL	*/
}

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic16_bulk_erase(struct k8048 *k)
{
	io_init_program_verify(k, pic16_key);

	switch (pic16_map[pic16_index].datasheet) {	/* Erase entire device */
     /* case DS30500A: */
	case DS39576B:
	case DS39592E:
		pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0080);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE		*/
		pic16_init_code_memory_access(k);	/* SWITCH TO CODE	*/
		break;

	case DS39622K:
		pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x3f3f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE		*/
		pic16_init_code_memory_access(k);	/* SWITCH TO CODE	*/
		break;

	case DS39752A:
		pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8787);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE		*/
		pic16_init_code_memory_access(k);	/* SWITCH TO CODE	*/
		break;

	case DS41398B:
	case DS41630B:
		pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(k, 0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(k, 0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(k, 0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction_nope(k);		/* NOP ERASE		*/
		pic16_init_code_memory_access(k);	/* SWITCH TO CODE	*/
		break;

        default:printf("%s: information: unimplemented\n", __func__);
                break;
	}

	/* io_standby(k); */
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY 200000 .. 200007, 300000 .. 30000d, 3ffffe .. 3fffff
 */
void
pic16_read_config_memory(struct k8048 *k)
{
	unsigned short deviceid, revision, error;
	int i;

	/* Device selected by user */
	if (k->devicename[0]) {
                for (i = 0; pic16_map[i].deviceid; i++) {
                        if (strcasecmp(pic16_map[i].devicename, k->devicename) == 0) {
				pic16_index = i; /* Datasheet    */
				pic16_key = KEY; /* Key sequence */
                                break;
                        }
                }
                if (pic16_map[i].deviceid == 0) {
                        printf("%s: fatal error: unknown device: [%s]\n", __func__, k->devicename);
                        exit(EX_SOFTWARE); /* Panic */
                }
	}

	io_init_program_verify(k, pic16_key);

	pic16_init_code_memory_access(k);

	/* Id location 1..8 */
	pic16_set_table_pointer(k, PIC16_ID_LOW);
	for (i = 0; i < 8; i++)
		pic16_conf.index[i] = pic16_table_read_post_increment(k);

	/* Config 1..7 */
	pic16_set_table_pointer(k, PIC16_CONFIG_LOW);
	for (i = 0; i < 14; i++)
		pic16_conf.index[i + 8] = pic16_table_read_post_increment(k);

	/* Device id 1..2 */
	pic16_set_table_pointer(k, PIC16_DEVICEID_LOW);
	for (i = 0; i < 2; i++)
		pic16_conf.index[i + 22] = pic16_table_read_post_increment(k);

	/* io_standby(k); */

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
		 * k8048 NO POWER		ffe0 rev 31 [ffff]
		 * k8048 SWITCH IN STANDBY	0000 rev  0 [0000]
		 * k0848 SWITCH IN RUN		ffe0 rev 31 [ffff]
		 * k0848 SWITCH IN PROG		XXXX
		 */
		error = (pic16_conf.index[PIC16_CONFIG_DEVICEID2] << 8) | pic16_conf.index[PIC16_CONFIG_DEVICEID1];
		switch (error) {
		case 0x0000:
			printf("%s: fatal error: switch is on standby, or device is not a PIC18F.\n",
				__func__);
			break;
		case 0xffff:
			printf("%s: fatal error: switch is on run, or no power is supplied, or no device is installed.\n",
				__func__);
			break;
		default:printf("%s: fatal error: unknown device: %04X rev %2d [%04X]\n",
				__func__, deviceid, revision, error);
			break;
		}
		exit(EX_SOFTWARE); /* Panic */
	}

	pic16_index = i;
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
		io_init_program_verify(k, pic16_key);

		pic16_init_code_memory_access(k);

		pic16_set_table_pointer(k, PIC16_CODE_LOW);
		
		for (i = 0; i < max; i++) {
			data[i] = pic16_table_read_post_increment(k);
			data[i] |= pic16_table_read_post_increment(k) << 8;
		}

		/* io_standby(k); */
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

	io_init_program_verify(k, pic16_key);

	pic16_init_data_memory_access(k);
	for (i = 0; i < max; i++) {
		pic16_set_data_pointer(k, i);
		data[i] = pic16_read_data_memory(k);
	}

	/* io_standby(k); */

	return PIC16_DATA_LOW;
}

/*****************************************************************************
 *
 * Programming: Data (EEPROM)
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
	case DS39972B:
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
	case DS39972B:
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

	switch (pic16_map[pic16_index].datasheet) {
	case DS39622K:
	case DS39752A:
	case DS39972B:
	case DS41398B: /* PIC18F25K22 */
	case DS41630B:
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		break;
	}

	return pic16_shift_out_tablat_register(k);
}

/*
 * WRITE DATA MEMORY (EEPROM)
 */
void
pic16_write_data_memory(struct k8048 *k, unsigned char data)
{
	pic16_core_instruction(k, 0x0E00 | data);	/* MOVLW <Data>		*/
	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		/* EEDATA @ 0x0F73 */
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x6E73);	/* MOVWF EEDATA		*/
		pic16_core_instruction(k, 0x847F);	/* BSF EECON1, WREN	*/
		break;
	default:/* EEDATA @ 0x0FA8 */
		/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x6EA8);	/* MOVWF EEDATA		*/
		pic16_core_instruction(k, 0x84A6);	/* BSF EECON1, WREN	*/
		break;
	}

	switch (pic16_map[pic16_index].datasheet) {
     /*	case DS30500A: */
	case DS39576B: /* PIC18F252  */
	case DS39592E: /* PIC18F1320 */
		/* EECON2 @ 0x0FA7 */
		pic16_core_instruction(k, 0x0E55);	/* MOVLW 0x55		*/
		pic16_core_instruction(k, 0x6EA7);	/* MOVWF EECON2		*/
		pic16_core_instruction(k, 0x0EAA);	/* MOVLW 0xAA		*/
		pic16_core_instruction(k, 0x6EA7);	/* MOVWF EECON2		*/
		break;
	}

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x827F);	/* BSF EECON1, WR	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x82A6);	/* BSF EECON1, WR	*/
		break;
	}

	switch (pic16_map[pic16_index].datasheet) {
	case DS39592E: /* PIC18F1320  */
	case DS41398B: /* PIC18F25K22 */
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		pic16_core_instruction(k, 0x0000);	/* NOP			*/
		break;
	}

	do	/* Until write complete */
	{
		switch (pic16_map[pic16_index].datasheet) {
		case DS39972B:
			/* EECON1 @ 0x0F7F */
			pic16_core_instruction(k, 0x507F);/* MOVF EECON1, W, 0	*/
			break;
		default:/* EECON1 @ 0x0FA6 */
			pic16_core_instruction(k, 0x50A6);/* MOVF EECON1, W, 0	*/
			break;
		}

		/* TABLAT @ 0x0FF5 */
		pic16_core_instruction(k, 0x6EF5);	/* MOVWF TABLAT		*/

		switch (pic16_map[pic16_index].datasheet) {
		case DS39752A:
		case DS39622K:
		case DS41398B:
		case DS39972B:
		case DS41630B:
		      pic16_core_instruction(k, 0x0000);/* NOP			*/
		break;
		}
	}
	while (pic16_shift_out_tablat_register(k) & 2);

	switch (pic16_map[pic16_index].datasheet) {
	case DS39972B:
		/* EECON1 @ 0x0F7F */
		pic16_core_instruction(k, 0x947F);	/* BCF EECON1, WREN	*/
		break;
	default:/* EECON1 @ 0x0FA6 */
		pic16_core_instruction(k, 0x94A6);	/* BCF EECON1, WREN	*/
		break;
	}
}

/*****************************************************************************
 *
 * Programming Code/ID Panel
 *
 *****************************************************************************/

/*
 * INIT SINGLE PANEL WRITES
 *
 *  DS39576B / DS39592E, NOT DS39622K
 */
void
pic16_init_single_panel_writes(struct k8048 *k)
{
	pic16_init_config_memory_access(k);	/* SWITCH TO CONFIG	*/
	pic16_set_table_pointer(k, 0x3C0006);	/* PANEL MODE CONFIG &	*/
	pic16_table_write(k, 0x0000);		/* SINGLE PANEL WRITES	*/
	pic16_init_code_memory_access(k);	/* SWITCH TO CODE	*/
}

/*
 * WRITE BUFFER (PANELSIZE BYTES INTO CODE PANEL, 8 BYTES INTO ID PANEL)
 */
void
pic16_write_buffer(struct k8048 *k, unsigned long address, unsigned char buffer[MAXPANELSIZE])
{
	int i, limit;

	if (address == PIC16_ID_LOW)	/* The `id panel' size is always 8 bytes */
		limit = 6;
	else				/* The `code panel' size varies per device */
		limit = pic16_map[pic16_index].panelsize - 2;

	pic16_set_table_pointer(k, address);

	for (i = 0; i < limit; i += 2)
		pic16_table_write_post_increment_2(k, (buffer[i + 1] << 8) | buffer[i]);

	pic16_table_write_start_programming(k, (buffer[i + 1] << 8) | buffer[i]);

	pic16_core_instruction_nopp(k);
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
 * Programming Config
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

void
pic16_write_configreg(struct k8048 *k, unsigned long address, int index)
{
	unsigned short data;

	data = pic16_conf.index[index] | pic16_conf.index[index + 1] << 8;

	pic16_set_table_pointer(k, address);		/* Address must be even */
	pic16_table_write_start_programming(k, data);
	pic16_core_instruction_nopp(k);

	switch (pic16_map[pic16_index].datasheet) {	/* Increment address */
	case DS39576B:					/* INCF TBLPTRL */
	case DS39592E:
	default:/* TBLPTRL @ 0x0FF6 */
		pic16_core_instruction(k, 0x2AF6);
		break;
	case DS39752A:					/* TBLPTRL += 1 */
	case DS39622K:
		pic16_set_table_pointer(k, address + 1);
		break;
	}

	pic16_table_write_start_programming(k, data);
	pic16_core_instruction_nopp(k);
}

/*
 * WRITE CONFIG (REGISTER 6H MUST BE WRITTEN LAST)
 */
void
pic16_write_config(struct k8048 *k)
{
	pic16_init_config_memory_access(k);
	pic16_goto100000(k);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 0, PIC16_CONFIG_1L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 2, PIC16_CONFIG_2L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 4, PIC16_CONFIG_3L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 6, PIC16_CONFIG_4L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 8, PIC16_CONFIG_5L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 12, PIC16_CONFIG_7L);
	pic16_write_configreg(k, PIC16_CONFIG_LOW + 10, PIC16_CONFIG_6L);
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

	printf("%s: warning: region unknown\n", __func__);
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
		switch (pic16_map[pic16_index].datasheet) {
		case DS39576B:
		case DS39592E:
			pic16_init_single_panel_writes(k);
			break;
		default:/* Modern devices are all single panel */
			break;
		}
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
	unsigned char vdata;

	switch (region) {
	case PIC16_REGIONCODEID:
		if (index == 0)
			pic16_set_table_pointer(k, address);
		vdata = pic16_table_read_post_increment(k);
		break;
	case PIC16_REGIONCONFIG:
		/* Can't verify the config area; assume okay */
		vdata = data;
		break;
	case PIC16_REGIONDATA:
		pic16_set_data_pointer(k, address & PIC16_DATA_MASK);
		vdata = pic16_read_data_memory(k);
		break;
	}
	if (vdata != data) {
		printf("%s: error: read [%02X] expected [%02X] at [%04lx]\n",
			__func__, vdata, data, address);
		return FAIL;
	}
	return PASS;
}

/*
 * PROGRAM FILE
 */
void
pic16_program(struct k8048 *k)
{
	int i, j;
	unsigned long PC_address;
	int new_region, current_region = PIC16_REGIONUNKNOWN;
	int total = 0;

	/* Initialise device for programming */
	pic16_bulk_erase(k);
	
	/* Program device */
	io_init_program_verify(k, pic16_key);

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

	/* io_standby(k); */

	/* Finalise device programming (write config words) */
	pic16_program_config(k);
	total += 14;

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
	io_init_program_verify(k, pic16_key);

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

	/* io_standby(k); */

	printf("Total: %d Pass: %d Fail: %d\n", total, total-fail, fail);
	return fail;
}
 
/*
 * PROGRAM CONFIGURATION (AFTER VERIFY)
 */
void
pic16_program_config(struct k8048 *k)
{
	io_init_program_verify(k, pic16_key);

	pic16_write_config(k);

	/* io_standby(k); */
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
	printf("[%06X] [PROGRAM]     %04X WORDS\n", PIC16_CODE_LOW, pic16_map[pic16_index].flash);
	printf("[200000] [IDLOCATION1] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION1]);
	printf("[200001] [IDLOCATION2] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION2]);
	printf("[200002] [IDLOCATION3] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION3]);
	printf("[200003] [IDLOCATION4] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION4]);
	printf("[200004] [IDLOCATION5] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION5]);
	printf("[200005] [IDLOCATION6] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION6]);
	printf("[200006] [IDLOCATION7] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION7]);
	printf("[200007] [IDLOCATION8] %02X\n", pic16_conf.index[PIC16_CONFIG_IDLOCATION8]);
	pic16_dumpconfig(k, BRIEF);
	printf("[3FFFFE] [DEVICEID]    %04X DEV:%X (%X:%X) REV:%X",
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
 * DUMP CONFIG WORD DETAILS FOR DEVICE (INCOMPLETE MAP)
 */
void
pic16_dumpconfig(struct k8048 *k, int verbose)
{
	printf("[300000] [CONFIG1]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_1H] << 8) |
			pic16_conf.index[PIC16_CONFIG_1L]);
	printf("[300002] [CONFIG2]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_2H] << 8) |
			pic16_conf.index[PIC16_CONFIG_2L]);
	printf("[300004] [CONFIG3]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_3H] << 8) |
			pic16_conf.index[PIC16_CONFIG_3L]);
	printf("[300006] [CONFIG4]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_4H] << 8) |
			pic16_conf.index[PIC16_CONFIG_4L]);
	printf("[300008] [CONFIG5]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_5H] << 8) |
			pic16_conf.index[PIC16_CONFIG_5L]);
	printf("[30000A] [CONFIG6]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_6H] << 8) |
			pic16_conf.index[PIC16_CONFIG_6L]);
	printf("[30000C] [CONFIG7]     %04X\n",
		(pic16_conf.index[PIC16_CONFIG_7H] << 8) |
			pic16_conf.index[PIC16_CONFIG_7L]);
	if (!verbose)
		return;

	unsigned char w;

	printf("\n[CONFIG_1H]\n");
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

	printf("\n[CONFIG_2L]\n");
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

	printf("\n[CONFIG_2H]\n");
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

	printf("\n[CONFIG_3H]\n");
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

	printf("\n[CONFIG_4L]\n");
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
	printf("\n[CONFIG_5L]");
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

	printf("\n[CONFIG_5H]");
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

	printf("\n[CONFIG_6L]");
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

	printf("\n[CONFIG_6H]");
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

	printf("\n[CONFIG_7L]");
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

	printf("\n[CONFIG_7H]");
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
	int i, header = 0;

	/* USERID */
	for (i = 0; i < 8; ++i) {
		if (pic16_conf.index[i] != 0xFF) {
			if (header == 0) {
				/* Extended address = 0x0020 */
				printf(":020000040020%02X\n", 0x100 - 0x26);
				header = 1;
			}
			pic_dumpbyte(i, pic16_conf.index[i]);
		}
	}

	/* CONFIG: Extended address = 0x0030 */
	printf(":020000040030%02X\n", 0x100 - 0x36);
	for (i = 0; i < 14; ++i) {
		pic_dumpbyte(i, pic16_conf.index[PIC16_CONFIG_1L + i]);
	}
}
