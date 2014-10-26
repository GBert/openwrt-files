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
 * R-PI 3V3 dsPIC33FJ06GS101A
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 2048 words Flash (24-bit)
 * 256 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR                     1-----18 VDD 3V3
 * AN0 RA0                   2     17 VSS GND
 * AN1 RA1                   3     16 PWM1L RA3
 * AN2 RA2                   4     15 PWM1H RA4
 * AN3 RP0 CN0 RB0           5     14 VCAP
 * OSC1 CLKI AN6 RP1 CN1 RB1 6     13 VSS GND
 * OSC2 CLKO AN7 RP2 CN2 RB2 7     12 PGC1 SDA RP7 CN7 RB7
 * PGD2 TCK INT0 RP3 CN3 RB3 8     11 PGD1 TDI SCL RP6 CN6 RB6
 * PGC2 TMS      RP4 CN4 RB4 9-----10 TDO RP5 CN5 RB5
 * 
 * POWER
 * -----
 *    3V3--------VDD                     18
 *
 *     +---------VCAP                    14
 *     |
 * 10u =
 *     |
 *     +---------VSS                     17, 13
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR         PROGRAMMING ENABLE      1
 * PGC1          PRIMARY CLOCK           12
 * PGD1          PRIMARY DATA            11
 * PGC2          SECONDARY CLOCK         9
 * PGD2          SECONDARY DATA          8
 *
 * Breadboard Pin
 * ---------- ---
 * LD1        RA0 (2)
 *
 *      +--390R--o GND
 *     _|_
 *   \\/_\ LD1
 *      |
 * RA0--+
 *
 * Usage
 * -----
 * kio led 0 # LD1 OFF
 * kio led 1 # LD1 ON
 *
 ******************************************************************************/

#include <stdint.h>
#include <p33Fxxxx.h>

/*
 * FAST RC DIV N, O/P ON CLKO (PIN 7).
 *
 * FOSC = 7,372,800 Hz
 * FCY  = 7,372,800 / 2
 */
#define FCY 3686400UL
#include <libpic30.h>	/* __delay32() */

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB3)
#define ICSPIO_PORTDAT (PORTBbits.RB3)
#define ICSPIO_TRISDAT (TRISBbits.TRISB3)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATBbits.LATB4)
#define ICSPIO_PORTCLK (PORTBbits.RB4)
#define ICSPIO_TRISCLK (TRISBbits.TRISB4)

#include <icspio.h>

#define FICD_RESERVED (0xFFEF) /* B4 = 0 */
_FICD(JTAGEN_OFF & FICD_RESERVED & ICS_PGD1)
_FWDT(FWDTEN_OFF & PLLKEN_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_OFF & POSCMD_NONE)
_FOSCSEL(IESO_OFF & FNOSC_FRCDIVN)
_FGS(GCP_OFF & GWRP_OFF)

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC Digital Mode */
	ADPCFG = 0xFFFF;

	/* Reset O/P */
	LATA = 0;
	LATB = 0;

	/* O/D Off */
	ODCA = 0;
	ODCB = 0;

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
}

int
main(void)
{
	static uint8_t arg;

	/* Init Clock */
	CLKDIVbits.FRCDIV = 0; /* 3.68Mhz (divide by 1) */

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
			LATA = arg;
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
