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
 * R-PI 3V3 PIC24F04KA201
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 1408 words Flash (24-bit)
 * 512 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR VPP RA5                1----20 VDD 3V3
 * PGC2 AN0 VREF+ CN2 RA0       2    19 VSS GND
 * PGD2 AN1 VREF+ CN2 RA1       3    18 REFO !SS1 T2CK T3CK CN11 RB15
 * AN2 C2INB CN4 RB0            4    17 AN10 CVREF SDI1 OCFA C1OUT INT1 CN12 RB14
 * AN3 C2INA CN5 RB1            5    16 AN11 SDO1 CTPLS CN13 RB13
 * U1RX CN6 RB2                 6    15 AN12 HLVDIN SCK1 CTED2 CN14 RB12
 * OSCI CKLI AN4 C1INB CN30 RA2 7    14 OC1 IC1 C2OUT INT2 CTED1 CN8 RA6
 * OSCO CLKO AN5 C1INA CN29 RA3 8    13 SDA1 U1BCLK !U1RTS CN21 RB9
 * PGD3 SOSCI CN1 RB4           9    12 SCL1 !U1CTS CN22 RB8
 * PGC3 SOSCO T1CK CN0 RA4      10---11 U1TX INT0 CN23 RB7
 *
 * No flash programming specification data-sheet for this part exists.
 *
 * POWER
 * -----
 *    3V3--------VDD                     20
 *
 *     +---------VSS                     19
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR         PROGRAMMING ENABLE      1
 * PGC2          PRIMARY CLOCK           2
 * PGD2          PRIMARY DATA            3
 * PGC3          SECONDARY CLOCK         9
 * PGD3          SECONDARY DATA          10
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
 * RCDIV = 0 (divide by 1)
 * FOSC  = 8,000,000 * PLL(4) = 32,000,000
 * FCY   = FOSC / 2           = 16,000,000
 */
#define FCY 16000000UL
#include <libpic30.h>	/* __delay32() */

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB4)
#define ICSPIO_PORTDAT (PORTBbits.RB4)
#define ICSPIO_TRISDAT (TRISBbits.TRISB4)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATAbits.LATA4)
#define ICSPIO_PORTCLK (PORTAbits.RA4)
#define ICSPIO_TRISCLK (TRISAbits.TRISA4)

#include <icspio.h>

_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(IESO_OFF & FNOSC_FRCPLL)
_FOSC(FCKSM_CSDCMD & SOSCSEL_SOSCHP & POSCFREQ_HS & OSCIOFNC_ON & POSCMOD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS1024)
_FPOR(MCLRE_ON & BORV_18V & PWRTEN_ON & BOREN_BOR1)
_FICD(ICS_PGx3)
_FDS(DSWDTEN_OFF & DSLPBOR_OFF & DSWDTPS_DSWDTPSF)

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC1 Digital Mode */
	AD1PCFG = 0xFFFF;

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
	CLKDIVbits.RCDIV = 0; /* 8Mhz (divide by 1) */

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
