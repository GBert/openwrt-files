/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _K8048_H
#define _K8048_H

#undef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <time.h>
#include <errno.h> 
#include <libgen.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

#define LOW  (0)
#define HIGH (1)

#ifndef FALSE
#define FALSE (LOW)
#endif

#ifndef TRUE
#define TRUE (HIGH)
#endif

#define STRLEN (1024)
#define STRMAX (STRLEN - 1)

#define UL_ON  ("\033[4m")
#define UL_OFF ("\033[0m")

#define ARCH12BIT (0x00000FFF)
#define ARCH14BIT (0x00003FFF)
#define ARCH16BIT (0x0000FFFF)
#define ARCH24BIT (0x00FFFFFF)
#define ARCH32BIT (0xFFFFFFFF) /* UNIMPLEMENTED */

#define NOKEY  (0) /* NO  KEY IN OPERATION  */
#define HVPKEY (1) /* HVP KEY IN OPERATION */
#define LVPKEY (2) /* LVP KEY IN OPERATION */

/* I/O backends */
#ifdef TTY
#include "serial_posix.h"
#endif
#ifdef RPI
#include "raspi.h"
#endif
#ifdef MCP23017
#include "mcp23017.h"
#endif
#ifdef BITBANG
#include "bit-bang-gpio.h"
#endif

struct k8048 {
	char dotfile[STRLEN];	/* configuration file name			*/
	char device[STRLEN];	/* I/O device name: tty or rpi			*/
	char devicename[STRLEN];/* overridden PICMicro device name		*/ 
	uint32_t arch;		/* architecture mask: 12, 14, 16, 24 or 32 bit	*/
	uint8_t bitrules;	/* I/O bit rules				*/
	uint32_t key;		/* MCHP LVP key					*/
	uint32_t busy;		/* I/O busy cursor speed			*/
	uint32_t sleep_low;	/* I/O clock low duration			*/
	uint32_t sleep_high;	/* I/O clock high duration			*/
	uint32_t fwsleep;	/* ICSPIO bit time				*/
	uint32_t debug;		/* default 0 (no debugging)			*/
	uint8_t run;		/* Auto-run after I/O standby on close		*/

	/* I/O backends */
	uint8_t iot;		/* I/O type (tty, rpi or i2c)			*/
#ifdef TTY
	int fd;			/* I/O handle (tty)				*/
#endif
#ifdef RPI
	GPIO gpio;		/* I/O handle (rpi)				*/
#endif
#ifdef MCP23017
	int i2c;		/* I/O handle (i2c)				*/
	int mcp;		/* I2C (MCP23017) address			*/
#endif
#ifdef BITBANG
	int bb;			/* I/O handle (bit-bang)			*/
#endif
#if defined(RPI) || defined(BITBANG)
        uint8_t vpp;		/* TX/!MCLR/VPP     */
        uint8_t pgc;		/* RTS/PGC CLOCK    */
        uint8_t pgdo;		/* DTR/PGD DATA_OUT */
        uint8_t pgdi;		/* CTS/PGD DATA_IN  */
        uint8_t pgm;		/* PGM OUT          */
#endif
};

#include "util.h"
#include "dotconf.h"
#include "inhx32.h"
#include "io.h"
#include "pic12.h"
#include "pic14.h"
#include "pic16.h"
#include "pic24.h"
#include "pic32.h"
#include "pic.h"

/* prototypes */
void usage_k8048(struct k8048 *);
void usage_ktest(struct k8048 *, char *);
void usage_k12(struct k8048 *, char *);
void usage_k14(struct k8048 *, char *);
void usage_k16(struct k8048 *, char *);
void usage_k24(struct k8048 *, char *);
void usage_k32(struct k8048 *, char *);
void usage(struct k8048 *, char *, char *);
int main(int, char **);

#endif /* !_K8048_H */
