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
; Velleman K8048 PIC18F2550 Triangle Wave Demo
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; !MCLR VPP RE3            1-----28 RB7 KBI3 PGD
; RA0 AN0                  2     27 RB6 KBI2 PGC
; RA1 AN1                  3     26 RB5 KBI1 PGM
; RA2 AN2 VREF- CVREF      4     25 RB4 KBI0 AN11
; RA3 AN3 VREF+            5     24 RB3 AN9  CCP2 VPO
; RA4 T0CKI C1OUT RCV      6     23 RB2 AN8  INT2 VMO
; RA5 AN4 !SS HLVDIN C2OUT 7     22 RB1 AN10 INT1 SCK SCL
; VSS GND                  8     21 RB0 AN12 INT0 FLT0 SDI SDA
; OSC1 CLKI                9     20 VDD VCC
; OSC2 CLKO RA6            10    19 VSS GND
; RC0 T1OSO T13CKI         11    18 RC7 RX DT SDO
; RC1 T1OSI CCP2 !UOE      12    17 RC6 TX CK
; RC2 CCP1                 13    16 RC5 D+ VP
; VUSB                     14----15 RC4 D- VM
;
; Setup
; -----
; PB7\
; .   \
; .    >===DAC 0800===>SIGNAL
; .   /
; PB0/
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F2550
#INCLUDE        "p18f2550.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
                CONFIG    PLLDIV=4
                CONFIG    CPUDIV=OSC1_PLL2
                CONFIG    USBDIV=2
                CONFIG    FOSC=XT_XT
                CONFIG    FCMEN=OFF
                CONFIG    IESO=OFF
                CONFIG    PWRT=ON
                CONFIG    BOR=OFF
                CONFIG    BORV=2
                CONFIG    WDT=OFF
                CONFIG    WDTPS=1024
                CONFIG    MCLRE=ON
                CONFIG    PBADEN=OFF
                CONFIG    CCP2MX=OFF
                CONFIG    STVREN=ON
                CONFIG    LVP=OFF
                CONFIG    DEBUG=OFF
                CONFIG    CP0=OFF
                CONFIG    CP1=OFF
                CONFIG    CP2=OFF
                CONFIG    CP3=OFF
                CONFIG    CPB=OFF
                CONFIG    CPD=OFF
                CONFIG    WRT0=OFF
                CONFIG    WRT1=OFF
                CONFIG    WRT2=OFF
                CONFIG    WRT3=OFF
                CONFIG    WRTB=OFF
                CONFIG    WRTC=OFF
                CONFIG    WRTD=OFF
                CONFIG    EBTR0=OFF
                CONFIG    EBTR1=OFF
                CONFIG    EBTR2=OFF
                CONFIG    EBTR3=OFF
                CONFIG    EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants 
;
DELAYTIML       EQU     0x01
DELAYTIMM       EQU     0x01
DELAYTIMH       EQU     0x01
;
; XTAL = 16MHz HSPLL
    CONSTANT CLOCK = 48000000
;  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
OUTPUT          EQU     0x20    ;OUTPUT SIGNAL
;
COUNTER1        EQU     0x30    ;GENERAL PURPOSE COUNTERS
COUNTER2        EQU     0x31
COUNTER3        EQU     0x32
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Macros
;
; decrement REG, goto LOC if not ZERO
DJNZ            MACRO   REG,LOC
                DECFSZ  REG,F
                GOTO    LOC
                ENDM
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Simple delay
;
DELAY           MOVLW   DELAYTIMH
                MOVWF   COUNTER3
DELAYLOOPH      MOVLW   DELAYTIMM
                MOVWF   COUNTER2
DELAYLOOPM      MOVLW   DELAYTIML
                MOVWF   COUNTER1
DELAYLOOPL      DJNZ    COUNTER1,DELAYLOOPL
                DJNZ    COUNTER2,DELAYLOOPM
                DJNZ    COUNTER3,DELAYLOOPH
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            CLRF    ADCON0              ;A/D OFF?
                MOVLW   0x7f
                MOVWF   ADCON1
                CLRF    ADCON2
                MOVLW   b'00000000'
                MOVWF   TRISB
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
; TRIANGLE WAVE @ 11.7 KHz
;
MAIN0           CLRF    PORTB               ; 0x00

MAIN1           INCF    PORTB,F             ; 0x01 => 0x80
                BTFSS   PORTB,7
                GOTO    MAIN1

                NOP
                NOP

MAIN2           DECF    PORTB,F             ; 0x7f => 0x00
                BTFSS   STATUS,Z
                GOTO    MAIN2

MAIN3           GOTO    MAIN1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
