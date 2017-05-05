/*
 * Copyright (C) 2005-2017 Darron Broad
 * Copyright (C) 2016 Gerhard Bertelsmann
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

/* Shadow output */
static uint8_t pin_latch = 0x00, pin_ddr = /* Magic */ 0xBB;

/* FTDI handle */
static struct ftdi_context handle = {0};

/* I/O buffer */
static uint8_t buffer[FTDI_BB_BUFFER_SIZE];

/*******************************************************************************
 *
 * I/O operations
 *
 ******************************************************************************/

struct io_ops ftdi_bb_ops = {
	.type		= IOFTDI,
	.single		= 1,
	.run		= 0,
	.open		= ftdi_bb_open,
	.close		= ftdi_bb_close,
	.error		= ftdi_bb_error,
	.usleep		= NULL,
	.set_pgm	= ftdi_bb_set_pgm,
	.set_vpp	= ftdi_bb_set_vpp,
	.set_pgd	= ftdi_bb_set_pgd,
	.set_pgc	= ftdi_bb_set_pgc,
	.get_pgd	= ftdi_bb_get_pgd,
	.configure	= NULL,
	.shift_in	= ftdi_bb_shift_in,
	.shift_out	= ftdi_bb_shift_out,
};

uint8_t
ftdi_bb_backend(void)
{
	p.io = &ftdi_bb_ops;

	return p.io->type;
}

int
ftdi_bb_open(void)
{
	/* Initialize and find device */
	if (ftdi_init(&handle) < 0) {
		printf("%s: ftdi_init failed [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (*p.usb_serial) {
		if ((ftdi_usb_open_desc(&handle, 0x0403, 0x6015, NULL, p.usb_serial) < 0) &&
			(ftdi_usb_open_desc(&handle, 0x0403, 0x6001, NULL, p.usb_serial) < 0)) {
			printf("%s: can't open FT232R/FT230X device [%s] with serial ID %s\n",
				__func__, ftdi_get_error_string(&handle), p.usb_serial);
			return -1;
		}
	} else {
		if ((ftdi_usb_open(&handle, 0x0403, 0x6015) < 0) &&
			(ftdi_usb_open(&handle, 0x0403, 0x6001) < 0)) {
			printf("%s: can't open FT230X device [%s]\n", __func__,
				ftdi_get_error_string(&handle));
			return -1;
		}
	}

	/* All input */
	if (ftdi_bb_bitmode(0x00) < 0)
		return -1;

	if (ftdi_set_baudrate(&handle, p.baudrate) < 0) {
		printf("%s: can't set baudrate [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (p.bitrules & TX_BREAK) {
		/* Winklepicker mode 2 */
		struct ftdi_bb_io io = {.dir = 0};

		/* PASS-THROUGH ON */
		io.pin = p.vpp;
		io.bit = LOW;
		ftdi_bb_io(&io);

		/* BREAK */
		io.pin = 0; /* TX */
		io.bit = LOW;
		ftdi_bb_io(&io);
		io_usleep(55000);
		io.pin = 0; /* TX */
		io.bit = HIGH;
		ftdi_bb_io(&io);
	}

	return 0;
}

void
ftdi_bb_close(void)
{
	if (p.bitrules & TX_BREAK) {
	        /* Winklepicker mode 2 */
	        struct ftdi_bb_io io = {.dir = 0};

		/* PASS-THROUGH OFF */
	        io.pin = p.vpp;
	        io.bit = HIGH;
	        ftdi_bb_io(&io);

	        /* BREAK */
	        io.pin = 0; /* TX */
	        io.bit = LOW;
	        ftdi_bb_io(&io);
	        io_usleep(55000);
	        io.pin = 0; /* TX */
	        io.bit = HIGH;
	        ftdi_bb_io(&io);
	}

	ftdi_disable_bitbang(&handle);
	ftdi_usb_reset(&handle);
	ftdi_usb_close(&handle);
}

char *
ftdi_bb_error(void)
{
	return "Can't open FTDI bit-bang I/O";
}

void
ftdi_bb_set_pgm(uint8_t pgm)
{
	if (p.pgm != GPIO_PGM_DISABLED) {
		struct ftdi_bb_io io = {.dir = 0, .pin = p.pgm, .bit = pgm};

		ftdi_bb_io(&io);
	}
}

void
ftdi_bb_set_vpp(uint8_t vpp)
{
	struct ftdi_bb_io io = {.dir = 0, .pin = p.vpp, .bit = vpp};

	ftdi_bb_io(&io);
}

void
ftdi_bb_set_pgd(uint8_t pgd)
{
	struct ftdi_bb_io io = {.dir = 0, .pin = p.pgdo, .bit = pgd};

	ftdi_bb_io(&io);
}

void
ftdi_bb_set_pgc(uint8_t pgc)
{
	struct ftdi_bb_io io = {.dir = 0, .pin = p.pgc, .bit = pgc};

	ftdi_bb_io(&io);
}

uint8_t
ftdi_bb_get_pgd(void)
{
	struct ftdi_bb_io io = {.dir = 1, .pin = p.pgdi, .bit = 0};

	ftdi_bb_io(&io);

	return io.bit;
}

uint32_t
ftdi_bb_shift_in(uint8_t nbits)
{
	struct ftdi_bb_shift shift = {1, nbits, 0};

	ftdi_bb_shift(&shift);

	return (uint32_t)(shift.bits);
}

void
ftdi_bb_shift_out(uint32_t bits, uint8_t nbits)
{
	struct ftdi_bb_shift shift = {0, nbits, (uint64_t)(bits)};

	ftdi_bb_shift(&shift);
}

int
ftdi_bb_bitmode(uint8_t new_ddr)
{
	if (pin_ddr != new_ddr) {
		int rc, retry = 10;
		
		pin_ddr = new_ddr;

		while ((rc = ftdi_set_bitmode(&handle, pin_ddr, BITMODE_SYNCBB)) < 0 && retry--)
			io_usleep(10000);

		if (rc < 0) {
			printf("%s: ftdi_set_bimode failed [%s]\n", __func__,
				ftdi_get_error_string(&handle));
			return -1;
		}
	}

	return 0;
}

int
ftdi_bb_io(struct ftdi_bb_io *io)
{
	uint8_t new_ddr = pin_ddr, pin_port;

	if (io->dir) {	/* In */
		new_ddr &= ~(1 << io->pin);
	} else {	/* Out */
		new_ddr |= (1 << io->pin);
		if (io->bit)	/* Set */
			pin_latch |= (1 << io->pin);
		else		/* Reset */
			pin_latch &= ~(1 << io->pin);
	}

	if (ftdi_bb_bitmode(new_ddr) < 0)
		return -1;

	if (ftdi_write_data(&handle, &pin_latch, 1) < 0) {
		printf("%s: ftdi_write_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (ftdi_read_data(&handle, &pin_port, 1) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (io->dir) /* In */
		io->bit = (pin_port & (1 << io->pin)) != 0;

	return 0;
}

static inline void
ftbi_bb_dumpbuffer(struct ftdi_bb_shift *shift, uint32_t ix)
{
	uint8_t c, d, bit;
	static char read[] = "READ", write[] = "WRITE", *dir[] = {write, read};

	printf("%s VALUE=%08" PRId64 " NBITS=%d MSB=%d IX=%d\n",
		dir[shift->dir & 1], shift->bits, shift->nbits, p.msb_first, ix);
	for (uint32_t i = 0; i < ix; i += FTDI_BB_STEP) {
		printf("[%02X]", i);
		for (uint32_t j = 0; j < FTDI_BB_STEP; ++j) {
			if (buffer[i + j] & (1 << p.pgc))
				c = 'C';
			else
				c = '_';
			if (shift->dir)
				bit = (1 << p.pgdi);
			else
				bit = (1 << p.pgdo);
			if (buffer[i + j] & bit)
				d = 'H';
			else
				d = '_';
			printf(" %c%c", c, d);
		}
		printf("\n");
	}
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
	uint32_t ix = 0, offset;
	uint64_t value, value_mask;

	assert(shift->nbits > 0 && shift->nbits < 65);

	if (p.pgdi == p.pgdo) {
		uint8_t new_ddr = pin_ddr;

		if (shift->dir) /* In */
			new_ddr &= ~(1 << p.pgdi);
		else		/* Out */
			new_ddr |= (1 << p.pgdi);

		if (ftdi_bb_bitmode(new_ddr) < 0)
			return -1;
	}

	bzero(buffer, FTDI_BB_BUFFER_SIZE);
	value = shift->bits;
	value_mask = (p.msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
	for (uint32_t i = 0; i < shift->nbits; ++i) {
		if (!shift->dir) {	/* Out */
			if (value & value_mask)	/* Set */
				pin_latch |= (1 << p.pgdo);
			else			/* Reset */
				pin_latch &= ~(1 << p.pgdo);
		}
		buffer[ix++] = pin_latch;	/* STEP 1     */
		pin_latch |= (1 << p.pgc);	/* CLOCK HIGH */
		buffer[ix++] = pin_latch;	/* STEP 2     */
		buffer[ix++] = pin_latch;	/* STEP 3     */
		pin_latch &= ~(1 << p.pgc);	/* CLOCK LOW  */
		buffer[ix++] = pin_latch;	/* STEP 4     */
		value_mask = (p.msb_first) ? (value_mask >> 1) : (value_mask << 1);
	}

	if (p.debug >= 10)
		ftbi_bb_dumpbuffer(shift, ix);

	assert(ix < FTDI_BB_BUFFER_SIZE);

	if ((ftdi_write_data(&handle, buffer, ix)) < 0) {
		printf("%s: ftdi_write_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if ((ftdi_read_data(&handle, buffer, ix)) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (shift->dir) { /* In */
		value = 0;
		value_mask = (p.msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
		offset = (p.clock_falling) ? FTDI_BB_FALLING : FTDI_BB_RISING;
		for (uint32_t i = 0; i < ix; i += FTDI_BB_STEP) {
			if (buffer[i + offset] & (1 << p.pgdi))
				value |= value_mask;
			value_mask = (p.msb_first) ? (value_mask >> 1) : (value_mask << 1);
		}
		shift->bits = value;

		if (p.debug >= 10)
			ftbi_bb_dumpbuffer(shift, ix);
	}

	return 0;
}
