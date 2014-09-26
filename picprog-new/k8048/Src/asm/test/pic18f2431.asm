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
; R-PI 3V3 PIC18F2431
;
; This demo allows control from `kio' to perform ICSPIO common operations.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Incompatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; !MCLR VPP 1-----28 RB7 PGD
; RA0       2     27 RB6 PGC
; RA1       3     26 RB5 PGM
; RA2       4     25 RB4
; RA3       5     24 RB3
; RA4       6     23 RB2
; VDD VCC   7     22 RB1
; VSS GND   8     21 RB0
; RA7       9     20 VDD VCC
; RA6       10    19 VSS GND
; RC0       11    18 RC7
; RC1       12    17 RC6
; RC2       13    16 RC5
; RC3       14----15 RC4
;
; Program
; -------
; k16 p pic18f2431.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F2431
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f2431.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; DS51537F-page 32
;
; Oscillator Selection bits:
                CONFIG    OSC=IRCIO
; Fail-Safe Clock Monitor Enable bit:
                CONFIG    FCMEN=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRTEN=OFF
; Brown-out Reset:
                CONFIG    BOREN=OFF
; Brown-out Voltage:
                CONFIG    BORV=27
; Watchdog Timer:
                CONFIG    WDTEN=OFF
; Watchdog Postscaler:
                CONFIG    WDPS=1024
; PWM output pins Reset state control:
                CONFIG    PWMPIN=OFF
; Low-Side Transistors Polarity:
;               CONFIG    LPOL=HIGH
; High-Side Transistors Polarity:
;               CONFIG    HPOL=HIGH
; Timer1 Oscillator MUX:
                CONFIG    T1OSCMX=OFF
; MCLR Enable:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=ON
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
; __IDLOCS _IDLOC0,1
; __IDLOCS _IDLOC1,2
; __IDLOCS _IDLOC2,3
; __IDLOCS _IDLOC3,4
; __IDLOCS _IDLOC4,5
; __IDLOCS _IDLOC5,6
; __IDLOCS _IDLOC6,7
; __IDLOCS _IDLOC7,8
;
; INTRC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
;               ORG     0xF00000
;               DE      "PIC18F2431",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Program flash (row 255)
;
                ORG     0x3FC0
                DB      "PIC18F2431",0
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
NPINS           SET     .28                 ;28-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVLW   b'01110010'         ;INIT CLOCK 8MHZ INTRC
                MOVWF   OSCCON
INITIOFS        BTFSS   OSCCON,IOFS         ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATA                ;INIT PORT A

WATCHDOG 
                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ANSEL0              ;DIGITAL I/O
                CLRF    PWMCON0             ;DISABLE PWM

                MOVLW   b'00000000'         ;PORT A ALL O/P
                MOVWF   TRISA

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
