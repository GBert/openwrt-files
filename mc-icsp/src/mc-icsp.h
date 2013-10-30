/*
 * Microchip ICSP driver header
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
#define MC_ICSP_ENCODE_XFER(command, data, encoded_command) \
    encoded_command = (data | ((command & 0xF) << 16))

/* decode 4 bit command and 16 bit data from 32 bits */
#define MC_ICSP_DECODE_XFER(encoded_command, command, data) \
    data = (encoded_command & 0xFFFF);                      \
    command = ((encoded_command >> 16) & 0xF)

/*
 * For IOC_CMD_ONLY
 */

/* PIC18 send only a command */
struct mc_icsp_cmd_only_t
{
    unsigned char command;
    unsigned char pgc_value_after_cmd;
    unsigned char pgd_value_after_cmd;
    unsigned int  mdelay;
    unsigned int  udelay;
    unsigned int  msleep;
};

/*
 * IOCTL types
 */

/* PIC18 */
#define MC_ICSP_IOC_TX          _IO(MC_ICSP_IOC_MAGIC,  1)
#define MC_ICSP_IOC_RX          _IOR(MC_ICSP_IOC_MAGIC, 2, unsigned int *)
#define MC_ICSP_IOC_CMD_ONLY    _IOW(MC_ICSP_IOC_MAGIC, 3, struct mc_icsp_cmd_only_t *)
#define MC_ICSP_IOC_DATA_ONLY   _IO(MC_ICSP_IOC_MAGIC,  4)

/* BASELINE / MID-RANGE / ENHANCED MID-RANGE */
#define MC_ICSP_IOC_CMD_6       _IO(MC_ICSP_IOC_MAGIC, 10)

/* SET BIT */
#define MC_ICSP_IOC_SET_PGC     _IO(MC_ICSP_IOC_MAGIC, 11)
#define MC_ICSP_IOC_SET_PGD     _IO(MC_ICSP_IOC_MAGIC, 12)
#define MC_ICSP_IOC_SET_PGM     _IO(MC_ICSP_IOC_MAGIC, 14)
#define MC_ICSP_IOC_SET_MCLR    _IO(MC_ICSP_IOC_MAGIC, 15)

/* GET BIT */
#define MC_ICSP_IOC_GET_PGD     _IOR(MC_ICSP_IOC_MAGIC, 16, unsigned char *)

/* SEND BITS */
#define MC_ICSP_IOC_SEND_32     _IO(MC_ICSP_IOC_MAGIC, 17)

/* READ BITS */
#define MC_ICSP_IOC_READ_16     _IOR(MC_ICSP_IOC_MAGIC, 18, unsigned short *)

/* CLOCK BIT IN/OUT */
#define MC_ICSP_IOC_CLK_OUT     _IO(MC_ICSP_IOC_MAGIC, 19)
#define MC_ICSP_IOC_CLK_IN      _IOR(MC_ICSP_IOC_MAGIC, 20, unsigned char *)

/* SET DATA DIRECTION IN/OUT */
#define MC_ICSP_IOC_SET_PGD_INPUT  _IO(MC_ICSP_IOC_MAGIC, 21)
#define MC_ICSP_IOC_SET_PGD_OUTPUT _IO(MC_ICSP_IOC_MAGIC, 22)

/* Platform-dependent data for Microchip ICSP */
struct mc_icsp_platform_data
{
    void *data;
    void (*set_pgc) (void *data, unsigned int state);
    void (*set_pgd) (void *data, unsigned int state);
    void (*set_pgd_input) (void *data);
    void (*set_pgd_output) (void *data, unsigned int state);
    void (*set_pgm) (void *data, unsigned int state);
    void (*set_mclr) (void *data, unsigned int state);
    int  (*get_pgd) (void *data);
    int  (*open) (void *data);
    int  (*release) (void *data);
    int  ndly_pgc_hold;
    int  ndly_pgc_low_hold;
    int  pgd_pullup;
};

#endif /* LINUX_MC_ICSP_H */

/*
 * vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
 */
