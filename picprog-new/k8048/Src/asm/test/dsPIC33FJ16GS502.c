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
 * R-PI 3V3 dsPIC33FJ16GS502
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 5632 words Flash (24-bit)
 * 2048 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR                                  1-----28 AVDD 3V3
 * AN0 CMP1A RA0                          2     27 AVSS GND
 * AN1 CMP1B RA1                          3     26 PWM1L RA3
 * AN2 CMP1C CMP2A RA2                    4     25 PWM1H RA4
 * AN3 CMP1D CMP2B RP0 CN0 RB0            5     24 PWM2L RP14 CN14 RB14
 * AN4 CMP2C CMP3A RP9 CN9 RB9            6     23 PWM2H RP13 CN13 RB13
 * AN5 CMP2D CMP3B RP10 CN10 RB10         7     22 TCK PWM3L RP12 CN12 RB12
 * VSS GND                                8     21 TMS PWM3H RP11 CN11 RB11
 * OSC1 CLKI AN6 CMP3C CMP4A RP1 CN1 RB1  9     20 VCAP
 * OSC2 CLKO AN7 CMP3D CMP4B RP2 CN2 RB2  10    19 VSS
 * PGD2 DACOUT INT0 RP3 CN3 RB3           11    18 PGC1 SDA RP7 CN7 RB7
 * PGC2 EXTREF RP4 CN4 RB4                12    17 PGD1 TDI SCL RP6 CN6 RB6
 * VDD 3V3                                13    16 TDO RP5 CN5 RB5
 * PGD3 CN8 RB8 RP8 CN8 RB8               14----15 PGC3 RP15 CN15 RB15
 *
 * POWER
 * -----
 *    3V3--------VDD			13, 28
 *
 *     +---------VCAP			20
 *     |
 * 10u = TANTALUM LOW E.S.R
 *     |
 *     +---------VSS			8, 27, 19
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR	PROGRAMMING ENABLE	1
 * PGC1		PRIMARY CLOCK		18
 * PGD1		PRIMARY DATA		17
 * PGC2		SECONDARY CLOCK		12
 * PGD2		SECONDARY DATA		11
 * PGC3		TERTIARY CLOCK		15
 * PGD3		TERTIARY DATA		14
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
 * FAST RC, O/P ON CLKO (PIN 10).
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

_FBS(BWRP_WRPROTECT_OFF & BSS_NO_BOOT_CODE)
_FGS(GWRP_OFF & GSS_OFF)
_FOSCSEL(FNOSC_FRCDIVN & IESO_OFF)
_FOSC(POSCMD_NONE & OSCIOFNC_OFF & IOL1WAY_OFF & FCKSM_CSECMD)
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & WINDIS_OFF & FWDTEN_OFF)
_FPOR(FPWRT_PWR1)
_FICD(ICS_PGD1 & JTAGEN_OFF)

#if 0
_FUID0(0xF0) 
_FUID1(0xF1) 
#endif

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

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
}

int
main(void)
{
	static uint8_t arg;

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
