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
; Velleman K8048 PIC16F877A ICSPIO Demo Test (Receive commands, send data).
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
; 8192 words Flash (14-bit)
; 368 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; !MCLR/VPP           1-----40 RB7/PGD
; RA0 AN0             2     39 RB6/PGC
; RA1 AN1             3     38 RB5
; RA2 AN2 VREF- CVREF 4     37 RB4
; RA3 AN3 VREF+       5     36 RB3/PGM
; RA4 T0CKI C1OUT     6     35 RB2
; RA5 AN4 !SS C2OUT   7     34 RB1
; RE0 !RD AN5         8     33 RB0 INT
; RE1 !WR AN6         9     32 VDD/VCC
; RE2 !CS AN7         10    31 VSS/GND
; VDD/VCC             11    30 RD7 PSP7
; VSS/GND             12    29 RD6 PSP6
; OSC1 CKLI           13    28 RD5 PSP5
; OSC2 CLKO           14    27 RD4 PSP4
; RC0 T1OSO T1CKI     15    26 RC7 RX DT
; RC1 T1OSI CCP2      16    25 RC6 TX CK
; RC2 CCP1            17    24 RC5 SDO
; RC3 SCK SCL         18    23 RC4 RC4 SDI SDA
; RD0 PSP0            19    22 RD3 PSP3
; RD1 PSP1            20----21 RD2 PSP2
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
; k14 program pic16f877a.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F877A
ERRORLEVEL      -302
#INCLUDE        "p16f877a.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CP_OFF & _DEBUG_OFF & _WRT_OFF & _CPD_OFF & _LVP_OFF & _BODEN_OFF & _PWRTE_ON & _WDT_ON & _XT_OSC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; XTAL = 4MHz
    CONSTANT CLOCK = 4000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0x2100
                DE      "PIC16F877A",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x7F
ENDC
#INCLUDE        "shadow.inc"                ;SHADOW I/O
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
NPINS           SET     .40                 ;40-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATD,F
                MOVF    LATD,W
                MOVWF   PORTD

                GOTO    WATCHDOG            ;CONTINUE
POWERUP
                CLRF    LATA                ;INIT PORT A SHADOW
                CLRF    PORTA               ;INIT PORT A

                CLRF    LATB                ;INIT PORT B SHADOW
                CLRF    PORTB               ;INIT PORT B

                CLRF    LATC                ;INIT PORT C SHADOW
                CLRF    PORTC               ;INIT PORT C

                CLRF    LATD                ;INIT PORT D SHADOW
                CLRF    PORTD               ;INIT PORT D

                CLRF    LATE                ;INIT PORT E SHADOW
                CLRF    PORTE               ;INIT PORT E

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    ADCON0              ;SHUTDOWN A/D CONVERTER

                BANKSEL BANK1

                MOVLW   B'00000110'         ;DISABLE A/D CONVERTER
                MOVWF   ADCON1

                MOVLW   B'10001111'         ;DISABLE PULLUPS / WATCHDOG PRESCALE
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORT A I/P
                MOVWF   TRISA

                MOVLW   B'11101111'         ;PORT B SW13 I/P
                MOVWF   TRISB

                MOVLW   B'11111111'         ;PORT C I/P TX/RX UART
                MOVWF   TRISC

                MOVLW   B'00000000'         ;PORT D D1..D8/LCD O/P    
                MOVWF   TRISD

                MOVLW   B'11111100'         ;PORT E LCD
                MOVWF   TRISE

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

                CALL    GETBYTE             ;GET D1-8 ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;SET D1..D8
                MOVWF   LATD                ;UPDATE SHADOW
                MOVWF   PORTD               ;UPDATE PORT D

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
