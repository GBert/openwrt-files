;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Copyright (C) 2005-2014 Darron Broad
; All rights reserved.
; 
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
; 
; 1. Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
; 
; 2. Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
;
; 3. Neither the name `Darron Broad' nor the names of any contributors
;    may be used to endorse or promote products derived from this
;    software without specific prior written permission.
; 
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
; POSSIBILITY OF SUCH DAMAGE.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Velleman K8048 PIC16F648A Demo
;
; Increment LD1..LD6 with switch inversion control.
;
; This demo is also compatible with the PIC16F627.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; RA2 AN2   VREF 1----18 RA1 AN1
; RA3 AN3   CMP1 2    17 RA0 AN0
; RA4 T0CKI CMP2 3    16 RA7 OSC1  CLKIN
; RA5 !MCLR VPP  4    15 RA6 OSC2  CLKOUT
; VSS GND        5    14 VDD VCC
; RB0 INT        6    13 RB7 T1OSI PGD
; RB1 RX    DT   7    12 RB6 T1OSO T1CKI PGC
; RB2 TX    CK   8    11 RB5
; RB3 CCP1       9----10 RB4 PGM
;
; K8048 Pin
; ----- ---
; LD1   RB0 (6)
; LD2   RB1 (7)
; LD3   RB2 (8)
; LD4   RB3 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; PGC   RB6 (12)
; PGD   RB7 (13)
; SW1   RA0 (17)
; SW2   RA1 (18)
; SW3   RA2 (1)
; SW4   RA3 (2)
; VPP   RA5 (4)
; OSC2  RA6 (15)
; OSC1  RA7 (16)
; GND   VSS (5)
; +5V   VDD (14)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F648A
ERRORLEVEL      -302
#INCLUDE        "p16f648a.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CP_OFF & _CPD_OFF & _LVP_OFF & _BODEN_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_OFF & _INTOSC_OSC_CLKOUT 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 4MHz
    CONSTANT CLOCK = 4000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0x2100
                DE      "PIC16F648A"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x6F
    OUTPUTCNT
    OUTPUTBYTE
    OUTPUTEOR
ENDC
CBLOCK          0x70                        ;RAM 0x70..0x7F
    W_SAVE
    S_SAVE
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0004
                GOTO    INTSR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS
                CLRF    PIR1                ;CLEAR PERIPHERAL IRQ FLAGS
                CLRF    T1CON               ;RESET COUNTER CONTROL

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    PORTB               ;INIT PORT B

                CLRF    OUTPUTCNT           ;INIT LED CONTROL
                CLRF    OUTPUTBYTE          ; "
                CLRF    OUTPUTEOR           ; "

                BANKSEL BANK1

                BSF     PCON,OSCF           ;INTRC OPERATION 4MHz

                MOVLW   B'10000000'         ;DISABLE PULLUPS
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORT A SW1..SW4 I/P
                MOVWF   TRISA

                MOVLW   B'11000000'         ;PORT B LD1..LD6 O/P
                MOVWF   TRISB
; Init Timer 1
                BANKSEL BANK1

                MOVLW   0x01
                MOVWF   PIE1                ;ENABLE OVERFLOW IRQ

                BANKSEL BANK0

                MOVLW   0xC0
                MOVWF   INTCON              ;ENABLE GIE + PEIE

                MOVLW   B'00100001'
                MOVWF   T1CON               ;START TIMER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAINLOOP        BTFSC   PORTA,0             ;SW1?
                GOTO    SW1
                BTFSC   PORTA,1             ;SW2?
                GOTO    SW2
                BTFSC   PORTA,2             ;SW3?
                GOTO    SW3
                BTFSC   PORTA,3             ;SW4?
                GOTO    SW4

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
SW1             MOVLW   0x00                ;SW1 PRESSED
                MOVWF   OUTPUTEOR
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
SW2             MOVLW   0x3F                ;SW2 PRESSED
                MOVWF   OUTPUTEOR
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
SW3             MOVLW   0x38                ;SW3 PRESSED
                MOVWF   OUTPUTEOR
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
SW4             MOVLW   0x07                ;SW4 PRESSED
                MOVWF   OUTPUTEOR
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine
;
INTSR           MOVWF   W_SAVE
                MOVF    STATUS,W
                MOVWF   S_SAVE

                BANKSEL BANK0

                INCF    OUTPUTCNT,F

; ld4..ld6 mirror
;               0->0 1->4 2->2 3->6
;               4->1 5->5 6->3 7->7
                MOVLW   0
                BTFSC   OUTPUTCNT,0
                IORLW   B'00100000'
                BTFSC   OUTPUTCNT,1
                IORLW   B'00010000'
                BTFSC   OUTPUTCNT,2
                IORLW   B'00001000'
                MOVWF   OUTPUTBYTE
; ld1..ld3
                MOVF    OUTPUTCNT,W
                ANDLW   0x07
                IORWF   OUTPUTBYTE,F

                MOVF    OUTPUTBYTE,W
                XORWF   OUTPUTEOR,W
; output
                MOVWF   PORTB

                MOVF    S_SAVE,W
                MOVWF   STATUS
                MOVF    W_SAVE,W

                CLRF    PIR1
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
