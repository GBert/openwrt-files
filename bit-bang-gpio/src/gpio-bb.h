/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of GPIO bit-bang driver
 * 
 * GPIO bit-bang driver is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * GPIO bit-bang driver is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with GPIO bit-bang driver. If not, see http://www.gnu.org/licenses/
 */

#ifndef _GPIO_BB_H
#define _GPIO_BB_H

struct gpio_bb_io {
	uint16_t pin;
	uint8_t dir;
	uint8_t bit;
};

struct gpio_bb_config {
	uint16_t clock_pin;
	uint16_t data_pin_input;
	uint16_t data_pin_output;
	uint8_t clock_falling;
	uint8_t clock_delay_low;
	uint8_t clock_delay_high;
	uint8_t lock;
};

struct gpio_bb_shift {
	uint8_t dir;
	uint8_t nbits;
	uint64_t bits;
};

#define GPIO_BB_MAX (512)
#define GPIO_BB_MASK (GPIO_BB_MAX - 1)

#define GPIO_BB_MAJOR (180)
#define GPIO_BB_IO		_IOWR(GPIO_BB_MAJOR, 200, struct gpio_bb_io *)
#define GPIO_BB_CONFIGURE	_IOW(GPIO_BB_MAJOR,  201, struct gpio_bb_config *)
#define GPIO_BB_SHIFT		_IOWR(GPIO_BB_MAJOR, 202, struct gpio_bb_shift *)

#ifndef __KERNEL__
int gpio_bb_open(const char *);
void gpio_bb_close(void);
int gpio_bb_io(struct gpio_bb_io *);
int gpio_bb_configure(struct gpio_bb_config *);
int gpio_bb_shift(struct gpio_bb_shift *);
#endif

#endif /* _GPIO_BB_H */
