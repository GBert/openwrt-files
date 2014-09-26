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
; Velleman K8048 PIC16F527 ICSPIO Demo Test (Receive commands, send data).
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
; 1024 words Flash (12-bit)
; 68 bytes RAM
; 64 bytes data flash
;
; Pinout
; ------
; VDD VCC       1----20 VSS GND
; RA5           2    19 RA0 ICSPDAT
; RA4           3    18 RA1 ICSPCLK
; RA3 !MCLR VPP 4    17 RA2
; RC5           5    16 RC0
; RC4           6    15 RC1
; RC3           7    14 RC2
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
; k12 select 16f527 program pic16f527.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F527
ERRORLEVEL      -302
#INCLUDE        "p16f527.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _FOSC_INTRC_IO & _WDTE_ON & _CP_OFF & _IOSCFS_8MHz & _CPSW_OFF & _BOREN_OFF & _DRTEN_OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; GPASM ASSIGNS __IDLOCS TO 0x0400 IN ERROR
; __IDLOCS 0xABCD                           ;0x0440..0x0443
ERRORLEVEL      -220
                ORG     0x0440
                DW      1,2,3,4
ERRORLEVEL      +220
;
; INTRC_OSC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x0C                        ;RAM 0x0C..
ENDC
#INCLUDE        "shadow.inc"                ;SHADOW I/O
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
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
; Initialise
;
INIT            MOVWF   OSCCAL              ;SAVE OSCILLATOR CALIBRATION

                MOVLB   0x00                ;BANK 000

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATC,F
                MOVF    LATC,W
                MOVWF   PORTC

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORT A SHADOW
                CLRF    PORTA               ;INIT PORT A

                CLRF    LATC                ;INIT PORT C SHADOW
                DECF    LATC,F
                MOVF    LATC,W              ;INIT PORT C
                MOVWF   PORTC

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                CLRF    INTCON0             ;DISABLE INTERRUPTS

                CLRF    ADCON0              ;SHUTDOWN A/D CONVERTER

                MOVLB   0x03                ;BANK 011

                CLRF    INTCON1             ;DISABLE INTERRUPTS

                MOVLB   0x01                ;BANK 001

                CLRF    ANSEL               ;DISABLE A/D CONVERTER
                CLRF    CM1CON0             ;DISABLE COMPARATORS
                CLRF    CM2CON0             ;DISABLE COMPARATORS

                MOVLB   0x00                ;BANK 000

                MOVLW   B'11001111'         ;DISABLE PULLUPS / WATCHDOG PRESCALE
                OPTION

                MOVLW   b'11000000'         ;PORT C LD1..LD6 O/P
                TRIS    PORTC

                MOVLW   b'11111111'         ;PORT A PGD/PGC I/P SW1..SW4 I/P
                TRIS    PORTA
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
                MOVWF   LATC                ;UPDATE SHADOW
                MOVWF   PORTC               ;UPDATE PORTB

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
