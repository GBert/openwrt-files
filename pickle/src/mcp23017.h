/*
 * Copyright (C) 2005-2019 Darron Broad
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

#ifndef _MCP23017_H
#define _MCP23017_H

#include <linux/i2c-dev.h>

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

#define IOCONX_BANK   (0x80)
#define IOCONX_MIRROR (0x40)
#define IOCONX_SEQOP  (0x20)
#define IOCONX_DISSLW (0x10)
#define IOCONX_HAEN   (0x08)
#define IOCONX_ODR    (0x04)
#define IOCONX_INTPOL (0x02)
#define IOCONX_U      (0x01)

/*
 * MCP23016
 */
#define GP0     (0x00)
#define GP1     (0x01)
#define OLAT0   (0x02)
#define OLAT1   (0x03)
#define IPOL0   (0x04)
#define IPOL1   (0x05)
#define IODIR0  (0x06)
#define IODIR1  (0x07)
#define INTCAP0 (0x08)
#define INTCAP1 (0x09)
#define IOCON0  (0x0A)
#define IOCON1  (0x0B)

/* I/O ports */
#define MCP23017_IOCON (IOCONA)
#define MCP23017_IODIR (IODIRA)
#define MCP23017_OUT   (OLATA)
#define MCP23017_IN    (GPIOA)

/* I/O bits */
#define MCP23017_VPP  (0x01)
#define MCP23017_PGC  (0x02)
#define MCP23017_PGDO (0x04)
#define MCP23017_PGDI (0x08)
#define MCP23017_PGM  (0x10)

uint8_t mcp23017_backend(void);
int mcp23017_open(void);
void mcp23017_close(void);
char *mcp23017_error(void);
void mcp23017_set_pgm(uint8_t);
void mcp23017_set_vpp(uint8_t);
void mcp23017_set_pgd(uint8_t);
void mcp23017_set_pgc(uint8_t);
uint8_t mcp23017_get_pgd(void);
void mcp23017_set(uint8_t, uint8_t);
void mcp23017_get(uint8_t, uint8_t *);

#endif /* !_MCP23017_H */
