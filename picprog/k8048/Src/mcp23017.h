/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _MCP23017_H
#define _MCP23017_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifdef __linux
#include <linux/i2c-dev.h>
#endif

#if 1
/*
 * IOCON_BANK = 0
 */
#define IODIRA   (0x00)
#define IODIRB   (0x01)
#define IPOLA    (0x02)
#define IPOLB    (0x03)
#define GPINTENA (0x04)
#define GPINTENB (0x05)
#define DEFVALA  (0x06)
#define DEFVALB  (0x07)
#define INTCONA  (0x08)
#define INTCONB  (0x09)
#define IOCONA   (0x0A)
#define IOCONB   (0x0B)
#define GPPUA    (0x0C)
#define GPPUB    (0x0D)
#define INTFA    (0x0E)
#define INTFB    (0x0F)
#define INTCAPA  (0x10)
#define INTCAPB  (0x11)
#define GPIOA    (0x12)
#define GPIOB    (0x13)
#define OLATA    (0x14)
#define OLATB    (0x15)
#else
/*
 * IOCON_BANK = 1
 */
#define IODIRA   (0x00)
#define IPOLA    (0x01)
#define GPINTENA (0x02)
#define DEFVALA  (0x03)
#define INTCONA  (0x04)
#define IOCONA   (0x05)
#define GPPUA    (0x06)
#define INTFA    (0x07)
#define INTCAPA  (0x08)
#define GPIOA    (0x09)
#define OLATA    (0x0A)
#define IODIRB   (0x10)
#define IPOLB    (0x11)
#define GPINTENB (0x12)
#define DEFVALB  (0x13)
#define INTCONB  (0x14)
#define IOCONB   (0x15)
#define GPPUB    (0x16)
#define INTFB    (0x17)
#define INTCAPB  (0x18)
#define GPIOB    (0x19)
#define OLATB    (0x1A)
#endif

#define IOCON (IOCONA)
#define IOCON_BANK   (0x80)
#define IOCON_MIRROR (0x40)
#define IOCON_SEQOP  (0x20)
#define IOCON_DISSLW (0x10)
#define IOCON_HAEN   (0x08)
#define IOCON_ODR    (0x04)
#define IOCON_INTPOL (0x02)
#define IOCON_U      (0x01)

/*
 * mcp23017 i2c address (default)
 */
#define MCP23017_ADDR (0x020)

/*
 * i/o ports
 */
#define MCP_IODIR (IODIRA)
#define MCP_OUT   (OLATA)
#define MCP_IN    (GPIOA)

/*
 * i/o bits
 */
#define MCP_VPP  (0x01)
#define MCP_PGC  (0x02)
#define MCP_PGDO (0x04)
#define MCP_PGDI (0x08)
#define MCP_PGM  (0x10)

/*
 * shadow output
 */
unsigned char mcp_latch;

int open_i2c(const char *, int);
void init_i2c(int);
void close_i2c(int);

void mcp_set(int, unsigned char, unsigned char);
void mcp_get(int, unsigned char, unsigned char *);

void mcp_set_pgd(int, int);
void mcp_set_pgc(int, int);
void mcp_set_pgd_pgc(int, int, int);
void mcp_set_vpp_pgm(int, int, int);

int mcp_get_pgd(int);

#endif /* !_MCP23017_H */
