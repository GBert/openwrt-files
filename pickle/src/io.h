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

#ifndef _IO_H
#define _IO_H

/* I/O OPERATIONS */
struct io_ops {
	uint8_t type;
	uint8_t single;
	uint8_t run;
	int (*open)(void);
	void (*close)(void);
	char *(*error)(void);
	void (*usleep)(int);
	void (*set_pgm)(uint8_t);
	void (*set_vpp)(uint8_t);
	void (*set_pgd)(uint8_t);
	void (*set_pgc)(uint8_t);
	uint8_t (*get_pgd)(void);
	void (*configure)(void);
	uint32_t (*shift_in)(uint8_t);
	void (*shift_out)(uint32_t, uint8_t);
};

/* I/O bit rules */
#define PGD_OUT_FLIP  (0x0001)	/* invert pgd o/p */
#define PGC_OUT_FLIP  (0x0002)	/* invert pgc o/p */
#define VPP_OUT_FLIP  (0x0004)	/* invert vpp o/p */
#define PGD_IN_FLIP   (0x0008)	/* invert pgd i/p */
#define PGD_IN_PULLUP (0x0010)	/* pgd o/p high for pgd i/p */
#define PGM_OUT_FLIP  (0x0020)	/* invert pgm o/p */
#define VPP_OUT_CLOCK (0x0040)	/* vpp clocks a T-type flip-flop */
/* GPIO bit rules */
#define PGD_RELEASE   (0x0100)	/* pgd released on exit */
#define PGC_RELEASE   (0x0200)	/* pgc released on exit */
#define PGM_RELEASE   (0x0400)	/* pgm released on exit */
#define VPP_RELEASE   (0x0800)	/* vpp released on exit */
#define VPP_RUN       (0x1000)	/* vpp high on exit if not released */
#define BB_LOCK       (0x2000)	/* gpio-bb shift with lock */
#define ALT_RELEASE   (0x4000)	/* re-enable ALT function if released */
/* Winklepicker reset */
#define TX_BREAK      (0x8000)	/* send BREAK after open and before close */

#define IONONE      (0) /* NO I/O                               */
#define IOALLWINNER (1)	/* LINUX ALLWINNER GPIO                 */
#define IOBITBANG   (2)	/* LINUX BIT-BANG DRIVER                */
#define IOCP2104    (3)	/* LINUX CP2104 USB UART                */
#define IOFTDI      (4)	/* LINUX FTDI BIT-BANG                  */
#define IOMCP2221   (5)	/* LINUX MCP2221 USB GPIO               */
#define IOMCP23017  (6)	/* LINUX MCP23017 I2C GPIO              */
#define IORPI       (7)	/* LINUX RPI GPIO DIRECT/VELLEMAN K8048 */
#define IOSERIAL    (8)	/* POSIX SERIAL BIT-BANG TTY/TTYUSB     */

#define GPIO_PGM_DISABLED (uint16_t)(-1)

typedef volatile uint32_t *GPIO_ADDR;

/* prototypes */
uint8_t io_backend(void);
void io_signal();
void io_signal_on();
void io_signal_off();
void io_config(void);
int io_open(void);
void io_close(int);
void io_exit(int);
char *io_fault(int);
char *io_error(void);
void io_usleep(uint32_t);
void io_set_pgm(uint8_t);
void io_set_vpp(uint8_t);
void io_set_pgd(uint8_t);
void io_set_pgc(uint8_t);
uint8_t io_get_pgd(void);
void io_configure(uint8_t, uint8_t);
void io_data_input(void);
uint32_t io_clock_in_bits(uint32_t, uint32_t, uint8_t);
void io_data_output(uint8_t);
void io_clock_out_bits(uint32_t, uint32_t, uint32_t, uint8_t);
uint8_t io_clock_bit_4phase(uint8_t, uint8_t);
uint32_t io_clock_bits_4phase(uint8_t, uint32_t, uint32_t);
uint32_t io_program_in(uint8_t);
void io_program_out(uint32_t, uint8_t);
void io_program_feedback(char);

#endif /* !_IO_H */
