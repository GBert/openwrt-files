;
; Velleman K8048 Midi Synthesizer
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
; K8048 PIC18F1320 Midi Symthesizer
;
; A simple synthesizer with envelope control (DS1267)
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
;Delay counter presets
DELAYTIML       EQU     0x00
DELAYTIMM       EQU     0x00
DELAYTIMH       EQU     0x28

;Voice envelope control bits (set if active)
ATTACKBIT       EQU     0       ;ATTACK  (1)
DECAYBIT        EQU     1       ;DECAY   (2)
SUSTAINBIT      EQU     2       ;SUSTAIN (4)
RELEASEBIT      EQU     3       ;RELEASE (8)

;Tmr3 reload
;
; period=10ms with 100ns instruction cycle time
; prescaler=8 giving 800ns prescaler period
;
;   10ms  / 800n  = 12500
;   65536 - 12500 = 53036
TMR3RELOAD      EQU     .53036
;
; XTAL = 10MHz HSPLL
    CONSTANT CLOCK = 40000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; EEPROM
;
                ORG     0xF00000
;Demo
PROGRAM0        ;ATTACK  DURATION
                ;ATTACK  INCREMENT
                ;DECAY   DURATION
                ;DECAY   DECREMENT
                ;SUSTAIN DURATION LOW
                ;SUSTAIN DURATION HIGH
                ;RELEASE DURATION
                ;RELEASE DECREMENT
                DB      .16,.16,    .32,.2,     .10,.0,     .6,.32
                DB      .16,.16,    .32,.2,     .15,.0,     .6,.32
                DB      .16,.16,    .32,.2,     .20,.0,     .6,.32
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;

; Interrupt
;Interrupt saves
W_SAVE          EQU     0x00
S_SAVE          EQU     0x01
B_SAVE          EQU     0x02

;Volatile temporary variables (interrupt)
ITEMP1          EQU     0x08    ;VOLATILE
ITEMP2          EQU     0x09    ;VOLATILE
ITEMP3          EQU     0x0a    ;VOLATILE

;Potentiometer settings
POT0            EQU     0x10    ;POT0 SETTING
POT1            EQU     0x11    ;POT1 SETTING

;Program envelope control
ENVCTRL         EQU     0x18    ;ENVELOPE CONTROL BITS
ENVCNTLOW       EQU     0x19    ;ENVELOPE COUNTER LOW BYTE
ENVCNTHIGH      EQU     0x1a    ;ENVELOPE COUNTER HIGH BYTE

; Main
;Volatile temporary variables (main)
MTEMP1          EQU     0x20    ;VOLATILE
MTEMP2          EQU     0x21    ;VOLATILE
MTEMP3          EQU     0x22    ;VOLATILE

;Program envelope (copy of EEPROM)
ATTACKDUR       EQU     0x30    ;ATTACK  DURATION
ATTACKINC       EQU     0x31    ;ATTACK  INCREMENT/INITIAL ATTACK VOLUME
DECAYDUR        EQU     0x32    ;DECAY   DURATION
DECAYDEC        EQU     0x33    ;DECAY   DECREMENT
SUSTAINDURL     EQU     0x34    ;SUSTAIN DURATION LOW
SUSTAINDURH     EQU     0x35    ;SUSTAIN DURATION HIGH
RELEASEDUR      EQU     0x36    ;RELEASE DURATION
RELEASEDEC      EQU     0x37    ;RELEASE DECREMENT

;Note velocity
VELOCITY        EQU     0x38
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
; Initialise
;
INIT            CLRF    ADCON0              ;A/D OFF
                MOVLW   0x7f
                MOVWF   ADCON1
                CLRF    ADCON2
;reset
                CLRF    INTCON
                CLRF    PIR1
                CLRF    TBLPTRU
;setup ports
                MOVLW   b'00001100'         ;RESET !RST, SET LDAC, SET LOAD, RESET DATA, RESET CLK
                MOVWF   PORTA
                MOVLW   b'00000000'         ;RA4:!RST RA3:LDAC RA2:LOAD RA1:DATA RA0:CLK
                MOVWF   TRISA
                
                MOVLW   b'00000000'         ;RESET ALL
                MOVWF   PORTB
                MOVLW   b'00000000'         ;PWM P1A OUTPUT ENABLED + MUX SELECT
                MOVWF   TRISB
;setup pwm
                MOVLW   b'00001100'         ;PWM MODE ENABLED
                MOVWF   CCP1CON

;setup int (Tmr3)
                MOVLW   b'00000010'
                MOVWF   PIE2
                MOVLW   b'11000000'
                MOVWF   INTCON
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main (Demo tune) (no note off for now)
;
MAIN            CALL    INIT

                MOVLW   0xFF
                MOVWF   VELOCITY

;*****************************************************************************
; CLOSE ENCOUNTERS OF THE THIRD KIND
;*****************************************************************************
MAIN1           MOVLW   0x00        ;short sustain
                CALL    PROGRAMCHANGE

                MOVLW   0x3c        ;midi 60 (C)
                CALL    NOTEON
                CALL    NOTEWAIT
                MOVLW   0x3e        ;midi 62 (D)
                CALL    NOTEON
                CALL    NOTEWAIT
                MOVLW   0x3a        ;midi 58 (A#)
                CALL    NOTEON
                CALL    NOTEWAIT
                
                MOVLW   0x01        ;medium sustain
                CALL    PROGRAMCHANGE

                MOVLW   0x2e        ;midi 46 (A#)
                CALL    NOTEON
                CALL    NOTEWAIT
                
                MOVLW   0x02        ;long sustain
                CALL    PROGRAMCHANGE

                MOVLW   0x35        ;midi 53 (F)
                CALL    NOTEON
                CALL    NOTEWAIT
                
                CALL    DELAY

                MOVLW   .21
                CALL    NOTEON
                CALL    NOTEWAIT

                CALL    DELAY

                MOVLW   .108
                CALL    NOTEON
                CALL    NOTEWAIT
                
                CALL    DELAY

                GOTO    MAIN1

NOTEWAIT        MOVF    ENVCTRL,W
                BNZ     NOTEWAIT
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt (10ms approx)
;
INTHIGH
INTLOW
                MOVWF   W_SAVE              ;SAVE WORKING REGISTER
                MOVFF   BSR,B_SAVE          ;SAVE BANK SELECT
                MOVFF   STATUS,S_SAVE       ;SAVE STATUS

;next envelope interrupt (in 10ms) (may be cancelled in release)
                CLRF    PIR2
                MOVLW   HIGH(TMR3RELOAD)
                MOVWF   TMR3H
                MOVLW   LOW(TMR3RELOAD)
                MOVWF   TMR3L
                
;*****************************************************************************
;envelope control
;*****************************************************************************
                BTFSC   ENVCTRL,RELEASEBIT
                GOTO    DORELEASE
                BTFSC   ENVCTRL,SUSTAINBIT
                GOTO    DOSUSTAIN
                BTFSC   ENVCTRL,DECAYBIT
                GOTO    DODECAY

;*****************************************************************************
;attack
;*****************************************************************************
DOATTACK        MOVF    ATTACKINC,W
                ADDWF   POT0
                BNC     DOATTACK0
                MOVLW   0xff
                MOVWF   POT0
DOATTACK0       DECF    ENVCNTLOW
                BNZ     INTSETVOLUME
                BSF     ENVCTRL,DECAYBIT
                MOVFF   DECAYDUR,ENVCNTLOW
                GOTO    INTSETVOLUME

;*****************************************************************************
;decay
;*****************************************************************************
DODECAY         MOVF    DECAYDEC,W
                SUBWF   POT0
                BC      DODECAY0
                CLRF    POT0
DODECAY0        DECF    ENVCNTLOW
                BNZ     INTSETVOLUME
                BSF     ENVCTRL,SUSTAINBIT
                MOVFF   SUSTAINDURL,ENVCNTLOW
                MOVFF   SUSTAINDURH,ENVCNTHIGH
                GOTO    INTSETVOLUME

;*****************************************************************************
;sustain
;*****************************************************************************
DOSUSTAIN       DECF    ENVCNTLOW
                BNZ     INTRETURN
                MOVF    ENVCNTHIGH,W
                BZ      DOSUSTAIN0
                DECF    ENVCNTHIGH
                GOTO    INTRETURN
DOSUSTAIN0      BSF     ENVCTRL,RELEASEBIT
                MOVFF   RELEASEDUR,ENVCNTLOW
                GOTO    INTRETURN

;*****************************************************************************
;release
;*****************************************************************************
DORELEASE       MOVF    RELEASEDEC,W
                SUBWF   POT0
                BC      DORELEASE0
                CLRF    POT0
DORELEASE0      DECF    ENVCNTLOW
                BNZ     INTSETVOLUME
RELEASEDONE     CLRF    ENVCTRL
                CLRF    T2CON
                CLRF    T3CON
INTSETVOLUME    CALL    SETVOLUME
INTRETURN       MOVF    W_SAVE,W            ;RESTORE WORKING REGISTER
                MOVFF   B_SAVE,BSR          ;RESTORE BANK SELECT
                MOVFF   S_SAVE,STATUS       ;RESTORE STATUS
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Select program envelope from eeprom (0..31)
;
PROGRAMCHANGE   LFSR    FSR0,ATTACKDUR      ;RAM ADDRESS
                MOVWF   MTEMP1
                CLRC
                RLCF    MTEMP1
                RLCF    MTEMP1
                RLCF    MTEMP1
                MOVFF   MTEMP1,EEADR        ;EEPROM ADDRESS
                CLRF    EECON1              ;ACCESS EEPROM

;copy 8 byte envelope from eeprom to ram
                MOVLW   0x08
                MOVWF   MTEMP1
PCHANGE0        BSF     EECON1,RD           ;READ EEPROM
                MOVFF   EEDATA,POSTINC0
                INCF    EEADR
                DJNZ    MTEMP1,PCHANGE0
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Play Midi Note
;
#INCLUDE        "notetable.inc"
NOTEON          MOVWF   MTEMP1
                MOVLW   LOW(NOTETABLE)
                MOVWF   TBLPTRL
                MOVLW   HIGH(NOTETABLE)
                MOVWF   TBLPTRH
                MOVLW   NOTESTART
                SUBWF   MTEMP1,F
                MOVF    MTEMP1,W
                ADDWF   TBLPTRL,F
                CLRW
                ADDWFC  TBLPTRH,F
                MOVF    MTEMP1,W
                ADDWF   TBLPTRL,F
                CLRW
                ADDWFC  TBLPTRH,F
;all off
                CLRF    ENVCTRL
                CLRF    T2CON
                CLRF    T3CON
;setup mux
                TBLRD*+
                MOVFF   TABLAT,PORTB
;setup pwm
                TBLRD*
                MOVFF   TABLAT,PR2
                MOVFF   TABLAT,CCPR1L
                CLRC
                RRCF    CCPR1L,F
                MOVLW   b'00000110'
                MOVWF   T2CON

;setup envelope interrupt (in 10ms)
                BSF     ENVCTRL,ATTACKBIT
                MOVFF   ATTACKDUR,ENVCNTLOW
                MOVFF   ATTACKINC,POT0
                CALL    SETVOLUME

                MOVLW   HIGH(TMR3RELOAD)    ;RD16 HIGH/LOW WRITE ORDER
                MOVWF   TMR3H
                MOVLW   LOW(TMR3RELOAD)
                MOVWF   TMR3L
                MOVLW   b'10110001'         ;START TIMER RD16+/PRE/8
                MOVWF   T3CON
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Simple delay
;
DELAY           MOVLW   DELAYTIMH
                MOVWF   MTEMP1
DELAYLOOPH      MOVLW   DELAYTIMM
                MOVWF   MTEMP2
DELAYLOOPM      MOVLW   DELAYTIML
                MOVWF   MTEMP3
DELAYLOOPL      DJNZ    MTEMP3,DELAYLOOPL
                DJNZ    MTEMP2,DELAYLOOPM
                DJNZ    MTEMP1,DELAYLOOPH
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set volume
;
SETVOLUME       MOVF    VELOCITY,W
                MULWF   POT0
                MOVFF   PRODH,POT0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set Potentiometers (DS1267)
;
SETPOTS         BSF     PORTA,4             ;RST HIGH (RA4 IS OPEN DRAIN, USE 2K2 PULLUP)
                NOP                         ;WAIT FOR PULLUP (READ/WRITE/MODIFY)
                NOP                         ; "    "    "
                NOP                         ; "    "    "
                BCF     PORTA,1             ;STACK SELECT DISABLE
                BSF     PORTA,0             ;CLOCK IN STACK SELECT
                BCF     PORTA,0
                MOVF    POT1,W
                MOVWF   ITEMP1
                MOVLW   0x08
                MOVWF   ITEMP2
SETPOT0         RLCF    ITEMP1,F
                BNC     SETPOT1
                BSF     PORTA,1
SETPOT1         BSF     PORTA,0             ;CLOCK IN DATA
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA
                DJNZ    ITEMP2,SETPOT0
                MOVF    POT0,W
                MOVWF   ITEMP1
                MOVLW   0x08
                MOVWF   ITEMP2
SETPOT2         RLCF    ITEMP1,F
                BNC     SETPOT3
                BSF     PORTA,1
SETPOT3         BSF     PORTA,0             ;CLOCK IN DATA
                BCF     PORTA,0
                BCF     PORTA,1             ;RESET DATA
                DJNZ    ITEMP2,SETPOT2
                BCF     PORTA,4             ;RST LOW
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
