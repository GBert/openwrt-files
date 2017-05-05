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

static int serial_port = -1;    /* File descriptor */

/*******************************************************************************
 *
 * I/O operations
 *
 ******************************************************************************/

struct io_ops serial_bb_ops = {
	.type		= IOSERIAL,
	.single		= 0,
	.run		= 0,
	.open		= serial_bb_open,
	.close		= serial_bb_close,
	.error		= serial_bb_error,
	.usleep		= serial_bb_usleep,
	.set_pgm	= NULL,
	.set_vpp	= serial_bb_set_vpp,
	.set_pgd	= serial_bb_set_pgd,
	.set_pgc	= serial_bb_set_pgc,
	.get_pgd	= serial_bb_get_pgd,
	.configure	= NULL,
	.shift_in	= NULL,
	.shift_out	= NULL,
};

uint8_t
serial_bb_backend(void)
{
	p.io = &serial_bb_ops;

	return p.io->type;
}

int
serial_bb_open(void)
{
	if (p.bitrules & VPP_OUT_CLOCK)
		serial_port = serial_open(p.device, B9600);
	else
		serial_port = serial_open(p.device, B0);
	if (serial_port < 0)
		return -1;

	return 0;
}

void
serial_bb_close(void)
{
	serial_close(serial_port);
	serial_port = -1;
}

char *
serial_bb_error(void)
{
	return "Can't open serial I/O";
}

void
serial_bb_usleep(int n)
{
	while (n--)
		serial_get_cts(serial_port); /* ~1us ttyS, ~3ms ttyUSB */
}

void
serial_bb_set_vpp(uint8_t vpp)
{
	if (p.bitrules & VPP_OUT_CLOCK) {
		static uint8_t level = 0;

		if (vpp != level) {
			serial_pulse_tx(serial_port);
			level = vpp;
		}
	} else
		serial_set_tx(serial_port, vpp);
}

void
serial_bb_set_pgd(uint8_t pgd)
{
	serial_set_dtr(serial_port, pgd);
}

void
serial_bb_set_pgc(uint8_t pgc)
{
	serial_set_rts(serial_port, pgc);
}

uint8_t
serial_bb_get_pgd(void)
{
	return serial_get_cts(serial_port);
}
