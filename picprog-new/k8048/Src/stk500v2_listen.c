/*
 * Copyright (C) 2005-2014 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "k8048.h"

#undef DEBUG

#ifdef DEBUG
#define DPRINT(...) printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

/******************************************************************************
 *
 * STK500V2 listener
 *
 *****************************************************************************/

/* I/O timeout and I/O address mode */
extern int stk_timeout, stk_addr_mode;

/* I/O buffer and Sign-on signature */
extern uint8_t stk_msg[], stk_signon_msg[11];

/* I/O data row address */
extern uint32_t stk_addr;

/* Device ready for programming, else in standby */
static int stk_progmode;

/******************************************************************************
 * CLEAN UP DEAD SESSION
 */
static void
stk500v2_cleanup(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_WRITE)
		pic_program_end(k, 1);
	else if (stk_progmode == STK_PROGMODE_READ)
		pic_verify_end(k);

	stk_progmode = STK_PROGMODE_NONE;
}

/******************************************************************************
 * RESPOND WITH COMMAND OKAY
 */
static inline void
stk500v2_ok(struct k8048 *k)
{
	stk_msg[1] = STK_STATUS_CMD_OK;
	stk500v2_send(k, 2);
}

/******************************************************************************
 * RESPOND WITH COMMAND FAILED
 */
static inline void
stk500v2_failed(struct k8048 *k)
{
	stk_msg[1] = STK_STATUS_CMD_FAILED;
	stk500v2_send(k, 2);
}

/******************************************************************************
 * RESPOND TO COMMAND SIGN-ON
 */
static inline void
stk500v2_signon(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	memcpy((void *)stk_msg, (void *)stk_signon_msg, sizeof(stk_signon_msg));
	stk500v2_send(k, sizeof(stk_signon_msg));
}

/******************************************************************************
 * RESPOND TO COMMAND SET PARAMETER
 */
static inline void
stk500v2_set_parameter(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	stk500v2_ok(k);
}

/******************************************************************************
 * RESPOND TO COMMAND GET PARAMETER
 */
static inline void
stk500v2_get_parameter(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	stk_msg[1] = STK_STATUS_CMD_OK;
	stk_msg[2] = /* Value */ 0;
	stk500v2_send(k, 3);
}

/******************************************************************************
 * RESPOND TO COMMAND SPI MULTI
 */
static inline void
stk500v2_spi_multi(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	if (stk_msg[1] == 4 && stk_msg[2] == 4 && stk_msg[3] == 0) {
		stk_msg[1] = STK_STATUS_CMD_OK;
		stk_msg[2] = 0;
		stk_msg[3] = 0;
		if (stk_msg[4] == 0x30) {
			/* Signature query */
			stk_msg[4] = 0;
			switch (stk_msg[6]) {
			case 0: stk_msg[5] = 0x50; /* chipKIT PIC32 [0] */
				break;
			case 1: stk_msg[5] = 0x49; /* chipKIT PIC32 [1] */
				break;
			case 2: stk_msg[5] = 0x43; /* chipKIT PIC32 [2] */
				break;
			}
		} else {
			stk_msg[4] = 0;
			stk_msg[5] = 0;
		}
		stk_msg[6] = STK_STATUS_CMD_OK;
		stk500v2_send(k, 7);
	} else {
		stk500v2_failed(k);
	}
}

/******************************************************************************
 * RESPOND TO COMMAND CHIP ERASE ISP
 */
static inline void
stk500v2_chip_erase_isp(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup(k);

	if (pic_read_config(k) == 0) {
		pic_bulk_erase(k);
		stk500v2_ok(k);
		return;
	}
	stk500v2_failed(k);
}

/******************************************************************************
 * RESPOND TO COMMAND ENTER PROGMODE ISP
 */
static inline void
stk500v2_enter_progmode_isp(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup(k);

	if (pic_read_config(k) == 0) {
		stk500v2_ok(k);
		return;
	}
	stk500v2_failed(k);
}

/******************************************************************************
 * RESPOND TO COMMAND LOAD ADDRESS
 */
static inline void
stk500v2_load_address(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	if (stk_addr_mode == 'a') {	/* AVR (NOT IMPLEMENTED) */
		stk_addr = stk_msg[1] << 25 | stk_msg[2] << 17 | stk_msg[3] << 9 | stk_msg[4] << 1;
	} else {			/* NATIVE */
		stk_addr = stk_msg[1] << 24 | stk_msg[2] << 16 | stk_msg[3] << 8 | stk_msg[4];
	}
	stk500v2_ok(k);
}

/******************************************************************************
 * RESPOND TO COMMAND PROGRAM FLASH ISP
 */
static inline void
stk500v2_program_flash_isp(struct k8048 *k)
{
	static uint32_t region;
	pic_data pdata;

	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_READ) {
		pic_verify_end(k);
	}
	if (stk_progmode != STK_PROGMODE_WRITE) {
		if (pic_read_config(k) == 0) {
			pic_program_begin(k);
			stk_progmode = STK_PROGMODE_WRITE;
			region = PIC_REGIONNOTSUP;
		} else  {
			stk_progmode = STK_PROGMODE_NONE;
			stk500v2_failed(k);
			return;
		}
	}

	pdata.nbytes = stk_msg[1] << 8 | stk_msg[2];

	if (pdata.nbytes <= PIC_BYTLEN) {
		memcpy((void *)&pdata.bytes, (void *)&stk_msg[10], pdata.nbytes);
		pdata.address = stk_addr;
		stk_addr += pdata.nbytes;
		region = pic_program_data(k, region, &pdata);
		stk500v2_ok(k);
	} else  {
		stk500v2_failed(k);
	}
}

/******************************************************************************
 * RESPOND TO COMMAND READ FLASH ISP
 */
static inline void
stk500v2_read_flash_isp(struct k8048 *k)
{
	static uint32_t region;
	uint32_t fail;
	pic_data pdata;

	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_WRITE) {
		pic_program_end(k, 1);
	}
	if (stk_progmode != STK_PROGMODE_READ) {
		if (pic_read_config(k) == 0) {
			pic_verify_begin(k);
			stk_progmode = STK_PROGMODE_READ;
			region = PIC_REGIONNOTSUP;
		} else  {
			stk_progmode = STK_PROGMODE_NONE;
			stk500v2_failed(k);
			return;
		}
	}

	pdata.nbytes = stk_msg[1] << 8 | stk_msg[2];

	if (pdata.nbytes <= PIC_BYTLEN) {
        	memset((void *)pdata.bytes, -1, pdata.nbytes);
		pdata.address = stk_addr;
		stk_addr += pdata.nbytes;
		region = pic_verify_data(k, region, &pdata, &fail);
		stk_msg[1] = STK_STATUS_CMD_OK;
		memcpy((void *)&stk_msg[2], (void *)pdata.bytes, pdata.nbytes);
		stk_msg[2 + pdata.nbytes] = STK_STATUS_CMD_OK;
		stk500v2_send(k, 3 + pdata.nbytes);
	} else {
		stk500v2_failed(k);
	}
}

/******************************************************************************
 * RESPOND TO COMMAND LEAVE PROGMODE ISP
 */
static inline void
stk500v2_leave_progmode_isp(struct k8048 *k)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup(k);
	stk500v2_ok(k);
}

/******************************************************************************
 * LISTEN FOR REMOTE PROGRAMMING
 */
int
stk500v2_listen(struct k8048 *k, char *interface, int port)
{
	int rc;

	DPRINT("%s()\n", __func__);

	/* Listen */
	rc = stk500v2_open(k, interface, port, STK_LISTEN);
	if (rc < 0) {
		printf("%s: failed to open interface\n", __func__);
		return rc;
	}

	/* Init */
	k->f = NULL; /* No console messages */
	stk_addr = 0;
	stk_addr_mode = 'n';
	stk_timeout = STK_TIMEOUT;
	stk_progmode = STK_PROGMODE_NONE;

	while (1) {
		rc = stk500v2_recv(k, STK_SEQECHO);
		if (rc == -EIO) {
			stk500v2_cleanup(k);
			break;
		}
		if (rc == -ETIMEDOUT) {
			continue;
		}
		if (rc == STK_PROTOCOL_ERROR) {
			continue;
		}
		if (rc == /* Empty message */ 0) {
			continue;
		}
		if (stk_msg[0] == STK_CMD_LEAVE_PROGMODE_ISP) {
			stk500v2_leave_progmode_isp(k);
			break;
		}
		switch (stk_msg[0]) {
		default:stk500v2_failed(k);
			break;
		case STK_CMD_SIGN_ON:
			stk500v2_signon(k);
			break;
		case STK_CMD_SET_PARAMETER:
			stk500v2_set_parameter(k);
			break;
		case STK_CMD_GET_PARAMETER:
			stk500v2_get_parameter(k);
			break;
		case STK_CMD_SPI_MULTI:
			stk500v2_spi_multi(k);
			break;
		case STK_CMD_CHIP_ERASE_ISP:
			stk500v2_chip_erase_isp(k);
			break;
		case STK_CMD_ENTER_PROGMODE_ISP:
			stk500v2_enter_progmode_isp(k);
			break;
		case STK_CMD_LOAD_ADDRESS:
			stk500v2_load_address(k);
			break;
		case STK_CMD_PROGRAM_FLASH_ISP:
			stk500v2_program_flash_isp(k);
			break;
		case STK_CMD_READ_FLASH_ISP:
			stk500v2_read_flash_isp(k);
			break;
		}
	}
	stk500v2_close(k, interface);

	return rc;
}
