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

#ifndef _STK500_H
#define _STK500_H

/* stk500v2 max message size */
#define STKLEN (275)

#define MESSAGE_START (0x1B)
#define TOKEN	      (0x0E)

#define CMD_SIGN_ON	       (0x01)
#define CMD_SET_PARAMETER      (0x02)
#define CMD_GET_PARAMETER      (0x03)
#define CMD_LOAD_ADDRESS       (0x06)
#define CMD_ENTER_PROGMODE_ISP (0x10)
#define CMD_LEAVE_PROGMODE_ISP (0x11)
#define CMD_CHIP_ERASE_ISP     (0x12)
#define CMD_PROGRAM_FLASH_ISP  (0x13)
#define CMD_READ_FLASH_ISP     (0x14)
#define CMD_SPI_MULTI	       (0x1D)

#define STATUS_CMD_OK		 (0x00)
#define STATUS_CMD_TOUT		 (0x80)
#define STATUS_RDY_BSY_TOUT	 (0x81)
#define STATUS_SET_PARAM_MISSING (0x82)
#define STATUS_CMD_FAILED	 (0xC0)
#define STATUS_CKSUM_ERROR	 (0xC1)

/* our skt500v2 protocol error code */
#define STK500_PROTOCOL_ERROR (-200)

/* Receiver echos sequence number received */
#define SEQECHO (0)
/* Sender validates sequence number received */
#define SEQTEST (1)

#define TIMEOUT (10)
#define RETRY (10)

/* our stk500v2 get/set params */
#define STK500_PARAM_ARCH    (0xF0) /* TODO set ARCH     */
#define STK500_PARAM_ROWSIZE (0xF1) /* TODO get row size */

int stk500v2_recvfile(struct k8048 *, char *);
int stk500v2_sendfile(struct k8048 *, char *, char *, int);

#endif /* !_STK500_H */
