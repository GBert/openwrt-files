;
; Velleman K8048 PIC16F506 ICSPIO Demo Test (Receive commands, send data).
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; VDD             1---14 VSS
; RB5/OSC1/CLKIN  2   13 RB0/AN0/C1IN+/ICSPDAT
; RB4/OSC2/CLKOUT 3   12 RB1/AN1/C1IN-/ICSPCLK
; RB3/!MCLR/VPP   4   11 RB2/AN2/C1OUT
; RC5/TOCKI       5   10 RC0/C2IN+
; RC4/C2OUT       6    9 RC1/C2IN-
; RC3             7----8 RC2/VCREF
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
; k12 select 16f506 program pic16f506.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F506
ERRORLEVEL      -302
#INCLUDE        "p16f506.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; K8048 PIC16F506 ICSPIO Demo Test (Receive commands, send data).
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
; Config
;
  __CONFIG _IOSCFS_ON & _MCLRE_ON & _CP_OFF & _WDT_ON & _IntRC_OSC_RB4EN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 4MHz
    CONSTANT CLOCK = 4000000
;   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x10                        ;RAM 0x10..
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

POWERUP         CLRF    LATB                ;INIT PORTA SHADOW
                CLRF    PORTB               ;INIT PORTA

                CLRF    LATC                ;INIT PORTB SHADOW
                DECF    LATC,F
                MOVF    LATC,W              ;INIT PORTB
                MOVWF   PORTC

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                MOVLW   B'00001111'         ;WATCHDOG PRESCALE/T0CS=0
                OPTION

                CLRF    ADCON0
                CLRF    CM1CON0
                CLRF    CM2CON0

                MOVLW   B'11111111'         ;PORT B SW2 I/P
                MOVWF   TRISB               ;INIT TRIS SHADOW
                TRIS    PORTB               ;INIT TRIS

                MOVLW   B'11100000'         ;PORT C PGD+PDC I/P LD1..LD5 O/P SW1 I/P
                MOVWF   TRISC               ;INIT TRIS SHADOW
                TRIS    PORTC               ;INIT TRIS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CLRF    LASTERROR
;
MAINLOOP        CLRF    CHECKSUM            ;START SESSION
                CALL    GETBYTE             ;GET COMMAND
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRWDT                      ;UPDATE WATCHDOG
;
; COMMAND VALIDATE
;
                MOVF    BUFFER,W            ;IS SLEEP?
                XORLW   CMD_SLEEP
                BZ      DOSLEEP

                MOVF    BUFFER,W            ;IS LED?
                XORLW   CMD_LED
                BZ      DOLED

                MOVF    BUFFER,W            ;IS SWITCH?
                XORLW   CMD_SWITCH
                BZ      DOSWITCH

                MOVF    BUFFER,W            ;IS ERROR?
                XORLW   CMD_ERROR
                BZ      DOERROR
;
; COMMAND UNSUPPORTED
;
                CALL    SENDNAK             ;COMMAND UNSUPPORTED
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
; Standby
;
DOSLEEP         CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                SLEEP                       ;SLEEP UNTIL WATCHDOG TIME-OUT
NOTREACHED
;
; Set LD1..LD5
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;SAVE LD ARG
                ANDLW   0x1F
                MOVWF   LATC                ;UPDATE SHADOW
                MOVWF   PORTC               ;UPDATE PORTC

                GOTO    DOEND               ;COMMAND COMPLETED
;
; Get SW1..SW2
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CLRW                        ;GET SW1+SW2
                BTFSC   PORTC,5
                IORLW   1
                BTFSC   PORTB,2
                IORLW   2

                CALL    SENDBYTE            ;SEND SW1..SW2
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
; Get last error
;
DOERROR         CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVF    LASTERROR,W
                CLRF    LASTERROR
                CALL    SENDBYTE
                BC      IOERROR
;
; Command completed
;
DOEND           CALL    SENDSUM             ;CLOSE SESSION
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
; Time-out, protocol or parity error
;
IOERROR         MOVWF   LASTERROR

                GOTO    MAINLOOP            ;CONTINUE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
