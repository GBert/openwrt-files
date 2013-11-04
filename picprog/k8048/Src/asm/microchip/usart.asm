;
; Software License Agreement
;
; The software supplied herewith by Microchip Technology Incorporated 
; (the “Company”) for its PICmicro® Microcontroller is intended and 
; supplied to you, the Company’s customer, for use solely and 
; exclusively on Microchip PICmicro Microcontroller products. The 
; software is owned by the Company and/or its supplier, and is 
; protected under applicable copyright laws. All rights are reserved. 
;  Any use in violation of the foregoing restrictions may subject the 
; user to criminal sanctions under applicable laws, as well as to 
; civil liability for the breach of the terms and conditions of this 
; license.  
;
; THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES, 
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
;***********************************************************
;	PIC18CXX2 EXAMPLE CODE FOR PICDEM-2
;
;	TITLE: USART Demo Demonstration
;	FILENAME: usart.asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program demonstrates basic functionality of the USART.
;
; Port B is connected to 8 LEDs. 
; When the PIC18C452 receives a word of data from
; the USART, the value is displayed on the LEDs and
; is retransmitted to the host computer.
;
; Set terminal program to 9600 baud, 1 stop bit, no parity

	list p=18c452	; set processor type
	list n=0		; supress page breaks in list file
	include "p18c452.inc"

;************************************************************
; Reset and Interrupt Vectors

	org	00000h	; Reset Vector
	goto	Start

	org	00008h	; Interrupt vector
	goto	IntVector

;************************************************************
; Program begins here

	org	00020h	; Beginning of program EPROM
Start
	clrf	LATB	; Clear PORTB output latches
	clrf	TRISB 	; Config PORTB as all outputs
	bcf	TRISC,6	; Make RC6 an output

	movlw	19h	; 9600 baud @4MHz
	movwf	SPBRG
	
	bsf	TXSTA,TXEN	; Enable transmit
	bsf	TXSTA,BRGH	; Select high baud rate
	
	bsf	RCSTA,SPEN	; Enable Serial Port
	bsf	RCSTA,CREN	; Enable continuous reception

	bcf	PIR1,RCIF	; Clear RCIF Interrupt Flag
	bsf	PIE1,RCIE	; Set RCIE Interrupt Enable
	bsf	INTCON,PEIE	; Enable peripheral interrupts
	bsf	INTCON,GIE	; Enable global interrupts

;************************************************************
; Main loop

Main
	goto	Main	; loop to self doing nothing

;************************************************************
; Interrupt Service Routine

IntVector
	; save context (WREG and STATUS registers) if needed.

	btfss	PIR1,RCIF	; Did USART cause interrupt?
	goto	OtherInt	; No, some other interrupt

	movlw	06h		; Mask out unwanted bits
	andwf	RCSTA,W		; Check for errors
	btfss	STATUS,Z	; Was either error status bit set?
	goto	RcvError	; Found error, flag it

	movf	RCREG,W		; Get input data
	movwf	LATB		; Display on LEDs
	movwf	TXREG		; Echo character back
	goto	ISREnd		; go to end of ISR, restore context, return
	
RcvError
	bcf	RCSTA,CREN	; Clear receiver status
	bsf	RCSTA,CREN
	movlw	0FFh		; Light all LEDs
	movwf	PORTB	
	goto	ISREnd	; go to end of ISR, restore context, return

OtherInt
	goto	$	; Find cause of interrupt and service it before returning from
			; interrupt. If not, the same interrupt will re-occur as soon
			; as execution returns to interrupted program.

ISREnd
	; Restore context if needed.
	retfie

	end

