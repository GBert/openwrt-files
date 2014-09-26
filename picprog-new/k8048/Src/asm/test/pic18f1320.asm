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
; Velleman K8048 PIC18F1320 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets the six LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the four switches.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; RA0 AN0            1----18 RB3 CCP1 P1A
; RA1 AN1 LVDIN      2    17 RB2 P1B INT2
; RA4 T0CKI          3    16 OSC1 CLKI RA7
; !MCLR VPP RA5      4    15 OSC2 CLKO RA6
; VSS AVSS GND       5    14 VDD AVDD VCC
; RA2 AN2 VREF-      6    13 RB7 PGD T1OSI P1D KBI3
; RA3 AN3 VREF+      7    12 RB6 PGC T1OSO T13CKI P1C KBI2
; RB0 AN4 INT0       8    11 RB5 PGM KBI1
; RB1 AN5 TX CK INT1 9----10 RB4 AN6 RX DT KBI0
;
; K8048 Pin
; ----- ---
; LD1   RA2 (6)
; LD2   RA3 (7)
; LD3   RB0 (8)
; LD4   RB1 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; SW1   RB2 (17)
; SW2   RB3 (18)
; SW3   RA0 (1)
; SW4   RA1 (2)
;       RA4 (3)
; VPP   RA5 (4)
; PGC   RB6 (12)
; PGD   RB7 (13)
; OSC2  RA6 (15)
; OSC1  RA7 (16)
; GND   VSS (5)
; +5V   VDD (14)
;
; Program
; -------
; k16 p pic18f1320.hex
;
; OSCCON Power management
; -----------------------
; IDLEN SCS1:SCS0   Mode            Clock
; 0     00          SLEEP           NONE
; 0     00          RUN PRIMARY     XTAL
; 0     01          RUN SECONDARY   TIMER1
; 0     1X          RUN RC          INTRC
; 1     00          IDLE PRIMARY    XTAL 
; 1     01          IDLE SECONDARY  TIMER1
; 1     1X          IDLE RC         INTRC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F1320
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f1320.inc"
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
; Oscillator Selection:
                CONFIG    OSC=INTIO2
; Fail-Safe Clock Monitor:
                CONFIG    FSCM=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=ON
; Brown-out Reset:
                CONFIG    BOR=OFF
; Brown-out Voltage:
                CONFIG    BORV=27
; Watchdog Timer:
                CONFIG    WDT=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; MCLR Enable:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset:
                CONFIG    STVR=ON
; Low Voltage ICSP:
                CONFIG    LVP=ON
; Background Debugger Enable:
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
; __IDLOCS _IDLOC0,1
; __IDLOCS _IDLOC1,2
; __IDLOCS _IDLOC2,3
; __IDLOCS _IDLOC3,4
; __IDLOCS _IDLOC4,5
; __IDLOCS _IDLOC5,6
; __IDLOCS _IDLOC6,7
; __IDLOCS _IDLOC7,8
;
ERRORLEVEL      -220 
                ORG     0x200000
                DB      1,2,3,4,5,6,7,8
ERRORLEVEL      +220 
;
; INTOSC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F1320",0
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
NPINS           SET     .18                 ;18-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
; XXX With LVP mode enabled RB5 is not available and this pin will not operate
;     as this demo expects.
;
INIT            MOVLW   b'01110010'         ;INIT CLOCK 8MHZ INTRC
                MOVWF   OSCCON
INITIOFS        BTFSS   OSCCON,IOFS         ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   b'00001100'
                XORWF   LATA,F              ;TOGGLE RA2 RA3
                MOVLW   b'00110011'
                XORWF   LATB,F              ;TOGGLE RB0 RB1 RB4 RB5

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         MOVLW   b'00001100'         ;INIT PORTA
                MOVWF   LATA                ;SET RA2 RA3
                MOVLW   b'00110011'         ;INIT PORTB
                MOVWF   LATB                ;SET RB0 RB1 RB4 RB5

WATCHDOG        MOVLW   b'00000000'         ;DISABLE A/D
                MOVWF   ADCON0
                MOVLW   b'00001111'         ;DIGITAL I/O
                MOVWF   ADCON1

                MOVLW   b'11110011'         ;PORTA O/P RA2 RA3
                MOVWF   TRISA               ;LD1+LD2  O/P SW3+SW4 I/P

                MOVLW   b'11001100'         ;PORTB O/P RB0 RB1 RB4 RB5
                MOVWF   TRISB               ;LD3..LD6 O/P SW1+SW2 I/P

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
; Set LD1..LD6
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRW                        ;SET LD1..LD2
                BTFSC   BUFFER,0
                IORLW   B'00000100'         ;LD1 = RA2
                BTFSC   BUFFER,1
                IORLW   B'00001000'         ;LD2 = RA3
                MOVWF   LATA
                CLRW                        ;SET LD3..LD6
                BTFSC   BUFFER,2
                IORLW   B'00000001'         ;LD3 = RB0
                BTFSC   BUFFER,3
                IORLW   B'00000010'         ;LD4 = RB1
                BTFSC   BUFFER,4
                IORLW   B'00010000'         ;LD5 = RB4
                BTFSC   BUFFER,5
                IORLW   B'00100000'         ;LD6 = RB5
                MOVWF   LATB

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1..SW4
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CLRW                        ;GET SW1..SW4
                BTFSC   PORTB,2
                IORLW   B'00000001'         ;SW1 = RB2
                BTFSC   PORTB,3
                IORLW   B'00000010'         ;SW2 = RB3
                BTFSC   PORTA,0
                IORLW   B'00000100'         ;SW2 = RA0
                BTFSC   PORTA,1
                IORLW   B'00001000'         ;SW2 = RA1

                CALL    SENDBYTE            ;SEND SW1..SW4
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
