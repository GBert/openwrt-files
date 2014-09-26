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

#include <stdio.h>

#include <pic18fregs.h>
#include <usart.h>

#include "PICHID/usb.h"
#include "PICHID/main.h"

#include "pic18F2550.h"

#pragma stack 0x0200 0x0100

// PIC18F2550 configuration
#pragma config PLLDIV=3	// 12MHz XTAL
#pragma config CPUDIV=OSC1_PLL2
#pragma config USBDIV=2

#pragma config FOSC=HSPLL_HS
#pragma config FCMEN=OFF
#pragma config IESO=OFF

#pragma config PWRT=OFF
#pragma config BOR=OFF
#pragma config BORV=2
#pragma config VREGEN=OFF	// RKP28sb LD1117V33

#pragma config WDT=OFF
#pragma config WDTPS=1024

#pragma config CCP2MX=OFF
#pragma config PBADEN=OFF
#pragma config LPT1OSC=OFF
#pragma config MCLRE=ON

#pragma config STVREN=ON
#pragma config LVP=OFF
#pragma config XINST=OFF
#pragma config DEBUG=OFF

#pragma config CP0=OFF
#pragma config CP1=OFF
#pragma config CP2=OFF
#pragma config CP3=OFF

#pragma config CPB=OFF
#pragma config CPD=OFF

#pragma config WRT0=OFF
#pragma config WRT1=OFF
#pragma config WRT2=OFF
#pragma config WRT3=OFF

#pragma config WRTB=OFF
#pragma config WRTC=OFF
#pragma config WRTD=OFF

#pragma config EBTR0=OFF
#pragma config EBTR1=OFF
#pragma config EBTR2=OFF
#pragma config EBTR3=OFF

#pragma config EBTRB=OFF

// PIC18F2550 initialisation
void
Initialise(void)
{
	// Initialise interrupts
	INTCON = 0x00;
	INTCON3 = 0x00;
	INTCON2 = 0x80;	// Disable port B pull-ups
	PIR1 = 0x00;
	PIR2 = 0x00;
	PIE1 = 0x00;
	PIE2 = 0x00;
	IPR1 = 0x00;
	IPR2 = 0x00;
	UIE = 0x00;
	UIR = 0x00;

	// Initialise USB
	UCON = 0x00;	// Disable USB
	UCFG = 0x14;	// Enable USB pull-up, enable USB full speed
	deviceState = DETACHED;
	remoteWakeup = 0x00;
	currentConfiguration = 0x00;

	// Initialise I/O ports
	CMCON = 0x07;	// Disable comparators
	ADCON0 = 0x00;	// Disable A/D
	ADCON1 = 0x0F;	// Digital I/O

#if DEBUG
	// Initialise UART for printf
	TRISC |= 0x80;	// Rx in
	TRISC &= 0xBF;	// Tx out
	RCSTA = 0x90;	// Enable UART, continuous Rx
	TXSTA = 0x24;	// Enable Tx, high baud rate select
	BAUDCON = 0x08;	// 16-bit baud rate select
	SPBRGH = 0x00;	// 115,200 baud
	SPBRG = 0x67;	// 115,200 baud
	stdout = STREAM_USART;
#endif
}

// Firmware loop
void
main(void)
{
	// Setup chip
	Initialise();
#if DEBUG
	printf("Initialised.\r\n");
#endif
	// Do forever...
	while (1)
	{
		EnableUSBModule();
		ProcessUSBTransactions();
		if (ProcessIn())
		{
			byte n;
#if DEBUG
			printf("Processing.\r\n");
#endif
			for (n = 0; n < HID_INPUT_REPORT_BYTES; ++n)
			{
				txBuffer[n] = rxBuffer[n] ^ 0xFF;
			}
			ProcessOut();
		}
	}
}
