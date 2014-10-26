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

/******************************************************************************
 *
 * STK500v2 emulator
 *
 * Only one instance of this emulator will exist.
 *
 *****************************************************************************/

/* Serial port descriptor */
static int stk_port;

/* I/O buffer */
static uint8_t msg[STKLEN + /* Checksum */ 1];

/* Packet sequence number */
static uint8_t seqnum = 0;

/* I/O timeout */
static int timeout = 5;

/* I/O data row address */
static uint32_t addr = 0;

/* I/O address memory region */
static uint32_t region = PIC_REGIONNOTSUP;

/* I/O address mode (0=native, 1=avr) */
static int addr_mode = 0;

/* Device ready for programming, else in standby */
static uint8_t online = 0;

/* Sign-on signature */
static uint8_t signon_msg[11] = {
	CMD_SIGN_ON, STATUS_CMD_OK,
	/* SIG LENGTH   */ 0x08,
	/* SIG STK500_2 */ 0x53,0x54,0x4B,0x35,0x30,0x30,0x5F,0x32
};

/*****************************************************************************
 *
 * STK500v2 Protocol I/O
 *
 *****************************************************************************/

/*****************************************************************************
 *
 * AVR068: STK500 Communication Protocol
 *
 *  Receive Packet
 *
 * Message Format & Protocol Layer State Table
 *
 * 2591C-AVR-06/06 Pages 2 & 3
 *
 *****************************************************************************/
int16_t
stk500v2_recv(struct k8048 *k, int flag)
{
	int rc;
	uint8_t sum = 0;
	uint16_t msgsize, i;

	/* Get message start */
	rc = serial_get(stk_port, (char *)msg, 1, timeout);
	if (rc < 0) {
		return rc;
	}

	/* Validate message start */
	if (msg[0] != MESSAGE_START) {
		return STK500_PROTOCOL_ERROR;
	}

	/* Message start checksum */
	sum ^= msg[0];

	/* Get sequence number */
	rc = serial_get(stk_port, (char *)msg, 1, timeout);
	if (rc < 0) {
		return rc;
	}

	/* Echo sequence number */
	if (flag == SEQECHO) {
		seqnum = msg[0];
	}
	/* Validate sequence number */
	else if (flag == SEQTEST) {
		if (msg[0] != seqnum) {
			return STK500_PROTOCOL_ERROR;
		}
		seqnum++;
	}

	/* Sequence number checksum */
	sum ^= msg[0];

	/* Get message size 1/2 */
	rc = serial_read(stk_port, (char *)msg, 2, timeout);
	if (rc < 0) {
		return rc;
	}

	/* Validate message size */
	msgsize = (msg[0] << 8) | msg[1];
	if (msgsize > STKLEN) {
		return STK500_PROTOCOL_ERROR;
	}

	/* Message size checksum */
	sum ^= msg[0];
	sum ^= msg[1];
	
	/* Get token */
	rc = serial_get(stk_port, (char *)msg, 1, timeout);
	if (rc < 0)
		return rc;

	/* Validate token */
	if (msg[0] != TOKEN) {
		return STK500_PROTOCOL_ERROR;
	}

	/* Token checksum */
	sum ^= msg[0];

	/* Get data & checksum */
	rc = serial_read(stk_port, (char *)msg, ++msgsize, timeout);
	if (rc < 0) {
		return rc;
	}
	msgsize--;

	/* Message checksum */
	for (i = 0; i < msgsize; ++i)
		sum ^= msg[i];

	/* Validate packet checksum */
	if (sum != msg[msgsize]) {
		return STK500_PROTOCOL_ERROR;
	}

	/* Return with number of bytes in message */
	return msgsize;
}

/******************************************************************************
 *
 * AVR068: STK500 Communication Protocol
 *
 *  Send Packet
 *
 * Message Format & General Commands
 *
 * 2591C-AVR-06/06 Pages 2 & 4
 *
 *****************************************************************************/
int16_t
stk500v2_send(struct k8048 *k, uint16_t msgsize)
{
	int rc;
	uint8_t header[5], sum = 0;
	uint16_t i;

	/* Send header */
	header[0] = MESSAGE_START;
	header[1] = seqnum;
	header[2] = msgsize >> 8;
	header[3] = msgsize & 0xFF;
	header[4] = TOKEN;
	rc = serial_write(stk_port, (char *)header, 5, timeout);
	if (rc < 0)
		return rc;
	if (rc != 5)
		return -1;

	/* Header checksum */
	for (i = 0; i < 5; ++i)
		sum ^= header[i];

	/* Message checksum */
	for (i = 0; i < msgsize; ++i)
		sum ^= msg[i];

	/* Packet checksum */
	msg[msgsize] = sum;

	/* Send message & Checksum */
	rc = serial_write(stk_port, (char *)msg, ++msgsize, timeout);
	if (rc < 0)
		return rc;
	if (rc != msgsize--)
		return -1;

	/* Return with number of bytes in message */
	return msgsize;
}

/*****************************************************************************
 *
 * Device
 *
 *****************************************************************************/

/******************************************************************************
 * Begin programming
 */
static inline void
stk500v2_program_begin(struct k8048 *k)
{
	if (!online) {
		if (pic_read_config(k) == 0) {
			pic_bulk_erase(k);
			pic_program_begin(k);
			region = PIC_REGIONNOTSUP;
			online = 1;
		}
	}
}

/******************************************************************************
 * Program data
 */
static inline void
stk500v2_program_data(struct k8048 *k, pic_data *pdata)
{
	if (online) {
		region = pic_program_data(k, region, pdata);
	}
}

/******************************************************************************
 * End programming
 */
static inline void
stk500v2_program_end(struct k8048 *k)
{
	if (online) {
		pic_program_end(k, 1);
		online = 0;
	}
}

/*****************************************************************************
 *
 * Programmer protocol
 *
 *****************************************************************************/

/******************************************************************************
 * RESPOND WITH COMMAND OKAY
 */
static inline void
stk500v2_recv_ok(struct k8048 *k)
{
	msg[1] = STATUS_CMD_OK;
	stk500v2_send(k, 2);
}

/******************************************************************************
 * RESPOND WITH COMMAND FAILED
 */
static inline void
stk500v2_recv_failed(struct k8048 *k)
{
	msg[1] = STATUS_CMD_FAILED;
	stk500v2_send(k, 2);
}

/******************************************************************************
 * SEND COMMAND
 */
static int
stk500v2_send_cmd(struct k8048 *k, uint16_t msgsize)
{
	int16_t rc;
	int r = RETRY;

	while (1) {
		rc = stk500v2_send(k, msgsize);
		if (rc < 0)
			break;
		rc = stk500v2_recv(k, SEQTEST);
		if (--r && rc == -ETIMEDOUT) {
			/* The MCT232 driver on Linux can eat packets... */
			printf("%s: reply timed out, resending ...\n", __func__);
			io_usleep(k, 10000);
			continue;
		}
		break;
	}
	return rc;
}

/******************************************************************************
 * RESPOND TO COMMAND SIGN-ON
 */
static inline void
stk500v2_recv_signon(struct k8048 *k)
{
	memcpy((void *)msg, (void *)signon_msg, sizeof(signon_msg));
	stk500v2_send(k, sizeof(signon_msg));
}

/******************************************************************************
 * SEND COMMAND SIGN-ON
 */
static inline int16_t
stk500v2_send_signon(struct k8048 *k)
{
	int16_t rc;

	msg[0] = CMD_SIGN_ON;

	rc = stk500v2_send_cmd(k, 1);

	if (rc < 0)
		return rc;
	if (rc != sizeof(signon_msg))
		return -1;
	if (memcmp((void *)msg, (void *)signon_msg, sizeof(signon_msg)) != 0)
		return -1;

	return 0;
}

/******************************************************************************
 * RESPOND TO COMMAND SET PARAMETER
 */
static inline void
stk500v2_recv_set_parameter(struct k8048 *k)
{
	stk500v2_recv_ok(k);
}

/******************************************************************************
 * RESPOND TO COMMAND GET PARAMETER
 */
static inline void
stk500v2_recv_get_parameter(struct k8048 *k)
{
	msg[1] = STATUS_CMD_OK;
	msg[2] = /* Value */ 0;
	stk500v2_send(k, 3);
}

/******************************************************************************
 * RESPOND TO COMMAND SPI MULTI
 */
static inline void
stk500v2_recv_spi_multi(struct k8048 *k)
{
	if (msg[1] == 4 && msg[2] == 4 && msg[3] == 0) {
		msg[1] = STATUS_CMD_OK;
		msg[2] = 0;
		msg[3] = 0;
		if (msg[4] == 0x30) {
			/* Signature query */
			msg[4] = 0;
			switch (msg[6]) {
			case 0: msg[5] = 0x50; /* chipKIT PIC32 [0] */
				break;
			case 1: msg[5] = 0x49; /* chipKIT PIC32 [1] */
				break;
			case 2: msg[5] = 0x43; /* chipKIT PIC32 [2] */
				break;
			}
		} else {
			msg[4] = 0;
			msg[5] = 0;
		}
		msg[6] = STATUS_CMD_OK;
		stk500v2_send(k, 7);
	} else {
		stk500v2_recv_failed(k);
	}
}

/******************************************************************************
 * RESPOND TO COMMAND CHIP ERASE ISP
 */
static inline void
stk500v2_recv_chip_erase_isp(struct k8048 *k)
{
	stk500v2_recv_ok(k);
}

/******************************************************************************
 * SEND COMMAND CHIP ERASE
 */
static inline int16_t
stk500v2_send_chip_erase_isp(struct k8048 *k)
{
	int16_t rc;

	msg[0] = CMD_CHIP_ERASE_ISP;
	msg[1] = 0;	/* eraseDelay */
	msg[2] = 0;	/* pollMethod */
	msg[3] = 0;	/* cmd1       */
	msg[4] = 0;	/* cmd2       */
	msg[5] = 0;	/* cmd3       */
	msg[6] = 0;	/* cmd4       */

	rc = stk500v2_send_cmd(k, 7);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (msg[1] != STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * RESPOND TO COMMAND ENTER PROGMODE ISP
 */
static inline void
stk500v2_recv_enter_progmode_isp(struct k8048 *k)
{
	stk500v2_recv_ok(k);
}

/******************************************************************************
 * SEND COMMAND ENTER PROGMODE ISP
 */
static inline int16_t
stk500v2_send_enter_progmode_isp(struct k8048 *k)
{
	int16_t rc;

	msg[0] = CMD_ENTER_PROGMODE_ISP;;
	msg[1] = 0;	/* timeout     */
	msg[2] = 0;	/* stabDelay   */
	msg[3] = 0;	/* cmdexeDelay */
	msg[4] = 0;	/* synchLoops  */
	msg[5] = 0;	/* byteDelay   */
	msg[6] = 0;	/* pollValue   */
	msg[7] = 0;	/* pollIndex   */
	msg[8] = 0;	/* cmd1       */
	msg[9] = 0;	/* cmd2       */
	msg[10] = 0;	/* cmd3       */
	msg[11] = 0;	/* cmd4       */

	rc = stk500v2_send_cmd(k, 12);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (msg[1] != STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * RESPOND TO COMMAND LOAD ADDRESS
 */
static inline void
stk500v2_recv_load_address(struct k8048 *k)
{
	if (addr_mode) {/* AVR */
		addr = msg[1] << 25 | msg[2] << 17 | msg[3] << 9 | msg[4] << 1;
	} else {	/* NATIVE */
		addr = msg[1] << 24 | msg[2] << 16 | msg[3] << 8 | msg[4];
	}
	stk500v2_recv_ok(k);
}

/******************************************************************************
 * SEND COMMAND LOAD ADDRESS
 */
static inline int16_t
stk500v2_send_load_address(struct k8048 *k, uint32_t address)
{
	int16_t rc;

	if (addr == address)
		return 0;

	addr = address;

	msg[0] = CMD_LOAD_ADDRESS;
	if (addr_mode) {/* AVR */
		msg[1] = 0;
		msg[2] = 0;
		msg[3] = (address & 0xFFFF) >> 9;
		msg[4] = (address & 0xFFFF) >> 1;
	} else { 	/* NATIVE */
		msg[1] = address >> 24;
		msg[2] = address >> 16;
		msg[3] = address >> 8;
		msg[4] = address;
	}

	rc = stk500v2_send_cmd(k, 5);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (msg[1] != STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * RESPOND TO COMMAND PROGRAM FLASH ISP
 */
static inline void
stk500v2_recv_program_flash_isp(struct k8048 *k)
{
	pic_data pdata;

	stk500v2_program_begin(k);

	pdata.address = addr;
	pdata.nbytes = msg[1] << 8 | msg[2];
	if (pdata.nbytes <= PIC_BYTLEN) {
		memcpy((void *)pdata.bytes, (void *)&msg[10], pdata.nbytes);
		stk500v2_program_data(k, &pdata);
	}
	addr += pdata.nbytes;

	stk500v2_recv_ok(k);
}

/******************************************************************************
 * SEND COMMAND PROGRAM FLASH ISP
 */
static int16_t
stk500v2_send_program_flash_isp(struct k8048 *k, uint8_t *panel, uint16_t panel_size)
{
	int16_t rc;

	addr += panel_size;

	msg[0] = CMD_PROGRAM_FLASH_ISP;
	msg[1] = panel_size >> 8;
	msg[2] = panel_size;
	msg[3] = 0x01;	/* mode  */
	msg[4] = 0;	/* delay */
	msg[5] = 0;	/* cmd1  */
	msg[6] = 0;	/* cmd2  */
	msg[7] = 0;	/* cmd3  */
	msg[8] = 0;	/* poll1 */
	msg[9] = 0;	/* poll2 */
	memcpy((void *)&msg[10], (void *)panel, panel_size);

	rc = stk500v2_send_cmd(k, 10 + panel_size);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (msg[1] != STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * RESPOND TO COMMAND READ FLASH ISP
 *  
 *  TODO
 */
static inline void
stk500v2_recv_read_flash_isp(struct k8048 *k)
{
	uint16_t size = msg[1] << 8 | msg[2];

	msg[1] = STATUS_CMD_OK;
	memset((void *)&msg[2], -1, size * sizeof(uint8_t));
	msg[2 + size] = STATUS_CMD_OK;
	addr += size;
	stk500v2_send(k, 3 + size);
}

/******************************************************************************
 * RESPOND TO COMMAND LEAVE PROGMODE ISP
 */
static inline void
stk500v2_recv_leave_progmode_isp(struct k8048 *k)
{
	stk500v2_program_end(k);
	stk500v2_recv_ok(k);
}

/******************************************************************************
 * SEND COMAND LEAVE PROGMODE ISP
 */
static inline int16_t
stk500v2_send_leave_progmode_isp(struct k8048 *k)
{
	int16_t rc;

	msg[0] = CMD_LEAVE_PROGMODE_ISP;;
	msg[1] = 0;	/* preDelay  */
	msg[2] = 0;	/* postDelay */

	rc = stk500v2_send_cmd(k, 3);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (msg[1] != STATUS_CMD_OK)
		return -1;

	return 0;
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/******************************************************************************
 *
 * Open device and receive file for programming
 *
 * May return on I/O error (USB device removed?)
 *
 *****************************************************************************/
int
stk500v2_recvfile(struct k8048 *k, char *stk_device)
{
	int rc;
	
	/* Open serial I/O */
	rc = serial_open(stk_device, serial_speed(k->baudrate));
	if (rc < 0) {
		printf("%s: failed to open serial device\n", __func__);
		return rc;
	}
	stk_port = rc;

	/* Init */
	k->f = NULL;
	addr_mode = 0;

	while (1) {
		rc = stk500v2_recv(k, SEQECHO);
		if (rc == -EIO) {
			break;
		}
		if (rc == -ETIMEDOUT) {
			continue;
		}
		if (rc == STK500_PROTOCOL_ERROR) {
			continue;
		}
		if (rc == /* Empty message */ 0) {
			continue;
		}
		switch (msg[0]) {
		default:stk500v2_recv_failed(k);
			break;
		case CMD_SIGN_ON:
			stk500v2_recv_signon(k);
			break;
		case CMD_SET_PARAMETER:
			stk500v2_recv_set_parameter(k);
			break;
		case CMD_GET_PARAMETER:
			stk500v2_recv_get_parameter(k);
			break;
		case CMD_SPI_MULTI:
			stk500v2_recv_spi_multi(k);
			break;
		case CMD_CHIP_ERASE_ISP:
			stk500v2_recv_chip_erase_isp(k);
			break;
		case CMD_ENTER_PROGMODE_ISP:
			stk500v2_recv_enter_progmode_isp(k);
			break;
		case CMD_LOAD_ADDRESS:
			stk500v2_recv_load_address(k);
			break;
		case CMD_PROGRAM_FLASH_ISP:
			stk500v2_recv_program_flash_isp(k);
			break;
		case CMD_READ_FLASH_ISP:
			stk500v2_recv_read_flash_isp(k);
			break;
		case CMD_LEAVE_PROGMODE_ISP:
			stk500v2_recv_leave_progmode_isp(k);
			continue;
		}
	}
	stk500v2_program_end(k);
	serial_close(stk_port);

	return rc;
}

/******************************************************************************
 *
 * Open device and send file for programming
 *
 *****************************************************************************/
int
stk500v2_sendfile(struct k8048 *k, char *stk_device, char *filename, int mode)
{
	int rc;
	uint32_t count = 0, nbytes, i, j;
	pic_data **pdata = NULL;

	/* Panel */
	uint8_t panel[PIC_BYTLEN];
	uint16_t panel_size, write_pending;
	uint32_t panel_address, boundary, mask, address, new_address;

	/* Open serial I/O */
	rc = serial_open(stk_device, serial_speed(k->baudrate));
	if (rc < 0) {
		printf("%s: failed to open serial device\n", __func__);
		return rc;
	}
	stk_port = rc;
	timeout = 3;

	/* Get HEX */
	nbytes = inhx32_array_create(k, &pdata, filename, &count);
	if (nbytes == 0) {
		serial_close(stk_port);
		return -1;
	}

	/* Sign-on */
	rc = stk500v2_send_signon(k);
	if (rc < 0) {
		printf("%s: failed to sign-on\n", __func__);
		goto error;
	}

	/* Enter programming mode */
	rc = stk500v2_send_enter_progmode_isp(k);
	if (rc < 0) {
		printf("%s: failed to enter programming mode\n", __func__);
		goto error;
	}

	/* Chip erase */
	rc = stk500v2_send_chip_erase_isp(k);
	if (rc < 0) {
		printf("%s: failed to erase chip\n", __func__);
		goto error;
	}

	/* Re-enter programming mode */
	rc = stk500v2_send_enter_progmode_isp(k);
	if (rc < 0) {
		printf("%s: failed to re-enter programming mode\n", __func__);
		goto error;
	}

	/* Init target */
	addr = UINT32_MAX;
	addr_mode = mode;

	/* Init panel */
	panel_address = UINT32_MAX;
	panel_size = PIC_BYTLEN;
	boundary = 0 - panel_size;
	mask = panel_size - 1;
	write_pending = 0;
	memset((void *)panel, -1, sizeof(uint8_t) * panel_size);

	/* For each line */
	for (i = 0; i < count; ++i) {
		/* For each byte in line */
		for (j = 0; j < pdata[i]->nbytes; ++j) {
			address = pdata[i]->address + j;
			new_address = address & boundary;
			if (new_address != panel_address) {
				if (write_pending) {
					write_pending = 0;
					rc = stk500v2_send_load_address(k, panel_address);
					if (rc < 0) {
						printf("%s: failed to load address\n", __func__);
						goto error;
					}
					rc = stk500v2_send_program_flash_isp(k, panel, panel_size);
					if (rc < 0) {
						printf("%s: failed to program flash\n", __func__);
						goto error;
					}
					memset((void *)panel, -1, sizeof(uint8_t) * panel_size);
				}
				panel_address = new_address;
			}
			panel[address & mask] = pdata[i]->bytes[j];
			write_pending++;
		}
	}
	if (write_pending) {
		rc = stk500v2_send_load_address(k, panel_address);
		if (rc < 0) {
			printf("%s: failed to load address\n", __func__);
			goto error;
		}
		rc = stk500v2_send_program_flash_isp(k, panel, panel_size);
		if (rc < 0) {
			printf("%s: failed to program flash\n", __func__);
			goto error;
		}
	}

	/* Exit programming mode */
	rc = stk500v2_send_leave_progmode_isp(k);
	if (rc < 0) {
		printf("%s: failed to leave programming mode\n", __func__);
		goto error;
	}

	printf("Total: %u\n", nbytes);
error:
	/* Clean up */
	inhx32_array_free(k, pdata, count);
	
	/* Close serial */
	serial_close(stk_port);

	return rc;
}
