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
 * Copyright (C) 2013 Gerhard Bertelsmann & Darron M Broad
 *  Refactored.
 *  IOCTLs added for baseline/mid-range/enhanced mid-range support and bit I/O.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "lpicp_icsp.h"

/* open access to driver */
int lpp_icsp_init(struct lpp_context_t *context, char *icsp_dev_name)
{
#ifdef __linux
    /* open ICSP driver */
    context->icsp_dev_file = open(icsp_dev_name, O_RDWR);

    /* check */
    if (context->icsp_dev_file >= 0)
    {
        /* save name */
        context->icsp_dev_name = icsp_dev_name;
    }
    else
    {
        /* failed */
        goto err_icsp_open;
    }

    /* success */
    return 1;

err_icsp_open:
#endif
    return 0;
}

/* close access to driver */
int lpp_icsp_destroy(struct lpp_context_t *context)
{
    /* check if file exists */
    if (context->icsp_dev_file)
    {
        /* close the file */
        close(context->icsp_dev_file);
    }

    /* nullify */
    context->icsp_dev_file = -1;
    context->icsp_dev_name = NULL;

    /* success */
    return 1;
}

/*********************************************************************
  PIC18          00000000 0000CCCC DDDDDDDD DDDDDDDD C=COMMAND D=DATA
 *********************************************************************/

/* execute a command via ICSP driver */
int lpp_icsp_write_16(struct lpp_context_t *context, const unsigned char command,
    const unsigned short data)
{
    unsigned int xfer_command = 0;

    /* encode the xfer */
    MC_ICSP_ENCODE_XFER(command, data, xfer_command);

    /* tx */
    return (ioctl(context->icsp_dev_file, MC_ICSP_IOC_TX, xfer_command) == 0);
}

/* Read 8 bits via ICSP driver */
int lpp_icsp_read_8(struct lpp_context_t *context, const unsigned char command,
    unsigned char *data)
{
    unsigned int xfer_command = 0;
    int ret;

    /* encode the xfer */
    MC_ICSP_ENCODE_XFER(command, 0, xfer_command);

    /* rx */
    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_RX, &xfer_command) == 0);

    /* get LSB */
    *data = (xfer_command >> 8);

    return ret;
}

/* send only a command */
int lpp_icsp_command_only(struct lpp_context_t *context, 
    const struct mc_icsp_cmd_only_t *cmd_config)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_CMD_ONLY, cmd_config) == 0);

    return ret;
}

/* send only data */
int lpp_icsp_data_only(struct lpp_context_t *context, const unsigned int data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_DATA_ONLY, data) == 0);

    return ret;
}

/*********************************************************************
  BASELINE / MID-RANGE / ENHANCED MID-RANGE
 *********************************************************************/

int lpp_icsp_command_6(struct lpp_context_t *context, const unsigned char command)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_CMD_6, command) == 0);

    return ret;
}

/*********************************************************************
  SET BIT
 *********************************************************************/

int lpp_icsp_set_pgc(struct lpp_context_t *context, const unsigned char data) 
{
    int ret;

	ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SET_PGC, data) == 0);

    return ret;
}

int lpp_icsp_set_pgd(struct lpp_context_t *context, const unsigned char data) 
{
    int ret;

	ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SET_PGD, data) == 0);

    return ret;
}

int lpp_icsp_set_pgd_dir(struct lpp_context_t *context, const unsigned char data) 
{
    int ret;

	ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SET_PGD_DIR, data) == 0);

    return ret;
}

int lpp_icsp_set_pgm(struct lpp_context_t *context, const unsigned char data) 
{
    int ret;

	ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SET_PGM, data) == 0);

    return ret;
}

int lpp_icsp_set_mclr(struct lpp_context_t *context, const unsigned char data) 
{
    int ret;

	ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SET_MCLR, data) == 0);

    return ret;
}

/*********************************************************************
  GET BIT
 *********************************************************************/

int lpp_icsp_get_pgd(struct lpp_context_t *context, unsigned char *data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_GET_PGD, data) == 0);

    return ret;
}

/*********************************************************************
  SEND BITS
 *********************************************************************/

int lpp_icsp_send_32(struct lpp_context_t *context, const unsigned int data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_SEND_32, data) == 0);

    return ret;
}

/*********************************************************************
  READ BITS
 *********************************************************************/

int lpp_icsp_read_16(struct lpp_context_t *context, unsigned short *data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_READ_16, data) == 0);

    return ret;
}

/*********************************************************************
  CLOCK BIT IN/OUT
 *********************************************************************/

int lpp_icsp_clock_out(struct lpp_context_t *context, const unsigned char data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_CLK_OUT, data) == 0);

    return ret;
}

int lpp_icsp_clock_in(struct lpp_context_t *context, unsigned char *data)
{
    int ret;

    ret = (ioctl(context->icsp_dev_file, MC_ICSP_IOC_CLK_IN, data) == 0);

    return ret;
}

/*
 * vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
 */
