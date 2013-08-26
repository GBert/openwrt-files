/*
 * Microchip ICSP bitbanging driver
 *
 * Author: Eran Duchan <pavius@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
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

/* major number */
#define MC_ICSP_MAJOR_NUMBER (245)

/* holds the current ICSP platform */
static struct mc_icsp_platform_data *mc_icsp_platform = NULL;

/* icsp lock */
static DEFINE_SPINLOCK(mc_icsp_lock);

/* bitbang macros that call platform callback */
#define mc_icsp_io_pgc_set_hi() mc_icsp_platform->set_pgc(mc_icsp_platform->data, 1)
#define mc_icsp_io_pgc_set_lo() mc_icsp_platform->set_pgc(mc_icsp_platform->data, 0)
#define mc_icsp_io_pgd_set_hi() mc_icsp_platform->set_pgd(mc_icsp_platform->data, 1)
#define mc_icsp_io_pgd_set_lo() mc_icsp_platform->set_pgd(mc_icsp_platform->data, 0)
#define mc_icsp_io_pgd_get()     mc_icsp_platform->get_pgd(mc_icsp_platform->data)

/* delay if not zero */
#define mc_icsp_udelay(dly)     do {if (dly) udelay(dly);} while(0)

/* clock out a bit with a value of 1 */
#define mc_icsp_io_output_one(cfg)    do {          \
    mc_icsp_io_pgd_set_hi();                        \
    mc_icsp_udelay(cfg->udly_pgd_val_to_clk_rise);  \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_udelay(cfg->udly_pgc_hold);             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_udelay(cfg->udly_pgc_low_hold);         \
    } while (0)

/* clock out a bit with a value of 0 */
#define mc_icsp_io_output_zero(cfg) do {            \
    mc_icsp_io_pgd_set_lo();                        \
    mc_icsp_udelay(cfg->udly_pgd_val_to_clk_rise);  \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_udelay(cfg->udly_pgc_hold);             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_udelay(cfg->udly_pgc_low_hold);         \
    } while (0)

/* read a bit */
#define mc_icsp_io_input(input_value, cfg) do {     \
    mc_icsp_io_pgc_set_hi();                        \
    mc_icsp_udelay(cfg->udly_pgc_hold);             \
    input_value = mc_icsp_io_pgd_get();             \
    mc_icsp_io_pgc_set_lo();                        \
    mc_icsp_udelay(cfg->udly_pgc_low_hold);         \
    } while (0)

/* send the command (expected at bits 16 - 19) */

inline void mc_icsp_tx_command_4(const unsigned int xfer_cmd)
{
    unsigned int current_bit_mask, bits_left;
    unsigned long flags;

    /* lock interrupts */
    spin_lock_irqsave(&mc_icsp_lock, flags);

    /* pump out command */
    for (current_bit_mask = (1 << 16), bits_left = 4;
          bits_left; 
          --bits_left, current_bit_mask <<= 1)
    {
        if (xfer_cmd & current_bit_mask)     mc_icsp_io_output_one(mc_icsp_platform);
        else                                 mc_icsp_io_output_zero(mc_icsp_platform);
    }

    /* make sure that pgd goes down after pumping out the command */
    mc_icsp_io_pgd_set_lo();

    /* unlock interrupts */
    spin_unlock_irqrestore(&mc_icsp_lock, flags);
}

/* send a 4 bit command and read 16 bits */
void mc_icsp_rx_4_16(unsigned int *xfer_cmd_and_data)
{
    unsigned int read_bit_mask, current_bit, bits_left;
    unsigned long flags;
    
    /* send the command */    
    mc_icsp_tx_command_4(*xfer_cmd_and_data);
        
    /* wait a bit */
    mc_icsp_udelay(mc_icsp_platform->udly_cmd_to_data);
        
    /* start read */    
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_INPUT);
            
    /* lock interrupts */
    spin_lock_irqsave(&mc_icsp_lock, flags);

    /* pump in data */
    for (read_bit_mask = (1 << 0), bits_left = 16; 
          bits_left; 
          --bits_left, read_bit_mask <<= 1)
    {
        /* read a bit (1 if set, 0 otherwise) */
        mc_icsp_io_input(current_bit, mc_icsp_platform);

        /* is the current bit set? */
        if (current_bit)
        {
            /* set in data */
            *xfer_cmd_and_data |= read_bit_mask;
        }
    }

    /* unlock interrupts */
    spin_unlock_irqrestore(&mc_icsp_lock, flags);

    /* end read */    
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_OUTPUT);
}

/* send 16 bits of data */
void mc_icsp_tx_data_16(const unsigned int xfer_cmd_and_data)
{
    unsigned int current_bit_mask, bits_left;
    unsigned long flags;

    /* lock interrupts */
    spin_lock_irqsave(&mc_icsp_lock, flags);

    /* pump out data */
    for (current_bit_mask = (1 << 0), bits_left = 16; 
          bits_left; 
          --bits_left, current_bit_mask <<= 1)
    {
        if (xfer_cmd_and_data & current_bit_mask)     mc_icsp_io_output_one(mc_icsp_platform);
        else                                          mc_icsp_io_output_zero(mc_icsp_platform);
    }

    /* make sure that pgd goes down after pumping out the data */
    mc_icsp_io_pgd_set_lo();

    /* unlock interrupts */
    spin_unlock_irqrestore(&mc_icsp_lock, flags);
}

/* send a 4 bit command and send 16 bits */
void mc_icsp_tx_4_16(const unsigned int xfer_cmd_and_data)
{
    /* send the command */    
    mc_icsp_tx_command_4(xfer_cmd_and_data);
    
    /* wait a bit */
    mc_icsp_udelay(mc_icsp_platform->udly_cmd_to_data);

    /* send the data */
    mc_icsp_tx_data_16(xfer_cmd_and_data);
}

/* send a command only */
void mc_icsp_command_only(struct mc_icsp_cmd_only_t *cmd_config)
{
    unsigned long flags;

    /* lock interrupts */
    spin_lock_irqsave(&mc_icsp_lock, flags);

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

    /* unlock interrupts */
    spin_unlock_irqrestore(&mc_icsp_lock, flags);

    /* delay 2 * P9 (1ms) */
    if (cmd_config->mdelay) mdelay(cmd_config->mdelay);
    if (cmd_config->udelay) udelay(cmd_config->udelay);

    /* make sure pgc is low */
    mc_icsp_io_pgc_set_lo();
}

/** 
 * ICSP device routines
 */ 

long mc_icsp_device_ioctl(struct file *filep, unsigned int cmd, unsigned long data)
{
    int err;
    unsigned int xfer_data;

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
        /* transmit 16 bits, user data just holds 12 bits 0, 4 bits cmd, 16 bits data */
        case MC_ICSP_IOC_TX:
        {
            /* data holds 12 bits 0, 4 bits command, 16 bits data */    
            mc_icsp_tx_4_16(data);
        }
        break;

        /* receive 16 bits */
        case MC_ICSP_IOC_RX:
        {
            /* get data holding the command id */
            if (__get_user(xfer_data, (unsigned int __user *)data) == 0)
            {
                /* data holds 12 bits 0, 4 bits command, 16 bits data */    
                mc_icsp_rx_4_16(&xfer_data);

                /* return the data to the user */
                __put_user(xfer_data, (unsigned int __user *)data);
            }
        }
        break;

        /* only send a command, not data. used in several cases */
        case MC_ICSP_IOC_CMD_ONLY:
        {
            /* copy configuration from userspace */
            struct mc_icsp_cmd_only_t cmd_config;
                copy_from_user(&cmd_config, (unsigned int __user *)data, sizeof(cmd_config));

            /* do the transaction */
            mc_icsp_command_only(&cmd_config);
        }
        break;

        /* only send data */
        case MC_ICSP_IOC_DATA_ONLY:
        {
            /* send the data requested by the user */
            mc_icsp_tx_data_16(data);
        }
        break;
    }

    /* pull down lines after the transaction */
    mc_icsp_platform->set_pgc(mc_icsp_platform->data, 0);
    mc_icsp_platform->set_pgd(mc_icsp_platform->data, 0);

    /* done */
    return 0;
}

/* file open */
static int mc_icsp_device_open(struct inode *inode, struct file *icsp_file)
{
    /* check if initialized */
    if (mc_icsp_platform == NULL)
    {
        printk(KERN_ERR "    mc_icsp_platform not defined\n");
        return -ENODEV;
    }

    /* call open callback */
    
    if (mc_icsp_platform->open != NULL) mc_icsp_platform->open(mc_icsp_platform->data);

    /* set pgd to output */
    mc_icsp_platform->set_pgd_dir(mc_icsp_platform->data, MC_ICSP_IO_DIR_OUTPUT);

    /* pull down lines initially */
    mc_icsp_platform->set_pgc(mc_icsp_platform->data, 0);
    mc_icsp_platform->set_pgd(mc_icsp_platform->data, 0);

    /* enter LVP */
    mc_icsp_platform->set_pgm(mc_icsp_platform->data, 1);

    /* wait a bit */
    mc_icsp_udelay(mc_icsp_platform->udly_pgm_to_mclr);

    /* start MCLR */
    mc_icsp_platform->set_mclr(mc_icsp_platform->data, 1);
    
    /* done */
    return 0;
}

/* close driver */
int mc_icsp_device_release(struct inode *inode, struct file *icsp_file)
{
    /* check if initialized */
    if (mc_icsp_platform == NULL) return -ENODEV;

    /* pull down lines initially */
    mc_icsp_platform->set_pgc(mc_icsp_platform->data, 0);
    mc_icsp_platform->set_pgd(mc_icsp_platform->data, 0);

    /* exit LVP */
    mc_icsp_platform->set_pgm(mc_icsp_platform->data, 0);
    mc_icsp_platform->set_mclr(mc_icsp_platform->data, 0);

    /* call open callback */
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

