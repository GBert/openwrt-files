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

#ifndef _PIC24_H
#define _PIC24_H

/******************************************************************************
 * 24-bit CONFIGURATION
 *****************************************************************************/

#define PIC24_MASK (0x00FFFFFF)

struct pic24_config {
	uint32_t config[2];	/* CONFIG2 .. CONFIG1     */
	uint32_t appid;		/* 8007F0                 */
	uint32_t calib;		/* OSCILLATOR CALIBRATION */
	uint32_t deviceid;	/* FF0000 		  */
	uint32_t revision;	/* FF0002 		  */
};

struct pic24_dsmap {
	char devicename[STRLEN];
	uint16_t deviceid;
	uint32_t flash;		/* flash size in words                   */
	uint16_t eeprom;	/* eeprom size in bytes 0=unsupported    */
	uint32_t datasheet;
	uint32_t configaddr;    /* CONFIG BASE                           */
	uint8_t nconfig;	/* NUMBER OF CONFIG WORDS AT CONFIG BASE */
	uint8_t panelsize;	/* PROGRAM PANEL SIZE                    */
};

/*
 * MEMORY MAP
 */
#define PIC24_CODE_LOW    (0x000000) /* PIC24/dsPIC			*/
#define PIC24_CODE_HIGH   (0x7FEFFE) /* ~4M 24-bit			*/
#define PIC24_DATA_LOW    (0x7FF000) /* dsPIC				*/
#define PIC24_DATA_HIGH   (0x7FFFFE) /*  2KB MAX			*/
#define PIC24_EXEC_LOW    (0x800000)
#define PIC24_EXEC_HIGH   (0x8007FE)
#define PIC24_UNIT_LOW    (0x8005C0) /* dsPIC				*/
#define PIC24_UNIT_HIGH   (0x8007EE)
#define PIC24_DIAG_LOW    (0x8007F0) /* PIC24FJ				*/
#define PIC24_DIAG_HIGH   (0x8007FE) /* ROLLS OVER AT 0x810000 		*/
#define PIC24_CONFIG_LOW  (0xF80000) /* dsPIC				*/
#define PIC24_CONFIG_HIGH (0xF8000E)

/*
 * EXECUTIVE (pe24f.hex and pe33f.hex are identical)
 *
 *  PIC24FJ  pe24f.hex
 *  PIC24HJ  pe33f.hex
 *  dsPIC30F pe.hex
 *  dsPIC33F pe33f.hex
 */
#define PIC24FJ_APPID	  (0x8007F0) /* APP ID WORD FOR PIC24FJ  (BB)   */
#define PIC24HJ_APPID	  (0x8007F0) /* APP ID WORD FOR PIC24HJ  (BB)   */
#define dsPIC30F_APPID	  (0x8005BE) /* APP ID WORD FOR dsPIC30F (BB)   */
#define dsPIC33F_APPID	  (0x8007F0) /* APP ID WORD FOR dsPIC33F (BB)   */

/*
 * OSCILLATOR CALIBRATION (unconfirmed)
 */
#define PIC24FJ_CALIB     (0x8007FE) /* PIC24FJ OSC. CALIBRATION        */

/*
 * MEMORY REGIONS
 */
#define PIC24_REGIONNOTSUP (0) /* NOT BELOW         */
#define PIC24_REGIONCODE   (1) /*  000000 .. 7FEFFE */
#define PIC24_REGIONDATA   (2) /*  7FF000 .. 7FFFFE */
#define PIC24_REGIONCONFIG (3) /*  F80000 .. F8000E */
#define PIC24_REGIONEXEC   (4) /*  800000 .. 8005BE */
#define PIC24_REGIONUNIT   (5) /*  8005C0 .. 8005FE */
#define PIC24_REGIONDIAG   (6) /*  8007F0 .. 8007FE */

/*
 * DEVID / DEVREV
 */
#define PIC24_DEVID      (0xFF0000) /* PIC24/dsPIC DEVID                     */
#define PIC24_DEVREV     (0xFF0002) /* PIC24/dsPIC DEVREV                    */
#define PIC24_FAMILY_MASK  (0x3FC0) /* FAMID<7:0>                            */
#define PIC24_DEV_MASK     (0x003F) /* DEV  <5:0>                            */
#define PIC24_MAJRV_MASK   (0x01C0) /* MAJRV<2:0>                            */
#define PIC24_DOT_MASK     (0x0007) /* DOT  <2:0>                            */
#define PIC24_DEV_SHIFT	   (6)      /* SHIFT RIGHT FAMID/MAJRV               */
#define dsPIC30F_PROC_MASK (0xF000) /* PROC<3:0> = 0x001                     */
#define dsPIC30F_REV_MASK  (0x0FC0) /* REV <5:0> 0000000=A 000001=B 000010=C */
#define dsPIC30F_DOT_MASK  (0x003F) /* DOT <5:0>                             */

/*
 * PROGRAM / ERASE
 */
#define PIC24_PANEL_BEGIN  (1)
#define PIC24_PANEL_UPDATE (2)
#define PIC24_PANEL_END    (3)

#define PIC24_TIMEOUT (1)

/*
 * PIC24FJ NVCON  NVMOP<3:0>
 *    1111 MEMORY BULK ERASE
 *    0011 MEMORY WRITE WORD
 *    0010 MEMORY PAGE ERASE
 *    0001 MEMORY WRITE ROW
 */
#define PIC24FJ_ERASE_CHIP (0x404F) /* WREN | ERASE  | NVMOP3 | NVMOP2 | NVMOP1 | NVMOP0 */
#define PIC24FJ_WRITE_WORD (0x4003) /* WREN |                            NVMOP1 | NVMOP0 */
#define PIC24FJ_ERASE_PAGE (0x4042) /* WREN | ERASE  |                   NVMOP1          */
#define PIC24FJ_WRITE_ROW  (0x4001) /* WREN |                                     NVMOP0 */
#define PIC24FJ_ERASE_PAGE_SIZE (512)
#define PIC24FJ_WRITE_ROW_SIZE  (64)

/******************************************************************************
 * PICMicro devices
 *****************************************************************************/

/*
 * DS39786D
 */
#define DS39786D (39786)
#define PIC24FJ16GA002  (0x0444) /* CONFIG1:002BFE CONFIG2:002BFC (11264) */
#define PIC24FJ16GA004  (0x044C)
#define PIC24FJ32GA002  (0x0445) /* CONFIG1:0057FE CONFIG2:0057FC (22528) */
#define PIC24FJ32GA004  (0x044D)
#define PIC24FJ48GA002  (0x0446) /* CONFIG1:0083FE CONFIG2:0083FC (33792) */
#define PIC24FJ48GA004  (0x044E)
#define PIC24FJ64GA002  (0x0447) /* CONFIG1:00ABFE CONFIG2:00ABFC (44032) */
#define PIC24FJ64GA004  (0x044F)
#define PIC24FJ64GA006  (0x0405)
#define PIC24FJ64GA008  (0x0408)
#define PIC24FJ64GA010  (0x040B)
#define PIC24FJ96GA006  (0x0406) /* CONFIG1:00FFFE CONFIG2:00FFFC (65536) */
#define PIC24FJ96GA008  (0x0409)
#define PIC24FJ96GA010  (0x040C)
#define PIC24FJ128GA006 (0x0407) /* CONFIG1:0157FE CONFIG2:0157FC (88064) */
#define PIC24FJ128GA008 (0x040A)
#define PIC24FJ128GA010 (0x040D)

/*
 * DS70102K
 */
#define DS70102K (70102)
#define dsPIC30F2010  (0x0040)
#define dsPIC30F2011  (0x0240)
#define dsPIC30F2012  (0x0241)
#define dsPIC30F3010  (0x01C0)
#define dsPIC30F3011  (0x01C1)
#define dsPIC30F3012  (0x00C1)
#define dsPIC30F3013  (0x00C3)
#define dsPIC30F3014  (0x0160)
#define dsPIC30F4011  (0x0101)
#define dsPIC30F4012  (0x0100)
#define dsPIC30F4013  (0x0141)
#define dsPIC30F5011  (0x0080)
#define dsPIC30F5013  (0x0081)
#define dsPIC30F5015  (0x0200)
#define dsPIC30F5016  (0x0201)
#define dsPIC30F6010  (0x0188)
#define dsPIC30F6010A (0x0281)
#define dsPIC30F6011  (0x0192)
#define dsPIC30F6011A (0x02C0)
#define dsPIC30F6012  (0x0193)
#define dsPIC30F6012A (0x02C2)
#define dsPIC30F6013  (0x0197)
#define dsPIC30F6013A (0x02C1)
#define dsPIC30F6014  (0x0198)
#define dsPIC30F6014A (0x02C3)
#define dsPIC30F6015  (0x0280)

/******************************************************************************
 * PROTOTYPES
 *****************************************************************************/

void pic24_selector(struct k8048 *);
void pic24_program_verify(struct k8048 *);
void pic24_standby(struct k8048 *);
void pic24_bulk_erase(struct k8048 *);
void pic24_read_config_memory(struct k8048 *);
uint32_t pic24_get_program_flash_size(uint32_t *);
uint32_t pic24_get_data_flash_size(uint32_t *);
uint32_t pic24_get_data_eeprom_size(uint32_t *);
uint32_t pic24_get_executive_size(uint32_t *);
uint32_t pic24_read_flash_memory_block(struct k8048 *, uint32_t *, uint32_t, uint32_t);
void pic24_write_buffer(struct k8048 *, uint32_t, uint32_t *);
void pic24_write_buffered(struct k8048 *, uint32_t, uint32_t, int);
void pic24_write_buffer_init(struct k8048 *);
uint32_t pic24_getregion(uint32_t);
uint32_t pic24_init_writeregion(struct k8048 *, uint32_t);
void pic24_writeregion(struct k8048 *, uint32_t, uint32_t, uint32_t);
uint32_t pic24_init_verifyregion(struct k8048 *, uint32_t);
uint32_t pic24_verifyregion(struct k8048 *, uint32_t, uint32_t, uint16_t, uint32_t);
void pic24_program(struct k8048 *, char *, int);
uint32_t pic24_verify(struct k8048 *, char *);
void pic24_dumpdeviceid(struct k8048 *);
void pic24_dumpconfig(struct k8048 *, int);
void pic24_dumpconfig_verbose(struct k8048 *);
void pic24_dumphexwords(struct k8048 *, uint32_t, uint32_t, uint32_t *);
void pic24_dumphexbytes(struct k8048 *, uint32_t, uint32_t, uint8_t *);
void pic24_dumpinhxwords(struct k8048 *, uint32_t, uint32_t, uint32_t *);
void pic24_dumpinhxbytes(struct k8048 *, uint32_t, uint32_t, uint8_t *);
void pic24_dumpdevice(struct k8048 *);

#endif /* !_PIC24_H */
