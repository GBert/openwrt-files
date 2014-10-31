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

#ifndef _STK500V2_H
#define _STK500V2_H

/* stk500v2 max message size */
#define STK_LEN (275)

#define STK_MESSAGE_START (0x1B)
#define STK_TOKEN	  (0x0E)

#define STK_CMD_SIGN_ON	       	   (0x01)
#define STK_CMD_SET_PARAMETER      (0x02)
#define STK_CMD_GET_PARAMETER      (0x03)
#define STK_CMD_LOAD_ADDRESS       (0x06)
#define STK_CMD_ENTER_PROGMODE_ISP (0x10)
#define STK_CMD_LEAVE_PROGMODE_ISP (0x11)
#define STK_CMD_CHIP_ERASE_ISP     (0x12)
#define STK_CMD_PROGRAM_FLASH_ISP  (0x13)
#define STK_CMD_READ_FLASH_ISP     (0x14)
#define STK_CMD_SPI_MULTI	   (0x1D)

#define STK_STATUS_CMD_OK	     (0x00)
#define STK_STATUS_CMD_TOUT	     (0x80)
#define STK_STATUS_RDY_BSY_TOUT	     (0x81)
#define STK_STATUS_SET_PARAM_MISSING (0x82)
#define STK_STATUS_CMD_FAILED        (0xC0)
#define STK_STATUS_CKSUM_ERROR	     (0xC1)

/* Our protocol error code */
#define STK_PROTOCOL_ERROR (-200)

/* Receiver echos sequence number received */
#define STK_SEQECHO (0)
/* Sender validates sequence number received */
#define STK_SEQTEST (1)

#define STK_TIMEOUT (5)
#define STK_RETRY   (10)

/* our stk500v2 get/set params */
#define STK_PARAM_ARCH    (0xF0) /* TODO set ARCH     */
#define STK_PARAM_ROWSIZE (0xF1) /* TODO get row size */

#define STK_PROGMODE_NONE  (0) /* current programming mode */
#define STK_PROGMODE_WRITE (1) /* program */
#define STK_PROGMODE_READ  (2) /* verify  */

#define STK_LISTEN (1) 
#define STK_CONNECT (2) 
int16_t stk500v2_open(struct k8048 *, const char *, int, int);
void stk500v2_close(struct k8048 *, const char *);
int16_t stk500v2_recv(struct k8048 *, int);
int16_t stk500v2_send(struct k8048 *, uint16_t);

#endif /* !_STK500V2_H */
