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

#include "led.h"

_FBS(RBS_NO_BOOT_RAM & BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF)
_FSS(RSS_NO_SEC_RAM & SSS_NO_SEC_CODE & SWRP_WRPROTECT_OFF)
_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(IESO_OFF & FNOSC_LPRCDIVN)
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FPOR(ALTI2C_OFF & FPWRT_PWR1)
_FICD(JTAGEN_OFF & ICS_PGD1)

void
init_io(void)
{
	/* ADC1 Digital Mode */
	AD1PCFGL = 0xFFFF;

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
	LATAbits.LATA0 = 0;
}

int
main(void)
{
	/* Init Clock */
	CLKDIVbits.PLLPRE = PLL_PRE;
	PLLFBD = PLL_DIV;
	CLKDIVbits.PLLPOST = PLL_POST;

	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(0x01);

	while (!OSCCONbits.COSC);
	while (!OSCCONbits.LOCK);

	/* Init PORT I/O */
	init_io();

	/* Blink Forever */
	while (true) {
		blink(); /* ~50 KHz */
		__delay_us(10);
	}
}
