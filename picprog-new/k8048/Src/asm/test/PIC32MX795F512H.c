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

/*******************************************************************************
 *
 * R-PI 3V3 Majenko SDXL PIC32MX795F512H
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * Program
 * --------
 * ssh pi k32 program < PIC32MX795F512H.hex
 *
 * Usage
 * -----
 * kio led 0   # LED OFF
 * kio led 128 # LED ON
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <plib.h>

#define FCY (80000000UL)

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB7)
#define ICSPIO_PORTDAT (PORTBbits.RB7)
#define ICSPIO_TRISDAT (TRISBbits.TRISB7)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATBbits.LATB6)
#define ICSPIO_PORTCLK (PORTBbits.RB6)
#define ICSPIO_TRISCLK (TRISBbits.TRISB6)

#include <icspio.h>

/*
 * Device config:
 *
 * include/proc/p32mx795f512h.h = lib/proc/32MX795F512H/configuration.data
 */

/* DEVCFG3 */
#pragma config FVBUSONIO = OFF
#pragma config FUSBIDIO = OFF
#pragma config FCANIO = ON
#pragma config FETHIO = ON
#pragma config FMIIEN = ON
#pragma config FSRSSEL = PRIORITY_7
#pragma config USERID = 0x0000

/* DEVCFG2 */
#pragma config FPLLODIV = DIV_1
#pragma config UPLLEN = ON
#pragma config UPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLIDIV = DIV_2

/* DEVCFG1 */
#pragma config FWDTEN = OFF
#pragma config WDTPS = PS1024
#pragma config FCKSM = CSECME
#pragma config FPBDIV = DIV_1
#pragma config OSCIOFNC = OFF
#pragma config POSCMOD = EC
#pragma config IESO = OFF
#pragma config FSOSCEN = OFF
#pragma config FNOSC = PRIPLL

/* DEVCFG0 */
#pragma config CP = OFF
#pragma config BWP = OFF
#pragma config PWP = OFF
#pragma config ICESEL = ICS_PGx2
#pragma config DEBUG = OFF

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC Digital Mode */
	AD1PCFG = 0xFFFF;

	/* Reset O/P */
	LATE = 0;

	/* Port E O/P */
	TRISE = 0;
}

int
main()
{
	static uint8_t arg;

	/*
	 * Init CPU: include/peripheral/system.h
	 */
	SYSTEMConfigPerformance(FCY);

	/* Disable JTAG */
	DDPCONbits.JTAGEN = 0;

	/* Init I/O */
	init_io();

	/* Init ICSP I/O */
	init_icspio();

	/* ICSP I/O */
	while (1) {
		switch (icspio_common()) {
		default:icspio_err = icspio_sendbyte(NAK);
			continue;

		case CMD_LED:
			icspio_err = icspio_sendbyte(ACK);
			if (icspio_err)
				continue;
			icspio_err = icspio_getbyte(&arg);
			if (icspio_err)
				continue;
			LATE = arg;
			break; 
		case CMD_SWITCH:
			icspio_err = icspio_sendbyte(ACK);
			if (icspio_err)
				continue;
			icspio_err = icspio_sendbyte(PORTB);
			if (icspio_err)
				continue;
		}
		icspio_err = icspio_sendbyte(icspio_checksum);
	}
}
