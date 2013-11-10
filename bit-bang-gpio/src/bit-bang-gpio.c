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

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

static struct cdev cdev;

#include "bit-bang-gpio.h"

static uint8_t pins[BIT_BANG_GPIO_MAX] = {0}, dirs[BIT_BANG_GPIO_MAX] = {0};
static struct bit_bang_gpio_config config = {0};

static int bit_bang_gpio_io(uint8_t dir, uint8_t pin, uint8_t *bit)
{
	int err = 0;

	if (pins[pin]) {
		if (dir == 0) {
			if (dirs[pin]) {
				gpio_direction_output(pin, *bit);
				dirs[pin] = 0;
			} else {
	        		gpio_set_value(pin, *bit);
			}
		} else {
			if (!dirs[pin]) {
				gpio_direction_input(pin);
				dirs[pin] = 1;
			}
        		*bit = (gpio_get_value(pin) != 0);
		}
		return 0;
	}
	if (dir == 0) {
		if (*bit == 0) {
			err = gpio_request_one(pin, GPIOF_OUT_INIT_LOW, "bit-bang-gpio");
		} else {
			err = gpio_request_one(pin, GPIOF_OUT_INIT_HIGH, "bit-bang-gpio");
		}
		if (err) {
			printk("bit-bang-gpio: pin %d is currently unavailable.\n", pin);
			return err;
		}
		dirs[pin] = 0;
	} else {
		err = gpio_request_one(pin, GPIOF_IN, "bit-bang-gpio");
		if (err) {
			printk("bit-bang-gpio: pin %d is currently unavailable.\n", pin);
			return err;
		}
		dirs[pin] = 1;
       		*bit = (gpio_get_value(pin) != 0);
	}
	pins[pin] = 1;
	return 0;
}

static inline void bit_bang_gpio_delay(uint8_t delay)
{

	while (delay--)
        	gpio_get_value(config.data_pin_input);
}

static inline void bit_bang_gpio_clock(void)
{

	gpio_set_value(config.clock_pin, 1);
	bit_bang_gpio_delay(config.clock_delay_high);

	gpio_set_value(config.clock_pin, 0);
	bit_bang_gpio_delay(config.clock_delay_low);
}

static void bit_bang_gpio_shift_out(uint64_t bits, uint8_t nbits)
{
	uint64_t mask = 1;
	uint8_t bit = bits & 1;

	bit_bang_gpio_io(0, config.data_pin_output, &bit);

	while (nbits--) {
		gpio_set_value(config.data_pin_output, bits & mask);
		bit_bang_gpio_clock();
		mask <<= 1;
	}
}

static void bit_bang_gpio_shift_in(uint64_t *bits, uint8_t nbits)
{
	uint64_t mask = 1;
	uint8_t bit;

	bit_bang_gpio_io(1, config.data_pin_input, &bit);

	*bits = 0;
	while (nbits--) {
		if (!config.clock_falling) {
            		bit_bang_gpio_clock();
        	}
        	if (gpio_get_value(config.data_pin_input)) {
			*bits |= mask;
		}
		if (config.clock_falling) {
            		bit_bang_gpio_clock();
        	}
		mask <<= 1;
	}
}

static long bit_bang_gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	if (_IOC_TYPE(cmd) != BIT_BANG_GPIO_MAJOR) {
		return -EINVAL;
	}
	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (!access_ok(VERIFY_WRITE, arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	}
	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		if (!access_ok(VERIFY_READ, arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	}
	switch (cmd) {
	case BIT_BANG_GPIO_IO:
		{
		struct bit_bang_gpio_io io;

		if (copy_from_user(&io, (struct bit_bang_gpio_io *)arg, sizeof(io)) != 0)
            		printk("bit-bang-gpio: copy_from_user failed.\n");
		else {
			bit_bang_gpio_io(io.dir, io.pin, &io.bit);
			if (copy_to_user((struct bit_bang_gpio_io *)arg, &io, sizeof(io)) != 0)
            			printk("bit-bang-gpio: copy_to_user failed.\n");
		}
		}
		break;
	case BIT_BANG_GPIO_CONFIGURE:
		{
		if (copy_from_user(&config, (struct bit_bang_gpio_config *)arg, sizeof(config)) != 0)
            		printk("bit-bang-gpio: copy_from_user failed.\n");
		else {
			uint8_t low = 0;

			bit_bang_gpio_io(0, config.clock_pin, &low);
		}
		}
		break;
	case BIT_BANG_GPIO_SHIFT:
		{
		struct bit_bang_gpio_shift shift;

		if (copy_from_user(&shift, (struct bit_bang_gpio_shift *)arg, sizeof(shift)) != 0)
            		printk("bit-bang-gpio: copy_from_user failed.\n");
		else {
			if (shift.dir == 0)
				bit_bang_gpio_shift_out(shift.bits, shift.nbits);
			else {
				bit_bang_gpio_shift_in(&shift.bits, shift.nbits);
				if (copy_to_user((struct bit_bang_gpio_shift *)arg, &shift, sizeof(shift)) != 0)
            				printk("bit-bang-gpio: copy_to_user failed.\n");
			}
		}
		}
		break;
	}
	return 0;
}

struct file_operations bit_bang_gpio_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = bit_bang_gpio_ioctl,
};

static int __init bit_bang_gpio_init(void)
{

	cdev_init(&cdev, &bit_bang_gpio_fops);
	cdev.owner = THIS_MODULE;
	return cdev_add(&cdev, MKDEV(BIT_BANG_GPIO_MAJOR, 0), 1);
}
subsys_initcall(bit_bang_gpio_init);

static void bit_bang_gpio_exit(void)
{
	uint16_t i;

	for (i = 0; i < BIT_BANG_GPIO_MAX; ++i) {
		if (pins[i])
			gpio_free(i);
	}
	cdev_del(&cdev);
}
module_exit(bit_bang_gpio_exit);

MODULE_DESCRIPTION("bit-bang-gpio driver");
MODULE_AUTHOR("Darron Broad <darron@kewl.org>");
MODULE_LICENSE("GPL");
