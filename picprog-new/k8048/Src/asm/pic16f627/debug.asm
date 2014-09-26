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
; Velleman K8048 PIC16F627 Debug Demo
;
; Velleman K8048 PIC to HOST communication
;
; K8048 (PGD) -> host (CTS)
; K8048 (PGC) <- host (RTS)
;
; PIC and host perform a simple handshake operation, then the PIC outputs a
; byte of data clocked by the host. If the host is not available then the PIC
; will perform this operation unclocked.
;
; $ ktest 4 100
; io_test4: read byte: 02 .
; io_test4: read byte: 48 H
; io_test4: read byte: 65 e
; io_test4: read byte: 6C l
; io_test4: read byte: 6C l
; io_test4: read byte: 6F o
; io_test4: read byte: 2C ,
; io_test4: read byte: 20  
; io_test4: read byte: 77 w
; io_test4: read byte: 6F o
; io_test4: read byte: 72 r
; io_test4: read byte: 6C l
; io_test4: read byte: 64 d
; io_test4: read byte: 0A .
; io_test4: read byte: 03 .
; io_test4: read line: Hello, world
; ^C
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
; K8048 Pin
; ----- ---
; LD1   RB0 (6)
; LD2   RB1 (7)
; LD3   RB2 (8)
; LD4   RB3 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; PGC   RB6 (12)
; PGD   RB7 (13)
; SW1   RA0 (17)
; SW2   RA1 (18)
; SW3   RA2 (1)
; SW4   RA3 (2)
; VPP   RA5 (4)
; OSC2  RA6 (15)
; OSC1  RA7 (16)
; GND   VSS (5)
; +5V   VDD (14)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F627
ERRORLEVEL      -302
#INCLUDE        "p16f627.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _BODEN_OFF & _CP_OFF & _CPD_OFF & _PWRTE_ON & _WDT_OFF & _LVP_OFF & _MCLRE_ON & _INTRC_OSC_CLKOUT
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
; EEPROM
;
                ORG     0x2100
                DE      "Hello, world\n",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x7F
    WAITTIMERH
    WAITTIMERL
    OUTPUTBYTE
    OUTPUTCOUNTER
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
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for PGC Low
;
WAITLOW         CLRF    WAITTIMERH
                CLRF    WAITTIMERL
WAITLOW2        BTFRC   PORTB,6
                DJNZ    WAITTIMERL,WAITLOW2
                DJNZ    WAITTIMERH,WAITLOW2
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for PGC High
;
WAITHIGH        CLRF    WAITTIMERH
                CLRF    WAITTIMERL
WAITHGH2        BTFRS   PORTB,6
                DJNZ    WAITTIMERL,WAITHGH2
                DJNZ    WAITTIMERH,WAITHGH2
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Output byte
;
OUT             MOVWF   OUTPUTBYTE
                BSF     PORTB,0             ;SET LD1

                CALL    WAITLOW             ;WAIT FOR LOW
                BSF     PORTB,7             ;SET PGD
                CALL    WAITHIGH            ;WAIT FOR HIGH
                BCF     PORTB,7             ;CLR PGD

                MOVLW   0x08
                MOVWF   OUTPUTCOUNTER
OUT1            CALL    WAITLOW             ;WAIT FOR LOW
                CLRW                        ;CLR BIT FOR O/P
                RRF     OUTPUTBYTE,F        ;SHIFT OUT DATA BIT
                SKPNC                       ;SKIP IF NO CARRY
                MOVLW   B'10000000'         ;SET BIT FOR O/P
                MOVWF   PORTB               ;SET/CLR O/P PGD
                CALL    WAITHIGH            ;WAIT FOR HIGH
                DJNZ    OUTPUTCOUNTER,OUT1  ;LOOP FOR 8 BITS

                BCF     PORTB,0             ;CLR LD1
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Output EEPROM Data String
;
OUTSTRING       BANKSEL BANK1
                MOVWF   EEADR               ;FIRST CHARACTER ADDRESS
                BANKSEL BANK0

                MOVLW   STX                 ;START OF TEXT RECORD
                CALL    OUT

OUTSTRING1      BANKSEL BANK1
                BSF     EECON1,RD           ;READ EEPROM DATA
                MOVF    EEDATA,W
                INCF    EEADR,F             ;NEXT CHARACTER ADDRESS
                BANKSEL BANK0

                IORLW   0                   ;TEST FOR END OF STRING
                BZ      OUTSTRING2
                CALL    OUT                 ;OUTPUT CURRENT CHARACTER
                GOTO    OUTSTRING1

OUTSTRING2      MOVLW   ETX                 ;END OF TEXT RECORD
                CALL    OUT
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    PORTB               ;PB7 LOW

                BANKSEL BANK1

                BSF     PCON,OSCF           ;INTRC OPERATION 4MHz

                MOVLW   B'10000000'         ;SET OPTIONS (DISABLE PULLUPS)
                MOVWF   OPTION_REG

                MOVLW   B'01111110'         ;PORT B PB7 OUT, PB6 IN, PB0 (LD1) OUT
                MOVWF   TRISB

                MOVLW   B'11111111'         ;PORT A ALL I/P
                MOVWF   TRISA

                BANKSEL BANK0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAINLOOP        MOVLW   0x00                ;hello, world
                CALL    OUTSTRING
                MOVLW   1
                CALL    DELAY
                GOTO    MAINLOOP            ;LOOP FOREVER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
