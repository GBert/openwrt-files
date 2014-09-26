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
; PWM Demo
;
; Start 3 independant PWMs
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
; k16 lvp program pwm.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F25K22
;ERRORLEVEL      -302
#INCLUDE        "p18f25k22.inc"
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
                DE      "PWM DEMO",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x00                        ;ACCESS RAM 0x00..0x5F
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0008
                GOTO    INTERRUPT           ;IPEN=0
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine
; 
INTERRUPT       RETFIE  FAST
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BCF     RCON,IPEN           ;DISABLE PRIORITY INTERRUPTS
                MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                CLRF    OSCCON2             ;INIT CLOCK 16MHZ HFINTOSC PLL X4
                BSF     OSCTUNE,PLLEN
                MOVLW   b'01110000'
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCCON,HFIOFS       ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITHFIOFS
;RESET
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

                CLRF    ADCON0              ;DISABLE A/D

                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC

                CLRF    LATA
                CLRF    LATB
                CLRF    LATC

                CLRF    TRISA
                CLRF    TRISB
                CLRF    TRISC

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

                CLRF    TMR0
                CLRF    TMR1
                CLRF    TMR2
                CLRF    TMR3
                CLRF    TMR4
                CLRF    TMR5
                CLRF    TMR6

                CLRF    PIR1
                CLRF    PIR2
                CLRF    PIR3
                CLRF    PIR4
                CLRF    PIR5
;
; TMR2/4/6 FOSC/4 (FOSC=64M)
;                                    TxCKPS
; PRESCALE/1  16MHz 62.5ns CYCLE 00
; PRESCALE/4   4MHz  250ns CYCLE 01
; PRESCALE/16  1MHz    1us CYCLE 1X
;
;PWM SETUP
                MOVLW   b'10001000'         ;PWM CCP3=TIMER6 CCP2=TIMER4 CCP1=TIMER2
                MOVWF   CCPTMRS0
                MOVLW   b'00001001'         ;PWM CCP5=TIMER6 CCP4=TIMER4
                MOVWF   CCPTMRS1

                MOVLW   0x0C                ;PWM MODE
                MOVWF   CCP1CON
                MOVWF   CCP2CON
                MOVWF   CCP3CON
;PWM GO
                CLRF    T2CON               ;RESET TIMERS
                CLRF    T4CON
                CLRF    T6CON
                CLRF    TMR2
                CLRF    TMR4
                CLRF    TMR6
                CLRF    PIR1                ;TMR2IF
                CLRF    PIR5                ;TMR4IF,TMR6IF

                MOVLW   0x80                ;MARK TIME
                MOVWF   CCPR1L
                MOVWF   CCPR2L
                MOVWF   CCPR3L

                MOVLW   0xFF                ;MARK + SPACE TIME
                MOVWF   PR2
                MOVWF   PR4
                MOVWF   PR6

                MOVLW   b'00000110'         ;TIMER ON PRESCALE/16
                MOVWF   T2CON
                MOVWF   T4CON
                MOVWF   T6CON

LOOP            GOTO    LOOP                ;DO NOTHING ELSE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
