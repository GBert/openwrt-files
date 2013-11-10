/*
 * Copyright (C) 2013 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BIT_BANG_GPIO_H
#define _BIT_BANG_GPIO_H

struct bit_bang_gpio_io {
	uint8_t dir;
	uint8_t pin;
	uint8_t bit;
};

struct bit_bang_gpio_config {
	uint8_t clock_pin;
	uint8_t clock_falling;
	uint8_t data_pin_input;
	uint8_t data_pin_output;
	uint8_t clock_delay_low;
	uint8_t clock_delay_high;
};

struct bit_bang_gpio_shift {
	uint8_t dir;
	uint8_t nbits;
	uint64_t bits;
};

#define BIT_BANG_GPIO_MAX (256)

#define BIT_BANG_GPIO_MAJOR (180)
#define BIT_BANG_GPIO_IO	_IOWR(BIT_BANG_GPIO_MAJOR, 100, struct bit_bang_gpio_io *)
#define BIT_BANG_GPIO_CONFIGURE _IOW(BIT_BANG_GPIO_MAJOR,  101, struct bit_bang_gpio_config *)
#define BIT_BANG_GPIO_SHIFT	_IOWR(BIT_BANG_GPIO_MAJOR, 102, struct bit_bang_gpio_shift *)

#ifndef __KERNEL__
int bit_bang_gpio_io(int, struct bit_bang_gpio_io *);
int bit_bang_gpio_configure(int, struct bit_bang_gpio_config *);
int bit_bang_gpio_shift(int, struct bit_bang_gpio_shift *);
#endif

#endif /* _BIT_BANG_GPIO_H */
