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
;************************************************************
;	PIC18CXX2 EXAMPLE CODE FOR PICDEM-2
;
;	TITLE:	Button Press Demonstration
;	FILENAME:  bttn.asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program demonstrates how to read a push-button and control LED's.
;
; Port B is connected to 8 LEDs. 
; RA4 is connected to a switch (S2).
; This program increments a file register count every time S2 is pressed.
; The value of count is displayed on the LEDs connected to Port B.
; The LEDs should increment in a binary manner each time S2 is pressed.

	list p=18c452
	#include	"p18c452.inc"

;************************************************************
; variables

Count	equ	0x000
	
;************************************************************
; reset vectors
 
	org	00000h	; Reset Vector
	goto	Start

;************************************************************
;program code starts here

	org	00020h	; Beginning of program EPROM
Start
	clrf	LATB	; Clear PORTB output latch
	clrf	TRISB	; Make PORTB pins all outputs
	clrf	Count	; Clear Count
Loop
	btfsc	PORTA,4	; Has S2 been pressed? (Normally high, goes low when pressed.)
	goto	Loop	; No, check again

IncCount
	incf	Count,F		; Increment Count
	movff	Count,LATB	; move Count to PORTB
	
Debounce
	btfss	PORTA,4		; Has key been released?
	goto	Debounce	; No, wait some more
	goto	Loop		; yes, wait for next key press


	END		; directive indicates end of code

