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
; Velleman K8048 PIC16F627A/628A/628A PS/2 to Serial protocol mouse
;
; TEST MOUSE #1 GENIUS GM-050021 TRAVELER 320 OPTICAL MOUSE
; TEST MOUSE #2 EXCEL AM-1310U+P MINI OPTICAL MOUSE
;
; JUMPERS 1..4
; ------------
;
;  1 RB4 ALGORITHM  0=DIVIDE 1=LIMIT
;    USE DIVIDE MODE FOR TEST MOUSE #1
;    USE LIMIT  MODE FOR TEST MOUSE #2
;    APPLIED IN REAL-TIME
;
;  2 RB5 SCALING    0=1:1 1=2:1
;    THIS SETTING APPEARED TO HAVE NO IMPACT ON THE TEST MICE
;    APPLIED AFTER RESET OR HOST INITIATED REINIT
;
;  3 RB6 RESOLUTION B'00XXXXXX'=1 B'01XXXXXX'=2 counts/millimeter
;  4 RB7 RESOLUTION B'10XXXXXX'=4 B'11XXXXXX'=8 counts/millimeter
;    THIS SETTING APPEARED TO HAVE NO IMPACT ON THE TEST MICE
;    APPLIED AFTER RESET OR HOST INITIATED REINIT
;
; TEST PLATFORMS
; --------------
;
;   WORKING
;   -------
;   MS-DOS MICROSOFT MOUSE 8.20 & 9.01
;   WINDOWS 98SE
;   WINDOWS XP
;   LINUX GPM
;
;   NOT WORKING
;   -----------
;   MS-DOS LOGITECH
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 2048 words Flash (14-bit)
; 224 bytes RAM
; 128 bytes EEPROM
;
; Pinout
; ------
; RA2 AN2   VREF 1----18 RA1 AN1
; RA3 AN3   CMP1 2    17 RA0 AN0
; RA4 T0CKI CMP2 3    16 RA7 OSC1  CLKIN
; RA5 !MCLR VPP  4    15 RA6 OSC2  CLKOUT
; VSS GND        5    14 VDD VCC
; RB0 INT        6    13 RB7 T1OSI PGD
; RB1 RX    DT   7    12 RB6 T1OSO T1CKI PGC
; RB2 TX    CK   8    11 RB5
; RB3 CCP1       9----10 RB4 PGM
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F628A
ERRORLEVEL      -302
#INCLUDE        "p16f628a.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CP_OFF & _CPD_OFF & _LVP_OFF & _BODEN_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTOSC_OSC_NOCLKOUT
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
; ASYNC BAUD RATE (BRGH = 0) SPBRG = FOSC / BAUD RATE / 64 - 1
;
; BAUD RATE
    CONSTANT SPEED = CLOCK / 1200 / 64 - 1
;
; MOUSE SAMPLE RATE
    CONSTANT SAMPLERATE = 20                ;20 SAMPLES/SECOND AT 1200 BAUD
;
    CONSTANT PS2OPTIONS = 1                 ;ENABLE/DISABLE PS/2 OPTIONS ON RB5/RB6/RB7
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x6F
    MOUSESW
    MOUSEDX
    MOUSEDY
ENDC
#INCLUDE        "shadow.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
; INTEDG takes a branch to REINIT and there is no return from interrupt.
; Stack overflow is unimportant.
;
                ORG     0x0000
                GOTO    INIT                ;RESET
                ORG     0x0004
                GOTO    REINIT              ;INTEDG
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; PS/2 mouse support
;
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "ps2.inc"                   ;PS/2 PROTOCOL
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                MOVLW   B'10101000'
                MOVWF   LATA                ;INIT PORT A SHADOW
                MOVWF   PORTA               ;INIT PORT A

REINIT          BANKSEL BANK0

                MOVLW   5                   ;5ms PAUSE ON RESET/REINIT
                CALL    DELAYM

                MOVF    LATA,W              ;TOGGLE TEST LED(S)
                XORLW   B'11111100'
                MOVWF   LATA
                MOVWF   PORTA

                MOVLW   B'00000000'
                MOVWF   LATB                ;INIT PORT B SHADOW
                MOVWF   PORTB               ;INIT PORT B

                CLRF    INTCON              ;DISABLE INTERRUPTS
                CLRF    PIR1                ;CLEAR INTERRRUPT FLAGS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                BANKSEL BANK1

                CLRF    PIE1                ;DISABLE PERIPHERAL INTERRUPTS

                MOVLW   B'00001111'         ;ENABLE PULL-UPS & INIT INTEDG FALLING
                MOVWF   OPTION_REG

                MOVLW   B'00000011'         ;PORT A RA0/RA1 PS/2 INPUT OTHERS TEST LED O/P
                MOVWF   TRISA               ;INIT TRISA

                MOVLW   B'11111111'         ;PORT B ALL I/P
                MOVWF   TRISB               ;INIT TRISB
;UART
                MOVLW   LOW SPEED           ;BAUD RATE GENERATOR SPEED
                MOVWF   SPBRG

                MOVLW   1<<TXEN             ;TX ENABLE
                MOVWF   TXSTA

                BANKSEL BANK0

                MOVLW   1<<SPEN | 1<<CREN   ;UART ENABLE + CONTINUOUS RX
                MOVWF   RCSTA

                MOVF    RCREG,W             ;DISCARD
                MOVF    RCREG,W             ;DISCARD
;INT
                MOVLW   B'10010000'         ;INTEDG ENABLED
                MOVWF   INTCON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Send serial mouse identification
; 
                MOVLW   'M'                 ;I'M A SERIAL MOUSE
                CALL    TRANSMIT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; PS/2 Init
; 
PS2INIT
;STOP!
                MOVLW   PS2_CMD_RESET_DIS
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
;RESET
                MOVLW   PS2_CMD_RESET_BAT
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
                CALL    PS2RECEIVE          ;AA
                BTFGS   STATUS,C,PS2INIT
                CALL    PS2RECEIVE          ;00
                BTFGS   STATUS,C,PS2INIT
;SET SAMPLE RATE
                MOVLW   PS2_CMD_SETRATE
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
                MOVLW   SAMPLERATE
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
#IF PS2OPTIONS == 1
;SET RESOLUTION
                MOVLW   PS2_CMD_SETRES
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
                MOVLW   0
                BTFSC   PORTB,6
                IORLW   1
                BTFSC   PORTB,7
                IORLW   2
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
;SET SCALING
                MOVLW   PS2_CMD_SETSCALE11
                BTFSC   PORTB,5
                MOVLW   PS2_CMD_SETSCALE21
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
#ENDIF
;GO!
                MOVLW   PS2_CMD_ENABLE
                CALL    PS2SEND             ;FA
                BTFGS   STATUS,C,PS2INIT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAINLOOP
;GET PS/2 PACKET
                CALL    PS2RECEIVE          ;SWITCHES
                BTFGS   STATUS,C,MAINLOOP
                MOVF    BUFFER,W
                MOVWF   MOUSESW

                CALL    PS2RECEIVE          ;DX
                BTFGS   STATUS,C,PS2INIT
                MOVF    BUFFER,W
                MOVWF   MOUSEDX

                CALL    PS2RECEIVE          ;DY
                BTFGS   STATUS,C,PS2INIT
                MOVF    BUFFER,W
                MOVWF   MOUSEDY
;SEND MS MOUSE CONVERSION
                CALL    MSM
;REPEAT
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; RS-232 transmit
;
TRANSMIT        BTFSS   PIR1,TXIF           ;WAIT FOR TX
                GOTO    TRANSMIT
                MOVWF   TXREG               ;SEND DATA
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Microsoft mouse conversion
;
MSM             BTFSC   PORTB,4             ;0=DIVIDE 1=LIMIT
                GOTO    LIMIT
DIVIDE
;ARITHMETIC SHIFT RIGHT X
                BCF     STATUS,C
                BTFSC   MOUSESW,4           ;SIGN X
                BSF     STATUS,C
                RRF     MOUSEDX,F
;ARITHMETIC SHIFT RIGHT Y
                BCF     STATUS,C
                BTFSC   MOUSESW,5           ;SIGN Y
                BSF     STATUS,C
                RRF     MOUSEDY,F
                GOTO    COMDY
LIMIT
;IF X < -128
;   LET X = -128
;ELSE IF X > 127 
;   LET X = 127
;ENDIF
                MOVLW   0xFF
                BTFSC   MOUSESW,4           ;SIGN X
                XORWF   MOUSEDX,F           ;INVERT X
                MOVLW   0x7F
                BTFSC   MOUSEDX,7
                MOVWF   MOUSEDX             ;LIMIT X
                MOVLW   0xFF
                BTFSC   MOUSESW,4           ;SIGN X
                XORWF   MOUSEDX,F           ;INVERT X
;IF Y < -128
;   LET Y = -128
;ELSE IF Y > 127 
;   LET Y = 127
;ENDIF
                MOVLW   0xFF
                BTFSC   MOUSESW,5           ;SIGN Y
                XORWF   MOUSEDY,F           ;INVERT Y
                MOVLW   0x7F
                BTFSC   MOUSEDY,7
                MOVWF   MOUSEDY             ;LIMIT Y
                MOVLW   0xFF
                BTFSC   MOUSESW,5           ;SIGN Y
                XORWF   MOUSEDY,F           ;INVERT Y
COMDY
;IF Y == -128
;   LET Y = 127
;ELSE
;   LET Y = !Y + 1
;ENDIF
                MOVF    MOUSEDY,W
                XORLW   0x80
                BNZ     COMDY1 

                MOVLW   0x7F                ;Y = 127
                MOVWF   MOUSEDY
                GOTO    COMDY2
COMDY1
                MOVLW   0xFF                ;Y = !Y + 1
                XORWF   MOUSEDY,F
                INCF    MOUSEDY,F
COMDY2
                MOVLW   B'11000000'         ;D6=1 D7=1=STOP BIT
                BTFSC   MOUSESW,0           ;LEFT BUTTON PRESSED
                IORLW   B'00100000'
                BTFSC   MOUSESW,1           ;RIGHT BUTTON PRESSED
                IORLW   B'00010000'

                BTFSC   MOUSEDX,6           ;X6
                IORLW   B'00000001'
                BTFSC   MOUSEDX,7           ;X7
                IORLW   B'00000010'
                BTFSC   MOUSEDY,6           ;Y6
                IORLW   B'00000100'
                BTFSC   MOUSEDY,7           ;Y7
                IORLW   B'00001000'
                CALL    TRANSMIT            ;SEND

                MOVF    MOUSEDX,W           ;X0..X5
                ANDLW   B'00111111'         ;D6=0
                IORLW   B'10000000'         ;D7=1=STOP BIT
                CALL    TRANSMIT            ;SEND

                MOVF    MOUSEDY,W           ;Y0..Y5
                ANDLW   B'00111111'         ;D6=0
                IORLW   B'10000000'         ;D7=1=STOP BIT
                GOTO    TRANSMIT            ;SEND
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
