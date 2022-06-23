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

#ifndef _SYSFSGPIO_H
#define _SYSFSGPIO_H

#define SYSFSGPIO_NPINS		(1024)	/* Max supported pins */
#define SYSFSGPIO_TIMEOUT	(5)	/* SAFE R/W timeout */
#define SYSFSGPIO_EXPORT	"/sys/class/gpio/export"
#define SYSFSGPIO_UNEXPORT	"/sys/class/gpio/unexport"
#define SYSFSGPIO_DIRECTION	"/sys/class/gpio/gpio%d/direction"
#define SYSFSGPIO_VALUE		"/sys/class/gpio/gpio%d/value"

uint8_t sysfs_gpio_backend(void);
int sysfs_gpio_open(void);
void sysfs_gpio_release(void);
char *sysfs_gpio_error(void);
void sysfs_gpio_set_pgm(uint8_t);
void sysfs_gpio_set_vpp(uint8_t);
void sysfs_gpio_set_pgd(uint8_t);
void sysfs_gpio_set_pgc(uint8_t);
uint8_t sysfs_gpio_get_pgd(void);
void sysfs_gpio(uint16_t, char *);
void sysfs_gpio_export(uint16_t);
void sysfs_gpio_unexport(uint16_t);
void sysfs_gpio_ddr(uint16_t, uint8_t);
void sysfs_gpio_set(uint16_t, uint8_t);
uint8_t sysfs_gpio_get(uint16_t);

#endif /* _SYSFSGPIO_H */
