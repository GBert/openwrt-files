;
; Velleman K8048 PIC12F508 ICSPIO Demo Test (Receive commands, send data).
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 512 words Flash (12-bit)
; 25 bytes RAM
; 0 bytes daa flash
;
; Pinout
; ------
; VDD VCC        1----8 VSS GND
; GP5 OSC1 CLKIN 2    7 GP0 ICSPDAT
; GP4 OSC2       3    6 GP1 ICSPCLK
; GP3 !MCLR VPP  4----5 GP2 T0CKI
;
; K8048 Pin
; ----- ---
; LD1   GP2 (5)
; LD2   GP4 (3)
; SW1   GP5 (2)
;
;        7654 3210
; TRISIO XX10 X0XX
;        LD1+LD2 O/P
;        SW1     I/P
;
; K8048 GPIO probes
; -----------------
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
; k12 select 12f508 program pic12f508.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC12F508
ERRORLEVEL      -302
#INCLUDE        "p12f508.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;******************************************************************************
;
; K8048 PIC12F508 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets two LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the first switch.
;
;******************************************************************************
;
; Config
;
  __CONFIG _MCLRE_ON & _CP_OFF & _WDT_ON & _IntRC_OSC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTRC_OSC = 4MHz
    CONSTANT CLOCK = 4000000
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
                XORWF   LATIO,F
                MOVF    LATIO,W
                MOVWF   GPIO
                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATIO               ;INIT GPIO SHADOW
                CLRF    GPIO                ;INIT GPIO

WATCHDOG        CLRWDT                      ;INIT WATCHDOG

                MOVLW   B'00001111'         ;WATCHDOG PRESCALE
                OPTION

                MOVLW   B'11101011'         ;GPIO SW1 I/P LD1+LD2 O/P
                MOVWF   TRISIO              ;INIT TRISIO SHADOW
                TRIS    GPIO                ;INIT TRIS
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
                MOVWF   LATIO               ;UPDATE SHADOW
                MOVWF   GPIO                ;UPDATE GPIO

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVLW   0                   ;GET SW1
                BTFSC   GPIO,5
                MOVLW   1

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
