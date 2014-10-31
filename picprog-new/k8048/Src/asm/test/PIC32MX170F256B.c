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
 * R-PI 3V3 PIC32MX170F256B
 *
 * R-PI ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * Pinout
 * ------
 * !MCLR                                 1======28 AVDD 3V3
 * VREF+/CVREF+/AN0/C3INC/RPA0/CTED1/RA0 2      27 AVSS GND
 * VREF-/CVREF-/AN1/RPA1/CTED2/RA1       3      26 AN9/C3INA/RPB15/SCK2/CTED6/PMCS1/RB15
 * PGD1/AN2/C1IND/C2INB/C3IND/RPB0/RB0   4      25 CVREFOUT/AN10/C3INB/RPB14/SCK1/CTED5/PMWR/RB14
 * PGC1/AN3/C1INC/C2INA/RPB1/CTED12/RB1  5      24 AN11/RPB13/CTPLS/PMRD/RB13
 * AN4/C1INB/C2IND/RPB2/SDA2/CTED13/RB2  6      23 AN12/PMD0/RB12
 * AN5/C1INA/C2INC/RTCC/RPB3/SCL2/RB3    7      22 PGC2/TMS/RPB11/PMD1/RB11
 * VSS GND                               8      21 PGD2/RPB10/CTED11/PMD2/RB10
 * OSC1/CLKI/RPA2/RA2                    9      20 VCAP
 * OSC2/CLKO/RPA3/PMA0/RA3               10     19 VSS GND
 * SOSCI/RPB4/RB4                        11     18 TDO/RPB9/SDA1/CTED4/PMD3/RB9
 * SOSCO/RPA4/T1CK/CTED9/PMA1/RA4        12     17 TCK/RPB8/SCL1/CTED10/PMD4/RB8
 * VDD 3V3                               13     16 TDI/RPB7/CTED3/PMD5/INT0/RB7
 * PGD3/RPB5/PMD7/RB5                    14=====15 PGC3/RPB6/PMD6/RB6
 *
 * POWER
 * -----
 *    3V3--------VDD                    13, 28
 *
 *     +---------VCAP                   20
 *     |
 * 10u = E.S.R. < 5 OHMS
 *     |
 *     +---------VSS                    8, 19, 27
 *     |
 *    ///
 *
 * ICSP
 * ----
 * !MCLR        PROGRAMMING ENABLE      1
 * PGC1         PRIMARY CLOCK           5
 * PGD1         PRIMARY DATA            4
 * PGC2         SECONDARY CLOCK         22
 * PGD2         SECONDARY DATA          21
 * PGC3         TERTIARY CLOCK          15
 * PGD3         TERTIARY DATA           14
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
 * make program
 *
 * Usage
 * -----
 * kio led 0 # LD1 OFF
 * kio led 1 # LD1 ON
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <plib.h>

#define MEGAHERTZ 48 // 40 or 48

#if MEGAHERTZ == 40
#define FCY (40000000UL)
#elif MEGAHERTZ == 48
#define FCY (48000000UL)
#endif

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB0)
#define ICSPIO_PORTDAT (PORTBbits.RB0)
#define ICSPIO_TRISDAT (TRISBbits.TRISB0)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATBbits.LATB1)
#define ICSPIO_PORTCLK (PORTBbits.RB1)
#define ICSPIO_TRISCLK (TRISBbits.TRISB1)

#include <icspio.h>

#include <delay.h> /* delay_ms() */

/*
 * Device config:
 *
 * include/proc/p32mx170f256b.h = lib/proc/32MX170F256B/configuration.data
 */

/* DEVCFG3 */
#if 0
#pragma config FVBUSONIO = ON
#pragma config FUSBIDIO = ON
#pragma config IOL1WAY = ON
#pragma config PMDL1WAY = ON
#pragma config USERID = 0xFFFF
#endif

/* DEVCFG2 */
#if MEGAHERTZ == 40
#pragma config FPLLIDIV = DIV_2 /*  8 /  2 */
#pragma config FPLLMUL = MUL_20 /*  4 * 20 */
#pragma config FPLLODIV = DIV_2 /* 80 /  2 */
#elif MEGAHERTZ == 48
#pragma config FPLLIDIV = DIV_2 /*  8    /  2 */
#pragma config FPLLMUL = MUL_24 /*  4    * 24 */
#pragma config FPLLODIV = DIV_2 /* 96    /  2 */
#endif

/* DEVCFG1 */
#pragma config FWDTWINSZ = WISZ_25
#pragma config FWDTEN = OFF
#pragma config WINDIS = OFF
#pragma config WDTPS = PS1024
#pragma config FCKSM = CSDCMD
#pragma config FPBDIV = DIV_1
#pragma config OSCIOFNC = OFF
#pragma config POSCMOD = OFF
#pragma config IESO = ON
#pragma config FSOSCEN = OFF
#pragma config FNOSC = FRCPLL

/* DEVCFG0 */
#if 0
#pragma config CP = OFF
#pragma config BWP = OFF
#pragma config PWP = OFF
#pragma config ICESEL = ICS_PGx1
#pragma config DEBUG = OFF // DON'T TOUCH THIS
#endif
#pragma config JTAGEN = OFF

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

/*
 * Blink LED n times
 */
static inline void
blink(uint32_t n)
{
	n <<= 1;
	while(n--) {
		/*  F = 1 Hz */
		delay_ms(500);

		/* RA0 = ! RA0 */
		LATAbits.LATA0 = ~LATAbits.LATA0;
	}
}

int
main()
{
	static uint8_t arg;

	/*
	 * Init CPU: include/peripheral/system.h
	 */
	SYSTEMConfigPerformance(FCY);

	/* Init I/O */
	init_io();

	/* Blink LED 10 times */
	blink(10);

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
