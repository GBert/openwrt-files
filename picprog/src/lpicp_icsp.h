/* 
 * Linux PIC Programmer (lpicp)
 * Microchip ICSP driver interface
 *
 * Author: Eran Duchan <pavius@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 */

#ifndef __LPICPC_ICSP_H
#define __LPICPC_ICSP_H


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
    unsigned int  mdelay;
    unsigned int  udelay;
};

/* IOCTL types */
#define MC_ICSP_IOC_TX             _IO(MC_ICSP_IOC_MAGIC, 1)
#define MC_ICSP_IOC_RX             _IOR(MC_ICSP_IOC_MAGIC, 2, unsigned int *)
#define MC_ICSP_IOC_CMD_ONLY       _IOW(MC_ICSP_IOC_MAGIC, 3, struct mc_icsp_cmd_only_t *)
#define MC_ICSP_IOC_DATA_ONLY_16   _IO(MC_ICSP_IOC_MAGIC, 4)
#define MC_ICSP_IOC_DATA_ONLY_24   _IO(MC_ICSP_IOC_MAGIC, 5)
#define MC_ICSP_IOC_DATA_ONLY_32   _IO(MC_ICSP_IOC_MAGIC, 6)

/* IO direction */
#define MC_ICSP_IO_DIR_OUTPUT (0)
#define MC_ICSP_IO_DIR_INPUT (1)

/* number of bits in command */
#define LPP_COMMAND_BIT_COUNT (4)

/* lpp context */
struct lpp_context_t
{
    char                        *icsp_dev_name;
    int                         icsp_dev_file;
};


/* open access to driver */
int lpp_icsp_init(struct lpp_context_t *context, char *icsp_dev_name);

/* close access to driver */
int lpp_icsp_destroy(struct lpp_context_t *context);

/* Write 16 bits via ICSP driver */
int lpp_icsp_write_16(struct lpp_context_t *context, 
                    const unsigned char command, 
                    const unsigned short data);

/* Read 8 bits via ICSP driver */
int lpp_icsp_read_8(struct lpp_context_t *context, 
                    const unsigned char command, 
                    unsigned char *data);

/* send only command */
int lpp_icsp_command_only(struct lpp_context_t *context, 
                          const struct mc_icsp_cmd_only_t *cmd_config);

/* set MCLR for key handling */
int lpp_icsp_mclr_set(struct lpp_context_t *context, 
                       const unsigned int data);

/* send only 16 bit data */
int lpp_icsp_data_only_16(struct lpp_context_t *context, 
                       const unsigned int data);

/* send only 24 bit data */
int lpp_icsp_data_only_24(struct lpp_context_t *context, 
                       const unsigned int data);

/* send only 32 bit data */
int lpp_icsp_data_only_32(struct lpp_context_t *context, 
                       const unsigned int data);

/* delay and return success */
int lpp_icsp_delay_us(struct lpp_context_t *context, const unsigned int delay_us);

#endif /* __LPICPC_ICSP_H */

