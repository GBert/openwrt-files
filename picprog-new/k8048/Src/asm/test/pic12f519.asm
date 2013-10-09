;
; Velleman K8048 PIC12F519 ICSPIO Demo Test (Receive commands, send data).
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
; VDD VCC        1----8 VSS GND
; RB5 OSC1 CLKIN 2    7 RB0 ICSPDAT
; RB4 OSC2       3    6 RB1 ICSPCLK
; RB3 !MCLR VPP  4----5 RB2 T0CKI
;
; K8048 Pin
; ----- ---
; LD1   RB2 (5)
; LD2   RB4 (3)
; SW1   RB5 (2)
;
;       7654 3210
; TRISB XX10 X0XX
;       LD1+LD2 O/P
;       SW1     I/P
;
; K8048 PORTB probes
; ------------------
;     8PIN 14PIN 18PIN 28PIN
;     ---- ----- ----- -----
; GP0    7    13    13    28 PGD
; GP1    6    12    12    27 PGC
; GP2    5    10     6     2 LD1
; GP3    4     4             VPP
; GP4    3     9     7     3 LD2
; GP5    2          17    21 SW1
;
; Program
; -------
; k12 select 12f519 program pic12f519.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC12F519
ERRORLEVEL      -302
#INCLUDE        "p12f519.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; K8048 PIC12F519 ICSPIO Demo Test (Receive commands, send data).
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
  __CONFIG _CPDF_OFF & _IOSCFS_8MHz & _MCLRE_ON & _CP_OFF & _WDTE_ON & _IntRC_OSC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS 0x1234
;
; INTRC_OSC = 8MHz
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x08                        ;RAM 0x08..
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
NPINS           SET     .8                  ;8-PIN PDIP
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
                XORWF   LATB,F
                MOVF    LATB,W
                MOVWF   PORTB
                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATB                ;INIT PORTB SHADOW
                DECF    LATB,F
                MOVF    LATB,W              ;INIT PORTB
                MOVWF   PORTB

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                MOVLW   B'00001111'         ;WATCHDOG PRESCALE
                OPTION

                MOVLW   B'11101011'         ;PORTB SW1 I/P LD1+LD2 O/P
                MOVWF   TRISB               ;INIT TRISB SHADOW
                TRIS    PORTB               ;INIT TRIS
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
; Set LD1+LD2
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVLW   0                   ;SET LD1+LD2
                BTFSC   BUFFER,0
                IORLW   0x04
                BTFSC   BUFFER,1
                IORLW   0x10
                MOVWF   LATB                ;UPDATE SHADOW
                MOVWF   PORTB               ;UPDATE PORTB

                GOTO    DOEND               ;COMMAND COMPLETED
;
; Get SW1
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVLW   0                   ;GET SW1
                BTFSC   PORTB,5
                MOVLW   1

                CALL    SENDBYTE            ;SEND SW1..SW4
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
