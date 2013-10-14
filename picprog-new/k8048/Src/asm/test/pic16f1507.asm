;
; Velleman K8048 PIC16F1507 ICSPIO Demo Test (Receive commands, send data).
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This assembly file was tested with gpasm-0.15.0 only.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Not pin compatible with the K8048, however, this device may be used in the
; 14P socket with pins 8 to 13 unconnected.
;
; 2048 words Flash (14-bit)
; 128 bytes RAM
; 0 bytes EEPROM
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
; LD1   RC0
; LD2   RC1
; LD3   RC2
; LD4   RC3
; LD5   RC4
; SW1   RC5
; SW2   RA2
;
; Program
; -------
; k14 program pic16f1507.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F1507
ERRORLEVEL      -302
#INCLUDE        "p16f1507.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;*****************************************************************************
;
; K8048 PIC16F1507 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets five LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the first two switches.
;
;*****************************************************************************
;
; Config
;
  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_ON & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF 
  __CONFIG _CONFIG2, _WRT_OFF & _STVREN_ON  & _BORV_LO & _LPBOR_OFF & _LVP_ON 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS 0x1234
;
ERRORLEVEL      -220
                ORG     0x8000
                DE      'A' ;[8000] USERID0
                DE      'B' ;[8001] USERID1
                DE      'C' ;[8002] USERID2
                DE      'D' ;[8003] USERID3
                DE      'E' ;[8004] RESERVED
                DE      'F' ;[8005] RESERVED
ERRORLEVEL      +220
;
; INTOSC = 16MHz
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

                MOVLW   0xFF
                XORWF   LATC,F
                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORTA
                CLRF    LATC                ;INIT PORTC
                DECF    LATC,F

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS

                BANKSEL BANK1

                CLRF    ADCON0              ;SHUTDOWN A/D CONVERTERS

                MOVLW   B'10001111'         ;DISABLE PULLUPS / WATCHDOG PRESCALE
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORTA PGD+PDC I/P SW2 I/P
                MOVWF   TRISA

                MOVLW   B'11100000'         ;PORTC LD1..LD5 O/P SW1 I/P
                MOVWF   TRISC

                BANKSEL BANK3

                CLRF    ANSELA              ;DISABLE A/D
                CLRF    ANSELB              ;DISABLE A/D
                CLRF    ANSELC              ;DISABLE A/D

                BANKSEL BANK4

                CLRF    WPUA                ;CLEAR WEAK PULLUPS
                CLRF    WPUB                ;CLEAR WEAK PULLUPS

                BANKSEL BANK0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CLRF    LASTERROR
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
