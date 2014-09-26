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
; Velleman K8048 PIC16F726 ICSPIO Demo Test (Receive commands, send data).
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
; 0 bytes EEPROM
;
; Pinout
; ------
; !MCLR VPP RE3         1-----28 RB7 PGD
; RA0 AN0 !SS VCAP      2     27 RB6 PGC
; RA1 AN1               3     26 RB5 AN13 CPS5 T1G
; RA2 AN2               4     25 RB4 AN11 CPS4
; RA3 AN3 VREF          5     24 RB3 AN9 CPS3 CCP2
; RA4 T0CKI CPS6        6     23 RB2 AN8 CPS2
; RA5 AN4 CPS7 !SS VCAP 7     22 RB1 AN10 CPS1
; VSS GND               8     21 RB0 AN12 CSP0 INT
; OSC1 CLKIN RA7        9     20 VDD VCC
; OSC2 CLKOUT RA6       10    19 VSS GND
; RC0 T1OSO T1CKI       11    18 RC7 RX DT
; RC1 T1OSI CCP2        12    17 RC6 TX CK
; RC2 CCP1              13    16 RC5 SDO
; RC3 SCK SCL           14----15 RC4 SDI SDA
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
; Program
; -------
; k14 program pic16f726.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F726
ERRORLEVEL      -302
#INCLUDE        "p16f726.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CONFIG1, _DEBUG_OFF & _PLL_EN & _BORV_2_5 & _BOR_OFF & _CP_OFF & _MCLR_EN & _PWRT_EN & _WDT_ON & _INTOSCIO 
  __CONFIG _CONFIG2, _VCAP_DIS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 16MHz PLLEN
    CONSTANT CLOCK = 16000000
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

                MOVLW   b'00110000'         ;INIT CLOCK 16MHZ INTRC
                MOVWF   OSCCON
INITICSS        BTFSS   OSCCON,ICSS         ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITICSS

                BANKSEL BANK0

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATA,F
                MOVF    LATA,W
                MOVWF   PORTA

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORT A SHADOW
                DECF    LATA,F
                MOVF    LATA,W
                MOVWF   PORTA               ;INIT PORT A

                CLRF    LATB                ;INIT PORT B SHADOW
                CLRF    PORTB               ;INIT PORT B

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                CLRF    INTCON              ;DISABLE INTERRUPTS

                CLRF    ADCON0              ;SHUTDOWN A/D CONVERTER

                BANKSEL BANK3

                CLRF    ANSELA              ;DISABLE A/D
                CLRF    ANSELB              ;DISABLE A/D

                BANKSEL BANK1

                MOVLW   B'10001111'         ;DISABLE PULLUPS / WATCHDOG PRESCALE
                MOVWF   OPTION_REG

                MOVLW   b'11000000'         ;PORT A LD1..LD6 O/P    
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PORT B PGD/PGC I/P SW1..SW4 I/P
                MOVWF   TRISB

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

                MOVF    BUFFER,W            ;IS TEST?
                XORLW   CMD_TEST
                BZ      DOTEST

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
; Firmware test
;
DOTEST          CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET TEST ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;GET ARG
                MOVWF   ARGUMENT            ;SAVE ARG

                CALL    SENDSUM             ;CLOSE SESSION
                BC      IOERROR             ;TIME-OUT
            
DOFWTEST        MOVLW   0xFE
                MOVWF   PORTA
                MOVLW   DEL50U
                CALL    DELAYU              ;50us

                CLRWDT

                MOVLW   0xFF
                MOVWF   PORTA
                MOVLW   16
                CALL    DELAYH              ;~1.6ms

                GOTO    DOFWTEST
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
