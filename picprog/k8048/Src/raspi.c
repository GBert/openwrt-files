/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.raspi
 */

#include "k8048.h"

/*
 * Open raspberry-pi gpio memory
 */
int
gpio_open(GPIO *gpio)
{
#ifdef RPI
	/* Already open? */
	if (gpio->fd >= 0)
		return -1;

	/* Open /dev/mem */
	gpio->fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (gpio->fd < 0)
		return -1;

	/* Memory map GPIO */
	gpio->map = mmap(NULL, GPIO_MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, gpio->fd, GPIO_BASE_ADDR);
	if (gpio->map == MAP_FAILED) {
		printf("%s: fatal error: mmap failed [%s]\n", __func__, strerror(errno));
		exit(EX_OSERR); /* Panic */
	}
	return 0;
#else
	return -1; /* Unsupported */
#endif
}

void
gpio_init(struct k8048 *k)
{
	/* TX/!MCLR/VPP     */
	gpio_select_output(&k->gpio, k->gpio.vpp, k->bitrules & VPP_OUT_FLIP);

	/* PGM              */
	gpio_select_output(&k->gpio, k->gpio.pgm, k->bitrules & PGM_OUT_FLIP);

	/* RTS/PGC CLOCK    */
	gpio_select_output(&k->gpio, k->gpio.pgc, k->bitrules & PGC_OUT_FLIP);

	/* CTS/PGD DATA_IN  */
	gpio_select_input(&k->gpio, k->gpio.pgdi, GPPUD_UP);
	
	/* DTR/PGD DATA_OUT */
	if (k->gpio.pgdo != k->gpio.pgdi)
		gpio_select_output(&k->gpio, k->gpio.pgdo, k->bitrules & PGD_OUT_FLIP);
}

int
gpio_close(GPIO *gpio)
{
	if (gpio->fd < 0)
		return -1;

	if (munmap(gpio->map, GPIO_MAP_LEN)) {
		printf("%s: warning: munmap failed\n", __func__);
	}
	gpio->map = NULL;

	if (close(gpio->fd)) {
		printf("%s: warning: close failed\n", __func__);
	}
	gpio->fd = -1;

	return 0;
}

int
gpio_read(GPIO *gpio, uint8_t gpio_reg, uint32_t *val)
{
	if (gpio->fd < 0)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_reg;

	*val = *reg;

	return 0;
}

int
gpio_write(GPIO *gpio, uint8_t gpio_reg, uint32_t val)
{
	if (gpio->fd < 0)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_reg;

	*reg = val;

	return 0;
}

int
gpio_pud(GPIO *gpio, uint8_t pin, uint8_t pud)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	gpio_write(gpio, GPPUD, pud);
	usleep(1); /* ? */
	gpio_write(gpio, GPPUDCLK0, (1 << pin));
	usleep(1); /* ? */
	gpio_write(gpio, GPPUD, 0);
	gpio_write(gpio, GPPUDCLK0, 0);

	return 0;
}

int
gpio_get(GPIO *gpio, uint8_t pin, uint8_t *level)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	uint32_t val;
	gpio_read(gpio, GPLEV0, &val);

	*level = (val & (1 << pin)) ? (HIGH) : (LOW);

	return 0;
}

int
gpio_set(GPIO *gpio, uint8_t pin, uint8_t level)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	return gpio_write(gpio, (level) ? (GPSET0) : (GPCLR0), (1 << pin));
}

static inline uint32_t
gpio_gpfsel(uint8_t pin)
{
	return /* GPFSEL0 + */(pin / 10);
}

int
gpio_select_input(GPIO *gpio, uint8_t pin, uint8_t pud)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_gpfsel(pin);

	uint32_t val = ~(7 << ((pin % 10) * 3));
	*reg &= val; /* 000 = Input */
#ifdef DEBUG
	printf("%s()  reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return gpio_pud(gpio, pin, pud);
}

int
gpio_reselect_input(GPIO *gpio, uint8_t pin)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_gpfsel(pin);

	uint32_t val = ~(7 << ((pin % 10) * 3));
	*reg &= val; /* 000 = Input */
#ifdef DEBUG
	printf("%s()  reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return 0;
}

int
gpio_select_output(GPIO *gpio, uint8_t pin, int level)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_gpfsel(pin);
	
	uint32_t val;
#if 1
	val = ~(7 << ((pin % 10) * 3));
	*reg &= val; /* 000 = Input */
#endif
	val = 1 << ((pin % 10) * 3);
	*reg |= val; /* 001 = Output */
#ifdef DEBUG
	printf("%s() reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return gpio_set(gpio, pin, level);
}

int
gpio_reselect_output(GPIO *gpio, uint8_t pin)
{
	if (gpio->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(gpio->map) + gpio_gpfsel(pin);
	
	uint32_t val = 1 << ((pin % 10) * 3);
	*reg |= val; /* 001 = Output */
#ifdef DEBUG
	printf("%s() reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return 0;
}

void
gpio_test(GPIO *gpio, int seconds)
{
	uint8_t output_level = 0, input_level;

	printf("\nTEST MODE 4 [RPI GPIO] CTRL-C TO STOP\n");

	while (1) {
		printf("\n");

		gpio_set(gpio, gpio->vpp, output_level);
		printf("GPIO %02d (TX/!MCLR/VPP)     = %d\n", gpio->vpp, output_level);

		gpio_set(gpio, gpio->pgm, output_level);
		printf("GPIO %02d (PGM)              = %d\n", gpio->pgm, output_level);

		gpio_set(gpio, gpio->pgc, output_level);
		printf("GPIO %02d (RTS/PGC CLOCK)    = %d\n", gpio->pgc, output_level);

		gpio_set(gpio, gpio->pgdo, output_level);
		printf("GPIO %02d (DTR/PGD DATA_OUT) = %d\n", gpio->pgdo,output_level);

		if (gpio->pgdi != gpio->pgdo) {
			gpio_get(gpio, gpio->pgdi, &input_level);
			printf("GPIO %02d (CTS/PGD DATA_IN)  = %d\n", gpio->pgdi, input_level);
		}

		fflush(stdout);
		
		sleep(seconds);
		output_level = 1 - output_level;
	}
}
