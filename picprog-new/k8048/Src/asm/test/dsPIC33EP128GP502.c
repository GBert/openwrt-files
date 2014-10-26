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
 * R-PI 3V3 dsPIC33EP128GP502
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 44032 words Flash (24-bit)
 * 16384 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR                                     1-----28 AVDD 3V3
 * AN0 OA2OUT RA0                            2     27 AVSS GND
 * AN1 C2IN1+ RA1                            3     26 RPI47 T5CK RB15
 * PGD3 VREF- AN2 C2IN- !SS1 RPI32 CTED2 RB0 4     25 RPI46 T3CK RB14
 * PGC3 VREF+ AN3 OA1OUT RPI33 CTED1 RB1     5     24 RPI45 CTPLS RB13
 * PGC1 AN4 C1IN+ RPI34 RB2                  6     23 RPI44 RB12
 * PGD1 AN5 C1IN- RP35 RB3                   7     22 TDI RP43 RB11
 * VSS GND                                   8     21 TDO RP42 RB10
 * OSC1 CLKI RA2                             9     20 VCAP
 * OSC2 CLKO RA3                             10    19 VSS
 * RP36 RB4                                  11    18 TMS ASDA1 SDI1 RP41 RB9
 * CVREF20 RP20 T1CK RA4                     12    17 TCK CVREF10 ASCL1 SDO1 RP40 T4CK RB8
 * VDD 3V3                                   13    16 SCK1 RP39 INT0 RB7
 * PGD2 ASDA2 RP37 RB5                       14----15 PGC2 ACSL2 RP38 RB6
 *
 * POWER
 * -----
 *    3V3--------VDD			13, 28
 *
 *     +---------VCAP			20
 *     |
 * 10u = TANTALUM E.S.R 1 OHM
 *     |
 *     +---------VSS			8, 27, 19
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR	PROGRAMMING ENABLE	1
 * PGC1		PRIMARY CLOCK		6
 * PGD1		PRIMARY DATA		7
 * PGC2		SECONDARY CLOCK		15
 * PGD2		SECONDARY DATA		14
 * PGC1		TERTIARY CLOCK		5
 * PGD1		TERTIARY DATA		4
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
#include <p33Exxxx.h>

/*
 * FAST RC, O/P ON CLKO (PIN 10).
 *
 * FOSC = 7,372,800 Hz
 * FCY  = 7,372,800 / 2
 */
#define FCY 3686400UL
#include <libpic30.h>	/* __delay32() */

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB0)
#define ICSPIO_PORTDAT (PORTBbits.RB0)
#define ICSPIO_TRISDAT (TRISBbits.TRISB0)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATBbits.LATB1)
#define ICSPIO_PORTCLK (PORTBbits.RB1)
#define ICSPIO_TRISCLK (TRISBbits.TRISB1)

#include <icspio.h>

#define FICD_RESERVED (0xFFEF) /* B4 = 0 */
_FICD(JTAGEN_OFF & FICD_RESERVED & ICS_PGD1)
_FPOR(WDTWIN_WIN25 & ALTI2C2_OFF & ALTI2C1_OFF)
_FWDT(FWDTEN_OFF & WINDIS_OFF & PLLKEN_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_OFF & POSCMD_NONE)
_FOSCSEL(IESO_OFF & FNOSC_FRC)
_FGS(GCP_OFF & GWRP_OFF)

#if 0
_FUID0(0xA1A1)
_FUID1(0xB2B2)
_FUID2(0xC3C3)
_FUID3(0xD4D4)
#endif

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC Digital Mode */
	ANSELA = 0;
	ANSELB = 0;

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
