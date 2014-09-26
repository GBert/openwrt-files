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
; Velleman K8048 PIC16F526 ICSPIO Demo Test (Receive commands, send data).
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
; 67 bytes RAM
; 64 bytes data flash
;
; Pinout
; ------
; VDD             1---14 VSS
; RB5/OSC1/CLKIN  2   13 RB0/C1IN+/AN0/ICSPDAT
; RB4/OSC2/CLKOUT 3   12 RB1/C1IN-/AN1/ICSPCLK
; RB3/!MCLR/VPP   4   11 RB2/C1OUT/AN2
; RC5/TOCKI       5   10 RC0/C2IN+
; RC4/C2OUT       6    9 RC1/C2IN-
; RC3             7----8 RC2/CVREF
;
; K8048 Pin
; ----- ------
; LD1   RC0 (10)
; LD2   RC1 (9)
; LD3   RC2 (8)
; LD4   RC3 (7)
; LD5   RC4 (6)
; SW1   RC5 (5)
; SW2   RB2 (11)
;
; Program
; -------
; k12 select 16f526 program pic16f526.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F526
ERRORLEVEL      -302
#INCLUDE        "p16f526.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _FOSC_INTRC_RB4 & _WDTE_ON & _CP_OFF & _MCLRE_ON & _IOSCFS_8MHz & _CPDF_OFF
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
; INTRC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x0D                        ;RAM 0x0D..
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
NPINS           SET     .14                 ;14-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVWF   OSCCAL              ;SAVE OSCILLATOR CALIBRATION

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATC,F
                MOVF    LATC,W
                MOVWF   PORTC

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATB                ;INIT PORT B SHADOW
                CLRF    PORTB               ;INIT PORT B

                CLRF    LATC                ;INIT PORT C SHADOW
                DECF    LATC,F
                MOVF    LATC,W              ;INIT PORT C
                MOVWF   PORTC

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                MOVLW   B'11001111'         ;DISABLE PULLUPS / WATCHDOG PRESCALE
                OPTION

                CLRF    ADCON0              ;SHUTDOWN A/D CONVERTER
                CLRF    CM1CON0             ;DISABLE COMPARATOR 1
                CLRF    CM2CON0             ;DISABLE COMPARATOR 2
                CLRF    VRCON               ;DISABLE VOLTAGE REFERENCE

                MOVLW   B'11111111'         ;PORT B SW2 I/P PGC+PDD I/P
                MOVWF   TRISB               ;INIT TRIS SHADOW
                TRIS    PORTB               ;INIT TRIS

                MOVLW   B'11100000'         ;PORT C SW1 I/P LD1..LD5 O/P
                MOVWF   TRISC               ;INIT TRIS SHADOW
                TRIS    PORTC               ;INIT TRIS
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
                BTFSC   PORTB,2
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
