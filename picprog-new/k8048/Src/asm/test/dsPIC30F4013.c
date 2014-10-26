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
 * Velleman K8048 dsPIC30F4013
 *
 * Velleman K8048 ICSPIO Demo Test (Receive commands, send data).
 *
 * This demonstrates how we may receive commands from the host computer
 * via the ISCP port and execute them. One command is implemented.
 * The command takes one argument which sets the LED to that value.
 *
 *******************************************************************************
 *
 * 16384 words Flash (24-bit)
 * 2048 bytes RAM
 * 1024 bytes EEPROM
 *
 * Pinout
 * ------
 * !MCLR                           1-----40 AVDD
 * AN0 VREF+ CN2 RB0               2     39 AVSS
 * AN1 VREF- CN3 RB1               3     38 AN9  RB9
 * AN2 !SS1 LVDIN CN4 RB2          4     37 AN10 RB10
 * AN3 CN5 RB3                     5     36 AN11 RB11
 * AN4 CN6 RB4                     6     35 AN12 RB12
 * AN5 CN7 RB5                     7     34 EMUC2 OC1 RD0
 * PGC EMUC AN6 OCFA RB6           8     33 EMUD2 OC2 RD1
 * PGD EMUD AN7 RB7                9     32 VDD
 * AN8 RB8                         10    31 VSS
 * VDD                             11    30 RF0
 * VSS                             12    29 RF1
 * OSC1 CLKI                       13    28 U2RX CN17 RF4
 * OSC2 CLKO RC15                  14    27 U2TX CN18 RF5
 * EMUD1 SOSCI T2CK U1ATX CN1 RC13 15    26 U1RX SDI1 SDA RF2
 * EMUC1 SOSCO T1CK U1ARX CN0 RC14 16    25 EMUD3 U1TX SDO1 SCL RF3
 * INT0 RA11                       17    24 EMUC3 SCK1 RF6
 * IC2 INT2 RD9                    18    23 IC1 INT1 RD8
 * RD3                             19    22 RD2
 * VSS                             20----21 VDD
 *
 * POWER
 * -----
 * VDD (2.5V .. 5.5V)                    40, 32, 21, 11
 * VSS                                   39, 31, 20, 12
 *
 * ICSP
 * ----
 * !MCLR (13V)   PROGRAMMING ENABLE      1
 * PGC           SERIAL CLOCK            8
 * PGD           SERIAL DATA             9
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
 * FOSC = 7,372,800 * 16 = 117,964,800 Hz
 * FCY  = FOSC / 4       = 29,491,200 Hz
 */
#define FCY 29491200UL
#include <libpic30.h>	/* __delay32() */

/* ICSP I/O Data I/O */
#define ICSPIO_LATDAT  (LATBbits.LATB7)
#define ICSPIO_PORTDAT (PORTBbits.RB7)
#define ICSPIO_TRISDAT (TRISBbits.TRISB7)

/* ICSP I/O Clock Input */
#define ICSPIO_LATCLK  (LATBbits.LATB6)
#define ICSPIO_PORTCLK (PORTBbits.RB6)
#define ICSPIO_TRISCLK (TRISBbits.TRISB6)

#include <icspio.h>

_FOSC(FRC_PLL16 & CSW_FSCM_OFF)
_FWDT(WDT_OFF)
_FBORPOR(PWRT_4 & BORV45 & PBOR_OFF & MCLR_EN)
_FGS(GWRP_OFF & CODE_PROT_OFF)

#if 0
_FUID0(0x0101)
_FUID1(0x0202)
_FUID2(0x0303)
_FUID3(0x0404) 

uint16_t _EEDATA(2) eedata[] = {
        0x0000,0x0101,0x0202,0x0303,0x0404,0x0505,0x0606,0x0707,
        0x0808,0x0909,0x0A0A,0x0B0B,0x0C0C,0x0D0D,0x0E0E,0x0F0F,
        0xFFFF,0xFEFE,0xFDFD,0xFCFC,0xFBFB,0xFAFA,0xF9F9,0xF8F8,
        0xF7F7,0xF6F6,0xF5F5,0xF4F4,0xF3F3,0xF2F2,0xF1F1,0xF0F0,
};
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
			icspio_err = icspio_sendbyte(PORTB);
			if (icspio_err)
				continue;
		}
		icspio_err = icspio_sendbyte(icspio_checksum);
	}
}
