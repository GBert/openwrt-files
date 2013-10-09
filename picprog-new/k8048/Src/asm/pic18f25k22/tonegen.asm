;
; Simple tone generator
;
; Copyright (C) 2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This file may be assembled with gpasm-0.15.0 and later.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; !MCLR VPP RE3                  1-----28 RB7 RX2 DT2 IOC PULLUP PGD
; RA0 AN0 C12IN0-                2     27 RB6 TX2 CK2 IOC PULLUP PGC
; RA1 AN1 C12IN1-                3     26 RB5 AN13 CCP3 P3A P2B T1G T3CKI IOC PULLUP
; RA2 AN2 C2IN+ VREF-            4     25 RB4 AN11 P1D T5G IOC PULLUP
; RA3 AN3 C1IN+ VREF+            5     24 RB3 AN9 C12IN2- CTED2 CCP2 P2A SDO2 PULLUP
; RA4 C1OUT SRQ CCP5 T0CKI       6     23 RB2 AN8 CTED1 P1B SDI2 SDA2 INT2 PULLUP
; RA5 AN4 C2OUT SRNQ HLVDIN !SS1 7     22 RB1 AN10 C12IN3- P1C SCK2 SCL2 INT1 PULLUP
; VSS GND                        8     21 RB0 AN12 SRI CCP4 FLT0 !SS2 INT0 PULLUP
; RA7 OSC1 CLKI                  9     20 VDD VCC
; RA6 OSC2 CLKO                  10    19 VSS GND
; RC0 P2B SOSCO T1CKI T3CKI T3G  11    18 RC7 AN19 P3B RX1 DT1
; RC1 CCP2 P2A SOSCI             12    17 RC6 AN18 CCP3 P3A TX1 CK1
; RC2 AN14 CTPLS CCP1 P1A T5CKI  13    16 RC5 AN17 SDO1
; RC3 AN15 SCK1 SCL1             14----15 RC4 AN16 SDI1 SDA1
;
; Program
; -------
; k16 mchp program tonegen.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F25K22
;ERRORLEVEL      -302
#INCLUDE        "p18f25k22.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Beep like an IBM PC and POKE like a VIC=20
;
; $ i2cdetect -y 1
;      0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
; 00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 40: 40 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
; 70: -- -- -- -- -- -- -- --                         
;
; $ i2cset -y 1 0x40 0x00 0x1C              # SOUND ON  VOICE=0 NOTE=28
;
; $ i2cset -y 1 0x40 0x00 0x00              # SOUND OFF VOICE=0 NOTE=0
;
; Voice may range from 0 to 3. All voices are square waves.
;
; Note may range from 24 to 107 and this value is equivalent to that
; found in the midi specification.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; Oscillator Selection bits:
                CONFIG    FOSC=INTIO67 ; Internal oscillator block
; 4X PLL Enable:
                CONFIG    PLLCFG=OFF
; Primary clock enable bit:
                CONFIG    PRICLKEN=OFF
; Fail-Safe Clock Monitor Enable bit:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover bit:
                CONFIG    IESO=OFF
; Power-up Timer Enable bit:
                CONFIG    PWRTEN=ON
; Brown-out Reset Enable bits:
                CONFIG    BOREN=SBORDIS
; Brown-out Reset Voltage bits:
                CONFIG    BORV=285
; Watchdog Timer Enable bits:
                CONFIG    WDTEN=SWON
; Watchdog Postscale Select bits:
                CONFIG    WDTPS=1024
; CCP2 MUX bit:
                CONFIG    CCP2MX=PORTC1 ;PORTB3 or PORTC1
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; P3A/CCP3 Mux bit:
                CONFIG    CCP3MX=PORTC6 ;PORTC6 or PORTB5
; HFINTOSC Fast Start-up:
                CONFIG    HFOFST=OFF
; Timer3 Clock input mux bit:
                CONFIG    T3CMX=PORTB5  ;PORTB5 or PORTC0
; ECCP2 B output mux bit:
                CONFIG    P2BMX=PORTC0  ;PORTC0 or PORTB5
; MCLR Pin Enable bit:
                CONFIG    MCLRE=EXTMCLR
; Stack Full/Overflow Reset Enable bit:
                CONFIG    STVREN=ON
; Single-Supply ICSP Enable bit:
                CONFIG    LVP=ON
; Extended Instruction Set Enable bit:
                CONFIG    XINST=OFF
; Background Debug:
                CONFIG    DEBUG=OFF
; Code Protection Block 0:
                CONFIG    CP0=OFF
; Code Protection Block 1:
                CONFIG    CP1=OFF
; Code Protection Block 2:
                CONFIG    CP2=OFF
; Code Protection Block 3:
                CONFIG    CP3=OFF
; Boot Block Code Protection:
                CONFIG    CPB=OFF
; Data EEPROM Code Protection:
                CONFIG    CPD=OFF
; Write Protection Block 0:
                CONFIG    WRT0=OFF
; Write Protection Block 1:
                CONFIG    WRT1=OFF
; Write Protection Block 2:
                CONFIG    WRT2=OFF
; Write Protection Block 3:
                CONFIG    WRT3=OFF
; Boot Block Write Protection:
                CONFIG    WRTB=OFF
; Configuration Register Write Protection:
                CONFIG    WRTC=OFF
; Data EEPROM Write Protection:
                CONFIG    WRTD=OFF
; Table Read Protection Block 0:
                CONFIG    EBTR0=OFF
; Table Read Protection Block 1:
                CONFIG    EBTR1=OFF
; Table Read Protection Block 2:
                CONFIG    EBTR2=OFF
; Table Read Protection Block 3:
                CONFIG    EBTR3=OFF
; Boot Block Table Read Protection:
                CONFIG    EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; HFINTOSC = 16MHz PLL X4 (16 MIPS)
    CONSTANT CLOCK = 64000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "TONE GENERATOR",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x00                        ;ACCESS RAM 0x00..0x5F
    B_SAVE      ;BANK SELECT REGISTER
    S_SAVE      ;STATUS REGISTER
    W_SAVE      ;WORKING REGISTER

    ARG0        ;FUNCTION ARGS
    ARG1

    TMR0LOW     ;TMR0 RELOAD LOW
    TMR0HIGH    ;TMR0 RELOAD HIGH

    TMR1LOW     ;TMR1 RELOAD LOW
    TMR1HIGH    ;TMR1 RELOAD HIGH

    TMR3LOW     ;TMR3 RELOAD LOW
    TMR3HIGH    ;TMR3 RELOAD HIGH

    TMR5LOW     ;TMR5 RELOAD LOW
    TMR5HIGH    ;TMR5 RELOAD HIGH
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0008
                GOTO    INTHIGH             ;When IPEN=0 or 1
                ORG     0x0018
                GOTO    INTLOW              ;When IPEN=1
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; High priority ISR
; 
INTHIGH
;               MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE
 
TMR0TEST        BTFSS   INTCON,TMR0IF
                GOTO    TMR1TEST
                BCF     INTCON,TMR0IF
                MOVFF   TMR0HIGH,TMR0H
                MOVFF   TMR0LOW,TMR0L
                BTG     LATA,0              ;TOGGLE PIN

TMR1TEST        BTFSS   PIR1,TMR1IF
                GOTO    TMR3TEST
                BCF     PIR1,TMR1IF
                MOVFF   TMR1HIGH,TMR1H
                MOVFF   TMR1LOW,TMR1L
                BTG     LATA,1              ;TOGGLE PIN

TMR3TEST        BTFSS   PIR2,TMR3IF
                GOTO    TMR5TEST
                BCF     PIR2,TMR3IF
                MOVFF   TMR3HIGH,TMR3H
                MOVFF   TMR3LOW,TMR3L
                BTG     LATA,2              ;TOGGLE PIN

TMR5TEST        BTFSS   PIR5,TMR5IF
                GOTO    RETINTFAST
                BCF     PIR5,TMR5IF
                MOVFF   TMR5HIGH,TMR5H
                MOVFF   TMR5LOW,TMR5L
                BTG     LATA,3              ;TOGGLE PIN

RETINTFAST      RETFIE  FAST
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Low priority ISR
;
INTLOW
                MOVWF   W_SAVE              ;SAVE WORKING REGISTER
                MOVFF   BSR,B_SAVE          ;SAVE BANK SELECT
                MOVFF   STATUS,S_SAVE       ;SAVE STATUS

;               MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

RETINT          MOVF    W_SAVE,W            ;RESTORE WORKING REGISTER
                MOVFF   B_SAVE,BSR          ;RESTORE BANK SELECT
                MOVFF   S_SAVE,STATUS       ;RESTORE STATUS

                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Reset device
;
RESETSFR
                BCF     RCON,IPEN           ;DISABLE PRIORITY INTERRUPTS
                MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                CLRF    INTCON
                CLRF    INTCON2
                CLRF    INTCON3

                CLRF    IOCB

                CLRF    PIE1
                CLRF    PIE2
                CLRF    PIE3
                CLRF    PIE4
                CLRF    PIE5

                CLRF    IPR1
                CLRF    IPR2
                CLRF    IPR3
                CLRF    IPR4
                CLRF    IPR5

                CLRF    ADCON0

                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC

                CLRF    LATA
                CLRF    LATB
                CLRF    LATC

                MOVLW   b'00000000'
                MOVWF   TRISA           ;RA0..RA3=TONE

                MOVLW   b'11000000'
                MOVWF   TRISB           ;RB6=PGC RB7=PGD

                MOVLW   b'00011000'
                MOVWF   TRISC           ;RC4=SDA RC3=SCL

                CLRF    PMD0
                CLRF    PMD1
                CLRF    PMD2

                CLRF    CCP1CON
                CLRF    CCP2CON
                CLRF    CCP3CON
                CLRF    CCP4CON
                CLRF    CCP5CON

                CLRF    T0CON
                CLRF    T1CON
                CLRF    T2CON
                CLRF    T3CON
                CLRF    T4CON
                CLRF    T5CON
                CLRF    T6CON

                CLRF    T1GCON
                CLRF    T3GCON
                CLRF    T5GCON

                CLRF    PIR1
                CLRF    PIR2
                CLRF    PIR3
                CLRF    PIR4
                CLRF    PIR5
            
                CLRF    SSP1CON1
                CLRF    SSP1CON2
                CLRF    SSP1CON3
                CLRF    SSP1BUF

                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Translate midi note value to timer reload value
;
; In:   ARG0=midi note 24..107
;       FSR0=timer reload destination
;
; Out:  OK:    W=0 CARRY=SET   (FSR0)=TIMER RELOAD
;       ERROR: W=1 CARRY=CLEAR
;
#INCLUDE        "notetable.inc"
RELOAD
;RANGECHECK=http://www.piclist.com/techref/microchip/rangechk.htm
                MOVF    ARG0,W
                ADDLW   255 - NOTELAST
                ADDLW   (NOTELAST - NOTEFIRST) + 1
                BTFSS   STATUS,C
                RETLW   1                   ;ERROR: W=1 CARRY=CLEAR

                MOVWF   ARG0                ;ARG0 = NOTE - NOTEFIRST
                CLRC
                RLCF    ARG0,F              ;ARG0 = 2 * (NOTE - NOTEFIRST)

                MOVLW   LOW(NOTETABLE)      ;TBLPTR = TABLE BASE
                MOVWF   TBLPTRL
                MOVLW   HIGH(NOTETABLE)
                MOVWF   TBLPTRH
                MOVLW   UPPER(NOTETABLE)
                MOVWF   TBLPTRU

                MOVF    ARG0,W              ;TBPLTR += 2 * (NOTE - NOTEFIRST)
                ADDWF   TBLPTRL,F
                MOVLW   0
                ADDWFC  TBLPTRH,F
                ADDWFC  TBLPTRU,F

                TBLRD*+
                MOVFF   TABLAT,POSTINC0     ;FREQUENCY LOW
                TBLRD*
                MOVFF   TABLAT,INDF0        ;FREQUENCY HIGH
        
                BSF     STATUS,C            ;OK: W=0 CARRY=SET
                RETLW   0    
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Voice on/off
;
DOVOICE0        CLRF    T0CON               ;TIMER OFF
                LFSR    0,TMR0LOW
                CALL    RELOAD
                BTFSS   STATUS,C
                RETLW   0

                MOVF    TMR0LOW,W           ;TIMER ON
                MOVWF   TMR0L
                MOVF    TMR0HIGH,W
                MOVWF   TMR0H
                MOVLW   b'10000001'         ;16-BIT. TIMER ON. PRESCALE/4.
                MOVWF   T0CON
                RETLW   0

DOVOICE1        CLRF    T1CON               ;TIMER OFF
                LFSR    0,TMR1LOW
                CALL    RELOAD
                BTFSS   STATUS,C
                RETLW   0

                MOVF    TMR1LOW,W           ;TIMER ON
                MOVWF   TMR1L
                MOVF    TMR1HIGH,W
                MOVWF   TMR1H
                MOVLW   b'00100011'         ;16-BIT LOAD. TIMER ON. FOSC/4. PRESCALE/4.
                MOVWF   T1CON
                RETLW   0

DOVOICE2        CLRF    T3CON               ;TIMER OFF
                LFSR    0,TMR3LOW
                CALL    RELOAD
                BTFSS   STATUS,C
                RETLW   0

                MOVF    TMR3LOW,W           ;TIMER ON
                MOVWF   TMR3L
                MOVF    TMR3HIGH,W
                MOVWF   TMR3H
                MOVLW   b'00100011'         ;16-BIT LOAD. TIMER ON. FOSC/4. PRESCALE/4.
                MOVWF   T3CON
                RETLW   0

DOVOICE3        CLRF    T5CON               ;TIMER OFF
                LFSR    0,TMR5LOW
                CALL    RELOAD
                BTFSS   STATUS,C
                RETLW   0

                MOVF    TMR5LOW,W           ;TIMER ON
                MOVWF   TMR5L
                MOVF    TMR5HIGH,W
                MOVWF   TMR5H
                MOVLW   b'00100011'         ;16-BIT LOAD. TIMER ON. FOSC/4. PRESCALE/4.
                MOVWF   T5CON
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Sound on/off
;
; In:   ARG0=midi note 24..107
;       ARG1=voice 0..3
;
DOSOUND         MOVF    ARG1,W              ;IS VOICE 0?
                XORLW   0
                BZ      DOVOICE0
                MOVF    ARG1,W              ;IS VOICE 1?
                XORLW   1
                BZ      DOVOICE1
                MOVF    ARG1,W              ;IS VOICE 2?
                XORLW   2
                BZ      DOVOICE2
                MOVF    ARG1,W              ;IS VOICE 3?
                XORLW   3
                BZ      DOVOICE3
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            

;INITIALISE CLOCK OSCILLATOR
                CLRF    OSCCON2             ;INIT CLOCK 16MHZ HFINTOSC PLL X4
                BSF     OSCTUNE,PLLEN
                MOVLW   b'01110000'
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCCON,HFIOFS       ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITHFIOFS

;INITIALISE DEVICE
                CALL    RESETSFR

;INITIALISE 16-BIT TIMERS
;TMR0
                MOVLW   b'11100000'
                MOVWF   INTCON
                MOVLW   b'00000000'
                MOVWF   INTCON2
;TMR1/3/5
                BSF     PIE1,TMR1IE
                BSF     PIE2,TMR3IE
                BSF     PIE5,TMR5IE

;INITIALISE I2C SLAVE
                MOVLW   0x80                ;SLAVE ADDRESS=0x40 (W=0x80 R=0x81)
                MOVWF   SSP1ADD
                MOVLW   b'00100110'         ;SP ENABLE, I2C SLAVE 7-BIT ADDRESS
                MOVWF   SSP1CON1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
I2CSKIP         MOVF    SSP1BUF,W           ;CLEAR BF

I2CLOOP         CALL    I2CNEXT
                BTFSC   SSP1STAT,R_NOT_W
                GOTO    I2CSKIP             ;!WRITE
                BTFSC   SSP1STAT,D_NOT_A
                GOTO    I2CSKIP             ;!ADDRESS

I2CADDRESS      MOVF    SSP1BUF,W           ;CLEAR BF

                CALL    I2CNEXT
                BTFSC   SSP1STAT,R_NOT_W
                GOTO    I2CSKIP             ;!WRITE
                BTFSS   SSP1STAT,D_NOT_A
                GOTO    I2CADDRESS          ;!DATA

                MOVFF   SSP1BUF,ARG1        ;GET VOICE, CLEAR BF

                CALL    I2CNEXT
                BTFSC   SSP1STAT,R_NOT_W
                GOTO    I2CSKIP             ;!WRITE
                BTFSS   SSP1STAT,D_NOT_A
                GOTO    I2CADDRESS          ;!DATA

                MOVFF   SSP1BUF,ARG0        ;GET MIDI NOTE, CLEAR BF

                CALL    DOSOUND             ;SOUND ON
                GOTO    I2CLOOP

I2CNEXT         BTFSS   PIR1,SSP1IF         ;WAIT FOR ADDRESS OR DATA
                GOTO    I2CNEXT
                BCF     PIR1,SSP1IF
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
