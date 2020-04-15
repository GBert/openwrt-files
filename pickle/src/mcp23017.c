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

#include "pickle.h"

/******************************************************************************
 *
 * Session
 *
 *****************************************************************************/

extern struct pickle p;

/******************************************************************************
 *
 * Back-end
 *
 *****************************************************************************/

static int mcp_fd = -1;		/* File descriptor */

static uint8_t mcp_latch;	/* Shadow output */

/*******************************************************************************
 *
 * I/O operations
 *
 ******************************************************************************/

struct io_ops mcp23017_ops = {
	.type		= IOMCP23017,
	.single		= 0,
	.run		= 1,
	.open		= mcp23017_open,
	.release        = NULL,
	.close		= mcp23017_close,
	.error		= mcp23017_error,
	.usleep		= NULL,
	.set_pgm	= mcp23017_set_pgm,
	.set_vpp	= mcp23017_set_vpp,
	.set_pgd	= mcp23017_set_pgd,
	.set_pgc	= mcp23017_set_pgc,
	.get_pgd	= mcp23017_get_pgd,
	.configure	= NULL,
	.shift_in	= NULL,
	.shift_out	= NULL,
};

uint8_t
mcp23017_backend(void)
{
	p.io = &mcp23017_ops;

	return p.io->type;
}

int
mcp23017_open(void)
{
	mcp_fd = open(p.device, O_RDWR);
	if (mcp_fd < 0) {
		mcp_fd = -1;
		return -1; /* Error */
	}

	if (ioctl(mcp_fd, I2C_SLAVE, p.mcp) < 0) {
		close(mcp_fd);
		mcp_fd = -1;
		return -1; /* Error */
	}

	/* Initialise mcp23017 */
	mcp23017_set(MCP23017_IOCON, 0x00);

	/* All O/P except PGDI I/P */
	mcp23017_set(MCP23017_IODIR, MCP23017_PGDI);

	/* All O/P low */
	mcp_latch = 0;
	mcp23017_set(MCP23017_OUT, mcp_latch);

	return mcp_fd;
}

void
mcp23017_close(void)
{
	close(mcp_fd);
	mcp_fd = -1;
}

char *
mcp23017_error(void)
{
	return "Can't open MCP23017 I2C I/O";
}

void
mcp23017_set_pgm(uint8_t pgm)
{
	if (pgm)
		mcp_latch |= MCP23017_PGM;
	else
		mcp_latch &= ~MCP23017_PGM;

	mcp23017_set(MCP23017_OUT, mcp_latch);
}

void
mcp23017_set_vpp(uint8_t vpp)
{
	if (vpp)
		mcp_latch |= MCP23017_VPP;
	else
		mcp_latch &= ~MCP23017_VPP;

	mcp23017_set(MCP23017_OUT, mcp_latch);
}

void
mcp23017_set_pgd(uint8_t pgd)
{
	if (pgd)
		mcp_latch |= MCP23017_PGDO;
	else
		mcp_latch &= ~MCP23017_PGDO;

	mcp23017_set(MCP23017_OUT, mcp_latch);
}

void
mcp23017_set_pgc(uint8_t pgc)
{
	if (pgc)
		mcp_latch |= MCP23017_PGC;
	else
		mcp_latch &= ~MCP23017_PGC;

	mcp23017_set(MCP23017_OUT, mcp_latch);
}

uint8_t
mcp23017_get_pgd(void)
{
	uint8_t pgd;

	mcp23017_get(MCP23017_IN, &pgd);

	return (pgd & MCP23017_PGDI) ? HIGH : LOW;
}

void
mcp23017_set(uint8_t reg, uint8_t val)
{
	uint8_t buf[2];

	buf[0] = reg;
	buf[1] = val;
	int rc = write(mcp_fd, buf, 2);
	if (rc != 2) {
		printf("%s: warning: unhandled error\n", __func__);
	}
}

void
mcp23017_get(uint8_t reg, uint8_t *val)
{
	uint8_t buf[1];

	buf[0] = reg;
	int rc = write(mcp_fd, buf, 1);
	if (rc != 1) {
		printf("%s: warning: unhandled error\n", __func__);
	}
	rc = read(mcp_fd, buf, 1);
	if (rc != 1) {
		printf("%s: warning: unhandled error\n", __func__);
	}
	*val = buf[0];
}
