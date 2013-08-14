/*
 * mc-icsp-gpio interface to platform code
 *
 * Copyright (C) 2013 Gerhard Bertelsmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _LINUX_PLATFORM_DATA_MC_ICSP_GPIO_CUSTOM_H
#define _LINUX_PLATFORM_DATA_MC_ICSP_GPIO_CUSTOM_H

/**
 * struct mc_icsp_gpio_platform_data - Platform-dependent data for mc_icsp_gpio
 * @mclr_pin: GPIO pin ID to use for MCLR
 * @pgd_pin:  GPIO pin ID to use for PGD
 * @pdc_pin:  GPIO pin ID to use for PGC
 * @pgm_pin:  GPIO pin ID to use for PGM
 */
struct mc_icsp_gpio_platform_data {
	unsigned int    mclr_pin;
	unsigned int    pgd_pin;
	unsigned int    pgc_pin;
	unsigned int    pgm_pin;
};

#endif /* _LINUX_PLATFORM_DATA_MC_ICSP_GPIO_CUSTOM_H */

