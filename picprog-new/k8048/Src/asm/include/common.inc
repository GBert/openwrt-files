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
; ICSP I/O common commands
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK
    ARGUMENT    ;FIRST ARG
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Macro
;
COMMON          MACRO   DONE, WAKEUP
; 
; DONE          Commmand completed GOTO Eg. MAINLOOP
; WAKEUP        Wake-up from sleep GOTO Eg. INIT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get command
;
                CLRF    CHECKSUM            ;START SESSION
                CALL    GETBYTE             ;GET COMMAND
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRWDT                      ;UPDATE WATCHDOG
;VALIDATE
                MOVF    BUFFER,W            ;IS SLEEP?
                XORLW   CMD_SLEEP
                BTFGS   STATUS,Z,DOSLEEP
#IFDEF WDTCON
                MOVF    BUFFER,W            ;IS WATCHDOG?
                XORLW   CMD_WATCHDOG
                BTFGS   STATUS,Z,DOWATCHDOG
#ENDIF
#IFDEF OSCCON
                MOVF    BUFFER,W            ;IS CLOCK?
                XORLW   CMD_CLOCK
                BTFGS   STATUS,Z,DOCLOCK
#ENDIF
                MOVF    BUFFER,W            ;IS DIRECTION?
                XORLW   CMD_DIRECTION
                BTFGS   STATUS,Z,DODIRECTION

                MOVF    BUFFER,W            ;IS OUTPUT?
                XORLW   CMD_OUTPUT
                BTFGS   STATUS,Z,DOOUTPUT

                MOVF    BUFFER,W            ;IS INPUT?
                XORLW   CMD_INPUT
                BTFGS   STATUS,Z,DOINPUT
#IFDEF ADRESH
                MOVF    BUFFER,W            ;IS SAMPLEW?
                XORLW   CMD_SAMPLEW
                BTFGS   STATUS,Z,DOSAMPLEW
#ENDIF
#IFDEF VRCON
                MOVF    BUFFER,W            ;IS VREF?
                XORLW   CMD_VREF
                BTFGS   STATUS,Z,DOVREF
#ENDIF
#IFDEF EEDATA
    IF ARCH != BASELINE

                MOVF    BUFFER,W            ;IS EEREAD?
                XORLW   CMD_EEREAD
                BTFGS   STATUS,Z,DOEEREAD

                MOVF    BUFFER,W            ;IS EEWRITE?
                XORLW   CMD_EEWRITE
                BTFGS   STATUS,Z,DOEEWRITE
    ENDIF ; !BASELINE
#ENDIF
#IFDEF EEDATH
                MOVF    BUFFER,W            ;IS READW?
                XORLW   CMD_READW
                BTFGS   STATUS,Z,DOREADW
#ENDIF
                MOVF    BUFFER,W            ;IS ERROR?
                XORLW   CMD_ERROR
                BTFGS   STATUS,Z,DOERROR
;UNHANDLED
                GOTO    CONTINUE            ;COMMAND NOT COMMON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Sleep
;
DOSLEEP         CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                SLEEP                       ;SLEEP UNTIL WATCHDOG TIME-OUT
IF ARCH != BASELINE                         ;BASELINE WILL RESET
                GOTO    WAKEUP              ;CARRY CLEAR
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Watchdog
;
#IFDEF WDTCON
DOWATCHDOG      CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL WDTCON
ENDIF
                ANDLW   0x01                ;0=DISABLE 1=ENABLE
                BTFGS   STATUS,Z,DISABLE

                BSF     WDTCON,0            ;ENABLE WATCHDOG
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
DISABLE
                BCF     WDTCON,0            ;DISABLE WATCHDOG
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF ;WDTCON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Internal RC clock divider
;
#IFDEF OSCCON
DOCLOCK         CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;PREPARE IRCF0..IRCF2
                ANDLW   0x07
                MOVWF   ARGUMENT
                CLRC
IF ARCH != PIC18
                RLF     ARGUMENT,F
                RLF     ARGUMENT,F
                RLF     ARGUMENT,F
                RLF     ARGUMENT,F
ELSE
                RLCF    ARGUMENT,F
                RLCF    ARGUMENT,F
                RLCF    ARGUMENT,F
                RLCF    ARGUMENT,F
ENDIF
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL OSCCON
ENDIF
                MOVF    OSCCON,W
IFDEF IOFS
                ANDLW   B'10000011'         ;GET IDLEN/SCS1/SCS0
ELSE
                ANDLW   B'10000001'         ;GET SCS
ENDIF
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                IORWF   ARGUMENT,W          ;GET IDLEN/IRCF0..IFCF2/SCS1/SCS0
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL OSCCON
ENDIF
                MOVWF   OSCCON              ;SET CLOCK FREQUENCY
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF ;OSCCON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data direction
;
DODIRECTION     CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET PORT ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W
                MOVWF   ARGUMENT            ;SAVE PORT ARG

                CALL    GETBYTE             ;GET DATA DIRECTION ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
                MOVF    ARGUMENT,W
                XORLW   0
                BTFGS   STATUS,Z,DOGPIO_DIR
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
                MOVF    ARGUMENT,W
                XORLW   1
                BTFGS   STATUS,Z,DOPORTA_DIR
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
                MOVF    ARGUMENT,W
                XORLW   2
                BTFGS   STATUS,Z,DOPORTB_DIR
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
                MOVF    ARGUMENT,W
                XORLW   3
                BTFGS   STATUS,Z,DOPORTC_DIR
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
                MOVF    ARGUMENT,W
                XORLW   4
                BTFGS   STATUS,Z,DOPORTD_DIR
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;               MOVF    ARGUMENT,W
;               XORLW   5
;               BTFGS   STATUS,Z,DOPORTE_DIR
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                GOTO    DOEND               ;PORT NOT FOUND
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
DOGPIO_DIR      MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISIO
ENDIF
                MOVWF   TRISIO
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                TRIS    TRISIO
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
DOPORTA_DIR     MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISA
ENDIF
                MOVWF   TRISA
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                TRIS    TRISA
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
DOPORTB_DIR     MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISB
ENDIF
                MOVWF   TRISB
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                TRIS    TRISB
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
DOPORTC_DIR     MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISC
ENDIF
                MOVWF   TRISC
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                TRIS    TRISC
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
DOPORTD_DIR     MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISD
ENDIF
                MOVWF   TRISD
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE
                TRIS    TRISD
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;DOPORTE_DIR    MOVF    BUFFER,W
;IF ARCH == MIDRANGE || ARCH == ENHANCED
;               BANKSEL TRISE
;ENDIF
;               MOVWF   TRISE
;IF ARCH == MIDRANGE || ARCH == ENHANCED
;               BANKSEL BANK0
;ENDIF
;IF ARCH == BASELINE
;               TRIS    TRISE
;ENDIF
;               GOTO    DOEND               ;COMMAND COMPLETED
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data output
;
DOOUTPUT        CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET PORT ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W
                MOVWF   ARGUMENT            ;SAVE PORT ARG

                CALL    GETBYTE             ;GET DATA 
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
                MOVF    ARGUMENT,W
                XORLW   0
                BTFGS   STATUS,Z,DOGPIO_OUT
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
                MOVF    ARGUMENT,W
                XORLW   1
                BTFGS   STATUS,Z,DOPORTA_OUT
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
                MOVF    ARGUMENT,W
                XORLW   2
                BTFGS   STATUS,Z,DOPORTB_OUT
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
                MOVF    ARGUMENT,W
                XORLW   3
                BTFGS   STATUS,Z,DOPORTC_OUT
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
                MOVF    ARGUMENT,W
                XORLW   4
                BTFGS   STATUS,Z,DOPORTD_OUT
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;               MOVF    ARGUMENT,W
;               XORLW   5
;               BTFGS   STATUS,Z,DOPORTE_OUT
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                GOTO    DOEND               ;PORT NOT FOUND
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
DOGPIO_OUT      MOVF    BUFFER,W
IF ARCH == ENHANCED
                BANKSEL LATIO
ENDIF
                MOVWF   LATIO
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE || ARCH == MIDRANGE
                MOVWF   GPIO
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
DOPORTA_OUT     MOVF    BUFFER,W
IF ARCH == ENHANCED
                BANKSEL LATA
ENDIF
                MOVWF   LATA
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE || ARCH == MIDRANGE
                MOVWF   PORTA
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
DOPORTB_OUT     MOVF    BUFFER,W
IF ARCH == ENHANCED
                BANKSEL LATB
ENDIF
                MOVWF   LATB
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE || ARCH == MIDRANGE
                MOVWF   PORTB
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
DOPORTC_OUT     MOVF    BUFFER,W
IF ARCH == ENHANCED
                BANKSEL LATC
ENDIF
                MOVWF   LATC
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE || ARCH == MIDRANGE
                MOVWF   PORTC
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
DOPORTD_OUT     MOVF    BUFFER,W
IF ARCH == ENHANCED
                BANKSEL LATD
ENDIF
                MOVWF   LATD
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
IF ARCH == BASELINE || ARCH == MIDRANGE
                MOVWF   PORTD
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;DOPORTE_OUT    MOVF    BUFFER,W
;IF ARCH == ENHANCED
;               BANKSEL LATE
;ENDIF
;               MOVWF   LATE
;IF ARCH == ENHANCED
;               BANKSEL BANK0
;ENDIF
;IF ARCH == BASELINE || ARCH == MIDRANGE 
;               MOVWF   PORTE
;ENDIF
;IFDEF __18F25K22
;               MOVWF   PORTE
;ENDIF
;               GOTO    DOEND               ;COMMAND COMPLETED
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data input
;
DOINPUT         CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET PORT ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
                MOVF    BUFFER,W
                XORLW   0
                BTFGS   STATUS,Z,DOGPIO_IN
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
                MOVF    BUFFER,W
                XORLW   1
                BTFGS   STATUS,Z,DOPORTA_IN
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
                MOVF    BUFFER,W
                XORLW   2
                BTFGS   STATUS,Z,DOPORTB_IN
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
                MOVF    BUFFER,W
                XORLW   4
                BTFGS   STATUS,Z,DOPORTC_IN
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
                MOVF    BUFFER,W
                XORLW   4
                BTFGS   STATUS,Z,DOPORTD_IN
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;               MOVF    BUFFER,W
;               XORLW   5
;               BTFGS   STATUS,Z,DOPORTE_IN
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                MOVLW   0
                GOTO    CMD_INPUTSEND           ;PORT NOT FOUND
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF GPIO
DOGPIO_IN       MOVF    GPIO,W
                GOTO    CMD_INPUTSEND
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTA
DOPORTA_IN      MOVF    PORTA,W
                GOTO    CMD_INPUTSEND
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTB
DOPORTB_IN      MOVF    PORTB,W
                GOTO    CMD_INPUTSEND
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTC
DOPORTC_IN      MOVF    PORTC,W
                GOTO    CMD_INPUTSEND
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
#IFDEF PORTD
DOPORTD_IN      MOVF    PORTD,W
                GOTO    CMD_INPUTSEND
#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;#IFDEF PORTE
;DOPORTE_IN     MOVF    PORTE,W
;#ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
CMD_INPUTSEND   CALL    SENDBYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get analog sample word
;
#IFDEF ADRESH
DOSAMPLEW       CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT
IF ARCH == ENHANCED
                BANKSEL ADCON0              ;BANK1
ENDIF
                BSF     ADCON0,GO_NOT_DONE  ;A/D GO!
DOSAMPLEWWAIT   BTFSC   ADCON0,GO_NOT_DONE  ;A/D DONE?
                GOTO    DOSAMPLEWWAIT       ;NOT DONE...
IF ARCH == MIDRANGE
                BANKSEL ADRESL              ;BANK1
ENDIF
                MOVF    ADRESL,W            ;GET LOW BYTE
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                MOVWF   ARGUMENT            ;SAVE LOW BYTE
IF ARCH == ENHANCED
                BANKSEL ADRESH              ;BANK1
ENDIF
                MOVF    ADRESH,W            ;GET HIGH BYTE
IF ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                CALL    SENDBYTE            ;SEND HIGH BYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                MOVF    ARGUMENT,W          ;GET SAVED LOW BYTE
                CALL    SENDBYTE            ;SEND LOW BYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF ; ADCON0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set VREF
;
#IFDEF VRCON
DOVREF          CALL    SENDACK             ;COMMAND SUPPORTED

                CALL    GETBYTE             ;GET VOLTAGE ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
        
                MOVF    BUFFER,W
                XORLW   0xFF
                BZ      DOVRCON             ;SET VRCON=0 (OFF)

                MOVF    BUFFER,W
                ANDLW   0x0F
                IORLW   0xE0
DOVRCON
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL VRCON
ENDIF
                MOVWF   VRCON
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                GOTO    DOEND
#ENDIF ; VRCON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; EEPROM Read/Write
;
#IFDEF EEDATA
    IF ARCH != BASELINE
DOEEREAD        CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET ADDRESS ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
        
                MOVF    BUFFER,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEADR
ENDIF
                MOVWF   EEADR               ;SET ADDRESS
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EECON1
ENDIF
IFDEF EEPGD
                BCF     EECON1,EEPGD
ENDIF
                BSF     EECON1,RD           ;READ EEPROM DATA
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEDATA
ENDIF
                MOVF    EEDATA,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                CALL    SENDBYTE            ;SEND DATA BYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOEEWRITE       CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET ADDRESS ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
        
                MOVF    BUFFER,W
                MOVWF   ARGUMENT            ;SAVE ADDRESS ARG
 
                CALL    GETBYTE             ;GET DATA ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
 
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EECON1
ENDIF
                BTFSC   EECON1,WR
                GOTO    $-1
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                MOVF    ARGUMENT,W          ;GET ADDRESS
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEADR
ENDIF
                MOVWF   EEADR               ;STORE ADDRESS
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                MOVF    BUFFER,W            ;GET DATA
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEDATA
ENDIF
                MOVWF   EEDATA              ;STORE DATA

IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EECON1
ENDIF
IFDEF EEPGD
                BCF     EECON1,EEPGD
ENDIF
                BSF     EECON1,WREN         ;WRITE ENABLE EEPROM DATA
                BCF     INTCON,GIE          ;DISABLE ANY INTERRUPTS
                BTFSC   INTCON,GIE
                GOTO    $-2

                MOVLW   0x55
                MOVWF   EECON2
                MOVLW   0xAA
                MOVWF   EECON2
                BSF     EECON1,WR           ;WRITE EEPROM DATA

                BSF     INTCON,GIE          ;ENABLE ANY INTERRUPTS
                BCF     EECON1,WREN         ;WRITE DISABLE EEPROM DATA

IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                GOTO    DOEND               ;COMMAND COMPLETED
    ENDIF ; !BASELINE
#ENDIF ; EEDATA
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Read flash word
;
#IFDEF EEDATH
DOREADW         CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                CALL    GETBYTE             ;GET ADDRESS HIGH ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR
        
                MOVF    BUFFER,W
                MOVWF   ARGUMENT            ;SAVE ADDRESS HIGH ARG
 
                CALL    GETBYTE             ;GET ADDRESS LOW ARG
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;ADDRESS LOW
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEADR
ENDIF
                MOVWF   EEADR
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                MOVF    ARGUMENT,W          ;ADDRESS HIGH
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEADRH
ENDIF
                MOVWF   EEADRH
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EECON1
ENDIF
                BSF     EECON1,EEPGD
                BSF     EECON1,RD           ;READ FLASH DATA
                NOP
                NOP
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEDATH
ENDIF
                MOVF    EEDATH,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                CALL    SENDBYTE            ;SEND HIGH BYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL EEDATA
ENDIF
                MOVF    EEDATA,W
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                CALL    SENDBYTE            ;SEND LOW BYTE
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
#ENDIF ; EEDATH
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get last error
;
DOERROR         CALL    SENDACK             ;COMMAND SUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                MOVF    LASTERROR,W
                CLRF    LASTERROR
                CALL    SENDBYTE
                BTFGS   STATUS,C,IOERROR

;               GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Command completed
;
DOEND           CALL    SENDSUM             ;CLOSE SESSION
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DONE                ;CARRY CLEAR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Command unsupported
;
UNSUPPORTED     CALL    SENDNAK             ;COMMAND UNSUPPORTED
                BTFGS   STATUS,C,IOERROR    ;TIME-OUT

                GOTO    DONE                ;CARRY CLEAR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Time-out, protocol or parity error
;
IOERROR         MOVWF   LASTERROR           ;SAVE ERROR

                GOTO    DONE                ;CARRY SET
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Other command handlers follow...
;
CONTINUE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
                LIST
