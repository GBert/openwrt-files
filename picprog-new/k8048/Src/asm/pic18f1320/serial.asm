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
; Velleman K8048 PIC18F1320 Serial Demo
;
; This demonstrates the PIC18F1320 communicating with two different
; serial devices:
;
; 1. TLC5620 4 OUTPUT DAC
; 2. DS1267  PROGRAMMABLE POTENTIOMETER
;
; THESE DEVICES USE DIFFERENT SERIAL INTERFACE PROTOCOLS AND IN THIS
; DEMO THE DATA AND CLOCK LINES ARE SHARED BETWEEN THE TWO AND
; THE CONTROL LINES ARE SEPARATE.
;
; XXX DUE TO BCF/BSF BEING R/W/M THESE ROUTINES NEED VERIFICATION
;     ON TARGET TO DETERMINE IF I/O ROUTINES ARE SUCCESSFUL.
;     ON DEMO BREADBOARD THERE ARE NO ISSUES OTHER THAN A DELAY
;     AFTER SETTING RA4 OPEN DRAIN PULLUP PRIOR TO NEXT R/W/M
;     CYCLE.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F1320
#INCLUDE        "p18f1320.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
                CONFIG  OSC=HSPLL
                CONFIG  FSCM=OFF
                CONFIG  IESO=OFF
                CONFIG  PWRT=ON
                CONFIG  BOR=OFF
                CONFIG  BORV=27
                CONFIG  WDT=OFF
                CONFIG  WDTPS=1024
                CONFIG  MCLRE=ON
                CONFIG  STVR=ON
                CONFIG  LVP=OFF
                CONFIG  DEBUG=OFF
                CONFIG  CP0=OFF
                CONFIG  CP1=OFF
                CONFIG  CPB=OFF
                CONFIG  CPD=OFF
                CONFIG  WRT0=OFF
                CONFIG  WRT1=OFF
                CONFIG  WRTB=OFF
                CONFIG  WRTC=OFF
                CONFIG  WRTD=OFF
                CONFIG  EBTR0=OFF
                CONFIG  EBTR1=OFF
                CONFIG  EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
DACADDRA        EQU     0x00                ;DAC A..D ADDRESS
DACADDRB        EQU     0x01
DACADDRC        EQU     0x02
DACADDRD        EQU     0x03
;
DELAYTIML       EQU     0x00
DELAYTIMM       EQU     0x00
DELAYTIMH       EQU     0x02
;
; XTAL = 10MHz HSPLL
    CONSTANT CLOCK = 40000000
;  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
TEMP1           EQU     0x20    ;VOLATILE
TEMP2           EQU     0x21    ;VOLATILE
;
DACA            EQU     0x22    ;DAC A SETTING
DACB            EQU     0x23    ;DAC B SETTING
DACC            EQU     0x24    ;DAC C SETTING
DACD            EQU     0x25    ;DAC D SETTING
;
POT0            EQU     0x26    ;POT0 SETTING
POT1            EQU     0x27    ;POT1 SETTING
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
                GOTO    MAIN
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
INIT            CLRF    ADCON0              ;A/D OFF
                MOVLW   0x7f
                MOVWF   ADCON1
                CLRF    ADCON2

                MOVLW   b'00001100'         ;RESET !RST, SET LDAC, SET LOAD, RESET DATA, RESET CLK
                MOVWF   PORTA
                MOVLW   b'00000000'         ;RA4:!RST RA3:LDAC RA2:LOAD RA1:DATA RA0:CLK
                MOVWF   TRISA
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set DAC
;
SETDAC          MOVWF   TEMP2
                BTFSC   TEMP2,1             ;A1
                BSF     PORTA,1
                BSF     PORTA,0             ;CLOCK IN A1
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA

                BTFSC   TEMP2,0             ;A0
                BSF     PORTA,1
                BSF     PORTA,0             ;CLOCK IN A0
                BCF     PORTA,0

                BSF     PORTA,1             ;RNG X2
                BSF     PORTA,0             ;CLOCK IN RNG
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA

                MOVLW   0x08                ;D7..D0
                MOVWF   TEMP2
DATA0           RLCF    TEMP1,F
                BNC     DATA1
                BSF     PORTA,1
DATA1           BSF     PORTA,0             ;CLOCK IN DN
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA
                DJNZ    TEMP2,DATA0

LOAD            BCF     PORTA,2
                NOP
                NOP
                BSF     PORTA,2
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set DACs
;
SETDACS         MOVF    DACA,W
                MOVWF   TEMP1
                MOVLW   DACADDRA
                CALL    SETDAC

                MOVF    DACB,W
                MOVWF   TEMP1
                MOVLW   DACADDRB
                CALL    SETDAC

                MOVF    DACC,W
                MOVWF   TEMP1
                MOVLW   DACADDRC
                CALL    SETDAC

                MOVF    DACD,W
                MOVWF   TEMP1
                MOVLW   DACADDRD
                CALL    SETDAC

LDAC            BCF     PORTA,3
                NOP
                NOP
                BSF     PORTA,3
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set POTs
;
SETPOTS         BSF     PORTA,4             ;RST HIGH (RA4 IS OPEN DRAIN, USE 2K2 PULLUP)
                NOP                         ;WAIT FOR PULLUP (READ/WRITE/MODIFY)
                NOP                         ; "    "    "
                NOP                         ; "    "    "

                BCF     PORTA,1             ;STACK SELECT DISABLE
                BSF     PORTA,0             ;CLOCK IN STACK SELECT
                BCF     PORTA,0

                MOVF    POT1,W
                MOVWF   TEMP1
                MOVLW   0x08
                MOVWF   TEMP2

SETPOT0         RLCF    TEMP1,F
                BNC     SETPOT1
                BSF     PORTA,1
SETPOT1         BSF     PORTA,0             ;CLOCK IN DATA
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA
                DJNZ    TEMP2,SETPOT0

                MOVF    POT0,W
                MOVWF   TEMP1
                MOVLW   0x08
                MOVWF   TEMP2

SETPOT2         RLCF    TEMP1,F
                BNC     SETPOT3
                BSF     PORTA,1
SETPOT3         BSF     PORTA,0             ;CLOCK IN DATA
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA
                DJNZ    TEMP2,SETPOT2

                BCF     PORTA,4             ;RST LOW
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAIN            CALL    INIT

                CLRF    DACA
                CLRF    DACB
                CLRF    DACC
                CLRF    DACD

                CLRF    POT0
                CLRF    POT1

MAIN1           CALL    SETDACS
                CALL    SETPOTS

                INCF    DACA
                INCF    POT0

                CALL    DELAY

MAIN2           GOTO    MAIN1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine
;
INTSR           RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
