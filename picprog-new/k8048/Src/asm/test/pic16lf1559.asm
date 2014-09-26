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
; R-PI 3V3 PIC16LF1559 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. One command is implemented.
; The command takes one argument which sets the LED to that value.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Incompatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 8192 words Flash (14-bit)
; 512 bytes RAM
; 0 bytes EEPROM
;
; Pinout
; ------
; VDD 3V3       1----20 VSS GND
; RA5           2    19 RA0 ICSPDAT
; RA4           3    18 RA1 ICSPCLK
; RA3 !MCLR VPP 4    17 RA2
; RC5           5    16 RC0
; RC4           6    15 RC1
; RC3           7    14 RC2
; RC6           8    13 RB4
; RC7           9    12 RB5
; RB7           10---11 RB6
;
; Breadboard Pin
; ---------- ---
; LD1        RA2 (17)
;
;      +--390R--o GND
;     _|_
;   \\/_\ LD1
;      |
; RA2--+
;
; Program
; -------
; k14 lvp program pic16lf1559.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16LF1559
ERRORLEVEL      -302
#INCLUDE        "p16lf1559.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF 
  __CONFIG _CONFIG2, _WRT_OFF & _STVREN_ON  & _BORV_LO & _LPBOR_OFF & _LVP_ON 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTRC = 16MHz
    CONSTANT CLOCK = 16000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x7F
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0004
                RETFIE
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
INIT            BANKSEL BANK1

                MOVLW   b'01111010'         ;INIT CLOCK 16MHZ INTRC
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCSTAT,HFIOFS      ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITHFIOFS

                BANKSEL BANK2

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF                ;TOGGLE PORT A
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORT A
                DECF    LATA,F

WATCHDOG        BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS

                BANKSEL BANK1

                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ADCON1              ;DISABLE A/D
                CLRF    ADCON2              ;DISABLE A/D
    
                MOVLW   B'10000000'         ;DISABLE PULLUPS
                MOVWF   OPTION_REG

                MOVLW   B'11111011'         ;PORTA LD O/P
                MOVWF   TRISA

                BANKSEL BANK3

                CLRF    ANSELA              ;DISABLE A/D
                CLRF    ANSELB              ;DISABLE A/D
                CLRF    ANSELC              ;DISABLE A/D

                BANKSEL BANK1

                CLRWDT                      ;INIT WATCHDOG TIMER
                MOVLW   b'00010011'         ;WATCHDOG TIMER PRESCALE 512ms + ENABLE
                MOVWF   WDTCON              ;START WATCHDOG TIMER

                BANKSEL BANK0
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

                GOTO    UNSUPPORTED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set LD1
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVLW   0                   ;SET LD1
                BTFSC   BUFFER,0
                IORLW   0x04
                MOVWF   LATA                ;UPDATE PORTA

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
