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
; Velleman K8048 PIC18F14K22 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets five LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the first two switches.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 8192 words Flash (16-bit)
; 512 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; VDD VCC       1----20 VSS GND
; RA5           2    19 RA0 PGD
; RA4           3    18 RA1 PGC
; RA3 !MCLR VPP 4    17 RA2
; RC5           5    16 RC0
; RC4           6    15 RC1
; RC3 PGM       7    14 RC2
; ---------------------------------
; RC6           8    13 RB4
; RC7           9    12 RB5
; RB7           10---11 RB6
;
; K8048 Pin
; ----- ---
; LD1   RC0 (10)
; LD2   RC1 (15)
; LD3   RC2 (14)
; LD4   RC3 (7)
; LD5   RC4 (6)
; SW1   RC5 (5)
; SW2   RA2 (17)
;
; Program
; -------
; k16 lvp program pic18f14k22.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F14K22
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f14k22.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
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
                CONFIG    PWRTEN=OFF
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
                CONFIG    LVP=OFF
; Boot Block Size Select bit:
                CONFIG    BBSIZ=OFF 
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
; IRC = 64MHz
    CONSTANT CLOCK = 64000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F14K22",0
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
; ICSP I/O
;
                CONSTANT ICSPTRIS = TRISA   ;ICSP PORT I/O BITS
                CONSTANT ICSPPORT = PORTA
                CONSTANT ICSPLAT  = LATA
                CONSTANT ICSPCLK  = 1
                CONSTANT ICSPDAT  = 0
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
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

                CLRF    OSCCON2             ;INIT CLOCK 16MHZ HFINTOSC PLL X4
                BSF     OSCTUNE,PLLEN
                MOVLW   b'01110000'
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCCON,HFIOFS       ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITHFIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT?
                GOTO    POWERUP

                MOVLW   0xFF                ;WATCHDOG TIMED OUT
                XORWF   LATC,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORT A
                CLRF    LATB                ;INIT PORT B
                CLRF    LATC                ;INIT PORT C
WATCHDOG
                CLRF    WPUA                ;CLEAR WEAK PULLUPS
                CLRF    WPUB                ;CLEAR WEAK PULLUPS

                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ANSEL
                CLRF    ANSELH

                MOVLW   B'11111111'         ;PORT A SW2 I/P PGC+PDD I/P
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PORT B ALL I/P
                MOVWF   TRISB

                MOVLW   B'11100000'         ;PORT C SW1 I/P LD1..LD5 O/P
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
; Set LD1..LD5
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;SET LD1..LD5
                ANDLW   0x1F
                MOVWF   LATC                ;UPDATE LATCH

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1+SW2
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CLRW                        ;GET SW1+SW2
                BTFSC   PORTC,5
                IORLW   1
                BTFSC   PORTA,2
                IORLW   2

                CALL    SENDBYTE            ;SEND SW1+SW2
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
