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
;	TITLE: Table Read Demonstration
;	FILENAME: table.asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program demonstrates the use of the TBLRD instruction
; to read program memory.  
;
; A pre-defined sequence of data
; bytes is included in program memory.  Each time the button
; on pin RA4 is pressed, the next byte of data is accessed
; and displayed on the PORTB LEDS.  When the beginning or end of 
; the data is reached, the direction of access is reversed. 

	list p=18c452, n=48, t=ON, st=OFF
	#include "p18c452.inc"

;-------------------18C452 RAM LOCATIONS------------------------------

DIRFLAG            EQU      0x0000

;-------------------18C452 ROM LOCATIONS------------------------------

TABLADDR          EQU      0x0003000

;------------------BIT DEFINITIONS------------------------------------

KEY               EQU      4

;------------------VECTORS--------------------------------------------

 ORG		0x000000	; rest vector
 GOTO		START
	
 ORG		0x000008	; high priority interrupt vector
 GOTO		START
   
 ORG		0x000018	; low priority interrupt vector
 GOTO		START


;--------------------PROGRAM-----------------------------------


START

   bsf		DDRA,4		; porta button input

   clrf		PORTB		; setup portb for outputs
   clrf		DDRB
   
   clrf		DIRFLAG

; Code should be written here to intialize the table pointer registers
; to the first location of the data stored in program memory.  Use 
; the appropriate assembler directives to accomplish this.
; (Refer to the 'DATA' statements in this source code.)

   movlw	UPPER(TABLADDR)
   movwf	TBLPTRU
   movlw	HIGH(TABLADDR)
   movwf	TBLPTRH
   movlw	LOW(TABLADDR)
   movwf	TBLPTRL
	   
MLOOP                            
   btfsc    PORTA,KEY		; keypress routine
   bra      $ - 2		; decrenment PC by 2 because
   btfss    PORTA,KEY		; of byte addressing!
   bra      $ - 2
   
   rcall    CHECK_ADDR
   
   btfsc    DIRFLAG,0
   bra      RD_DOWN

RD_UP

; Code here does a table read, post-increments, and writes the
; value to the PORTB LEDS
   
   tblrd*+
   movff    TABLAT,PORTB
      
   bra      MLOOP   
   
RD_DOWN

; Code here does a table read, post-decrements, and writes the 
; value to the PORTB LEDS

   tblrd*-  
   movff    TABLAT,PORTB
           
   bra      MLOOP
   
;-----------------------------SUBROUTINES--------------------------

CHECK_ADDR
   movlw    LOW(TABEND) - 1
   subwf    TBLPTRL,W
   bnz      CHECK_LOW
   setf     DIRFLAG
   return
   
CHECK_LOW
   movlw    LOW(TABLADDR)
   subwf    TBLPTRL,W
   bnz      DONE_CHECK
   clrf     DIRFLAG
DONE_CHECK
   return


;-------------------------------DATA---------------------------------------

   org TABLADDR
   
   DATA  0x0201,0x0804,0x2010,0x8040      ; 0x003000 - 0x003007
   DATA  0x4281,0x1824,0x2400,0x8142      ; 0x003008 - 0x00300F
   DATA  0x1211,0x1814,0x2818,0x8848      ; 0x003010 - 0x003017
   DATA  0xaa55,0xaa55,0x0100,0x0703      ; 0x003018 - 0x00301F
   DATA  0x1F0F,0x7F3F,0xFFFF,0xFFFF      ; 0x003020 - 0x003027
   
TABEND

	END	
