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

#ifndef _RASPI_H
#define _RASPI_H

struct k8048;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <sysexits.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Access from ARM Running Linux */
#define BCM2708_PERI_BASE_ADDR (0x20000000)
#define GPIO_BASE_ADDR (BCM2708_PERI_BASE_ADDR + 0x200000)
#define GPIO_MAP_LEN (0x201000 - 0x200000)

/* I/O access */
typedef volatile uint32_t *GPIO_ADDR;

/* GPIO registers (32-bit) */
#define GPFSEL0 (0x00)
#define GPFSEL1 (0x01)
#define GPFSEL2 (0x02)
#define GPFSEL3 (0x03)
#define GPFSEL4 (0x04)
#define GPFSEL5 (0x05)
/* RESERVED 0x06 */
#define GPSET0 (0x07)
#define GPSET1 (0x08)
/* RESERVED 0x09 */
#define GPCLR0 (0x0A)
#define GPCLR1 (0x0B)
/* RESERVED 0x0C */
#define GPLEV0 (0x0D)
#define GPLEV1 (0x0E)
/* RESERVED 0x0F */
#define GPEDS0 (0x10)
#define GPEDS1 (0x11)
/* RESERVED 0x12 */
#define GPREN0 (0x13)
#define GPREN1 (0x14)
/* RESERVED 0x15 */
#define GPFEN0 (0x16)
#define GPFEN1 (0x17)
/* RESERVED 0x18 */
#define GPHEN0 (0x19)
#define GPHEN1 (0x1A)
/* RESERVED 0x1B */
#define GPLEN0 (0x1C)
#define GPLEN1 (0x1D)
/* RESERVED 0x1E */
#define GPAREN0 (0x1F)
#define GPAREN1 (0x20)
/* RESERVED 0x21 */
#define GPAFEN0 (0x22)
#define GPAFEN1 (0x23)
/* RESERVED 0x24 */
#define GPPUD (0x25)
#define GPPUDCLK0 (0x26)
#define GPPUDCLK1 (0x27)
/* RESERVED 0x28 */
/* RESERVED 0x29 */
/* RESERVED 0x2A */
/* RESERVED 0x2B */
/* TEST 0x2C */

#define GPPUD_OFF (0)
#define GPPUD_DOWN (1)
#define GPPUD_UP (2)
#define GPPUD_RESERVED (3)
#define GPPUD_DELAY (150)

int gpio_open(struct k8048 *);
void gpio_init(struct k8048 *);
int gpio_close(struct k8048 *);

int gpio_read(struct k8048 *, uint8_t, uint32_t *);
int gpio_write(struct k8048 *, uint8_t, uint32_t);

int gpio_pud(struct k8048 *, uint8_t, uint8_t);

int gpio_get(struct k8048 *, uint8_t, uint8_t *);
int gpio_set(struct k8048 *, uint8_t, uint8_t);

int gpio_select_input(struct k8048 *, uint8_t, uint8_t);
int gpio_reselect_input(struct k8048 *, uint8_t);
int gpio_select_output(struct k8048 *, uint8_t, int);
int gpio_reselect_output(struct k8048 *, uint8_t);

void gpio_test(struct k8048 *k, int);

/*
 * Default GPIO pins
 */
#define GPIO_VPP  (14)
#define GPIO_PGC  (15)
#define GPIO_PGDO (23)
#define GPIO_PGDI (24)
#define GPIO_PGM  (22)

#endif /* !_RASPI_H */
