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
; Velleman K8048 PIC18F4520 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets the eight LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of switch 13.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 16384 words Flash (16-bit)
; 1536 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; !MCLR/VPP RE3            1-----40 RB7/PGD  KBI3
; RA0 AN0                  2     39 RB6/PGC  KBI2
; RA1 AN1                  3     38 RB5/PGM  KBI1
; RA2 AN2 VREF- CVREF      4     37 RB4 AN11 KBI0
; RA3 AN3 VREF+            5     36 RB3 AN9 CCP2
; RA4 T0CKI C1OUT          6     35 RB2 AN8 INT2
; RA5 AN4 !SS HLVDIN C2OUT 7     34 RB1 AN10 INT1
; RE0 AN5 !RD              8     33 RB0 AN12 INT0 FLT0
; RE1 AN6 !WR              9     32 VDD/VCC
; RE2 AN7 !CS              10    31 VSS/GND
; VDD/VCC                  11    30 RD7 PSP7 P1D
; VSS/GND                  12    29 RD6 PSP6 P1C
; RA7 OSC1 CLKI            13    28 RD5 PSP5 P1B
; RA6 OSC2 CLKO            14    27 RD4 PSP4
; RC0 T1OSO T13CKI         15    26 RC7 RX DT
; RC1 T1OSI CCP2           16    25 RC6 TX CK
; RC2 CCP1 P1A             17    24 RC5 SDO
; RC3 SCK SCL              18    23 RC4 SDI SDA
; RD0 PSP0                 19    22 RD3 PSP3
; RD1 PSP1                 20----21 RD2 PSP2
;
; Proton MK2 Pin
; ---------- ---
; D1         RD0
; D2         RD1
; D3         RD2
; D4         RD3
; D5         RD4
; D6         RD5
; D7         RD6
; D8         RD7
; S13        RB3
;
; ICSP
; ----
; 5:PGC
; 4:PGD
; 3:VSS/GND
; 2:VDD 5V
; 1:VPP 12V
;
; Program
; -------
; k16 p pic18f4520.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F4520
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f4520.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; Oscillator Selection:
                CONFIG    OSC=HS
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Reset:
                CONFIG    BOREN=OFF
; Brown-out Voltage:
                CONFIG    BORV=0
; Watchdog Timer:
                CONFIG    WDT=ON
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; MCLR Enable:
                CONFIG    MCLRE=ON
; T1 Oscillator Enable:
                CONFIG    LPT1OSC=OFF
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; CCP2 Pin Function:
                CONFIG    CCP2MX=PORTBE
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=ON
; XINST Enable:
                CONFIG    XINST=OFF
; Background Debugger Enable:
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
; XTAL = 20MHz
    CONSTANT CLOCK = 20000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F4520",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x00                        ;ACCESS RAM 0x00..0x7F
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
NPINS           SET     .40                 ;40-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATD,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATB                ;INIT PORT B
                SETF    LATD                ;INIT PORT D

WATCHDOG        MOVLW   b'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                MOVLW   b'00000000'         ;DISABLE A/D
                MOVWF   ADCON0
                MOVLW   b'00001111'         ;DIGITAL I/O
                MOVWF   ADCON1

                MOVLW   b'11101111'         ;SW13 I/P
                MOVWF   TRISB

                MOVLW   b'00000000'         ;D1..D8 O/P    
                MOVWF   TRISD

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

                MOVF    BUFFER,W            ;IS LED?
                XORLW   CMD_LED
                BZ      DOLED

                MOVF    BUFFER,W            ;IS SWITCH?
                XORLW   CMD_SWITCH
                BZ      DOSWITCH

                GOTO    UNSUPPORTED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set D1..D8
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET D1-D8 ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;SET D1..D8
                MOVWF   LATD

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get S13
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVLW   0                   ;GET S13
                BTFSC   PORTB,3
                MOVLW   1

                CALL    SENDBYTE            ;SEND S13
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
