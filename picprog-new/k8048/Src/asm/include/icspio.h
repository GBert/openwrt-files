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

#ifndef _ICSPIO_H
#define _ICSPIO_H

/*
 * ICSP I/O
 */
void init_icspio(void);
int8_t icspio_getbyte(uint8_t *);
int8_t icspio_sendbyte(uint8_t);
uint8_t icspio_common(void);

/*
 * Commands
 */

/*
 * CONSTANT CMD_LED        = 0x01  ; 0x00..0x3F        SET K8048 LEDS
 * CONSTANT CMD_SWITCH     = 0x02  ;                   GET K8048 SWITCHES
 * CONSTANT CMD_SLEEP      = 0x10  ;                   SLEEP UNTIL WATCHDOG TIME-OUT
 * CONSTANT CMD_WATCHDOG   = 0x11  ; 1|0               SET WATCHDOG ENABLE/DISABLE
 * CONSTANT CMD_CLOCK      = 0x12  ; 0..7              SET INTERNAL RC CLOCK DIVIDER
 * CONSTANT CMD_DIRECTION  = 0x20  ; 0..5 0x00..0xFF   SET PORT DATA DIRECTION
 * CONSTANT CMD_OUTPUT     = 0x21  ; 0..5 0x00..0xFF   SET PORT DATA OUTPUT
 * CONSTANT CMD_INPUT      = 0x22  ; 0..5              GET PORT DATA INPUT
 * CONSTANT CMD_ANALOG     = 0x30  ; 0..N|0xFF         SET ANALOG CHANNEL ENABLE/DISABLE
 * CONSTANT CMD_SAMPLE     = 0x31  ;                   GET ANALOG CHANNEL INPUT
 * CONSTANT CMD_VREF       = 0x38  ; 0..15|0xFF        SET VREF VOLTAGE LEVEL OR DISABLE
 * CONSTANT CMD_EEREAD     = 0x40  ; ADDRESS           READ DATA EEPROM
 * CONSTANT CMD_EEWRITE    = 0x41  ; ADDRESS DATA      WRITE DATA EEPROM
 * CONSTANT CMD_READ       = 0x50  ; ADDRESH ADDRESL   READ PROGRAM FLASH
 * CONSTANT CMD_WRITE      = 0x51  ; 		       UNIMPLIMENTED
 * CONSTANT CMD_ARG8       = 0xF0  ; 0..0xFF           SEND AN 8-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG16      = 0xF1  ; 0..0xFFFF         SEND A 16-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG24      = 0xF2  ; 0..0xFFFFFF       SEND A 24-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_ARG32      = 0xF3  ; 0..0xFFFFFFFF     SEND A 32-BIT ARG & GET AN 8-BIT ANSWER
 * CONSTANT CMD_TEST       = 0xFE  ; 0..0xFF           SEND AN 8-BIT TEST ARG & GET NO REPLY
 * CONSTANT CMD_ERROR      = 0xFF  ;                   GET LAST FIRMWARE ERROR
 */
#define CMD_LED       (0x01)
#define CMD_SWITCH    (0x02)
#define CMD_SLEEP     (0x10)
#define CMD_WATCHDOG  (0x11)
#define CMD_CLOCK     (0x12)
#define CMD_DIRECTION (0x20)
#define CMD_OUTPUT    (0x21)
#define CMD_INPUT     (0x22)
#define CMD_ANALOG    (0x30)
#define CMD_SAMPLE    (0x31)
#define CMD_VREF      (0x38)
#define CMD_EEREAD    (0x40)
#define CMD_EEWRITE   (0x41)
#define CMD_READ      (0x50)
#define CMD_WRITE     (0x51)
#define CMD_ARG8      (0xF0)
#define CMD_ARG16     (0xF1)
#define CMD_ARG24     (0xF2)
#define CMD_ARG32     (0xF3)
#define CMD_TEST      (0xFE)
#define CMD_ERROR     (0xFF)

/* Command supported */
#define ACK (0x06)

/* Command not supported */
#define NAK (0x15)

/*
 * Errors
 */

/*
 * CONSTANT ERRNONE     = 0    ; OK
 * CONSTANT ERRTIMEOUT  = 1    ; CLOCK TIMED OUT
 * CONSTANT ERRPROTOCOL = 2    ; INVALID STOP OR START BIT
 * CONSTANT ERRPARITY   = 3    ; INVALID PARITY BIT
 * CONSTANT ERRNOTSUP   = 4    ; COMMAND NOT SUPPORTED
 * CONSTANT ERRINVALID  = 5    ; INVALID RESPONSE
 */
#define ERRNONE     (0)
#define ERRTIMEOUT  (1)
#define ERRPROTOCOL (2)
#define ERRPARITY   (3)
#define ERRNOTSUP   (4)
#define ERRINVALID  (5)

/* Clock Input Time-out Reload */
#define ICSPIO_TOLOAD (FCY / 32)

/* I/O Check-Sum */
uint8_t icspio_checksum;

/* I/O Error */
uint8_t icspio_err, icspio_lasterr;

/*
 * Init ICSP I/O
 */
void
init_icspio(void)
{
	/* ICSP I/O Data I/O (internal sink) */
	ICSPIO_LATDAT = 0;

	/* ICSP I/O Clock Input */
	ICSPIO_LATCLK = 0;

	/* ICSP I/O Data I/O */
	ICSPIO_TRISDAT = 1; /* I/P */

	/* ICSP I/O Clock Input */
	ICSPIO_TRISCLK = 1; /* I/P */
}

/*
 * Wait for ICSP I/O Clock High
 *
 * Return: 0=OK, -1=TIME-OUT
 */
static uint8_t
icspio_waithigh(void)
{
	uint32_t timeout = ICSPIO_TOLOAD;

	do {
		if (ICSPIO_PORTCLK)
			return 0;

		asm volatile ("NOP");
	}
	while (--timeout);

	return ERRTIMEOUT;
}

/*
 * Wait for ICSP I/O Clock Low
 *
 * Return: 0=OK, -1=TIME-OUT
 */
static uint8_t
icspio_waitlow(void)
{
	uint32_t timeout = ICSPIO_TOLOAD;

	do {
		if (!ICSPIO_PORTCLK)
			return 0;

		asm volatile ("NOP");
	}
	while (--timeout);

	return ERRTIMEOUT;
}

/*
 * Wait for ICSP I/O Clock Cycle
 *
 * Return: 0=OK, -1=TIME-OUT
 */
static uint8_t
icspio_waitclock(void)
{
	if (icspio_waithigh())
		return ERRTIMEOUT;

	if (icspio_waitlow())
		return ERRTIMEOUT;

	return ERRNONE;
}

/*
 * Pull-Up Data (external resistor)
 */
static inline void
icspio_trispu(void)
{
	ICSPIO_TRISDAT = 1; /* I/P */
}

/*
 * Pull-Down Data (internal sink)
 */
static inline void
icspio_trispd(void)
{
	ICSPIO_TRISDAT = 0; /* O/P */
}

/*
 * Get ICSP I/O Byte
 *
 * Return: 0=OK, -1=TIME-OUT or PROTOCOL ERROR
 */
int8_t
icspio_getbyte(uint8_t *buffer)
{
	uint8_t counter = 8, mask = 1, parity = 0;

	*buffer = 0;

	/* Re-Init ICSP I/O */
	init_icspio();

	/* Start bit clock */
	if (icspio_waitclock()) {
		return ERRTIMEOUT;
	}

	/* Start bit should be low */
	if (ICSPIO_PORTDAT) {
		return ERRPROTOCOL;
	}

	while (counter--) {
		/* Data bit clock */
		if (icspio_waitclock()) {
			return ERRTIMEOUT;
		}

		/* Data bit */
		if (ICSPIO_PORTDAT) {
			*buffer |= mask;
			parity ^= 1;
		}

		mask <<= 1;
	}

	/* Parity bit clock */
	if (icspio_waitclock()) {
		return ERRTIMEOUT;
	}

	/* Parity? */
	if (ICSPIO_PORTDAT != parity) {
		return ERRPARITY;
	}

	/* Stop bit clock */
	if (icspio_waitclock()) {
		return ERRTIMEOUT;
	}

	/* Stop bit should be high */
	if (!ICSPIO_PORTDAT) {
		return ERRPROTOCOL;
	}

	icspio_checksum += *buffer;

	return ERRNONE;
}

/*
 * Send ICSP I/O Byte
 *
 * Return: 0=OK, -1=TIME-OUT or PROTOCOL ERROR
 */
int8_t
icspio_sendbyte(uint8_t buffer)
{
	uint8_t counter = 8, mask = 1, parity = 0;

	icspio_checksum += buffer;

	/* Re-Init ICSP I/O */
	init_icspio();

	/* Start bit clock high */
	if (icspio_waithigh()) {
		return ERRTIMEOUT;
	}

	/* Start bit low */
	icspio_trispd();
	
	/* Start bit clock low */
	if (icspio_waitlow()) {
		return ERRTIMEOUT;
	}

	while (counter--) {
		/* Data bit clock high */
		if (icspio_waithigh()) {
			return ERRTIMEOUT;
		}
	
		/* Data bit */
		if (buffer & mask) {
			icspio_trispu();
			parity ^= 1;
		} else {
			icspio_trispd();
		}
	
		/* Data bit clock low */
		if (icspio_waitlow()) {
			return ERRTIMEOUT;
		}

		mask <<= 1;
	}

	/* Parity bit clock high */
	if (icspio_waithigh()) {
		return ERRTIMEOUT;
	}
		
	/* Parity bit */
	if (parity) {
		icspio_trispu();
	} else {
		icspio_trispd();
	}

	/* Parity bit clock low */
	if (icspio_waitlow()) {
		return ERRTIMEOUT;
	}

	/* Stop bit clock high */
	if (icspio_waithigh()) {
		return ERRTIMEOUT;
	}
		
	/* Stop bit high */
	icspio_trispu();

	/* Stop bit clock low */
	if (icspio_waitlow()) {
		return ERRTIMEOUT;
	}

	return ERRNONE;
}

/*
 * ICSP I/O Common Commands
 */
uint8_t
icspio_common(void)
{
	static uint8_t command;

	icspio_lasterr = icspio_err = ERRNONE;

	while (1) {
		if (icspio_err != ERRNONE)
			icspio_lasterr = icspio_err;

		icspio_checksum = 0;

		icspio_err = icspio_getbyte(&command);
		if (icspio_err)
			continue;

		switch (command) {
		default:return command;

		case CMD_ERROR:
			icspio_err = icspio_sendbyte(ACK);
			if (icspio_err)
				continue;

			icspio_err = icspio_sendbyte(icspio_lasterr);
			if (icspio_err)
				continue;

			icspio_lasterr = ERRNONE;
			break;
		}
		icspio_err = icspio_sendbyte(icspio_checksum);
	}
}

#endif /* !_ICSPIO_H */
