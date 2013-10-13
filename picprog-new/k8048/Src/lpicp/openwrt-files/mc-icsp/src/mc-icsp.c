/*
 * Microchip ICSP bitbanging driver
 *
 * Original Author: Eran Duchan <pavius@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Copyright (C) 2013 Gerhard Bertelsmann & Darron M Broad
 *  Refactored.
 *  IOCTLs added for baseline/mid-range/enhanced mid-range support and bit I/O.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include "mc-icsp.h"
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>

/* debugging */
static int debug;
module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "enable debugging (default:0)");
#define dprintk(args...) \
    do { \
        if (debug) \
            printk(KERN_INFO "mc_icsp: " args); \
    } while (0)

/* major number */
#define MC_ICSP_MAJOR_NUMBER (245)

/* holds the current ICSP platform */
static struct mc_icsp_platform_data *mc_icsp_platform = NULL;

/* bitbang macros that call platform callback */
#define mc_icsp_io_pgc_set_hi() mc_icsp_platform->set_pgc(mc_icsp_platform->data, 1)
#define mc_icsp_io_pgc_set_lo() mc_icsp_platform->set_pgc(mc_icsp_platform->data, 0)
#define mc_icsp_io_pgd_set_hi() mc_icsp_platform->set_pgd(mc_icsp_platform->data, 1)
#define mc_icsp_io_pgd_set_lo() mc_icsp_platform->set_pgd(mc_icsp_platform->data, 0)
#define mc_icsp_io_pgd_get()    mc_icsp_platform->get_pgd(mc_icsp_platform->data)

/* delay if not zero */
#define mc_icsp_ndelay(dly)     do {if (dly) ndelay(dly);} while(0)
#define mc_icsp_udelay(dly)     do {if (dly) udelay(dly);} while(0)

/* clock out a bit with a value of 1 */
#define mc_icsp_io_output_one(cfg)    do {          \
    mc_icsp_io_pgd_set_hi();                        \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_hold);             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_low_hold);         \
    } while (0)

/* clock out a bit with a value of 0 */
#define mc_icsp_io_output_zero(cfg) do {            \
    mc_icsp_io_pgd_set_lo();                        \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_hold);             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_low_hold);         \
    } while (0)

/* read a bit */
#define mc_icsp_io_input(input_value, cfg) do {     \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_hold);             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_ndelay(cfg->ndly_pgc_low_hold);         \
    input_value = mc_icsp_io_pgd_get();             \
    } while (0)

/* read 1 to 32 bits */
void mc_icsp_rx_bits(unsigned int *bits, const unsigned int nbits)
{
    unsigned int current_bit_mask = 1, current_bit, bits_left = nbits;

    dprintk("%s(,%d)\n", __func__, nbits);

    /* make sure that pgd goes down before pumping in the data */
    mc_icsp_io_pgd_set_lo();

    /* start read */
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_INPUT);

    /* reset bits */
    *bits = 0;

    /* pump in bits */
    while (bits_left--)
    {
        /* read a bit (1 if set, 0 otherwise) */
        mc_icsp_io_input(current_bit, mc_icsp_platform);

        /* is the current bit set? */
        if (current_bit) {
            /* set in data */
            (*bits) |= current_bit_mask;
        }
        current_bit_mask <<= 1;
    }

    /* end read */
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_OUTPUT);

    dprintk("%s()=0x%08X\n", __func__, *bits);
}

/* send 1 to 32 bits */
void mc_icsp_tx_bits(const unsigned int bits, const unsigned int nbits)
{
    unsigned int current_bit_mask = 1, bits_left = nbits;

    dprintk("%s(0x%08X, %d)\n", __func__, bits, nbits);

    /* pump out bits */
    while (bits_left--)
    {
        if (bits & current_bit_mask)
            mc_icsp_io_output_one(mc_icsp_platform);
        else
            mc_icsp_io_output_zero(mc_icsp_platform);

        current_bit_mask <<= 1;
    }
}

/* send a command only */
void mc_icsp_command_only(struct mc_icsp_cmd_only_t *cmd_config)
{
    /* 3 clocks outputting 0 */
    mc_icsp_io_output_zero(mc_icsp_platform);
    mc_icsp_io_output_zero(mc_icsp_platform);
    mc_icsp_io_output_zero(mc_icsp_platform);

    /* now hold pgc hi */
    if (cmd_config->pgc_value_after_cmd)
    {
        /* set high */
        mc_icsp_io_pgc_set_hi();
    }
    else
    {
        /* output another clock */
        mc_icsp_io_output_zero(mc_icsp_platform);
    }

    /* delay P9/P9A + P5 or P10 + P11 */
    if (cmd_config->msleep) msleep(cmd_config->msleep);
    if (cmd_config->mdelay) mdelay(cmd_config->mdelay);
    if (cmd_config->udelay) udelay(cmd_config->udelay);

    /* make sure pgc is low */
    if (cmd_config->pgc_value_after_cmd)
    {
        mc_icsp_io_pgc_set_lo();
    }
}

/**
 * ICSP device routines
 */

long mc_icsp_device_ioctl(struct file *filep, unsigned int cmd, unsigned long data)
{
    int err;
    unsigned int xfer_data = 0;
    unsigned short xfer_word = 0;
    unsigned char xfer_byte = 0;

    dprintk("%s(0x%p,0x%08X,0x%lX)\n", __func__, filep, cmd, data);

    /* check if initialized */
    if (mc_icsp_platform == NULL) {
        printk("  MC error : mc_icsp_platform == NULL)\n");
        return -ENODEV;
    }

    /* init locals */
    err = 0;

    /* Check type and command number */
    if (_IOC_TYPE(cmd) != MC_ICSP_IOC_MAGIC)
        return -ENOTTY;

    /* check access for all commands */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void __user *)data, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void __user *)data, _IOC_SIZE(cmd));

    /* access ok? */
    if (err) {
        printk("  MC error : _IOC_DIR or _IOC_READ\n");
        return -EFAULT;
    }

    /* by command type */
    switch (cmd)
    {

    /*********************************************************************
      PIC18          00000000 0000CCCC DDDDDDDD DDDDDDDD C=COMMAND D=DATA
     *********************************************************************/

    /* transmit 16 bits, user data just holds 12 bits 0, 4 bits cmd, 16 bits data */
    case MC_ICSP_IOC_TX:
    {
        /* send the command */    
        mc_icsp_tx_bits(data >> 16, 4);

        /* wait a bit */
        mc_icsp_udelay(mc_icsp_platform->udly_cmd_to_data);

        /* send the 16 bit data */
        mc_icsp_tx_bits(data, 16);
    }
    break;

    /* receive 16 bits */
    case MC_ICSP_IOC_RX:
    {
        /* get data holding the command id */
        if (__get_user(xfer_data, (unsigned int __user *)data) == 0)
        {
            /* send the command */
            mc_icsp_tx_bits(xfer_data >> 16, 4);

            /* wait a bit */
            mc_icsp_udelay(mc_icsp_platform->udly_cmd_to_data);

            /* receive the 16 bit data */
            mc_icsp_rx_bits(&xfer_data, 16);

            /* return the data to the user */
            __put_user(xfer_data, (unsigned int __user *)data);
        }
    }
    break;

    /* only send a command, not data. used for program & erase */
    case MC_ICSP_IOC_CMD_ONLY:
    {
        struct mc_icsp_cmd_only_t cmd_config;

        /* copy configuration from userspace */
        if (copy_from_user(&cmd_config, (unsigned int __user *)data,
            sizeof(cmd_config)) == 0)
        {
            /* do operation */
            mc_icsp_command_only(&cmd_config);
        }
    }
    break;

    /* only send data */
    case MC_ICSP_IOC_DATA_ONLY:
    {
        /* send the data requested by the user */
        mc_icsp_tx_bits(data, 16);
    }
    break;

    /*********************************************************************
      BASELINE / MID-RANGE / ENHANCED MID-RANGE
     *********************************************************************/

    case MC_ICSP_IOC_CMD_6:
    {
        mc_icsp_tx_bits(data, 6);
    }
    break;

    /*********************************************************************
      SET BIT
     *********************************************************************/

    case MC_ICSP_IOC_SET_PGC:
    {
        mc_icsp_platform->set_pgc(mc_icsp_platform->data, data);
    }
    break;

    case MC_ICSP_IOC_SET_PGD:
    {
        mc_icsp_platform->set_pgd(mc_icsp_platform->data, data);
    }
    break;

    case MC_ICSP_IOC_SET_PGD_DIR:
    {
        mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, data);
    }
    break;

    case MC_ICSP_IOC_SET_PGM:
    {
        mc_icsp_platform->set_pgm(mc_icsp_platform->data, data);
    }
    break;

    case MC_ICSP_IOC_SET_MCLR:
    {
        mc_icsp_platform->set_mclr(mc_icsp_platform->data, data);
    }
    break;

    /*********************************************************************
      GET BIT
     *********************************************************************/

    case MC_ICSP_IOC_GET_PGD:
    {
        if (__get_user(xfer_byte, (unsigned char __user *)data) == 0)
        {
            xfer_byte = mc_icsp_platform->get_pgd(mc_icsp_platform->data);
            __put_user(xfer_byte, (unsigned char __user *)data);
        }
    }
    break;

    /*********************************************************************
     SEND BITS
     *********************************************************************/

    case MC_ICSP_IOC_SEND_32:
    {
        mc_icsp_tx_bits(data, 32);
    }
    break;

    /*********************************************************************
     READ BITS
     *********************************************************************/

    case MC_ICSP_IOC_READ_16:
    {
        if (__get_user(xfer_word, (unsigned short __user *)data) == 0)
        {
            mc_icsp_rx_bits(&xfer_data, 16);
            xfer_word = xfer_data;
            __put_user(xfer_word, (unsigned short __user *)data);
        }
    }
    break;

    /*********************************************************************
     CLOCK BIT IN/OUT
     *********************************************************************/

    case MC_ICSP_IOC_CLK_OUT:
    {
        if (data)
            mc_icsp_io_output_one(mc_icsp_platform);
        else
            mc_icsp_io_output_zero(mc_icsp_platform);
    }
    break;

    case MC_ICSP_IOC_CLK_IN:
    {
        if (__get_user(xfer_byte, (unsigned char __user *)data) == 0)
        {
            mc_icsp_io_input(xfer_byte, mc_icsp_platform);
            __put_user(xfer_byte, (unsigned char __user *)data);
        }
    }
    break;

    }

    /* done */
    return 0;
}

/* file open */
static int mc_icsp_device_open(struct inode *inode, struct file *icsp_file)
{
    /* check if initialized */
    if (mc_icsp_platform == NULL) return -ENODEV;

    /* call open callback */
    if (mc_icsp_platform->open != NULL) mc_icsp_platform->open(mc_icsp_platform->data);

    /* set pgd to output */
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_OUTPUT);

    /* done */
    return 0;
}

/* file close */
int mc_icsp_device_release(struct inode *inode, struct file *icsp_file)
{
    /* check if initialized */
    if (mc_icsp_platform == NULL) return -ENODEV;

    /* call close callback */
    if (mc_icsp_platform->release != NULL) mc_icsp_platform->release(mc_icsp_platform->data);

    /* done */
    return 0;
}

/* dev driver data */
static const struct file_operations mc_icsp_device_fops = 
{
    .owner              = THIS_MODULE,
    .open               = mc_icsp_device_open,
    .release            = mc_icsp_device_release,
    .unlocked_ioctl     = mc_icsp_device_ioctl
};

/** 
 * ICSP platform routines
 */ 

static int mc_icsp_platform_probe(struct platform_device *pdev)
{
    /* check platform data structure passed */
    if (pdev->dev.platform_data == NULL)
        return -ENXIO;

    /* save platform data (must be static) */
    mc_icsp_platform = pdev->dev.platform_data;

    return 0;
}

static int mc_icsp_platform_remove(struct platform_device *pdev)
{
    return 0;
}

/* platform driver data */
static struct platform_driver mc_icsp_platform_driver = 
{
    .driver    = 
    {
        .name     = MC_ICSP_DEVICE_NAME,
        .owner    = THIS_MODULE,
    },
    .probe        = mc_icsp_platform_probe,
    .remove       = mc_icsp_platform_remove,
};

/* driver init */
static int __init mc_icsp_platform_init(void)
{
    int ret;

    /* register the platform driver */
    ret = platform_driver_register(&mc_icsp_platform_driver);

    /* check if registered successfully */
    if (ret)
    {
        /* error */
        printk(KERN_ERR "Microchip ICSP: probe failed: %d\n", ret);

        /* cleanup */
        goto err_plat_drv_register;
    }
    printk("Microchip ICSP: probe success (platform_driver_register)\n");

    /* register character device */
    if (register_chrdev(MC_ICSP_MAJOR_NUMBER, MC_ICSP_DEVICE_NAME, &mc_icsp_device_fops)) 
    {
        /* log */
        printk("Microchip ICSP: Unable to get major number (%d)\n", MC_ICSP_MAJOR_NUMBER);

        /* cleanup */
        goto err_chrdev_register;
    }

    printk("Microchip ICSP: Got major number (%d)\n", MC_ICSP_MAJOR_NUMBER);

    /* log */
    printk(KERN_INFO "Microchip ICSP driver initialized\n");

    /* success */
    return 0;

err_chrdev_register:
    platform_driver_unregister(&mc_icsp_platform_driver);
err_plat_drv_register:
    return ret;
}
subsys_initcall(mc_icsp_platform_init);

/* driver teardown */
static void __exit mc_icsp_platform_exit(void)
{
    /* unregister from platform */
    platform_driver_unregister(&mc_icsp_platform_driver);

    /* unregister char device */
    unregister_chrdev(MC_ICSP_MAJOR_NUMBER, MC_ICSP_DEVICE_NAME);
    printk("Microchip ICSP: removed\n");
}
module_exit(mc_icsp_platform_exit);

MODULE_AUTHOR("Eran Duchan <pavius@gmail.com>");
MODULE_DESCRIPTION("Microchip ICSP bitbanging driver");
MODULE_LICENSE("GPL");

/*
 * vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
 */
