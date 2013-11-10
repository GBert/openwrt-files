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
gpio_open(struct k8048 *k)
{
#ifdef RPI
	/* Already open? */
	if (k->fd >= 0)
		return -1;

	/* Open /dev/mem */
	k->fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (k->fd < 0)
		return -1;

	/* Memory map GPIO */
	k->map = mmap(NULL, GPIO_MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, k->fd, GPIO_BASE_ADDR);
	if (k->map == MAP_FAILED) {
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
	gpio_select_output(k, k->vpp, k->bitrules & VPP_OUT_FLIP);

	/* PGM              */
	gpio_select_output(k, k->pgm, k->bitrules & PGM_OUT_FLIP);

	/* RTS/PGC CLOCK    */
	gpio_select_output(k, k->pgc, k->bitrules & PGC_OUT_FLIP);

	/* CTS/PGD DATA_IN  */
	gpio_select_input(k, k->pgdi, GPPUD_UP);
	
	/* DTR/PGD DATA_OUT */
	if (k->pgdo != k->pgdi)
		gpio_select_output(k, k->pgdo, k->bitrules & PGD_OUT_FLIP);
}

int
gpio_close(struct k8048 *k)
{
	if (k->fd < 0)
		return -1;

	if (munmap(k->map, GPIO_MAP_LEN)) {
		printf("%s: warning: munmap failed\n", __func__);
	}
	k->map = NULL;

	if (close(k->fd)) {
		printf("%s: warning: close failed\n", __func__);
	}
	k->fd = -1;

	return 0;
}

int
gpio_read(struct k8048 *k, uint8_t gpio_reg, uint32_t *val)
{
	if (k->fd < 0)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_reg;

	*val = *reg;

	return 0;
}

int
gpio_write(struct k8048 *k, uint8_t gpio_reg, uint32_t val)
{
	if (k->fd < 0)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_reg;

	*reg = val;

	return 0;
}

int
gpio_pud(struct k8048 *k, uint8_t pin, uint8_t pud)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	gpio_write(k, GPPUD, pud);
	usleep(1); /* ? */
	gpio_write(k, GPPUDCLK0, (1 << pin));
	usleep(1); /* ? */
	gpio_write(k, GPPUD, 0);
	gpio_write(k, GPPUDCLK0, 0);

	return 0;
}

int
gpio_get(struct k8048 *k, uint8_t pin, uint8_t *level)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	uint32_t val;
	gpio_read(k, GPLEV0, &val);

	*level = (val & (1 << pin)) ? (HIGH) : (LOW);

	return 0;
}

int
gpio_set(struct k8048 *k, uint8_t pin, uint8_t level)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	return gpio_write(k, (level) ? (GPSET0) : (GPCLR0), (1 << pin));
}

static inline uint32_t
gpio_gpfsel(uint8_t pin)
{
	return /* GPFSEL0 + */(pin / 10);
}

int
gpio_select_input(struct k8048 *k, uint8_t pin, uint8_t pud)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_gpfsel(pin);

	uint32_t val = ~(7 << ((pin % 10) * 3));
	*reg &= val; /* 000 = Input */
#ifdef DEBUG
	printf("%s()  reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return gpio_pud(k, pin, pud);
}

int
gpio_reselect_input(struct k8048 *k, uint8_t pin)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_gpfsel(pin);

	uint32_t val = ~(7 << ((pin % 10) * 3));
	*reg &= val; /* 000 = Input */
#ifdef DEBUG
	printf("%s()  reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return 0;
}

int
gpio_select_output(struct k8048 *k, uint8_t pin, int level)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_gpfsel(pin);
	
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
	return gpio_set(k, pin, level);
}

int
gpio_reselect_output(struct k8048 *k, uint8_t pin)
{
	if (k->fd < 0 || pin > 31)
		return -1;

	GPIO_ADDR reg = (GPIO_ADDR)(k->map) + gpio_gpfsel(pin);
	
	uint32_t val = 1 << ((pin % 10) * 3);
	*reg |= val; /* 001 = Output */
#ifdef DEBUG
	printf("%s() reg=%p *reg=0x%08X val=0x%08X\n", __func__, reg, *reg, val);
#endif
	return 0;
}

void
gpio_test(struct k8048 *k, int seconds)
{
	uint8_t output_level = 0, input_level;

	printf("\nTEST MODE 4 [RPI GPIO] CTRL-C TO STOP\n");

	while (1) {
		printf("\n");

		gpio_set(k, k->vpp, output_level);
		printf("GPIO %02d (TX/!MCLR/VPP)     = %d\n", k->vpp, output_level);

		gpio_set(k, k->pgm, output_level);
		printf("GPIO %02d (PGM)              = %d\n", k->pgm, output_level);

		gpio_set(k, k->pgc, output_level);
		printf("GPIO %02d (RTS/PGC CLOCK)    = %d\n", k->pgc, output_level);

		gpio_set(k, k->pgdo, output_level);
		printf("GPIO %02d (DTR/PGD DATA_OUT) = %d\n", k->pgdo,output_level);

		if (k->pgdi != k->pgdo) {
			gpio_get(k, k->pgdi, &input_level);
			printf("GPIO %02d (CTS/PGD DATA_IN)  = %d\n", k->pgdi, input_level);
		}

		fflush(stdout);
		
		sleep(seconds);
		output_level = 1 - output_level;
	}
}
