/*
 *  Custom GPIO-based MC-ICSP platform define
 *
 *  Copyright (C) 2013 Gerhard Bertelsmann
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  Based on i2c-gpio-custom by:
 *  Copyright (C) 2007-2008 Gabor Juhos <juhosg@openwrt.org>
 * ---------------------------------------------------------------------------
 *
 *  The behaviour of this driver can be altered by setting some parameters
 *  from the insmod command line.
 *
 *  The following parameters are adjustable:
 *
 *	<mclr>,<pgd>,<pgc>,<pgm>
 *
 *  where:
 *
 *  <mclr>	GPIO pin ID to used for MCLR (required)
 *  <pgd>	GPIO pin ID to used for PGD (required)
 *  <pgc>	GPIO pin ID to used for PGC (required)
 *  <pgm>	GPIO pin ID to used for PGM (required)
 *
 *  See include/mc-icsp-gpio.h for more information about the parameters.
 *
 *  If this driver is built into the kernel, you can use the following kernel
 *  command line parameters, with the same values as the corresponding module
 *  parameters listed above:
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

#include "mc-icsp.h"
#include "mc-icsp-gpio-custom.h"

#define DRV_NAME	"mc-icsp-gpio-custom"
#define DRV_DESC	"Custom GPIO-based Microchip ICSP driver"
#define DRV_VERSION	"0.0.1"

#define PFX		DRV_NAME ": "

#define BUS_PARAM_MCLR		0
#define BUS_PARAM_PGD		1
#define BUS_PARAM_PGC		2
#define BUS_PARAM_PGM		3

#define BUS_PARAM_REQUIRED	4
#define BUS_PARAM_COUNT		4

#define BUS_PARM_DESC " config -> mclr,pgd,pgc,pgm"

/*
 * Timing parameters
 */
static int P2B = 0; /* ndly_pgc_hold */
module_param(P2B, int, 0644);
MODULE_PARM_DESC(P2B, "DS39592F P2B TsclkH 40..400ns (default:0ns)");

static int P2A = 0; /* ndly_pgc_low_hold */
module_param(P2A, int, 0644);
MODULE_PARM_DESC(P2A, "DS39592F P2A TsclkL 40..400ns (default:0ns)");

/*
 * Pull-up parameter
 */
static int PU = 1; /* pgd_pullup */
module_param(PU, int, 0644);
MODULE_PARM_DESC(PU, "PGD level before input. 0=low, 1=high (default:1)");

static unsigned int mc_icsp[BUS_PARAM_COUNT] __initdata;
static unsigned int bus_nump __initdata;
static struct mc_icsp_gpio_platform_data pdata;

module_param_array(mc_icsp, uint, &bus_nump, 0);
MODULE_PARM_DESC(mc_icsp, "mc_icsp" BUS_PARM_DESC);

static struct platform_device *pdev;

static void set_pgc(void *data, unsigned int state) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        gpio_set_value(pdata->pgc_pin, state);
}

static void set_pgd(void *data, unsigned int state) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        gpio_set_value(pdata->pgd_pin, state);
}

static void set_pgd_input(void *data) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        gpio_direction_input(pdata->pgd_pin);
}

static void set_pgd_output(void *data, unsigned int state) {
        struct mc_icsp_gpio_platform_data *pdata = data;
	gpio_direction_output(pdata->pgd_pin, state);
}

static void set_pgm(void *data, unsigned int state) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        gpio_set_value(pdata->pgm_pin, state);
}

static void set_mclr(void *data, unsigned int state) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        gpio_set_value(pdata->mclr_pin, state);
}

static int get_pgd(void *data) {
        struct mc_icsp_gpio_platform_data *pdata = data;
        return gpio_get_value(pdata->pgd_pin);
}

static void mc_icsp_gpio_custom_cleanup(void) {
	gpio_free(pdata.mclr_pin);
	gpio_free(pdata.pgd_pin);
	gpio_free(pdata.pgc_pin);
	gpio_free(pdata.pgm_pin);
	platform_device_unregister(pdev);
}

static int __init mc_icsp_gpio_custom_add(unsigned int id, unsigned int *params)
{
	struct mc_icsp_platform_data mc_icsp_pdata;

	int err;
	
	if (bus_nump < BUS_PARAM_REQUIRED) {
		printk(KERN_ERR PFX "not enough parameters for ICSP%d (mc_icsp=1,2,3,4)\n", id);
		err = -EINVAL;
		goto err;
	}

	printk(KERN_INFO PFX " requesting GPIOs\n");

	err = gpio_request_one(params[BUS_PARAM_MCLR], GPIOF_OUT_INIT_LOW, "MC ICSP MCLR");
        if (err) {
		printk(KERN_ERR PFX " didn't get GPIO %d for MCLR\n", params[BUS_PARAM_MCLR] );
		goto err;
        }

	err = gpio_request_one(params[BUS_PARAM_PGD], GPIOF_IN, "MC ICSP PGD");
        if (err) {
		printk(KERN_ERR PFX " didn't get GPIO %d for PGD\n", params[BUS_PARAM_PGD] );
		goto err_get_pgd;
        }

	err = gpio_request_one(params[BUS_PARAM_PGC], GPIOF_OUT_INIT_LOW, "MC ICSP PGC");
        if (err) {
		printk(KERN_ERR PFX " didn't get GPIO %d for PGC\n", params[BUS_PARAM_PGC] );
		goto err_get_pgc;
        }

	err = gpio_request_one(params[BUS_PARAM_PGM], GPIOF_OUT_INIT_LOW, "MC ICSP PGM");
        if (err) {
		printk(KERN_ERR PFX " didn't get GPIO %d for PGM\n", params[BUS_PARAM_PGM] );
		goto err_get_pgm;
        }

	printk(KERN_INFO PFX "MCLR %d PGD %d PGC %d PGM %d\n", \
               params[BUS_PARAM_MCLR],params[BUS_PARAM_PGD],params[BUS_PARAM_PGC],params[BUS_PARAM_PGM]);

	pdata.mclr_pin = params[BUS_PARAM_MCLR];
	pdata.pgd_pin  = params[BUS_PARAM_PGD];
	pdata.pgc_pin  = params[BUS_PARAM_PGC];
	pdata.pgm_pin  = params[BUS_PARAM_PGM];

	mc_icsp_pdata.data = &pdata;
	mc_icsp_pdata.set_pgc = set_pgc;
	mc_icsp_pdata.set_pgd = set_pgd;
	mc_icsp_pdata.set_pgd_input = set_pgd_input;
	mc_icsp_pdata.set_pgd_output = set_pgd_output;
	mc_icsp_pdata.set_pgm = set_pgm;
	mc_icsp_pdata.set_mclr = set_mclr;
	mc_icsp_pdata.get_pgd = get_pgd;
	mc_icsp_pdata.open = NULL;
	mc_icsp_pdata.release = NULL;
	mc_icsp_pdata.ndly_pgc_hold = P2B;
	mc_icsp_pdata.ndly_pgc_low_hold = P2A;
	mc_icsp_pdata.pgd_pullup = PU;

	pdev = platform_device_alloc("mc-icsp", 0);
	if (!pdev) {
		printk(KERN_ERR PFX " can't alloc MC ICSP device\n");
		err = -ENOMEM;
		goto err_alloc;
	}

	err = platform_device_add_data(pdev, &mc_icsp_pdata, sizeof(mc_icsp_pdata));
	if (err) goto err_put;

	err = platform_device_add(pdev);
        if (err) goto err_put;

	printk(KERN_INFO PFX "init done.\n");

	return 0;

err_put:
	platform_device_put(pdev);
err_alloc:
	gpio_free(params[BUS_PARAM_PGM]);
err_get_pgm:
	gpio_free(params[BUS_PARAM_PGC]);
err_get_pgc:
	gpio_free(params[BUS_PARAM_PGD]);
err_get_pgd:
	gpio_free(params[BUS_PARAM_MCLR]);
err:
	return err;
}

static int __init mc_icsp_gpio_custom_probe(void)
{
	int err;

	printk(KERN_INFO DRV_DESC " version " DRV_VERSION "\n");

	err = mc_icsp_gpio_custom_add(0, mc_icsp);
	if (err) goto err;

	return 0;

err:
	mc_icsp_gpio_custom_cleanup();
	return err;
}



#ifdef MODULE
static int __init mc_icsp_gpio_custom_init(void)
{
	return mc_icsp_gpio_custom_probe();
}
module_init(mc_icsp_gpio_custom_init);

static void __exit mc_icsp_gpio_custom_exit(void)
{
	mc_icsp_gpio_custom_cleanup();
}
module_exit(mc_icsp_gpio_custom_exit);
#else
subsys_initcall(mc_icsp_gpio_custom_probe);
#endif /* MODULE*/

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Gerhard Bertelsmann <info@gerhard-bertelsmann.de");
MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
