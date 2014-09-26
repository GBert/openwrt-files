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
; Velleman K8048 PIC16F886 ICSPIO Demo Test (Receive commands, send data).
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
; 8192 words Flash (14-bit)
; 368 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; RE3 !MCLR VPP             1-----28 RB7 PGD
; RA0 AN0 ULWPU C12IN0-     2     27 RB6 PGC
; RA1 AN1 C12IN1-           3     26 RB5 AN13 !T1G
; RA2 AN2 VREF- CVREF C2IN+ 4     25 RB4 AN11 P1D
; RA3 AN3 VREF+ C1IN+       5     24 RB3 AN9 PGM C12IN2-
; RA4 T0CKI C1OUT           6     23 RB2 AN8 P1B
; RA5 AN4 !SS C2OUT         7     22 RB1 AN10 P1C C12IN3-
; VSS GND                   8     21 RB0 AN12 INT
; RA7 OSC1 CLKIN            9     20 VDD VCC
; RA6 OSC2 CLKOUT           10    19 VSS GND
; RC0 T1OSO T1CKI           11    18 RC7 RX DT
; RC1 T1OSI CCP2            12    17 RC6 TX CK
; RC2 P1A CCP1              13    16 RC5 SDO
; RC3 SCK SCL               14----15 RC4 SDI SDA
;
; Function Pin Note
; -------- --- ----
; LD1      RA0
; LD2      RA1
; LD3      RA2
; LD4      RA3
; LD5      RA4
; LD6      RA5
; SW1      RB0
; SW2      RB1
; SW3      RB2
; SW4      RB4
;
; Analog input
; ------------
; +-----+
; |TMP36| 1 +V
; |GZ   | 2 VOUT
; ++-+-++ 3 GND
;  | | |
;  1 2 3
;
; TMP36GZ VOUT is 10mV per degree centigrade and provides 750mV at 25'C.
; Analog input resolution is 10-bit which is approximately 5mV/bit.
;
; For example, when temperature is 30'C VOUT=800mV which gives 800/5=160
; in the A/D input registers. Temperature 'C = (ANX - 100) / 2.
; 
; A functional equivalent to the TMP36 is the LM50.
;
; Program
; -------
; k14 program pic16f886.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F886
;    0 ALL
;    1 WARN
;    2 ERROR
; -306 NO PAGE
; -302 NO BANK
; -202 NO RANGE
ERRORLEVEL      -302,+306                   ;SEE gperror.h
#INCLUDE        "p16f886.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CONFIG1, _DEBUG_OFF & _LVP_ON & _FCMEN_OFF & _IESO_OFF & _BOR_OFF & _CPD_OFF & _CP_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_OFF & _INTOSCIO
  __CONFIG _CONFIG2, _WRT_OFF & _BOR21V
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; XTAL = 11059200Hz
;   CONSTANT CLOCK = 11059200
; INTOSC
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0x2100
                DE      "PIC16F886",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK 0x20     ;RAM 0x20..0x7F
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
NPINS           SET     .28                 ;28-PIN PDIP
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
INITHTS         BTFSS   OSCCON,HTS          ;WAIT FOR INTOSC FREQUENCY STABLE
                GOTO    INITHTS

                BANKSEL BANK0

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF                ;TOGGLE PORT A
                XORWF   LATA,F
                MOVF    LATA,W
                MOVWF   PORTA

                MOVLW   0xFF                ;TOGGLE PORT B
                XORWF   LATB,F
                MOVF    LATB,W
                MOVWF   PORTB

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         MOVLW   0xFF
                MOVWF   LATA                ;INIT PORT A SHADOW
                MOVWF   PORTA               ;INIT PORT A

                CLRF    LATB                ;INIT PORT B SHADOW
                CLRF    PORTB               ;INIT PORT B

                CLRF    LATC                ;INIT PORT C SHADOW
                CLRF    PORTC               ;INIT PORT C

WATCHDOG        CLRF    INTCON              ;DISABLE INTERRUPTS

                CLRF    ADCON0              ;A/D OFF
                CLRF    ADRESH              ;A/D CLEAR HIGH

                BANKSEL BANK3
                CLRF    ANSEL               ;A/D AN0..AN7  DISABLED
                CLRF    ANSELH              ;A/D AN8..AN13 DISABLED

                BANKSEL BANK1
                CLRF    ADRESL              ;A/D CLEAR LOW

                MOVLW   B'10000000'         ;A/D RIGHT JUSTIFIED
                MOVWF   ADCON1

                MOVLW   B'10001100'         ;DISABLE PULLUPS/WATCHDOG PRESCALE
                MOVWF   OPTION_REG

                MOVLW   B'00000000'         ;PORT A: LD1..LD6 O/P    
                MOVWF   TRISA

                MOVLW   B'00010111'         ;PORT B: SW1..SW4 I/P
                MOVWF   TRISB

                MOVLW   B'00000000'         ;PORT C: ALL O/P
                MOVWF   TRISC

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
#IF 1
                MOVF    BUFFER,W            ;IS ANALOG?
                XORLW   CMD_ANALOG
                BZ      DOANALOG
#ENDIF
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
                MOVWF   LATA                ;UPDATE SHADOW
                MOVWF   PORTA               ;UPDATE PORT A

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1..SW4
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVF    PORTB,W             ;GET SW1..SW3
                ANDLW   B'00000111'
                BTFSC   PORTB,4             ;GET SW4
                IORLW   B'00001000'

                CALL    SENDBYTE            ;SEND SW1..SW4
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set analog enable/disable.
;
; ANSEL     ANSELH
; -----     ------
; AN0/RA0   AN8/RB2 *
; AN1/RA1   AN9/RB3 *
; AN2/RA2   AN10/RB1
; AN3/RA3   AN11/RB4
; AN4/RA5   AN13/RB5
;
; *=supported channel
; all unsupported channels disable analog
;
DOANALOG        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET CHANNEL ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;AN8?
                XORLW   0x08
                BZ      DOAN8

                MOVF    BUFFER,W            ;AN9?
                XORLW   0x09
                BZ      DOAN9

                BANKSEL BANK3
                CLRF    ANSELH

                BANKSEL BANK0
                CLRF    ADCON0              ;A/D DISABLE

                GOTO    DOEND               ;COMMAND COMPLETED

DOAN8           BANKSEL BANK1
                BSF     TRISB,2

                BANKSEL BANK3
                BSF     ANSELH,0

                BANKSEL BANK0
                MOVLW   B'11100001'         ;FRC + AN8 + ADON
                MOVWF   ADCON0

                GOTO    DOEND               ;COMMAND COMPLETED

DOAN9           BANKSEL BANK1
                BSF     TRISB,3

                BANKSEL BANK3
                BSF     ANSELH,1

                BANKSEL BANK0
                MOVLW   B'11100101'         ;FRC + AN9 + ADON
                MOVWF   ADCON0

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
