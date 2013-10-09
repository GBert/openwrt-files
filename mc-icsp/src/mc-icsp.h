/*
 * Microchip ICSP driver header
 *
 * Author: Eran Duchan <pavius@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef LINUX_MC_ICSP_H
#define LINUX_MC_ICSP_H

/* device name (platform and device) */
#define MC_ICSP_DEVICE_NAME     ("mc-icsp")

/* magic number for ioctls */
#define MC_ICSP_IOC_MAGIC       (0x67)

/* 
 * For IOC_TX/IOC_RX
 */

/* encode a 4 bit command and 16 bit data into 32 bits */
#define MC_ICSP_ENCODE_XFER(command, data, encoded_command)                 \
    encoded_command = (data | ((command & 0xF) << 16))

/* decode 4 bit command and 16 bit data from 32 bits */
#define MC_ICSP_DECODE_XFER(encoded_command, command, data)                 \
    data = (encoded_command & 0xFFFF);                                      \
    command = ((encoded_command >> 16) & 0xF)

/* 
 * For IOC_CMD_ONLY
 */

/* send only a command */
struct mc_icsp_cmd_only_t
{
    unsigned char command;
    unsigned char pgc_value_after_cmd;
    unsigned char pgd_value_after_cmd;
    unsigned int  msleep;
    unsigned int  mdelay;
    unsigned int  udelay;
};

/* IOCTL types */
#define MC_ICSP_IOC_TX             _IO(MC_ICSP_IOC_MAGIC, 1)
#define MC_ICSP_IOC_RX             _IOR(MC_ICSP_IOC_MAGIC, 2, unsigned int *)
#define MC_ICSP_IOC_CMD_ONLY       _IOW(MC_ICSP_IOC_MAGIC, 3, struct mc_icsp_cmd_only_t *)
#define MC_ICSP_IOC_MCLR_LOW	   _IO(MC_ICSP_IOC_MAGIC, 4)
#define MC_ICSP_IOC_MCLR_HIGH	   _IO(MC_ICSP_IOC_MAGIC, 5)
#define MC_ICSP_IOC_DATA_ONLY_16   _IO(MC_ICSP_IOC_MAGIC, 6)
#define MC_ICSP_IOC_DATA_ONLY_24   _IO(MC_ICSP_IOC_MAGIC, 7)
#define MC_ICSP_IOC_DATA_ONLY_32   _IO(MC_ICSP_IOC_MAGIC, 8)

/* IO direction */
#define MC_ICSP_IO_DIR_OUTPUT (0)
#define MC_ICSP_IO_DIR_INPUT (1)

/* Platform-dependent data for Microchip ICSP */
struct mc_icsp_platform_data
{
    void *data;
    void (*set_pgc) (void *data, unsigned int state);
    void (*set_pgd) (void *data, unsigned int state);
    void (*set_pgd_dir) (void *data, unsigned int dir);
    void (*set_pgm) (void *data, unsigned int state);
    void (*set_mclr) (void *data, unsigned int state);
    int  (*get_pgd) (void *data);
    int  (*open) (void *data);
    int  (*release) (void *data);
    int  udly_pgd_val_to_clk_rise;
    int  ndly_pgc_hold;
    int  ndly_pgc_low_hold;
    int  udly_cmd_to_data;
    int  udly_pgm_to_mclr;
};

#endif /* LINUX_MC_ICSP_H */
