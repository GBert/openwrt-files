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
; Velleman K8048 PIC16F627 Walsh Demo
;
; Demonstrate Walsh Functions on PB0(msb)->PB3(lsb) (R2R required) 
;
; http://www.milton.arachsys.com/nj71/index.php?menu=2&submenu=2&subsubmenu=5&page=5
;   n   Sine    Saw Triangle
;   0   0.0     0.0     0.0
; * 1   0.663   0.500   0.250
; * 2   0.063   0.0     1.0
; * 3   0.0     0.250   0.0
;   4   0.0     0.0     0.0
;   5   -0.263  0.0     0.0
; * 6   0.025   0.0     0.50
; * 7   0.0     0.125   0.0
;   8   0.0     0.0     0.0
;   9   -0.052  0.0     0.0
;   10  -0.006  0.0     0.0
;   11  0.0     0.0     0.0
;   12  0.0     0.0     0.0
;   13  -0.126  0.0     0.0
;   14  0.013   0.0     0.25
;   15  0.0     0.063   0.0
; *=coefficient used (sine not supported)
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
                LIST    P=PIC16F627
ERRORLEVEL      -302
#INCLUDE        "p16f627.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _BODEN_OFF & _CP_OFF & _CPD_OFF & _PWRTE_ON & _WDT_OFF & _LVP_OFF & _MCLRE_ON & _INTRC_OSC_CLKOUT
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
    CONSTANT WALSH1 = B'11110000'           ;WALSH FUNCTION BIT STREAMS
    CONSTANT WALSH2 = B'11000011'
    CONSTANT WALSH3 = B'11001100'
    CONSTANT WALSH6 = B'10100101'
    CONSTANT WALSH7 = B'10101010'
    CONSTANT TIMERRELOAD = 0x80             ;SAMPLE RATE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; EEPROM
;
                ORG     0x2100
                DE      "PIC16F627",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x6F
    OUT_1000 ;ROTATED OUTPUT STREAMS
    OUT_0500
    OUT_0250
    OUT_0125
    BUF_1000 ;OUTPUT STREAM RELOAD CACHE
    BUF_0500
    BUF_0250
    BUF_0125
    COUNTER  ;BIT COUNTER
    PORTBITS ;OUTPUT BITS
ENDC
CBLOCK          0x70                        ;RAM 0x70..0x7F
    W_SAVE
    S_SAVE
    DELAY1
    DELAY2
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
; SAWTOOTH
;
; WALSH1 x 0.500 *
; WALSH2 x 0.000
; WALSH3 x 0.250 *
; WALSH6 x 0.000
; WALSH7 x 0.125 *
;
SAWTOOTH        MOVLW   WALSH1
                MOVWF   BUF_0500
                MOVLW   WALSH3
                MOVWF   BUF_0250
                MOVLW   WALSH7
                MOVWF   BUF_0125
                CLRF    BUF_1000
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; SQUARE
;
; WALSH1 x 1.000 *
; WALSH2 x 0.000
; WALSH3 x 0.000
; WALSH6 x 0.000
; WALSH7 x 0.000
;
SQUARE          MOVLW   WALSH1
                MOVWF   BUF_1000
                CLRF    BUF_0500
                CLRF    BUF_0250
                CLRF    BUF_0125
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; TRIANGLE
;
; WALSH1 x 0.250 *
; WALSH2 x 1.000 *
; WALSH3 x 0.000
; WALSH6 x 0.500 *
; WALSH7 x 0.000
;
TRIANGLE        MOVLW   WALSH1
                MOVWF   BUF_0250
                MOVLW   WALSH2
                MOVWF   BUF_1000
                MOVLW   WALSH6
                MOVWF   BUF_0500
                CLRF    BUF_0125
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Start timer0
;
TIMERON         CLRF    COUNTER

                MOVLW   TIMERRELOAD
                MOVWF   TMR0

                MOVLW   0xA0
                MOVWF   INTCON              ;INT TMR0 ON
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    PORTA               ;INIT PORT A
                CLRF    PORTB               ;INIT PORT B

                BANKSEL BANK1

                BSF     PCON,OSCF           ;INTRC OPERATION 4MHz

                MOVLW   B'10000000'         ;SET OPTIONS (DISABLE PULLUPS)
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORT A ALL I/P
                MOVWF   TRISA

                MOVLW   B'00000000'         ;PORT B ALL O/P
                MOVWF   TRISB

                BANKSEL BANK0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CALL    SAWTOOTH
                CALL    TIMERON
MAINLOOP
                BTFSC   PORTA,0             ;SW1?
                GOTO    DOSW1

                BTFSC   PORTA,1             ;SW2?
                GOTO    DOSW2

                BTFSC   PORTA,2             ;SW3?
                GOTO    DOSW3

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW1           CALL    SAWTOOTH

SW1DELAY        MOVLW   0x20
                MOVWF   DELAY1
                CLRF    DELAY2
SW1DELAYLOOP    DJNZ    DELAY2,SW1DELAYLOOP
                DJNZ    DELAY1,SW1DELAYLOOP

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW2           CALL    SQUARE

                GOTO    SW1DELAY
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW3           CALL    TRIANGLE

                GOTO    SW1DELAY
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine
;
INTSR           MOVWF   W_SAVE
                MOVF    STATUS,W
                MOVWF   S_SAVE

                BANKSEL BANK0
; output bits
                CLRF    PORTBITS

                RRF     OUT_0125,F
                RLF     PORTBITS,F

                RRF     OUT_0250,F
                RLF     PORTBITS,F

                RRF     OUT_0500,F
                RLF     PORTBITS,F

                RRF     OUT_1000,F
                RLF     PORTBITS,F

                MOVF    PORTBITS,W
                MOVWF   PORTB

; update counter
                INCF    COUNTER,F
                BTFSS   COUNTER,3
                GOTO    RESETTIMER

; reload output streams
                MOVF    BUF_1000,W
                MOVWF   OUT_1000
                MOVF    BUF_0500,W
                MOVWF   OUT_0500
                MOVF    BUF_0250,W
                MOVWF   OUT_0250
                MOVF    BUF_0125,W
                MOVWF   OUT_0125
                CLRF    COUNTER

; reset timer
RESETTIMER      MOVLW   TIMERRELOAD
                MOVWF   TMR0

                MOVF    S_SAVE,W
                MOVWF   STATUS
                MOVF    W_SAVE,W

                BCF     INTCON,T0IF
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
