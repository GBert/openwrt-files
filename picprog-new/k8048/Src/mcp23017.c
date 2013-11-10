/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "mcp23017.h"

int
open_i2c(const char *i2c_device, int address)
{
#ifdef __linux
	int fd = open(i2c_device, O_RDWR);
	if (fd < 0) {
		return -1; /* Error */
	}
	if (ioctl(fd, I2C_SLAVE, address) < 0) {
		close(fd);
		return -1; /* Error */
	}
	return fd; /* Okay */
#else
	return -1; /* Unsupported */
#endif
}

void
init_i2c(int fd)
{
	if (fd < 0) {
		printf("%s: fatal error: i2c device not open\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	/* Initialise mcp23017 */
	mcp_set(fd, IOCON, 0x00);

	/* All O/P except PGDI I/P */
	mcp_set(fd, MCP_IODIR, MCP_PGDI);

	/* All O/P low */
	mcp_latch = 0;
	mcp_set(fd, MCP_OUT, mcp_latch);
}

void
close_i2c(int fd)
{
	if (fd < 0) {
		printf("%s: fatal error: i2c device not open\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	close(fd);
}

void
mcp_set(int fd, uint8_t reg, uint8_t val)
{
	uint8_t buf[2];

	buf[0] = reg;
	buf[1] = val;
	int rc = write(fd, buf, 2);
	if (rc != 2) {
		printf("%s: fatal error: unhandled error\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
}

void
mcp_get(int fd, uint8_t reg, uint8_t *val)
{
	uint8_t buf[1];

	buf[0] = reg;
	int rc = write(fd, buf, 1);
	if (rc != 1) {
		printf("%s: fatal error: unhandled error\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	rc = read(fd, buf, 1);
	if (rc != 1) {
		printf("%s: fatal error: unhandled error\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	*val = buf[0];
}

void
mcp_set_pgd(int fd, int pgd)
{
	if (pgd)
		mcp_latch |=  MCP_PGDO;
	else
		mcp_latch &= ~MCP_PGDO;
	mcp_set(fd, MCP_OUT, mcp_latch);
}

void
mcp_set_pgc(int fd, int pgc)
{
	if (pgc)
		mcp_latch |=  MCP_PGC;
	else
		mcp_latch &= ~MCP_PGC;
	mcp_set(fd, MCP_OUT, mcp_latch);
}

void
mcp_set_pgd_pgc(int fd, int pgd, int pgc)
{
	if (pgd)
		mcp_latch |=  MCP_PGDO;
	else
		mcp_latch &= ~MCP_PGDO;
	if (pgc)
		mcp_latch |=  MCP_PGC;
	else
		mcp_latch &= ~MCP_PGC;
	mcp_set(fd, MCP_OUT, mcp_latch);
}

void
mcp_set_vpp_pgm(int fd, int vpp, int pgm)
{
	if (vpp)
		mcp_latch |=  MCP_VPP;
	else
		mcp_latch &= ~MCP_VPP;
	if (pgm)
		mcp_latch |=  MCP_PGM;
	else
		mcp_latch &= ~MCP_PGM;
	mcp_set(fd, MCP_OUT, mcp_latch);
}

int
mcp_get_pgd(int fd)
{
	uint8_t pgd;

	mcp_get(fd, MCP_IN, &pgd);

	return pgd & MCP_PGDI;
}
