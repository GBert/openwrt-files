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
 * STK500V2 sender
 *
 *****************************************************************************/

/* I/O timeout and I/O address mode */
extern int stk_timeout, stk_addr_mode;

/* I/O buffer, Sign-on signature and Packet sequence number */
extern uint8_t stk_msg[], stk_signon_msg[11], stk_seqnum;

/* I/O data row address */
extern uint32_t stk_addr;

/******************************************************************************
 * SEND COMMAND
 */
static int
stk500v2_cmd(struct k8048 *k, uint16_t msgsize)
{
	int16_t rc;
	int r = STK_RETRY;

	while (1) {
		rc = stk500v2_send(k, msgsize);
		if (rc < 0)
			break;
		rc = stk500v2_recv(k, STK_SEQTEST);
		if (--r && rc == -ETIMEDOUT) {
			/* The MCT232 driver on Linux can eat packets... */
			printf("%s: command timed out, resending ...\n", __func__);
			continue;
		}
		break;
	}
	return rc;
}

/******************************************************************************
 * SEND COMMAND SIGN-ON
 */
static inline int16_t
stk500v2_signon(struct k8048 *k)
{
	int16_t rc;

	stk_msg[0] = STK_CMD_SIGN_ON;

	rc = stk500v2_cmd(k, 1);

	if (rc < 0)
		return rc;
	if (rc != sizeof(stk_signon_msg))
		return -1;
	if (memcmp((void *)stk_msg, (void *)stk_signon_msg, sizeof(stk_signon_msg)) != 0)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND CHIP ERASE
 */
static inline int16_t
stk500v2_chip_erase_isp(struct k8048 *k)
{
	int16_t rc;

	stk_msg[0] = STK_CMD_CHIP_ERASE_ISP;
	stk_msg[1] = 0;	/* eraseDelay */
	stk_msg[2] = 0;	/* pollMethod */
	stk_msg[3] = 0;	/* cmd1       */
	stk_msg[4] = 0;	/* cmd2       */
	stk_msg[5] = 0;	/* cmd3       */
	stk_msg[6] = 0;	/* cmd4       */

	rc = stk500v2_cmd(k, 7);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND ENTER PROGMODE ISP
 */
static inline int16_t
stk500v2_enter_progmode_isp(struct k8048 *k)
{
	int16_t rc;

	stk_msg[0] = STK_CMD_ENTER_PROGMODE_ISP;;
	stk_msg[1] = 0;	/* timeout     */
	stk_msg[2] = 0;	/* stabDelay   */
	stk_msg[3] = 0;	/* cmdexeDelay */
	stk_msg[4] = 0;	/* synchLoops  */
	stk_msg[5] = 0;	/* byteDelay   */
	stk_msg[6] = 0;	/* pollValue   */
	stk_msg[7] = 0;	/* pollIndex   */
	stk_msg[8] = 0;	/* cmd1       */
	stk_msg[9] = 0;	/* cmd2       */
	stk_msg[10] = 0;/* cmd3       */
	stk_msg[11] = 0;/* cmd4       */

	rc = stk500v2_cmd(k, 12);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND LOAD ADDRESS
 */
static inline int16_t
stk500v2_load_address(struct k8048 *k, uint32_t address)
{
	int16_t rc;

	if (stk_addr == address)
		return 0;

	stk_addr = address;

	stk_msg[0] = STK_CMD_LOAD_ADDRESS;
	if (stk_addr_mode == 'a') {	/* AVR */
		stk_msg[1] = 0;
		stk_msg[2] = 0;
		stk_msg[3] = (address & 0xFFFF) >> 9;
		stk_msg[4] = (address & 0xFFFF) >> 1;
	} else { 			/* NATIVE */
		stk_msg[1] = address >> 24;
		stk_msg[2] = address >> 16;
		stk_msg[3] = address >> 8;
		stk_msg[4] = address;
	}

	rc = stk500v2_cmd(k, 5);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND PROGRAM FLASH ISP
 */
static int16_t
stk500v2_program_flash_isp(struct k8048 *k, uint8_t *panel, uint16_t panel_size)
{
	int16_t rc;

	stk_addr += panel_size;

	stk_msg[0] = STK_CMD_PROGRAM_FLASH_ISP;
	stk_msg[1] = panel_size >> 8;
	stk_msg[2] = panel_size;
	stk_msg[3] = 1;	/* mode  */
	stk_msg[4] = 0;	/* delay */
	stk_msg[5] = 0;	/* cmd1  */
	stk_msg[6] = 0;	/* cmd2  */
	stk_msg[7] = 0;	/* cmd3  */
	stk_msg[8] = 0;	/* poll1 */
	stk_msg[9] = 0;	/* poll2 */
	memcpy((void *)&stk_msg[10], (void *)panel, panel_size);

	rc = stk500v2_cmd(k, 10 + panel_size);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND READ FLASH ISP
 */
static int16_t
stk500v2_read_flash_isp(struct k8048 *k, uint16_t panel_size)
{
	int16_t rc;

	stk_addr += panel_size;

	stk_msg[0] = STK_CMD_READ_FLASH_ISP;
	stk_msg[1] = panel_size >> 8;
	stk_msg[2] = panel_size;
	stk_msg[3] = 0;	/* cmd1  */

	rc = stk500v2_cmd(k, 4);

	if (rc < 0)
		return rc;
	if (rc != (3 + panel_size))
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMAND LEAVE PROGMODE ISP
 */
static inline int16_t
stk500v2_leave_progmode_isp(struct k8048 *k)
{
	int16_t rc;

	stk_msg[0] = STK_CMD_LEAVE_PROGMODE_ISP;;
	stk_msg[1] = 0;	/* preDelay  */
	stk_msg[2] = 0;	/* postDelay */

	rc = stk500v2_cmd(k, 3);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * PROCESS PANEL
 */
int
stk500v2_panel(struct k8048 *k, int prog_mode, uint32_t panel_address,
	uint8_t *panel, uint16_t panel_size, uint16_t *panel_count, uint32_t *fail)
{
	int rc, i;

	if (*panel_count == 0) {
		return 0;
	}
	rc = stk500v2_load_address(k, panel_address);
	if (rc < 0) {
		printf("%s: failed to load address\n", __func__);
		return rc;
	}
	if (prog_mode == 'p'/* rogram */) {
		rc = stk500v2_program_flash_isp(k, panel, panel_size);
		if (rc < 0) {
			printf("%s: failed to program flash\n", __func__);
			return rc;
		}
	} else { /* 'v' erify */
		rc = stk500v2_read_flash_isp(k, panel_size);
		if (rc < 0) {
			printf("%s: failed to read flash\n", __func__);
			return rc;
		}
		for (i = 0; i < panel_size; ++i) {
			if (panel[i] != stk_msg[2 + i]) {
				printf("%s: error: read [%02X] expected [%02X] at [%08X]\n",
					__func__, stk_msg[2 + i], panel[i], panel_address + i);
				(*fail)++;
			}
		}
	}
	/* Reset panel */
	*panel_count = 0;
	memset((void *)panel, -1, panel_size);

	return 0;
}

/******************************************************************************
 * PROGRAM OR VERIFY FILE
 */
int
stk500v2_load(struct k8048 *k, int prog_mode, char *interface, char *filename, int addr_mode)
{
	int rc;
	uint32_t count = 0, fail = 0, nbytes, i, j;
	pic_data **pdata = NULL;

	/* Panel */
	uint8_t panel[PIC_BYTLEN];
	uint16_t panel_size, panel_count;
	uint32_t panel_address, boundary, mask, address, new_address;

	/* Connect */
	rc = stk500v2_open(k, interface, 8048, STK_CONNECT);
	if (rc < 0) {
		printf("%s: failed to open interface\n", __func__);
		return rc;
	}
	stk_seqnum = 0;
	stk_timeout = 3;

	/* Get HEX */
	nbytes = inhx32_array_create(k, &pdata, filename, &count);
	if (nbytes == 0) {
		stk500v2_close(k, interface);
		return -1;
	}

	/* Sign-on */
	rc = stk500v2_signon(k);
	if (rc < 0) {
		printf("%s: failed to sign-on\n", __func__);
		goto error;
	}

	/* Erase */
	if (prog_mode == 'p' && addr_mode != 'a') {
		/* Enter programming mode */
		rc = stk500v2_enter_progmode_isp(k);
		if (rc < 0) {
			printf("%s: failed to enter programming mode\n", __func__);
			goto error;
		}
		
		/* Chip erase */
		rc = stk500v2_chip_erase_isp(k);
		if (rc < 0) {
			printf("%s: failed to erase chip\n", __func__);
			goto error;
		}
	}

	/* Enter programming mode */
	rc = stk500v2_enter_progmode_isp(k);
	if (rc < 0) {
		printf("%s: failed to re-enter programming mode\n", __func__);
		goto error;
	}

	/* Init target */
	stk_addr = UINT32_MAX;
	stk_addr_mode = addr_mode;

	/* Init panel */
	panel_address = UINT32_MAX;
	panel_size = PIC_BYTLEN;
	boundary = 0 - panel_size;
	mask = panel_size - 1;

	/* Reset panel */
	panel_count = 0;
	memset((void *)panel, -1, panel_size);

	/* For each line */
	for (i = 0; i < count; ++i) {
		/* For each byte in line */
		for (j = 0; j < pdata[i]->nbytes; ++j) {
			address = pdata[i]->address + j;
			new_address = address & boundary;
			if (new_address != panel_address) {
				rc = stk500v2_panel(k, prog_mode, panel_address,
					panel, panel_size, &panel_count, &fail);
				if (rc < 0)
					goto error;
				panel_address = new_address;
			}
			panel[address & mask] = pdata[i]->bytes[j];
			panel_count++;
		}
	}
	rc = stk500v2_panel(k, prog_mode, panel_address,
		panel, panel_size, &panel_count, &fail);
	if (rc < 0)
		goto error;

	/* Exit programming mode */
	rc = stk500v2_leave_progmode_isp(k);
	if (rc < 0) {
		printf("%s: failed to leave programming mode\n", __func__);
		goto error;
	}

	if (prog_mode == 'p')
		printf("Total: %u\n", nbytes);
	else
		printf("Total: %u Fail: %u\n", nbytes, fail);
error:
	/* Clean up */
	inhx32_array_free(k, pdata, count);
	
	/* Close interface */
	stk500v2_close(k, interface);

	return rc;
}
