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

#ifndef _PIC14N_H
#define _PIC14N_H

/******************************************************************************
 * 14-bit CONFIGURATION
 *****************************************************************************/

#define PIC14N_MASK (0x3FFF)
#define PIC14N_USERID_MAX  (4)
#define PIC14N_CONFIG_MAX  (5)
#define PIC14N_DEVINFO_MAX (32)
#define PIC14N_DEVCONF_MAX (5)
#define PIC14N_CALIB_MAX   (4)

struct pic14n_config {
	uint16_t userid[PIC14N_USERID_MAX];	/* 8000 .. 8003  */
	uint16_t reserved;              	/* 8004          */
	uint16_t revisionid;     	        /* 8005          */
	uint16_t deviceid;              	/* 8006          */
	uint16_t config[PIC14N_CONFIG_MAX]; 	/* 8007 .. 800X  */
	uint16_t devinfo[PIC14N_DEVINFO_MAX];  	/* 8100 .. 811F  */
	uint16_t devconf[PIC14N_DEVCONF_MAX];   /* 8200 .. 8205  */
	uint16_t calib[PIC14N_CALIB_MAX]; 	/* E000 .. E003  */
};

struct pic14n_dsmap {
	char devicename[STRLEN];	/*                           */
	uint16_t deviceid;    		/*                           */
	uint32_t datasheet;     	/* programming specification */
	uint32_t flash;    		/* program flash size        */
	uint32_t config;		/* configuration size        */
	uint32_t eeprom;		/* nvm eeprom size           */
	uint32_t latches;		/* latch / erase row size    */
	uint32_t devinfo;		/* devinfo size              */
	uint32_t devconf;		/* devconf size              */
	uint32_t calib;			/* calib size                */
	uint16_t *masks;                /* config masks              */
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

#define PIC14N_WORD(X) (((X) * 8192.0) / 14) /* KB to words */

#define PIC14N_USERID_ADDR  (0x8000)
#define PIC14N_RESRV_ADDR   (0x8004)
#define PIC14N_REVID_ADDR   (0x8005)
#define PIC14N_DEVID_ADDR   (0x8006)
#define PIC14N_CONFIG_ADDR  (0x8007)
#define PIC14N_DEVINFO_ADDR (0x8100)
#define PIC14N_DEVCONF_ADDR (0x8200)
#define PIC14N_CALIB_ADDR   (0xE000)
#define PIC14N_EEPROM_ADDR  (0xF000)

#define PIC14N_DEVICEID_MASK (0x0FFF)
#define PIC14N_MAJOR_SHIFT   (6)
#define PIC14N_REV_MASK      (0x003F)

#define PIC14N_ERASE   (0x8200)
#define PIC14N_LATCHES (0x8201)
#define PIC14N_ROWS    (0x8202)
#define PIC14N_EEPROM  (0x8203)
#define PIC14N_PINS    (0x8204)

#define PIC14N_TPINT_CODE   (2800)
#define PIC14N_TPINT_CONFIG (5600)
#define PIC14N_TERAB        (8400)
#define PIC14N_TERAR        (2800)

/******************************************************************************
 * PICMicro devices (8-bit data devices using new programming protocol)
 *****************************************************************************/

/* DS40001753B */
#define DS40001753B (40001753)
#define PIC16F18854  (0x306A)
#define PIC16LF18854 (0x306B)
#define PIC16F18855  (0x306C)
#define PIC16F18875  (0x306D)
#define PIC16LF18855 (0x306E)
#define PIC16LF18875 (0x306F)
#define PIC16F18856  (0x3070)
#define PIC16F18876  (0x3071)
#define PIC16LF18856 (0x3072)
#define PIC16LF18876 (0x3073)
#define PIC16F18857  (0x3074)
#define PIC16F18877  (0x3075)
#define PIC16LF18857 (0x3076)
#define PIC16LF18877 (0x3077)

/* DS40001838B */
#define DS40001838B (40001838)
#define PIC16F15313  (0x30BE)
#define PIC16LF15313 (0x30BF)
#define PIC16F15323  (0x30C0)
#define PIC16LF15323 (0x30C1)
#define PIC16F15324  (0x30C2)
#define PIC16LF15324 (0x30C3)
#define PIC16F15344  (0x30C4)
#define PIC16LF15344 (0x30C5)
#define PIC16F15325  (0x30C6)
#define PIC16LF15325 (0x30C7)
#define PIC16F15345  (0x30C8)
#define PIC16LF15345 (0x30C9)
#define PIC16F15354  (0x30AC)
#define PIC16LF15354 (0x30AD)
#define PIC16F15355  (0x30AE)
#define PIC16LF15355 (0x30AF)
#define PIC16F15356  (0x30B0)
#define PIC16LF15356 (0x30B1)
#define PIC16F15375  (0x30B2)
#define PIC16LF15375 (0x30B3)
#define PIC16F15376  (0x30B4)
#define PIC16LF15376 (0x30B5)

/* DS40001846B */
#define DS40001846B (40001846)
#define PIC16F19195  (0x309E)
#define PIC16LF19195 (0x309F)
#define PIC16F19196  (0x30A0)
#define PIC16LF19196 (0x30A1)
#define PIC16F19197  (0x30A2)
#define PIC16LF19197 (0x30A3)

/* DS40001880A */
#define DS40001880A (40001880)
#define PIC16F19155  (0x3096)
#define PIC16LF19155 (0x3097)
#define PIC16F19156  (0x3098)
#define PIC16LF19156 (0x3099)
#define PIC16F19175  (0x309A)
#define PIC16LF19175 (0x309B)
#define PIC16F19176  (0x309C)
#define PIC16LF19176 (0x309D)
#define PIC16F19185  (0x30BA)
#define PIC16LF19185 (0x30BB)
#define PIC16F19186  (0x30BC)
#define PIC16LF19186 (0x30BD)

/* DS40001970A */
#define DS40001970A (40001970)
#define PIC16F18424  (0x30CA)
#define PIC16LF18424 (0x30CB)
#define PIC16F18425  (0x30CC)
#define PIC16LF18425 (0x30CD)
#define PIC16F18426  (0x30D2)
#define PIC16LF18426 (0x30D3)
#define PIC16F18444  (0x30CE)
#define PIC16LF18444 (0x30CF)
#define PIC16F18445  (0x30D0)
#define PIC16LF18445 (0x30D1)
#define PIC16F18446  (0x30D4)
#define PIC16LF18446 (0x30D5)
#define PIC16F18455  (0x30D7)
#define PIC16LF18455 (0x30D8)
#define PIC16F18456  (0x30D9)
#define PIC16LF18456 (0x30DA)

/* DS40002149A */
#define DS40002149A (40002149)
#define PIC16F15213 (0x30E3)
#define PIC16F15214 (0x30E6)
#define PIC16F15223 (0x30E4)
#define PIC16F15224 (0x30E7)
#define PIC16F15225 (0x30E9)
#define PIC16F15243 (0x30E5)
#define PIC16F15244 (0x30E8)
#define PIC16F15245 (0x30EA)
#define PIC16F15254 (0x30F0)
#define PIC16F15255 (0x30EF)
#define PIC16F15256 (0x30EB)
#define PIC16F15274 (0x30EE)
#define PIC16F15275 (0x30ED)
#define PIC16F15276 (0x30EC)

/******************************************************************************/

uint32_t pic14n_arch(void);
void pic14n_selector(void);
void pic14n_bootloader(void);
void pic14n_program_verify(void);
void pic14n_standby(void);
void pic14n_bulk_erase(void);
void pic14n_row_erase(uint32_t, uint32_t);
int pic14n_read_config_memory(void);
void pic14n_write_panel(uint32_t, uint32_t, uint32_t *, uint32_t);
uint32_t pic14n_get_program_count(void);
uint32_t pic14n_get_program_size(uint32_t *, uint32_t);
uint32_t pic14n_get_data_size(uint32_t *);
uint32_t pic14n_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic14n_read_data_memory_block(uint16_t *, uint32_t, uint16_t);
uint32_t pic14n_write_config(void);
uint16_t pic14n_getregion(uint32_t);
uint32_t pic14n_program_data(uint32_t, pic_data *);
void pic14n_program_begin(void);
void pic14n_program_end(int);
uint32_t pic14n_verify_data(uint32_t, pic_data *, uint32_t *);
void pic14n_view_data(pic_data *);
void pic14n_dumpdeviceid(void);
void pic14n_dumpconfig(uint32_t, uint32_t);
void pic14n_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic14n_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic14n_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic14n_dumpinhxdata(uint32_t, uint32_t, uint16_t *);
void pic14n_dumpdevice(void);

#endif /* !_PIC14N_H */
