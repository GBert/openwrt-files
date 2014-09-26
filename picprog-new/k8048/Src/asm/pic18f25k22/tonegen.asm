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
; Simple tone generator
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
; k16 lvp program tonegen.hex
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
NOTEFIRST EQU .24
NOTELAST  EQU .107
NOTETABLE
    DW 0x111C ;MIDI= 24(0x18) OCTAVE=-3 NOTE=C     f=32.70    (32.70)
    DW 0x1E84 ;MIDI= 25(0x19) OCTAVE=-3 NOTE=C#/Db f=34.65    (34.65)
    DW 0x2B2C ;MIDI= 26(0x1A) OCTAVE=-3 NOTE=D     f=36.71    (36.71)
    DW 0x371E ;MIDI= 27(0x1B) OCTAVE=-3 NOTE=D#/Eb f=38.89    (38.89)
    DW 0x4264 ;MIDI= 28(0x1C) OCTAVE=-3 NOTE=E     f=41.20    (41.20)
    DW 0x4D09 ;MIDI= 29(0x1D) OCTAVE=-3 NOTE=F     f=43.65    (43.65)
    DW 0x5714 ;MIDI= 30(0x1E) OCTAVE=-3 NOTE=F#/Gb f=46.25    (46.25)
    DW 0x608F ;MIDI= 31(0x1F) OCTAVE=-3 NOTE=G     f=49.00    (49.00)
    DW 0x6982 ;MIDI= 32(0x20) OCTAVE=-3 NOTE=G#,Ab f=51.91    (51.91)
    DW 0x71F4 ;MIDI= 33(0x21) OCTAVE=-3 NOTE=A     f=55.00    (55.00)
    DW 0x79ED ;MIDI= 34(0x22) OCTAVE=-3 NOTE=A#/Bb f=58.27    (58.27)
    DW 0x8174 ;MIDI= 35(0x23) OCTAVE=-3 NOTE=B     f=61.74    (61.74)
    DW 0x888E ;MIDI= 36(0x24) OCTAVE=-2 NOTE=C     f=65.41    (65.41)
    DW 0x8F42 ;MIDI= 37(0x25) OCTAVE=-2 NOTE=C#/Db f=69.30    (69.30)
    DW 0x9596 ;MIDI= 38(0x26) OCTAVE=-2 NOTE=D     f=73.42    (73.42)
    DW 0x9B8F ;MIDI= 39(0x27) OCTAVE=-2 NOTE=D#/Eb f=77.78    (77.78)
    DW 0xA132 ;MIDI= 40(0x28) OCTAVE=-2 NOTE=E     f=82.41    (82.41)
    DW 0xA684 ;MIDI= 41(0x29) OCTAVE=-2 NOTE=F     f=87.31    (87.31)
    DW 0xAB8A ;MIDI= 42(0x2A) OCTAVE=-2 NOTE=F#/Gb f=92.50    (92.50)
    DW 0xB048 ;MIDI= 43(0x2B) OCTAVE=-2 NOTE=G     f=98.00    (98.00)
    DW 0xB4C1 ;MIDI= 44(0x2C) OCTAVE=-2 NOTE=G#,Ab f=103.83   (103.83)
    DW 0xB8FA ;MIDI= 45(0x2D) OCTAVE=-2 NOTE=A     f=110.00   (110.00)
    DW 0xBCF7 ;MIDI= 46(0x2E) OCTAVE=-2 NOTE=A#/Bb f=116.54   (116.54)
    DW 0xC0BA ;MIDI= 47(0x2F) OCTAVE=-2 NOTE=B     f=123.47   (123.47)
    DW 0xC447 ;MIDI= 48(0x30) OCTAVE=-1 NOTE=C     f=130.81   (130.81)
    DW 0xC7A1 ;MIDI= 49(0x31) OCTAVE=-1 NOTE=C#/Db f=138.59   (138.59)
    DW 0xCACB ;MIDI= 50(0x32) OCTAVE=-1 NOTE=D     f=146.83   (146.83)
    DW 0xCDC8 ;MIDI= 51(0x33) OCTAVE=-1 NOTE=D#/Eb f=155.56   (155.57)
    DW 0xD099 ;MIDI= 52(0x34) OCTAVE=-1 NOTE=E     f=164.81   (164.81)
    DW 0xD342 ;MIDI= 53(0x35) OCTAVE=-1 NOTE=F     f=174.61   (174.61)
    DW 0xD5C5 ;MIDI= 54(0x36) OCTAVE=-1 NOTE=F#/Gb f=185.00   (185.00)
    DW 0xD824 ;MIDI= 55(0x37) OCTAVE=-1 NOTE=G     f=196.00   (196.00)
    DW 0xDA61 ;MIDI= 56(0x38) OCTAVE=-1 NOTE=G#,Ab f=207.65   (207.66)
    DW 0xDC7D ;MIDI= 57(0x39) OCTAVE=-1 NOTE=A     f=220.00   (220.00)
    DW 0xDE7B ;MIDI= 58(0x3A) OCTAVE=-1 NOTE=A#/Bb f=233.08   (233.07)
    DW 0xE05D ;MIDI= 59(0x3B) OCTAVE=-1 NOTE=B     f=246.94   (246.94)
    DW 0xE223 ;MIDI= 60(0x3C) OCTAVE=0  NOTE=C     f=261.63   (261.61)
    DW 0xE3D1 ;MIDI= 61(0x3D) OCTAVE=0  NOTE=C#/Db f=277.18   (277.20)
    DW 0xE566 ;MIDI= 62(0x3E) OCTAVE=0  NOTE=D     f=293.66   (293.69)
    DW 0xE6E4 ;MIDI= 63(0x3F) OCTAVE=0  NOTE=D#/Eb f=311.13   (311.14)
    DW 0xE84D ;MIDI= 64(0x40) OCTAVE=0  NOTE=E     f=329.63   (329.65)
    DW 0xE9A1 ;MIDI= 65(0x41) OCTAVE=0  NOTE=F     f=349.23   (349.22)
    DW 0xEAE3 ;MIDI= 66(0x42) OCTAVE=0  NOTE=F#/Gb f=369.99   (370.03)
    DW 0xEC12 ;MIDI= 67(0x43) OCTAVE=0  NOTE=G     f=392.00   (392.00)
    DW 0xED30 ;MIDI= 68(0x44) OCTAVE=0  NOTE=G#,Ab f=415.30   (415.28)
    DW 0xEE3F ;MIDI= 69(0x45) OCTAVE=0  NOTE=A     f=440.00   (440.04)
    DW 0xEF3E ;MIDI= 70(0x46) OCTAVE=0  NOTE=A#/Bb f=466.16   (466.20)
    DW 0xF02E ;MIDI= 71(0x47) OCTAVE=0  NOTE=B     f=493.88   (493.83)
    DW 0xF112 ;MIDI= 72(0x48) OCTAVE=1  NOTE=C     f=523.25   (523.29)
    DW 0xF1E8 ;MIDI= 73(0x49) OCTAVE=1  NOTE=C#/Db f=554.37   (554.32)
    DW 0xF2B3 ;MIDI= 74(0x4A) OCTAVE=1  NOTE=D     f=587.33   (587.37)
    DW 0xF372 ;MIDI= 75(0x4B) OCTAVE=1  NOTE=D#/Eb f=622.25   (622.28)
    DW 0xF426 ;MIDI= 76(0x4C) OCTAVE=1  NOTE=E     f=659.26   (659.20)
    DW 0xF4D1 ;MIDI= 77(0x4D) OCTAVE=1  NOTE=F     f=698.46   (698.57)
    DW 0xF571 ;MIDI= 78(0x4E) OCTAVE=1  NOTE=F#/Gb f=739.99   (739.92)
    DW 0xF609 ;MIDI= 79(0x4F) OCTAVE=1  NOTE=G     f=783.99   (784.01)
    DW 0xF698 ;MIDI= 80(0x50) OCTAVE=1  NOTE=G#,Ab f=830.61   (830.56)
    DW 0xF71F ;MIDI= 81(0x51) OCTAVE=1  NOTE=A     f=880.00   (879.89)
    DW 0xF79F ;MIDI= 82(0x52) OCTAVE=1  NOTE=A#/Bb f=932.33   (932.40)
    DW 0xF817 ;MIDI= 83(0x53) OCTAVE=1  NOTE=B     f=987.77   (987.65)
    DW 0xF889 ;MIDI= 84(0x54) OCTAVE=2  NOTE=C     f=1046.50  (1046.57)
    DW 0xF8F4 ;MIDI= 85(0x55) OCTAVE=2  NOTE=C#/Db f=1108.73  (1108.65)
    DW 0xF959 ;MIDI= 86(0x56) OCTAVE=2  NOTE=D     f=1174.66  (1174.40)
    DW 0xF9B9 ;MIDI= 87(0x57) OCTAVE=2  NOTE=D#/Eb f=1244.51  (1244.56)
    DW 0xFA13 ;MIDI= 88(0x58) OCTAVE=2  NOTE=E     f=1318.51  (1318.39)
    DW 0xFA68 ;MIDI= 89(0x59) OCTAVE=2  NOTE=F     f=1396.91  (1396.65)
    DW 0xFAB9 ;MIDI= 90(0x5A) OCTAVE=2  NOTE=F#/Gb f=1479.98  (1480.38)
    DW 0xFB04 ;MIDI= 91(0x5B) OCTAVE=2  NOTE=G     f=1567.98  (1567.40)
    DW 0xFB4C ;MIDI= 92(0x5C) OCTAVE=2  NOTE=G#,Ab f=1661.22  (1661.13)
    DW 0xFB90 ;MIDI= 93(0x5D) OCTAVE=2  NOTE=A     f=1760.00  (1760.56)
    DW 0xFBCF ;MIDI= 94(0x5E) OCTAVE=2  NOTE=A#/Bb f=1864.66  (1863.93)
    DW 0xFC0C ;MIDI= 95(0x5F) OCTAVE=2  NOTE=B     f=1975.53  (1976.28)
    DW 0xFC44 ;MIDI= 96(0x60) OCTAVE=3  NOTE=C     f=2093.00  (2092.05)
    DW 0xFC7A ;MIDI= 97(0x61) OCTAVE=3  NOTE=C#/Db f=2217.46  (2217.29)
    DW 0xFCAD ;MIDI= 98(0x62) OCTAVE=3  NOTE=D     f=2349.32  (2350.18)
    DW 0xFCDC ;MIDI= 99(0x63) OCTAVE=3  NOTE=D#/Eb f=2489.02  (2487.56)
    DW 0xFD0A ;MIDI=100(0x64) OCTAVE=3  NOTE=E     f=2637.02  (2638.52)
    DW 0xFD34 ;MIDI=101(0x65) OCTAVE=3  NOTE=F     f=2793.83  (2793.30)
    DW 0xFD5C ;MIDI=102(0x66) OCTAVE=3  NOTE=F#/Gb f=2959.96  (2958.58)
    DW 0xFD82 ;MIDI=103(0x67) OCTAVE=3  NOTE=G     f=3135.96  (3134.80)
    DW 0xFDA6 ;MIDI=104(0x68) OCTAVE=3  NOTE=G#,Ab f=3322.44  (3322.26)
    DW 0xFDC8 ;MIDI=105(0x69) OCTAVE=3  NOTE=A     f=3520.00  (3521.13)
    DW 0xFDE8 ;MIDI=106(0x6A) OCTAVE=3  NOTE=A#/Bb f=3729.31  (3731.34)
    DW 0xFE06 ;MIDI=107(0x6B) OCTAVE=3  NOTE=B     f=3951.07  (3952.57)
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
