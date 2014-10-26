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
 * R-PI 3V3 PIC24F16KL402
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
 * 1024 bytes RAM
 * 512 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR VPP RA5                           1-----28 VDD 3V3
 * VREF+ CVREF+ AN0 SDA2 CN2 RA0           2     27 VSS GND
 * VREF- CVREF- AN1 CN3 RA1                3     26 AN9 T3CK REFO !SS1 CN11 RB15
 * PGD1 AN2 ULPWU C1IND C2INB U2TX CN4 RB0 4     25 CVREF AN10 C1OUT FLT0 INT1 CN12 RB4
 * PGC1 AN3 C1INC C2INA U2RX CN5 RB1       5     24 AN11 SDO1 CN13 RB13
 * AN4 C1INB C2IND T3G U1RX CN6 RB2        6     23 AN12 HLVDIN !SS2 CCP2 CN14 RB12
 * C1INA C2INC SCL2 CN7 RB3                7     22 PGC2 SCK1 P1C CN15 RB11
 * VSS GND                                 8     21 PGD2 SDI1 P1B CN16 RB10
 * OSCI AN13 CLKI CN30 RA2                 9     20 C2OUT CCP1 P1A INT2 CN8 RA6
 * OSCO AN14 CLKO CN29 RA3                 10    19 SDI2 CCP3 CN9 RA7
 * SOSCI AN15 !U2RTS CN1 RB4               11    18 SDA1 T1CK !U1RTS P1D CN21 RB9
 * SOSCO SCLKI !U2CTS CN0 RA4              12    17 SCL1 !U1CTS CN22 RB8
 * VDD 3V3                                 13    16 U1TX INT0 CN23 RB7
 * PGD3 ASDA1 SCK2 CN27 RB5                14----15 PGC3 ASCL1 SDO2 CN24 RB6
 *
 * POWER
 * -----
 *    3V3--------VDD			13, 28
 *
 *     +---------VSS			8, 27
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
#include <p24Fxxxx.h>

/*
 * FOSC  = 8,000,000
 * FCY   = FOSC / 2
 */
#define FCY 4000000UL
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

#ifndef _EEDATA
#define _EEDATA(N) __attribute__((space(eedata),aligned(N)))
#endif
#if 0
uint16_t _EEDATA(2) eedata[] = {
        0x0000,0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,
        0x0808,0x0909,0x0A0A,0x0B0B,0x0C0C,0x0D0D,0x0E0E,0x0F0F,
        0xFFFF,0xFEFE,0xFDFD,0xFCFC,0xFBFB,0xFAFA,0xF9F9,0xF8F8,
        0xF7F7,0xF6F6,0xF5F5,0xF4F4,0xF3F3,0xF2F2,0xF1F1,0xF0F0,
};
#endif

_FBS(BSS_OFF & BWRP_OFF)
_FGS(GSS0_OFF & GWRP_OFF)
_FOSCSEL(IESO_OFF & LPRCSEL_LP & SOSCSRC_DIG & FNOSC_FRC)
_FOSC(FCKSM_CSECMD & SOSCSEL_SOSCHP & POSCFREQ_HS & OSCIOFNC_OFF & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)
_FPOR(MCLRE_ON & BORV_V18 & PWRTEN_ON & I2C1SEL_PRI & BOREN_BOR0)
_FICD(ICS_PGx1)

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* Digital Mode */
	ANSA = 0;
	ANSB = 0;

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
