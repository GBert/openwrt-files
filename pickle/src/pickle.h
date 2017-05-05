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

#ifndef _PICKLE_H
#define _PICKLE_H

#undef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <strings.h>
#include <sysexits.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <search.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <time.h>
#include <errno.h> 
#include <libgen.h>
#include <fcntl.h>
#include <limits.h>
#include <assert.h>
#include <dirent.h>
#include <poll.h>

#define LOW (0x00)
#define HIGH (0x01)

#ifndef FALSE
#define FALSE (LOW)
#endif

#ifndef TRUE
#define TRUE (HIGH)
#endif

#define STRLEN (512)
#define STRMAX (STRLEN - 1)

#define BUFLEN (512)
#define BUFMAX (BUFLEN - 1)

#define UL_ON ("\033[4m")
#define UL_OFF ("\033[0m")

#define ARCH12BIT (0x00000FFF)
#define ARCH14BIT (0x00003FFF)
#define ARCH16BIT (0x0000FFFF)
#define ARCH24BIT (0x00FFFFFF)
#define ARCH32BIT (0xFFFFFFFF)

#define NOKEY (0) /* NO KEY IN OPERATION */
#define HVPKEY (1) /* HVP KEY IN OPERATION */
#define LVPKEY (2) /* LVP KEY IN OPERATION */

struct pickle;

#ifdef RPI
#include "raspi.h"
#endif
#ifdef ALLWINNER
#include "allwinner.h"
#endif
#ifdef MCP23017
#include "mcp23017.h"
#endif
#ifdef BITBANG
#include "gpio-bb.h"
#endif
#ifdef FTDI
#include "ftdi-bb.h"
#endif
#ifdef MCP2221
#include "mcp2221.h"
#endif
#ifdef SERIAL
#include "serial_posix.h"
#include "serial-bb.h"
#endif
#ifdef CP2104
#include "cp2104-bb.h"
#endif

#if defined(STK500) || defined(PLOAD)
#include "serial_posix.h"
#include "rw.h"
#include "ip.h"
#include "stk500v2.h"
#include "stk500v2_listen.h"
#include "stk500v2_load.h"
#endif

#include "util.h"
#include "dotconf.h"
#include "io.h"
#include "icspio.h"
#include "pic.h"
#include "pic12.h"
#include "pic14.h"
#include "pic14n.h"
#include "pic16.h"
#include "pic16n.h"
#include "pic24.h"
#include "pic32.h"
#include "inhx32.h"

/* SESSION */
struct pickle {
	/* Message output stream */
	FILE *f;

	/* Command line options */
	char devicename[STRLEN];/* PICMicro device name     */
	uint32_t key;		/* MCHP LVP key             */
	uint32_t partition;	/* dsPIC/PIC24 partition    */

	/* Dot file settings */
	char dotfile[STRLEN];	/* configuration file name  */
	char device[STRLEN];	/* I/O device name          */
	char etc[STRLEN];	/* user PE directory        */
	uint32_t bitrules;	/* I/O bit rules            */
	uint32_t busy;		/* I/O busy cursor speed    */
	uint32_t sleep_low;	/* I/O clock low duration   */
	uint32_t sleep_high;	/* I/O clock high duration  */
	uint32_t fwsleep;	/* ICSPIO bit time          */
	uint32_t debug;		/* default 0 (no debugging) */
	uint32_t baudrate;	/* STK500v2 baud rate       */
	uint32_t mcp;		/* MCP23017 I2C address     */
	char usb_serial[STRLEN];/* USB serial number        */
	uint16_t vpp;		/* TX/!MCLR/VPP             */
	uint16_t pgc;		/* RTS/PGC CLOCK            */
	uint16_t pgdo;		/* DTR/PGD DATA_OUT         */
	uint16_t pgdi;		/* CTS/PGD DATA_IN          */
	uint16_t pgm;		/* PGM OUT                  */

	/* Device configuration */
	uint8_t clock_falling;	/* Clock falling edge for shifting in bits        */
	uint8_t msb_first;	/* Shift direction is MSB>LSB rather than LSB>MSB */

	/* Hardware operations */
	struct pic_ops *pic;

	/* I/O operations */
	struct io_ops *io;

	/* I/O open */
	uint8_t iot;
};

/* prototypes */
int main(int, char **);

#endif /* !_PICKLE_H */
