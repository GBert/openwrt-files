                NOLIST
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
; ICSP I/O for 12/14/16-bit word architecture
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Port I/O bits.
;
; SETUP: Set NPINS to package type to gain default port I/O bits.
;        For user-defined port I/O bits just define as required
;        and do not set NPINS.
;
#IFDEF NPINS

IF NPINS == .6 || NPINS == .8
IFDEF TRISIO
    CONSTANT ICSPTRIS = TRISIO
    CONSTANT ICSPPORT = GPIO
    CONSTANT ICSPLAT  = LATIO
ELSE ; PIC12F519 
    CONSTANT ICSPTRIS = TRISB
    CONSTANT ICSPPORT = PORTB
    CONSTANT ICSPLAT  = LATB
ENDIF
    CONSTANT ICSPCLK  = 1
    CONSTANT ICSPDAT  = 0
ENDIF

IF NPINS == .14 || NPINS == .20
IFDEF TRISA
    CONSTANT ICSPTRIS = TRISA
    CONSTANT ICSPPORT = PORTA
    CONSTANT ICSPLAT  = LATA
ELSE ; PIC16F505/506
    CONSTANT ICSPTRIS = TRISB
    CONSTANT ICSPPORT = PORTB
    CONSTANT ICSPLAT  = LATB
ENDIF
    CONSTANT ICSPCLK  = 1
    CONSTANT ICSPDAT  = 0
ENDIF

IF NPINS == .18 || NPINS == .28 || NPINS == .40
    CONSTANT ICSPTRIS = TRISB
    CONSTANT ICSPPORT = PORTB
    CONSTANT ICSPLAT  = LATB
    CONSTANT ICSPCLK  = 6
    CONSTANT ICSPDAT  = 7
ENDIF

#ENDIF ; NPINS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Time-out high counter reload
;
IFNDEF TOHIGHLOAD
    IF CLOCK <= 125000
        CONSTANT TOHIGHLOAD = 1     ; minimum
    ELSEIF CLOCK < 32000000
        CONSTANT TOHIGHLOAD = CLOCK / 125000
    ELSE
        CONSTANT TOHIGHLOAD = 0     ; maximum (0x100)
    ENDIF
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Commands (where available)
;
    CONSTANT CMD_LED        = 0x01  ; 0x00..0x3F        SET K8048 LEDS
    CONSTANT CMD_SWITCH     = 0x02  ;                   GET K8048 SWITCHES

    CONSTANT CMD_SLEEP      = 0x10  ;                   SLEEP UNTIL WATCHDOG TIME-OUT
    CONSTANT CMD_WATCHDOG   = 0x11  ; 0|1               SET WATCHDOG ENABLE/DISABLE
    CONSTANT CMD_CLOCK      = 0x12  ; 0x00..0xFF        SET INTERNAL RC CLOCK DIVIDER

    CONSTANT CMD_DIRECTION  = 0x20  ; 0..5 0x00..0xFF   SET PORT DATA DIRECTION
    CONSTANT CMD_OUTPUT     = 0x21  ; 0..5 0x00..0xFF   SET PORT DATA OUTPUT
    CONSTANT CMD_INPUT      = 0x22  ; 0..5              GET PORT DATA INPUT

    CONSTANT CMD_ANALOG     = 0x30  ; 0..N|0xFF         SET ANALOG CHANNEL ENABLE/DISABLE
    CONSTANT CMD_SAMPLEW    = 0x31  ;                   GET ANALOG CHANNEL WORD INPUT
;   CONSTANT CMD_SAMPLEB    = 0x32  ;                   GET ANALOG CHANNEL BYTE INPUT
    CONSTANT CMD_VREF       = 0x38  ; 0..15|0xFF        SET VREF VOLTAGE LEVEL OR DISABLE

    CONSTANT CMD_EEREAD     = 0x40  ; ADDRESS           READ DATA EEPROM
    CONSTANT CMD_EEWRITE    = 0x41  ; ADDRESS DATA      WRITE DATA EEPROM

    CONSTANT CMD_READW      = 0x50  ; ADDRESH ADDRESL   READ PROGRAM FLASH WORD
;   CONSTANT CMD_READB      = 0x51  ; ADDRESH ADDRESL   READ PROGRAM FLASH BYTE
;   CONSTANT CMD_WRITEW     = 0x52  ;                   UNIMPLIMENTED
;   CONSTANT CMD_WRITEB     = 0x53  ;                   UNIMPLIMENTED

    CONSTANT CMD_ARG8       = 0xF0  ; 0..0xFF           SEND AN 8-BIT ARG & GET AN 8-BIT ANSWER
    CONSTANT CMD_ARG16      = 0xF1  ; 0..0xFFFF         SEND A 16-BIT ARG & GET AN 8-BIT ANSWER
    CONSTANT CMD_ARG24      = 0xF2  ; 0..0xFFFFFF       SEND A 24-BIT ARG & GET AN 8-BIT ANSWER
    CONSTANT CMD_ARG32      = 0xF3  ; 0..0xFFFFFFFF     SEND A 32-BIT ARG & GET AN 8-BIT ANSWER

    CONSTANT CMD_TEST       = 0xFE  ; 0..0xFF           SEND AN 8-BIT TEST ARG & GET NO REPLY
    CONSTANT CMD_ERROR      = 0xFF  ;                   GET LAST FIRMWARE ERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Return codes
;
    CONSTANT ERRNONE     = 0    ; OK
    CONSTANT ERRTIMEOUT  = 1    ; CLOCK TIMED OUT
    CONSTANT ERRPROTOCOL = 2    ; INVALID STOP OR START BIT
    CONSTANT ERRPARITY   = 3    ; INVALID PARITY BIT
    CONSTANT ERRNOTSUP   = 4    ; COMMAND NOT SUPPORTED
    CONSTANT ERRINVALID  = 5    ; INVALID RESPONSE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK
    BUFFER      ;DATA
    PARITY      ;PARITY BIT
    CHECKSUM    ;I/O CHECK SUM
    LASTERROR   ;LAST ERROR CODE
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise latch for ICSPIO (INIT ICSPCLK = 0 & ICSPDAT = 0)
;
; params  : NONE
; result  : W=0
; affects : LASTERROR
;
INITIO          CLRF    LASTERROR
;
; DATA/CLOCK = LOW
;
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL ICSPLAT
ENDIF
                BCF     ICSPLAT,ICSPDAT ;INIT LATCH
                BCF     ICSPLAT,ICSPCLK
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IFDEF SHADOWLATCH                       ;INIT PORT
                MOVF    ICSPLAT,W
                MOVWF   ICSPPORT
ENDIF
;               FOLLOW-THROUGH
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise TRIS for ICSPIO (INIT ICSPCLK = I/P & ICSPDAT = I/P)
;
INITTRIS
;
; DATA/CLOCK = INPUT
;
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL ICSPTRIS
ENDIF
                BSF     ICSPTRIS,ICSPDAT;INIT TRIS
                BSF     ICSPTRIS,ICSPCLK
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for ICSPCLK I/P High (REQUIRES ICSPCLK = I/P)
;
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR
; error   : W=ERRTIMEOUT, CARRY SET
; affects : COUNTER1, COUNTER2
;
WAITHIGH        BCF     STATUS,C
                MOVLW   TOHIGHLOAD
                MOVWF   COUNTER1
                CLRF    COUNTER2
WAITHIGHTEST    BTFSC   ICSPPORT,ICSPCLK
                RETLW   ERRNONE         ;NO ERROR (CARRY CLEAR)
                DJNZ    COUNTER2,WAITHIGHTEST
                DJNZ    COUNTER1,WAITHIGHTEST
                BSF     STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT (CARRY SET)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for ICSPCLK I/P Low (REQUIRES ICSPCLK = I/P)
;
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR
; error   : W=ERRTIMEOUT, CARRY SET
; affects : COUNTER1, COUNTER2
;
WAITLOW         BCF     STATUS,C
                MOVLW   TOHIGHLOAD
                MOVWF   COUNTER1
                CLRF    COUNTER2
WAITLOWTEST     BTFSS   ICSPPORT,ICSPCLK
                RETLW   ERRNONE         ;NO ERROR (CARRY CLEAR)
                DJNZ    COUNTER2,WAITLOWTEST
                DJNZ    COUNTER1,WAITLOWTEST
                BSF     STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT (CARRY SET)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get in a clocked byte (REQUIRES ICSPDAT = I/P)
; 
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR, BUFFER
; error   : W=ERRTIMEOUT OR ERRPROTOCOL OR ERRPARITY, CARRY SET
; affects : COUNTER0, COUNTER1, COUNTER2
;
GETBYTE         CLRF    BUFFER          ;CLEAR INPUT BYTE
                CLRF    PARITY          ;EVEN PARITY
                MOVLW   0x08            ;8 BITS IN A BYTE
                MOVWF   COUNTER0

                CALL    INITTRIS        ;RE-INIT I/O TRIS
;
; START BIT CLOCK HIGH
;
                CALL    WAITHIGH        ;START BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; START BIT CLOCK LOW
;
                CALL    WAITLOW         ;START BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; START BIT INPUT
;
                BTFSS   ICSPPORT,ICSPDAT;TEST START BIT LOW
                GOTO    GETBIT          ;START BIT LOW...
                BSF     STATUS,C        ;START BIT NOT LOW
                RETLW   ERRPROTOCOL     ;PROTOCOL ERROR (CARRY SET)
;
; DATA BIT CLOCK HIGH
;
GETBIT          CALL    WAITHIGH        ;DATA BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; DATA BIT CLOCK LOW
;
                CALL    WAITLOW         ;DATA BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; DATA BIT INPUT
;
IF ARCH == PIC18
                RRCF    BUFFER,F        ;ROTATE IN CLR BIT
ELSE
                RRF     BUFFER,F        ;ROTATE IN CLR BIT
ENDIF
                MOVLW   0
                BTFSS   ICSPPORT,ICSPDAT
                GOTO    GETPAR
                BSF     BUFFER,7        ;SET BIT
                MOVLW   1
GETPAR          XORWF   PARITY,F
                DJNZ    COUNTER0,GETBIT ;FOR ALL BITS
;
; PARITY BIT CLOCK HIGH
;
                CALL    WAITHIGH        ;PARITY BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; PARITY BIT CLOCK LOW
;
                CALL    WAITLOW         ;PARITY BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; PARITY BIT INPUT
;
                MOVLW   0               ;UPDATE PARITY
                BTFSC   ICSPPORT,ICSPDAT
                MOVLW   1
                XORWF   PARITY,F
;
; CHECK PARITY
;
                BTFSC   STATUS,Z
                GOTO    GETSTOP
                BSF     STATUS,C
                RETLW   ERRPARITY       ;PARITY ERROR (CARRY SET)
;
; STOP BIT CLOCK HIGH
;
GETSTOP         CALL    WAITHIGH        ;STOP BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; STOP BIT CLOCK LOW
;
                CALL    WAITLOW         ;STOP BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; STOP BIT INPUT
;
                BTFSC   ICSPPORT,ICSPDAT;TEST STOP BIT HIGH
                GOTO    GETSUM          ;STOP BIT HIGH
                BSF     STATUS,C        ;STOP BIT NOT HIGH
                RETLW   ERRPROTOCOL     ;PROTOCOL ERROR (CARRY SET)
GETSUM
                MOVF    BUFFER,W
                ADDWF   CHECKSUM,F
                BCF     STATUS,C
                RETLW   ERRNONE         ;NO ERROR (CARRY CLEAR)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Send negative acknowledge
;
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR
; error   : W=ERRTIMEOUT, CARRY SET
; affects : BUFFER, COUNTER0, COUNTER1, COUNTER2
;
SENDNAK         MOVLW   NAK
                GOTO    SENDBYTE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Send acknowledge
;
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR
; error   : W=ERRTIMEOUT, CARRY SET
; affects : BUFFER, COUNTER0, COUNTER1, COUNTER2
;
SENDACK         MOVLW   ACK
                GOTO    SENDBYTE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Send checksum
;
; params  : NONE
; result  : W=ERRNONE, CARRY CLEAR
; error   : W=ERRTIMEOUT, CARRY SET
; affects : BUFFER, COUNTER0, COUNTER1, COUNTER2
;
SENDSUM         MOVF    CHECKSUM,W
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Send out a clocked byte (W)
;
; params  : W
; result  : W=0, CARRY CLEAR
; error   : W=1, CARRY SET (TIME-OUT)
; affects : BUFFER, COUNTER0, COUNTER1, COUNTER2
;
SENDBYTE        MOVWF   BUFFER          ;SAVE OUTPUT BYTE
                ADDWF   CHECKSUM,F      ;UPDATE CHECKSUM
                CLRF    PARITY          ;EVEN PARITY
                MOVLW   0x08            ;8 BITS IN A BYTE
                MOVWF   COUNTER0

                CALL    INITTRIS        ;RE-INIT I/O TRIS
;
; START BIT CLOCK HIGH
;
                CALL    WAITHIGH        ;START BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; START BIT OUTPUT (ICSPDAT = O/P)
;
                TRISPD  ICSPTRIS,ICSPDAT;SET START BIT LOW
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
;
; START BIT CLOCK LOW
;
                CALL    WAITLOW         ;START BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; DATA BIT CLOCK HIGH
;
SENDBIT         CALL    WAITHIGH        ;DATA BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; DATA BIT OUTPUT (ICSPDAT = I/P OR O/P)
;
IF ARCH == PIC18
                RRCF    BUFFER,F        ;ROTATE OUT BIT
ELSE
                RRF     BUFFER,F        ;ROTATE OUT BIT
ENDIF
                BTFSC   STATUS,C
                GOTO    SENDSET
                TRISPD  ICSPTRIS,ICSPDAT;SET DATA BIT LOW
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
                MOVLW   0
                GOTO    SENDPAR
SENDSET
                TRISPU  ICSPTRIS,ICSPDAT;SET DATA BIT HIGH
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
                MOVLW   1
SENDPAR
                XORWF   PARITY,F        ;UPDATE PARITY
;
; DATA BIT CLOCK LOW
;
                CALL    WAITLOW         ;DATA BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT

                DJNZ    COUNTER0,SENDBIT;FOR ALL BITS
;
; PARITY BIT CLOCK HIGH (ICSPDAT = I/P OR O/P)
; 
                CALL    WAITHIGH        ;PARITY BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; PARITY BIT OUTPUT
;
                MOVF    PARITY,F        ;TEST PARITY
                BTFGC   STATUS,Z,SENDSET2
                TRISPD  ICSPTRIS,ICSPDAT;SET PARITY BIT LOW
                GOTO    SENDPAR2
SENDSET2
                TRISPU  ICSPTRIS,ICSPDAT;SET PARITY BIT HIGH
SENDPAR2
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
;
; PARITY BIT CLOCK LOW
; 
                CALL    WAITLOW         ;PARITY BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; STOP BIT CLOCK HIGH
;
                CALL    WAITHIGH        ;STOP BIT CLOCK HIGH
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
;
; STOP BIT OUTPUT (ICSPDAT = I/P)
;
                TRISPU  ICSPTRIS,ICSPDAT;SET STOP BIT HIGH
IF ARCH == BASELINE
                MOVF    ICSPTRIS,W
                TRIS    ICSPPORT
ENDIF
;
; STOP BIT CLOCK LOW
;
                CALL    WAITLOW         ;STOP BIT CLOCK LOW
                BTFSC   STATUS,C
                RETLW   ERRTIMEOUT      ;TIME-OUT
                RETLW   ERRNONE         ;SUCCESS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; .vimrc:
;  " GPASM Include
;  augroup module
;   autocmd BufRead *.inc set filetype=asm
;  augroup END
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
                LIST
