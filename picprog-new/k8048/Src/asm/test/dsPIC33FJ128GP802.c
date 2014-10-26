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
 * R-PI 3V3 dsPIC33FJ128GP802
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
 * !MCLR                        1-----28 AVDD 3V3
 * AN0 VREF+ CN2 RA0            2     27 AVSS GND
 * AN1 VREF- CN3 RA1            3     26 AN9 RP15 CN11 PMCS1 RB15      DAC1LN
 * PGD1 AN2 C2IN- RP0 CN4 RB0   4     25 AN10 RTCC RP14 CN12 PMWR RB14 DAC1LP
 * PGC1 AN3 C2IN+ RP1 CN5 RB1   5     24 AN11 RP13 CN13 PMRD RB13      DAC1RN
 * AN4 C1IN- RP2 CN6 RB2        6     23 AN12 RP12 CN14 PMD0 RB12      DAC1RP
 * AN5 C1IN+ RP3 CN7 RB3        7     22 PGC2 TMS RP11 CN15 PMD1 RB11
 * VSS GND                      8     21 PGD2 TDI RP10 CN16 PMD2 RB10
 * OSC1 CLKI CN30 RA2           9     20 VCAP
 * OSC2 CLKO CN29 PMA0 RA3      10    19 VSS
 * SOSCI RP4 CN1 PMBE RB4       11    18 TDO SDA1 RP9 CN21 PMD3 RB9
 * SOSCO T1CK CN0 PMA1 RA4      12    17 TCK SCL1 RP8 CN22 PMD4 RB8
 * VDD 3V3                      13    16 INT0 RP7 CN23 PMD5 RB7
 * PGD3 ASDA1 RP5 CN27 PMD7 RB5 14----15 PGC3 ASCL1 RP6 CN24 PMD6 RB6
 *
 * POWER
 * -----
 *    3V3--------VDD			13, 28
 *
 *     +---------VCAP E.S.R < 5 OHMS	20
 *     |
 * 10u =
 *     |
 *     +---------VSS			8, 27, 19
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR	PROGRAMMING ENABLE	1
 * PGC1		PRIMARY CLOCK		5
 * PGD1		PRIMARY DATA		4
 * PGC2		SECONDARY CLOCK		22
 * PGD2		SECONDARY DATA		21
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
 * FOSC = 7,372,800 / 3 * 65 / 2 = 79,872,000 Hz
 * FCY  = 39,936,000 Hz
 */
#define FCY 39936000UL
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

_FBS(RBS_NO_BOOT_RAM & BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF)
_FSS(RSS_NO_SEC_RAM & SSS_NO_SEC_CODE & SWRP_WRPROTECT_OFF)
_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(IESO_OFF & FNOSC_LPRCDIVN)
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FPOR(ALTI2C_OFF & FPWRT_PWR1)
_FICD(JTAGEN_OFF & ICS_PGD1)

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

	/* Init Clock */
	CLKDIVbits.FRCDIV = 0;	/* divide by 1 (default) */
	CLKDIVbits.PLLPRE = 1;	/* PLLPRE divide by 3    */
	PLLFBD = 65 - 2;	/* PLLDIV = 65           */
	CLKDIVbits.PLLPOST = 0;	/* PLLPOST divide by 2   */

	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 1);
	while (OSCCONbits.LOCK != 1);

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
