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
 * R-PI 3V3 dsPIC33FJ32MC102
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 11264 words Flash (24-bit)
 * 2048 bytes RAM
 * 0 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR                                        1-----28 AVDD 3V3
 * PGD2 AN0 C3INB C1INA CTED1 CN2 RA0           2     27 AVSS GND
 * PGC2 AN1 C3INA C1INB CTED2 CN3 RA1           3     26 PWM1L1 RP15 CN11 RB15
 * PGD1 AN2 C2INA C1INC RP0 CN4 RB0             4     25 PWM1H1 RTCC RP14 CN12 RB14
 * PGC1 AN3 CVREFIN/OUT C2INB C1IND RP1 CN5 RB1 5     24 PWM1L2 RP13 CN13 RB13
 * AN4 C3INC C2INC RP2 CN6 RB2                  6     23 PWM1H2 RP12 CN14 RB12
 * AN5 C3IND C2IND RP3 CN7 RB3                  7     22 PWM1L3 RP11 CN15 RB11
 * VSS GND                                      8     21 PWM1H3 RP10 CN16 RB10
 * OSC1 CLKI CN30 RA2                           9     20 VCAP
 * OSC2 CLKO CN29 RA3                           10    19 VSS
 * PGD3 SOSCI AN9 RP4 CN1 RB4                   11    18 SDA1 RP9 CN21 RB9
 * PGC3 SOSCO AN10 T1CK CN0 RA4                 12    17 SCL1 RP8 CN22 RB8
 * VDD 3V3                                      13    16 INT0 RP7 CN23 RB7
 * !FLTB1 ASDA1 RP5 CN27 RB5                    14----15 !FLTA1 ASCL1 RP6 CN24 RB6
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
 * PGC1		PRIMARY CLOCK		5
 * PGD1		PRIMARY DATA		4
 * PGC2		SECONDARY CLOCK		3
 * PGD2		SECONDARY DATA		2
 * PGC3		TERTIARY CLOCK		12
 * PGD3		TERTIARY DATA		11
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
#define ICSPIO_LATDAT  (LATBbits.LATB4)
#define ICSPIO_PORTDAT (PORTBbits.RB4)
#define ICSPIO_TRISDAT (TRISBbits.TRISB4)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATAbits.LATA4)
#define ICSPIO_PORTCLK (PORTAbits.RA4)
#define ICSPIO_TRISCLK (TRISAbits.TRISA4)

#include <icspio.h>

#define CONFIG1_RESERVED (0x3FFF) /* B15 = B14 = 0 */
_CONFIG1(CONFIG1_RESERVED & WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_OFF & WINDIS_OFF & FWDTEN_OFF & ICS_PGD3 & HPOL_ON & GWRP_ON & GCP_OFF)
_CONFIG2(POSCMOD_NONE & ALTI2C_OFF & LPOL_ON & IOL1WAY_ON & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_FRC & WDTWIN_WDTWIN75 & PWMPIN_ON & PWMLOCK_ON & IESO_OFF)

/*
 * Init I/O
 */
static inline void
init_io(void)
{
	/* ADC Digital Mode */
	AD1PCFGL = 0xFFFF;

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
