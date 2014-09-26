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
; Velleman K8048 PIC16F88 ICSPIO Demo Test (Receive commands, send data).
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
; 4096 words Flash (14-bit)
; 368 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; RA2 AN2 VREF- CVREF 1----18 RA1 AN1
; RA3 AN3 VREF+ C1OUT 2    17 RA0 AN0
; RA4 AN4 T0CKI C2OUT 3    16 RA7 OSC1 CLKI
; RA5 !MCLR VPP       4    15 RA6 OSC2 CLKO
; VSS GND             5    14 VDD VCC
; RB0 INT CCP1        6    13 RB7 AN6 PGD T1OSI
; RB1 SDI SDA         7    12 RB6 AN5 PGC T1OSO T1CKI
; RB2 SDO RX DT       8    11 RB5 !SS TX CK
; RB3 PGM CCP1        9----10 RB4 SCK SCL
;
; K8048 Pin
; ----- ---
; LD1   RB0 (6)
; LD2   RB1 (7)
; LD3   RB2 (8)
; LD4   RB3 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; PGC   RB6 (12)
; PGD   RB7 (13)
; SW1   RA0 (17)
; SW2   RA1 (18)
; SW3   RA2 (1)
; SW4   RA3 (2)
;
; Analog input
; ------------
; +-----+
; |LM35 | 1 +V
; |DZ   | 2 VOUT
; ++-+-++ 3 GND
;  | | |
;  1 2 3
;
; Setup using I.C. test clip:
; Pin 1 +V   to I.C. Pin 14 VDD/VCC
; Pin 2 VOUT to I.C. Pin 3  RA4/AN4
; Pin 3 GND  to I.C. Pin 5  VSS/GND
;
; LM35DZ VOUT is 10mV per degree centigrade and provides 250mV at 25'C.
; Analog input resolution is 10-bit which is approximately 5mV/bit.
; For example, when temperature is 30'C VOUT=300mV which gives 300/5=60
; in the A/D input registers. Temperature 'C = ANX / 2.
;
; A functional equivalent to the LM35 is the TMP35.
;
; Program
; -------
; k14 program pic16f88.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F88
;    0 ALL
;    1 WARN
;    2 ERROR
; -306 NO PAGE
; -302 NO BANK
; -202 NO RANGE
ERRORLEVEL      -302,+306                   ;SEE gperror.h
#INCLUDE        "p16f88.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CONFIG1, _CP_OFF & _CCP1_RB0 & _DEBUG_OFF & _WRT_PROTECT_OFF & _CPD_OFF & _LVP_ON & _BODEN_OFF & _MCLR_ON & _PWRTE_ON & _WDT_OFF & _INTRC_CLKOUT
  __CONFIG _CONFIG2, _IESO_OFF & _FCMEN_OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0x2100
                DE      "PIC16F88",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK  0x20    ;RAM 0x20..0x7F
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
NPINS           SET     .18                 ;18-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK1

                MOVLW   b'01110000'         ;INIT CLOCK 8MHZ INTRC
                MOVWF   OSCCON
INITIOFS        BTFSS   OSCCON,IOFS         ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITIOFS

                BANKSEL BANK0

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATB,F              ;TOGGLE PORT B
                MOVF    LATB,W
                MOVWF   PORTB

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         MOVLW   0xFF
                MOVWF   LATB                ;INIT PORT B SHADOW
                MOVWF   PORTB               ;INIT PORT B

WATCHDOG        CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    ADCON0              ;A/D OFF
                CLRF    ADRESH              ;A/D CLEAR HIGH

                BANKSEL BANK1

                CLRF    ANSEL               ;A/D AN0..AN6 DISABLED
                CLRF    ADRESL              ;A/D CLEAR LOW

                MOVLW   B'10000000'         ;A/D RIGHT JUSTIFIED
                MOVWF   ADCON1

                MOVLW   B'10001111'         ;DISABLE PULLUPS/WATCHDOG PRESCALE
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORT A: SW1..SW4 I/P
                MOVWF   TRISA

                MOVLW   B'11000000'         ;PORT B: LD1..LD6 O/P
                MOVWF   TRISB

                BANKSEL BANK2

                CLRWDT                      ;INIT WATCHDOG TIMER
                BSF     WDTCON,SWDTEN       ;START WATCHDOG TIMER

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

                MOVF    BUFFER,W            ;IS ANALOG?
                XORLW   CMD_ANALOG
                BZ      DOANALOG

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

                MOVF    BUFFER,W            ;SET LD1..LD6
                ANDLW   0x3F
                MOVWF   LATB                ;UPDATE SHADOW
                MOVWF   PORTB               ;UPDATE PORTB

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1..SW4
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVF    PORTA,W             ;GET SW1..SW4
                ANDLW   0x0F 

                CALL    SENDBYTE            ;SEND SW1..SW4
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set analog enable/disable.
;
; ANSEL
; -----
; AN0/RA0
; AN1/RA1
; AN2/RA2
; AN3/RA3
; AN4/RA5 *
; AN5/RB7
; AN6/RB7
;
; *=supported channel
; all unsupported channels disable analog
;
DOANALOG        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET CHANNEL ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;AN4?
                XORLW   0x04
                BZ      DOAN4

                BANKSEL BANK1
                CLRF    ANSEL

                BANKSEL BANK0
                CLRF    ADCON0              ;A/D DISABLE

                GOTO    DOEND               ;COMMAND COMPLETED

DOAN4           BANKSEL BANK1
                BSF     TRISA,5
                BSF     ANSEL,4

                BANKSEL BANK0
                MOVLW   B'11100001'         ;FRC + AN4 + ADON
                MOVWF   ADCON0

                GOTO    DOEND               ;COMMAND COMPLETED
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
