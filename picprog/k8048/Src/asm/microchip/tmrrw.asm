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
;	TITLE: Timer Read/Write Demonstration
;	FILENAME: tmrrw.asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program uses Timer1 and Timer3 to demonstrate the use of
; 8 and 16 bit write modes.  
;
; The counters are used to maintain
; overflow count registers (similar to a RTCC).  The 16 bit write to 
; Timer3 will introduce an error between the overflow registers.  
; This error is calculated in the main program loop and displayed on 
; the PORTB LEDS.

	list p=18c452, n=48, t=ON, st=OFF
	#include "p18c452.inc"
		
;-------------------18C452 RAM LOCATIONS------------------------------

T1COUNT            EQU      0x0000
T3COUNT            EQU      0x0001


;------------------BIT DEFINITIONS------------------------------------

F                 EQU      0x0001

;------------------VECTORS--------------------------------------------

	org		0x000000	; reset vector
	bra		START
	
	org		0x000008	; high priority interrupt vector
	bra		TMR_ISR
   
	org		0x000018	; low priority interrupt vector
	bra		START


;--------------------PROGRAM-----------------------------------

START

	rcall	INIT

; Setup Timer1

	clrf     T1CON
	bsf	 T1CON,T1CKPS1		; set prescaler 1:8
	bsf	 T1CON,T1CKPS0		; set prescaler 1:8
	bsf      T1CON,RD16		; enable 16 bit read/write mode
	movlw    0x80			; initialize TMR1 with 8000
	movwf    TMR1H			;
	clrf     TMR1L			;
	bsf      T1CON,TMR1ON		; turn on TMR1

; Setup Timer3

   	clrf     T3CON
   	bsf      T3CON,TMR3CS
   	bsf      T3CON,RD16
	bsf	 T3CON,T3CKPS1		; set prescaler 1:8
	bsf	 T3CON,T3CKPS0		; set prescaler 1:8
  	bcf      T3CON,RD16		; enable 8 bit read/write mode
   	movlw    0x80			; initialize TMR3 with 8000
   	movwf    TMR3H
   	clrf     TMR3L
   	bsf      T3CON,TMR3ON
   		  
   
MLOOP

; Subtract T3COUNT from T1COUNT
; and write the result to PORTB LEDS.

	movf     T3COUNT,W
	subwf    T1COUNT,W
	movwf    PORTB
                
	bra      MLOOP

;-------------------------------SUBROUTINES---------------------------------

TMR_ISR

	btfsc    PIR1,TMR1IF       ; check which timer caused the interrupt
	bra      T1_HANDLER
   
	btfsc    PIR2,TMR3IF
	bra      T3_HANDLER
   
	retfie
   
T1_HANDLER
; Load Timer1 with 0x8000 by writing to the high byte only.
; Increment the T1COUNT register.
; Clear the Timer1 interrupt flag.

	movlw    0x80
	movwf    TMR1H
	incf     T1COUNT
	bcf      PIR1,TMR1IF
   
   retfie
   
T3_HANDLER

; Load Timer3 with 0x8000 by performing a 16-bit timer write.
; Increment the T3COUNT register.
; Clear the Timer3 interrupt flag.

	movlw    0x80
	movwf    TMR3H
	clrf     TMR3L
	incf     T3COUNT
	bcf      PIR2,TMR3IF
 
   retfie   


;-----------------------------SUBROUTINES----------------------------------

INIT
   clrf     T1COUNT
   clrf     T3COUNT

   bsf      DDRA,4               ; porta

   clrf     PORTB                ; setup portb for outputs
   clrf     DDRB
   
   bsf      PIE1,TMR1IE          ; enable interrupts
   bsf      PIE2,TMR3IE
   bsf      INTCON,PEIE          ; enable peripheral interrupts
   bsf      INTCON,GIE           ; enable global interrupts
   return

	END	
