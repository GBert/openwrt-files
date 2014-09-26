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
; R-PI 3V3 PIC18F26K20
;
; This demo allows control from `kio' to perform ICSPIO common operations.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Incompatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 32768 words Flash (16-bit)
; 3936 bytes RAM
; 1024 bytes EEPROM
;
; Pinout
; ------
; !MCLR VPP RE3             1-----28 RB7 KBI3 PGD
; RA0 AN0 C12IN0-           2     27 RB6 KBI2 PGC
; RA1 AN1 C12IN1-           3     26 RB5 KBI1 PGM
; RA2 AN2 C2IN+ VREF- CVREF 4     25 RB4 KBI0 AN11 P1D
; RA3 AN3 C1IN+ VREF+       5     24 RB3 AN9  C12IN2- CCP2
; RA4 C1OUT T0CKI           6     23 RB2 AN8  INT2 P1B
; RA5 AN4 C2OUT HLVDIN !SS  7     22 RB1 AN10 INT1 C12IN3- P1C
; VSS GND                   8     21 RB0 AN12 INT0 FLT0
; RA7 OSC1 CLKI             9     20 VDD VCC
; RA6 OSC2 CLKO             10    19 VSS GND
; RC0 T1OSO T3CKI           11    18 RC7 RX DT
; RC1 T1OSI CCP2            12    17 RC6 TX CK
; RC2 CCP1 P1A              13    16 RC5 SDO
; RC3 SCK SCL               14----15 RC4 SDI SDA
;
; Program
; -------
; k16 program pic18f26k20.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F26K20
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f26k20.inc"
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
; Fail-Safe Clock Monitor Enable bit:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover bit:
                CONFIG    IESO=OFF
; Power-up Timer Enable bit:
                CONFIG    PWRT=OFF
; Brown-out Reset Enable bits:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=30
; Watchdog Timer Enable bits:
                CONFIG    WDTEN=OFF
; Watchdog Postscale Select bits:
                CONFIG    WDTPS=1024
; CCP2 MUX bit:
                CONFIG    CCP2MX=PORTC
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; Low-Power Timer1 Oscillator Enable bit:                
                CONFIG    LPT1OSC=ON
; HFINTOSC Fast Start-up:
                CONFIG    HFOFST=OFF
; MCLR Pin Enable bit:
                CONFIG    MCLRE=ON
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
  __IDLOCS _IDLOC0,1
  __IDLOCS _IDLOC1,2
  __IDLOCS _IDLOC2,3
  __IDLOCS _IDLOC3,4
  __IDLOCS _IDLOC4,5
  __IDLOCS _IDLOC5,6
  __IDLOCS _IDLOC6,7
  __IDLOCS _IDLOC7,8
;
; HFINTOSC = 16MHz PLL X4 (16 MIPS)
    CONSTANT CLOCK = 64000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F26K20",0
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
                RETFIE
                ORG     0x0018
                RETFIE
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; ICSP I/O
;
NPINS           SET     .28                 ;28-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                BSF     OSCTUNE,PLLEN       ;INIT CLOCK 16MHZ HFINTOSC PLL X4
                MOVLW   b'01110000'
                MOVWF   OSCCON
INITIOFS        BTFSS   OSCCON,IOFS         ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT?
                GOTO    POWERUP

                MOVLW   0xFF                ;WATCHDOG TIMED OUT
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATA                ;INIT PORT A
                CLRF    LATB                ;INIT PORT B
                CLRF    LATC                ;INIT PORT C

WATCHDOG
                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ANSEL
                CLRF    ANSELH

                CLRF    CM1CON0             ;DISABLE COMPARATORS
                CLRF    CM2CON0

                MOVLW   b'00000000'         ;PORT A ALL O/P
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PORT B PGC/PGD I/P
                MOVWF   TRISB

                MOVLW   b'11111111'         ;PORT C ALL I/P
                MOVWF   TRISC

                CLRWDT                      ;INIT WATCHDOG TIMER
                BSF     WDTCON,SWDTEN       ;START WATCHDOG TIMER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CALL    INITIO              ;INITIALISE ICSPIO
;
MAINLOOP        COMMON  MAINLOOP, INIT      ;DO COMMON COMMANDS

                GOTO    UNSUPPORTED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
