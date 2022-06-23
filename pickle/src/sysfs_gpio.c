/*
 * Copyright (C) 2017-2020 Darron Broad
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

#undef DEBUG

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

/* I/O Pins 0..NPINS-1 DDR */
static uint8_t sysfs_gpio_dirs[SYSFSGPIO_NPINS];

/*******************************************************************************
 *
 * I/O operations
 *
 ******************************************************************************/

struct io_ops sysfs_gpio_ops = {
	.type		= IOSYSFSGPIO,
	.run		= 1,
	.uid		= -1,
	.open		= sysfs_gpio_open,
	.release	= sysfs_gpio_release,
	.close		= NULL,
	.error		= sysfs_gpio_error,
	.usleep		= NULL,
	.set_pgm	= sysfs_gpio_set_pgm,
	.set_vpp	= sysfs_gpio_set_vpp,
	.set_pgd	= sysfs_gpio_set_pgd,
	.set_pgc	= sysfs_gpio_set_pgc,
	.get_pgd	= sysfs_gpio_get_pgd,
	.configure	= NULL,
	.shift_in	= NULL,
	.shift_out	= NULL,
};

uint8_t
sysfs_gpio_backend(void)
{
	p.io = &sysfs_gpio_ops;

	return p.io->type;
}

/*
 * Allocate SysFs GPIO
 */
int
sysfs_gpio_open(void)
{
	if (access(SYSFSGPIO_EXPORT, W_OK) < 0)
		return -1;

	sysfs_gpio_export(p.vpp);
	sysfs_gpio_export(p.pgc);
	sysfs_gpio_export(p.pgdo);
	if (p.pgdi != p.pgdo)
		sysfs_gpio_export(p.pgdi);
	if (p.pgm != GPIO_DISABLED)
		sysfs_gpio_export(p.pgm);

	/* Reset DDR cache */
	memset(&sysfs_gpio_dirs, 0xFF, SYSFSGPIO_NPINS);

	return 0;
}

/*
 * Deallocate SysFs GPIO
 */
void
sysfs_gpio_release(void)
{
	if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_DISABLED)
		sysfs_gpio_unexport(p.pgm);
	if (p.bitrules & PGD_RELEASE) {
		sysfs_gpio_unexport(p.pgdo);
		if (p.pgdi != p.pgdo)
			sysfs_gpio_unexport(p.pgdi);
	}
	if (p.bitrules & PGC_RELEASE)
		sysfs_gpio_unexport(p.pgc);
	if (p.bitrules & VPP_RELEASE)
		sysfs_gpio_unexport(p.vpp);
}

char *
sysfs_gpio_error(void)
{
	return "Can't open SysFs GPIO";
}

void
sysfs_gpio_set_pgm(uint8_t pgm)
{
	if (p.pgm != GPIO_DISABLED)
		sysfs_gpio_set(p.pgm, pgm);
}

void
sysfs_gpio_set_vpp(uint8_t vpp)
{
	sysfs_gpio_set(p.vpp, vpp);
}

void
sysfs_gpio_set_pgd(uint8_t pgd)
{
	sysfs_gpio_set(p.pgdo, pgd);
}

void
sysfs_gpio_set_pgc(uint8_t pgc)
{
	sysfs_gpio_set(p.pgc, pgc);
}

uint8_t
sysfs_gpio_get_pgd(void)
{
	return sysfs_gpio_get(p.pgdi);
}

/*
 * EXPORT / UNEXPORT
 */
void
sysfs_gpio(uint16_t pin, char *fname)
{
	char str[STRLEN];
	int rc, fd, len;

	fd = open(fname, O_WRONLY);
	if (fd < 0) {
		printf("%s: fatal error: open failed [%s] [%s]\n",
			__func__, fname, strerror(errno));
		io_exit(EX_SOFTWARE); /* Panic */
	}
	len = snprintf(str, STRLEN, "%d", pin);
	if (len < 0) {
		printf("%s: fatal error: snprintf failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	} else if (len >= STRLEN) {
		printf("%s: fatal error: snprintf overrun\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	if (p.bitrules & SYSFS_SAFE)
		rc = rw_write(fd, str, len, SYSFSGPIO_TIMEOUT);
	else
		rc = write(fd, str, len);
	if (rc != len) {
		printf("%s: fatal error: write failed [%d]\n",
			__func__, rc);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	close(fd);
}

/*
 * DIRECTION/VALUE TEMPLATE
 */
static char *
sysfs_gpio_template(uint16_t pin, char *template)
{
	static char str[STRLEN];
	int rc;

	rc = snprintf(str, STRLEN, template, pin);
	if (rc < 0) {
		printf("%s: fatal error: snprintf failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	} else if (rc >= STRLEN) {
		printf("%s: fatal error: snprintf overrun\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	return str;
}

/*
 * DIRECTION/VALUE TEMPLATE OPEN
 */
static int
sysfs_gpio_template_open(uint16_t pin, char *template)
{
	char *fn;
	int fd;

	fn = sysfs_gpio_template(pin, template);

	fd = open(fn, O_RDWR);
	if (fd < 0) {
		printf("%s: fatal error: open failed [%s] [%s]\n",
			__func__, fn, strerror(errno));
		io_exit(EX_SOFTWARE); /* Panic */
	}
	return fd;
}

/*
 * EXPORT
 */
void
sysfs_gpio_export(uint16_t pin)
{
	char *fn;
	int retry;

	if (pin >= SYSFSGPIO_NPINS) {
		printf("%s: fatal error: unsupported pin [%d]\n", __func__, pin);
		io_exit(EX_SOFTWARE); /* Panic */
	}

	fn = sysfs_gpio_template(pin, SYSFSGPIO_DIRECTION);
	if (access(fn, W_OK) == 0)
		return;

	sysfs_gpio(pin, SYSFSGPIO_EXPORT);

	for (retry = 0; retry < 100; ++retry) {
		io_usleep(50000); /* 50ms */

		if (access(fn, W_OK) == 0)
			return;
	}

	printf("%s: fatal error: export failed [%s]\n", __func__, fn);
	io_exit(EX_SOFTWARE); /* Panic */
}

/*
 * UNEXPORT
 */
void
sysfs_gpio_unexport(uint16_t pin)
{
	sysfs_gpio_ddr(pin, 1);

	sysfs_gpio(pin, SYSFSGPIO_UNEXPORT);
}

void
sysfs_gpio_ddr(uint16_t pin, uint8_t dir)
{
	int rc, fd, len;

	if (sysfs_gpio_dirs[pin] == dir)
		return; /* No change */
	sysfs_gpio_dirs[pin] = dir;

	fd = sysfs_gpio_template_open(pin, SYSFSGPIO_DIRECTION);
	if (dir == 0) { 
		len = 3;
		if (p.bitrules & SYSFS_SAFE)
			rc = rw_write(fd, "out", len, SYSFSGPIO_TIMEOUT);
		else 
			rc = write(fd, "out", len);
	} else {
		len = 2;
		if (p.bitrules & SYSFS_SAFE)
			rc = rw_write(fd, "in", len, SYSFSGPIO_TIMEOUT);
		else
			rc = write(fd, "in", len);
	}
	if (rc != len) {
		printf("%s: fatal error: write failed [%d]\n",
			__func__, rc);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	close(fd);
}

void
sysfs_gpio_set(uint16_t pin, uint8_t value)
{
	char ch = value + '0';
	int rc, fd;

	sysfs_gpio_ddr(pin, 0);

	fd = sysfs_gpio_template_open(pin, SYSFSGPIO_VALUE);
	if (p.bitrules & SYSFS_SAFE)
		rc = rw_write(fd, &ch, 1, SYSFSGPIO_TIMEOUT);
	else
		rc = write(fd, &ch, 1);
	if (rc != 1) {
		printf("%s: fatal error: write failed [%d]\n",
			__func__, rc);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	close(fd);
}

uint8_t
sysfs_gpio_get(uint16_t pin)
{
	char ch;
	int rc, fd;

	sysfs_gpio_ddr(pin, 1);

	fd = sysfs_gpio_template_open(pin, SYSFSGPIO_VALUE);
	if (p.bitrules & SYSFS_SAFE)
		rc = rw_read(fd, &ch, 1, SYSFSGPIO_TIMEOUT);
	else
		rc = read(fd, &ch, 1);
	if (rc != 1 || (ch != '0' && ch != '1')) {
		printf("%s: fatal error: read failed [%d]\n",
			__func__, rc);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	close(fd);

	return ch - '0';
}
