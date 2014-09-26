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
; Velleman K8048 PIC10F320 ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. One command is implemented.
; The command takes one argument which sets the LED to that value.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Not pin compatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 256 words Flash (14-bit)
; 32 bytes RAM
; 0 bytes EEPROM
;
; Pinout
; ------
; NC          1---8 RA3 !MCLR VPP
; VDD VCC     2   7 VSS GND
; RA2         3   6 NC
; RA1 ICSPCLK 4---5 RA0 ICSPDAT
;
; Breadboard Pin
; ---------- ---
; LD1        RA2 (3)
;
;      +--390R--o GND
;     _|_
;   \\/_\ LD1
;      |
; RA2--+
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
; k14 p pic10f320.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC10F320
ERRORLEVEL      -312
#INCLUDE        "p10f320.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _FOSC_INTOSC & _BOREN_OFF & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _LVP_OFF & _LPBOR_OFF & _BORV_HI & _WRT_OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 16MHz
    CONSTANT CLOCK = 16000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x60                        ;RAM 0x60..0x7F
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
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVLW   b'01110000'         ;INIT CLOCK 16MHZ INTRC
                MOVWF   OSCCON

                BTFSC   STATUS,NOT_TO       ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         CLRF    LATA                ;INIT PORTA
                DECF    LATA,F

WATCHDOG        CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'10000000'         ;DISABLE PULLUPS
                MOVWF   OPTION_REG

                CLRF    ADCON               ;SHUTDOWN A/D CONVERTERS
                CLRF    ANSELA              ;DISABLE A/D
                CLRF    WPUA                ;CLEAR WEAK PULLUPS

                MOVLW   B'11111011'         ;PORTA LD O/P
                MOVWF   TRISA

                CLRWDT                      ;INIT WATCHDOG TIMER
                MOVLW   b'00010011'         ;WATCHDOG TIMER PRESCALE 512ms + ENABLE
                MOVWF   WDTCON              ;START WATCHDOG TIMER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CALL    INITIO              ;INITIALISE ICSPIO
;
MAINLOOP        CLRF    CHECKSUM            ;START SESSION
                CALL    GETBYTE             ;GET COMMAND
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRWDT                      ;UPDATE WATCHDOG
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Validate command
;
                MOVF    BUFFER,W            ;IS SLEEP?
                XORLW   CMD_SLEEP
                BZ      DOSLEEP

                MOVF    BUFFER,W            ;IS LED?
                XORLW   CMD_LED
                BZ      DOLED

                MOVF    BUFFER,W            ;IS ERROR?
                XORLW   CMD_ERROR
                BZ      DOERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Unsupported command
;
                CALL    SENDNAK             ;COMMAND UNSUPPORTED
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Standby
;
DOSLEEP         CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                SLEEP                       ;SLEEP UNTIL WATCHDOG TIME-OUT
                GOTO    INIT                ;RESET ON WAKE-UP
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Command completed
;
DOEND           CALL    SENDSUM             ;CLOSE SESSION
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
