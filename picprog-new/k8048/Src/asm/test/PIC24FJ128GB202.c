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
 * R-PI 3V3 PIC24FJ128GB202
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
 * 8192 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR        1-----28 VDD 3V3
 * PGD3 RA0     2     27 VSS GND
 * PGC3 RA1     3     26 RB15
 * PGD1 RB0 RB0 4     25 RB14
 * PGC1 RB1 RB1 5     24 RB13
 * RB2          6     23 VUSB 3V3
 * RB3          7     22 PGC2 RB11
 * VSS GND      8     21 PGD2 RB10
 * RA2          9     20 VCAP VDDCORE
 * RA3          10    19 VBAT 3V3
 * RB4          11    18 RB9
 * RA4          12    17 RB8
 * VDD 3V3      13    16 RB7
 * RB5          14----15 VBUS 5V
 *
 * POWER
 * -----
 *    3V3---+----VDD			13, 28
 *	    |
 *          +----VBAT			19
 *
 *     +---------VCAP			20
 *     |
 * 10u =
 *     |
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
 * PGC3		TERTIARY CLOCK		3
 * PGD3		TERTIARY DATA		2
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
 * Program
 * --------
 * k24 SELECT PIC24FJ128GB202 LVP PROGRAM PIC24FJ128GB202.hex
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
 * FAST RC, O/P ON CLKO (PIN 10).
 *
 * FOSC = 8 MHz
 * FCY  = 8 / 2
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

_CONFIG4(DSWDTPS_DSWDTPS5 & DSWDTOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF & DSSWEN_OFF & PLLDIV_DISABLED & I2C1SEL_DISABLE & IOL1WAY_OFF)
_CONFIG3(WPFP_WPFP63 & SOSCSEL_OFF & WDTWIN_PS75_0 & PLLSS_PLL_FRC & BOREN_OFF & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPSTARTMEM)
_CONFIG2(POSCMD_NONE & WDTCLK_LPRC & OSCIOFCN_OFF & FCKSM_CSDCMD & FNOSC_FRC & ALTRB6_RETAIN & ALTCMPI_CxINC_RX & WDTCMX_WDTCLK & IESO_OFF)
_CONFIG1(WDTPS_PS1024 & FWPSA_PR128 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & LPCFG_ON & GCP_OFF & JTAGEN_OFF)

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
