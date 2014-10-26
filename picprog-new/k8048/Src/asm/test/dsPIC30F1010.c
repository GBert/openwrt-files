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
 * R-PI 3V3 dsPIC30F1010
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
 * !MCLR                                1-----28 AVDD AVCC
 * AN0 CMP1A CN2 RB0                    2     27 AVSS AGND
 * AN1 CMP1B CN3 RB1                    3     26 PWM1L RE0
 * AN2 CMP1C CMP2A CN4 RB2              4     25 PWM1H RE1
 * AN3 CMP1D CMP2B CN5 RB3              5     24 PWM2L RE2
 * AN4 CMP2C CN6 RB4                    6     23 PWM2H RE3
 * AN5 CMP2D CN7 RB5                    7     22 RE4
 * VSS GND                              8     21 RE5
 * OSC1 CLKI RB6                        9     20 VDD VCC
 * OSC2 CLKO RB7                        10    19 VSS GND
 * PGD1 EMUD1 T2CK U1ATX CN1 RE7        11    18 PGC EMUC SDI1 SDA U1RX RF7
 * PGC1 EMUC1 T1CK U1ARX CN0 RE6 EXTREF 12    17 PGD EMUD SDO1 SCL U1TX RF8
 * VDD VCC                              13    16 SFLT2 INT0 OCFLTA RA9
 * PGD2 EMUD2 SCK1 SFLT3 INT2 RF6       14----15 PGC2 EMUC2 OC1 SFLT1 IC1 INT1 RD0
 *
 * POWER
 * -----
 * VDD (2.5V .. 5.5V) 			13, 20, 28
 * VSS					8,  19, 27
 *
 * ICSP
 * ----
 * !MCLR	PROGRAMMING ENABLE	1
 * PGC		PRIMARY CLOCK		18
 * PGD		PRIMARY DATA		17
 * PGC1		SECONDARY CLOCK		12
 * PGD1		SECONDARY DATA		11
 * PGC2		TERTIARY CLOCK		15
 * PGD2		TERTIARY DATA		14
 *
 * Breadboard Pin
 * ---------- ---
 * LD1        RB0 (2)
 *
 *      +--390R--o GND
 *     _|_
 *   \\/_\ LD1
 *      |
 * RB0--+
 *
 * Usage
 * -----
 * kio led 0 # LD1 OFF
 * kio led 1 # LD1 ON
 *
 ******************************************************************************/

#include <stdint.h>
#include <p30Fxxxx.h>

/*
 * FAST RC HI-RANGE O/P ON CLKO (PIN 10).
 *
 * FOSC = 14,550,000 Hz
 * FCY  = 14,550,000 / 2
 */
#define FCY 7275000UL
#include <libpic30.h>	/* __delay32() */

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATEbits.LATE7)
#define ICSPIO_PORTDAT (PORTEbits.RE7)
#define ICSPIO_TRISDAT (TRISEbits.TRISE7)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATEbits.LATE6)
#define ICSPIO_PORTCLK (PORTEbits.RE6)
#define ICSPIO_TRISCLK (TRISEbits.TRISE6)

#include <icspio.h>

_FBS(WR_PROT_BOOT_OFF & NO_BOOT_CODE)
_FGS(CODE_PROT_OFF & GWRP_OFF)
_FOSCSEL(FRC)
_FOSC(PRIOSC_OFF & OSC2_CLKO & FRC_HI_RANGE & CSW_FSCM_OFF)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FPOR(PWRT_128)
_FICD(ICS_PGD)

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC Digital Mode */
	ADPCFG = 0xFFFF;

	/* Reset O/P */
	LATB = 0;

	/* RB0 O/P */
        TRISBbits.TRISB0 = 0;
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
			LATB = arg;
			break; 
		case CMD_SWITCH:
			icspio_err = icspio_sendbyte(ACK);
			if (icspio_err)
				continue;
			icspio_err = icspio_sendbyte(PORTE);
			if (icspio_err)
				continue;
		}
		icspio_err = icspio_sendbyte(icspio_checksum);
	}
}
