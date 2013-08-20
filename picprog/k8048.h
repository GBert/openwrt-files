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

/*
 * system includes
 */
#ifndef __USE_ISOC99
#define __USE_ISOC99
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
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
#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

/*
 * LF device support
 *
 * Enable this only if you are certain that your
 * setup can support LF low voltage devices.
 */
#if 0
#define PICLF
#endif

/*
 * debug malloc
 */
#ifdef DMALLOC
#include "dmalloc.h"
#endif

/*
 * operating mode defined in cli args
 */
#define PROGRAM (1)
#define VERIFY  (2)

/* 
 * operation success return codes (nb: to update counters)
 */
#define PASS (0) /* pass: add none to fault count */
#define FAIL (1) /* fail: add one to fault count */

/*
 * generic c string
 */
#define STRLEN (1024)
#define STRMAX (STRLEN - 1)

/*
 * bulk_erase
 *
 * restore internal osccal or bandgap bits
 */
#define INTERNAL (0xFFFF)
/*
 * bulk_erase
 *
 * don't restore internal osccal or bandgap bits
 */
#define NOINTERNAL (0xFFFE)

/*
 * 12/14-bit multi-word latch
 */
#define MW_NODATA (0xFFFF) /* Do not cache */
#define MW_FLUSH  (0xFFFF) /* Cache flush (address mask hit) */

/*
 * device parameters
 */
#define ARCH12BIT (0x0fff)
#define MASK12BIT (ARCH12BIT)
#define ARCH14BIT (0x3fff)
#define MASK14BIT (ARCH14BIT)
#define ARCH16BIT (0xffff)
#define MASK16BIT (ARCH16BIT)

/*
 * logic
 */
#define LOW (0)
#define HIGH (1)

/*
 * dump
 */
#define BRIEF   (0)	/* HEX FORMAT CONFIG WORD */
#define VERBOSE (1)	/* HEX FORMAT CONFIG WORD AND ANY KNOWN DATA */
#define HEXDEC  (2)	/* HEX FORMAT */
#define INHX32  (3)	/* INTEL HEX FORMAT */

/*
 * bit rules
 */
#define PGD_OUT_FLIP (1)
#define PGC_OUT_FLIP (2)
#define VPP_OUT_FLIP (4)
#define PGD_IN_FLIP (8)
#define PGD_IN_PULLUP (16)

/*
 * session
 */
struct k8048 {
	char dotfile[STRLEN];   /* configuration */
	char device[STRLEN];	/* I/O device name: tty or rpi */
	unsigned char bitrules;	/* I/O bit rules */
	int fd;			/* I/O handle (tty) */
	int busy;		/* I/O busy cursor speed */
	unsigned short arch;	/* architecture mask: 12, 14 or 16 bit */
	char devicename[STRLEN];/* overridden PICMicro device name */
	int sleep;		/* I/O bit time (default 1us) */
	int fwsleep;		/* ICSPIO bit time */
	int debug;		/* default 0 (no debugging) */
};

/*
 * local includes
 */
#include "util.h"
#include "inhx32.h"
#include "pic12.h"
#include "pic14.h"
#include "pic16.h"
#include "pic.h"

/*
 * prototypes
 */
void usage_k8048(struct k8048 *);
void usage_ktest(struct k8048 *, char *);
void usage_k12(struct k8048 *, char *);
void usage_k14(struct k8048 *, char *);
void usage_k16(struct k8048 *, char *);
void usage(struct k8048 *, char *, char *);
int main(int, char **);

#endif /* !_K8048_H */
