;
; Velleman K8048 PWM Demo (Close encounters!)
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F1320
#INCLUDE        "p18f1320.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; k8048 PIC18F1320 PWM Demo (Close encounters!)
;
; This demonstrates with the aide of two external devices how to get the
; picmicro to provide an almost full range of midi notes utilising the pwm.
; See for pwm.sch for connexions.
;
; The accompanying .c file shows how to generate the necessary counter and
; post scaling details for a given oscillator frequency and timer prescaler.
;
;*****************************************************************************
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
DELAYTIML       EQU     0x00
DELAYTIMM       EQU     0x00
DELAYTIMH       EQU     0x28
;
; XTAL = 10MHz HSPLL
    CONSTANT CLOCK = 40000000
;  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
TEMP            EQU     0x28
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
                ORG     0x0008
                GOTO    INTHIGH
                ORG     0x0018
                GOTO    INTLOW
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
;
                CLRF    INTCON
                CLRF    PIR1
                CLRF    PORTB
                CLRF    TBLPTRU
;
;15.5.9 setup pwm
;
                CLRF    PORTB
                MOVLW   b'00000000'         ;PWM P1A OUTPUT ENABLED + MUX SELECT
                MOVWF   TRISB
                MOVLW   b'00001100'         ;PWM MODE ENABLED
                MOVWF   CCP1CON
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Play Midi Note
;
#INCLUDE        "notetable.inc"
NOTEON          MOVWF   TEMP                ;SAVE MIDI NOTE

                MOVLW   LOW(NOTETABLE)      ;TABLE BASE
                MOVWF   TBLPTRL
                MOVLW   HIGH(NOTETABLE)
                MOVWF   TBLPTRH
                MOVLW   NOTESTART           ;TABLE BASE= TABLE BASE + 2 * (NOTE - NOTESTART)
                SUBWF   TEMP,F
                MOVF    TEMP,W
                ADDWF   TBLPTRL,F
                CLRW
                ADDWFC  TBLPTRH,F
                MOVF    TEMP,W
                ADDWF   TBLPTRL,F
                CLRW
                ADDWFC  TBLPTRH,F

                CLRF    T2CON               ;OFF
                TBLRD*+
                MOVFF   TABLAT,PORTB        ;MUX SELECT
                TBLRD*
                MOVFF   TABLAT,PR2          ;MARK/SPACE TIME
                MOVFF   TABLAT,CCPR1L
                CLRC
                RRCF    CCPR1L,F            ;MARK TIME=MARK/SPACE TIME/2
                MOVLW   b'00000110'         ;TMR2 ON PRE/16
                MOVWF   T2CON
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAIN            CALL    INIT

MAIN1
;               CLOSE ENCOUNTERS OF THE THIRD KIND
;
                MOVLW   0x3c        ;midi 60 (C)
                CALL    NOTEON
                CALL    DELAY
                MOVLW   0x3e        ;midi 62 (D)
                CALL    NOTEON
                CALL    DELAY
                MOVLW   0x3a        ;midi 58 (A#)
                CALL    NOTEON
                CALL    DELAY
                MOVLW   0x2e        ;midi 46 (A#)
                CALL    NOTEON
                CALL    DELAY
                MOVLW   0x35        ;midi 53 (F)
                CALL    NOTEON
                CALL    DELAY

                CLRF    T2CON       ;OFF
                CALL    DELAY
                GOTO    MAIN1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine (unused)
;
INTHIGH
INTLOW          RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
