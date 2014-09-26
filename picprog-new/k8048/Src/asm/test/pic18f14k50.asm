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
; R-PI 3V3 PIC18F14K50
;
; This demo allows control from `kio' to perform ICSPIO common operations.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Incompatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 8192 words Flash (16-bit)
; 768 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; VDD VCC       1----20 VSS GND
; RA5           2    19 RA0 ICSPDAT
; RA4           3    18 RA1 ICSPCLK
; RA3 !MCLR VPP 4    17 RA2
; RC5           5    16 RC0
; RC4           6    15 RC1
; RC3 PGM       7    14 RC2
; RC6           8    13 RB4
; RC7           9    12 RB5
; RB7           10---11 RB6
;
; RA0/RA1 ARE INPUT ONLY WHEN DIGITAL
;
; Program
; -------
; k16 lvp program pic18f14k50.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F14K50
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f14k50.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; CPU System Clock Selection bits:
                CONFIG    CPUDIV=NOCLKDIV ; No CPU System Clock divide
; USB Clock Selection bit:
                CONFIG    USBDIV=OFF ; USB clock comes directly from the OSC1/OSC2 oscillator block; no divide
; Oscillator Selection bits:
                CONFIG    FOSC=IRC ; Internal RC oscillator
; 4 X PLL Enable bit:
                CONFIG    PLLEN=OFF
; Primary Clock Enable bit:
                CONFIG    PCLKEN=OFF
; Fail-Safe Clock Monitor Enable:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover bit:
                CONFIG    IESO=OFF
; Power-up Timer Enable bit:
                CONFIG    PWRTEN=ON
; Brown-out Reset Enable bits:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=30
; Watchdog Timer Enable bit:
                CONFIG    WDTEN=OFF ; WDT is controlled by SWDTEN bit of the WDTCON register
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; HFINTOSC Fast Start-up bit:
                CONFIG    HFOFST=OFF 
; Master Clear Reset Pin Enable:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Single-Supply ICSP Enable bit:
                CONFIG    LVP=ON
; Extended Instruction Set Enable bit:
                CONFIG    XINST=OFF
; Background Debugger Enable bit:
                CONFIG    DEBUG=OFF
; Code Protection Block 0:
                CONFIG    CP0=OFF
; Code Protection Block 1:
                CONFIG    CP1=OFF
; Boot Block Code Protection:
                CONFIG    CPB=OFF
; Data EEPROM Code Protection:
                CONFIG    CPD=OFF
; Write Protection Block 0:
                CONFIG    WRT0=OFF
; Write Protection Block 1:
                CONFIG    WRT1=OFF
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
; Boot Block Table Read Protection:
                CONFIG    EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; IRC = 32MHz
    CONSTANT CLOCK = 32000000
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
                GOTO    INTHIGH             ;When IPEN=0 or 1
                ORG     0x0018
                GOTO    INTLOW              ;When IPEN=1
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; High priority ISR
; 
INTHIGH
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Low priority ISR
;
INTLOW
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                CLRF    OSCCON2             ;INIT CLOCK 8MHZ HFINTOSC PLL X4
                BSF     OSCTUNE,SPLLEN
                MOVLW   b'01100000'
                MOVWF   OSCCON
INITHFIOFL      BTFSS   OSCCON2,HFIOFL      ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITHFIOFL

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT?
                GOTO    POWERUP

                MOVLW   0xFF                ;WATCHDOG TIMED OUT
                XORWF   LATB,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORT A
                CLRF    LATB                ;INIT PORT B
                CLRF    LATC                ;INIT PORT C
WATCHDOG
                CLRF    UCON                ;DISABLE USB
                CLRF    UCFG                ;DISABLE PULLUPS

                SETF    IOCA                ;ENABLE RA0/1 INPUTS

                CLRF    WPUA                ;CLEAR WEAK PULLUPS
                CLRF    WPUB                ;CLEAR WEAK PULLUPS

                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ANSEL
                CLRF    ANSELH

                MOVLW   b'11111111'         ;PORTA ALL I/P
                MOVWF   TRISA

                MOVLW   b'00000000'         ;PORTB ALL O/P
                MOVWF   TRISB

                MOVLW   b'11111111'         ;PORTC ALL I/P
                MOVWF   TRISC

                CLRWDT                      ;INIT WATCHDOG TIMER
                BSF     WDTCON,SWDTEN       ;START WATCHDOG TIMER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAINLOOP        
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
