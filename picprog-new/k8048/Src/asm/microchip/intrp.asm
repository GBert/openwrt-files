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
;	TITLE: Interrupt Priority Demonstration
;	FILENAME: intrp_ex .asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program uses Timer1 and Timer3 to demonstrate the use of
; interrupt priority.  
;
; Timer1 is configured for high-priority interrupts
; and Timer3 is configured for low-priority interrupts.  By writing
; to the PORTB LEDS, it is shown that a high-priority interrupts
; override low-priority interrupts.

	list p=18c452, n=48, t=ON, st=OFF
	#include "p18c452.inc"
		
;------------------BIT DEFINITIONS------------------------------------

F                 EQU      0x0001

;------------------VECTORS--------------------------------------------

 org	0x000000		; reset vector
 bra	START
	
 org	0x000008		; high priority interrupt vector
 bra	TMR1_ISR
   
 org	0x000018		; low priority interrupt vector
 bra	TMR3_ISR

;--------------------PROGRAM-----------------------------------
START

   rcall    INIT

;Set up priority interrupts.
	bsf	RCON,IPEN	;enable priority interrupts.
	bsf	IPR1,TMR1IP	;set Timer1 as a high priority interrupt source
	bcf	IPR2,TMR3IP	;set Timer3 as a low priority interrupt source
	bcf	PIR1,TMR1IF	;clear the Timer1 interrupt flag
	bcf	PIR2,TMR3IF	;clear the Timer3 interrupt flag
	bsf	PIE1,TMR1IE	;enable Timer1 interrupts
	bsf	PIE2,TMR3IE	;enable Timer3 interrupts
	bsf	INTCON,GIEH	;set the global interrupt enable bits
	bsf	INTCON,GIEL	;	"
  
;Timer1 setup
	clrf	T1CON
	clrf	TMR1H		;clear Timer1 high
	clrf	TMR1L		;clear Timer1 low
	bsf	T1CON,TMR1ON	;turn on Timer1
   
;Timer3 setup
	clrf	T3CON
	movlw	0xF0
	movwf	TMR3H		;write 0xf000 to Timer3
	clrf	TMR3L
	bsf	T3CON,TMR3ON	;turn on Timer3
		  
MLOOP                                                  
	goto	MLOOP
   
;-------------------------------SUBROUTINES---------------------------------
TMR1_ISR			; high priority isr                      

	bcf	PIR1,TMR1IF	;Clear the Timer1 interrupt flag.
	bcf	PORTB,0		;Turn off PORTB<0> to indicate high priority
				; interrupt has overridden low priority.
	bsf	PORTB,7		;Turn on PORTB<7> to indicate high priority 
				; interrupt is occuring.
T1POLL
	btfss	PIR1,TMR1IF	;Poll TMR11 interrupt flag to wait for another
				; TMR1 overflow.
	bra	T1POLL		
	bcf	PIR1,TMR1IF	;Clear the Timer1 interrupt flag again.
	bcf	PORTB,7		;Turn off PORTB<7> to indicate the 
				; high-priority ISR is over.
	retfie


TMR3_ISR			;low priority isr			
	bcf	PIR2,TMR3IF	;Clear the TMR3 interrupt flag.
	movlw	0xF0		;Load TMR3 with the value 0xF000
	movwf	TMR3H		
	clrf	TMR3L
	bsf	PORTB,0		;Turn on PORTB<0> to indicate low priority
				; interrupt is occurring.
T3POLL
	btfss	PIR2,TMR3IF	;Poll TMR3 interrupt flag to wait for another TMR3 overflow.
	bra	T3POLL
	movlw	0xF0		;Load TMR3 with the value 0xF000 again.
	movwf	TMR3H
	clrf	TMR3L
	bcf	PIR2,TMR3IF	;Clear the Timer3 interrupt flag again.
	bcf	PORTB,0		;Turn off PORTB<0> to indicate the low-priority ISR is over.
      
	retfie


INIT
	clrf	PORTB		; setup portb for outputs
	clrf	DDRB
      
	return

	END	
