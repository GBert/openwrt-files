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
; AMPIRE AC162D Driver for CROWNHILL PROTON MKII
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; MK2   AC162D
; ---   ------
; RD0   D0
; RD1   D1
; RD2   D2
; RD3   D3
; RD4   D4
; RD5   D5
; RD6   D6
; RD7   D7
; RE0   RS
; RE1   EN
; GND   R/W
; R6    VO
; 5V    VDD
; GND   VSS
;
; Function set (8-bit interface, 2 lines, 5*7 Pixels)       0x38
; Function set (8-bit interface, 1 line, 5*7 Pixels)        0x30
; Function set (4-bit interface, 2 lines, 5*7 Pixels)       0x28
; Function set (4-bit interface, 1 line, 5*7 Pixels)        0x20
; Entry mode set command (left to right)                    0x06 
; Scroll display one character right (all lines)            0x1E
; Scroll display one character left (all lines)             0x18
; Home (move cursor to top/left character position)         0x02
; Move cursor one character left                            0x10
; Move cursor one character right                           0x14
; Turn on visible underline cursor                          0x0E
; Turn on visible blinking-block cursor                     0x0F
; Make cursor invisible                                     0x0C
; Blank the display (without clearing)                      0x08
; Restore the display (with cursor hidden)                  0x0C
; Clear Screen                                              0x01
; Set cursor position (DDRAM address)                       0x80 + addr
; Set pointer in character-generator RAM (CG RAM address)   0x40 + addr
; Read DDRAM/CGRAM & Check Busy Flag                        N/A
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
    CONSTANT CLEARSCREEN = B'00000001'
    CONSTANT RETURNHOME  = B'00000010'
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK
    AC162DBUF
    AC162DIX
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Init. Proton MK2 & Init. LCD
;
AC162DINITMK2
                MOVLW   0
                MOVWF   LATD
                MOVWF   LATE
IFDEF SHADOWLATCH                       ;BASELINE OR MOST MIDRANGE
                MOVWF   PORTD
                MOVWF   PORTE
ENDIF
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL TRISD
ENDIF
                MOVLW   B'00000000'
                MOVWF   TRISD
                MOVLW   B'11111100'
                MOVWF   TRISE
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise LCD
;
AC162DINIT      CLRF    AC162DIX
; POWER ON
                MOVLW   31
                CALL    DELAYM          ;>30ms
; DISPLAY CLEAR
                MOVLW   CLEARSCREEN
                CALL    AC162DINSTRUCT
                MOVLW   16
                CALL    DELAYH          ;>1.53ms
; FUNCTION SET
                MOVLW   B'00111000'     ;N=2-LINE MODE F=FONT 5X8
                CALL    AC162DINSTRUCT
; DISPLAY ON/OFF CONTROL
                MOVLW   B'00001100'     ;D=DISPLAY ON C=CURSOR OFF B=BLINK OFF
                CALL    AC162DINSTRUCT
; ENTRY MODE SET
                MOVLW   B'00000110'     ;I/D=INCREMENT MODE SH=ENTIRE SHIFT OFF
;               GOTO    AC162DINSTRUCT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Write Instruction W to AC162D (RS=0)
;
AC162DINSTRUCT  BCF     LATE,0          ;RS=0
;               GOTO    AC162DWRITE
;                
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Write Instruction or Data
;
AC162DWRITE     MOVWF   LATD
IFDEF SHADOWLATCH                       ;BASELINE OR MOST MIDRANGE
                MOVWF   PORTD
                MOVF    LATE,W
                MOVWF   PORTE           ;RS=?
ENDIF
                BSF     LATE,1          ;E=1
IFDEF SHADOWLATCH                       ;BASELINE OR MOST MIDRANGE
                MOVF    LATE,W
                MOVWF   PORTE
ENDIF
                NOP
                NOP
                BCF     LATE,1          ;E=0
IFDEF SHADOWLATCH                       ;BASELINE OR MOST MIDRANGE
                MOVF    LATE,W
                MOVWF   PORTE
ENDIF
                MOVLW   DEL50U
                GOTO    DELAYU          ;50us
;                
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Write Data W to AC162D (RS=1)
;
AC162DDATA      BSF     LATE,0          ;RS=1
                CALL    AC162DWRITE

                INCF    AC162DIX,F

                MOVF    AC162DIX,W
                XORLW   0x10
                BZ      AC162DLINE2

                MOVF    AC162DIX,W
                XORLW   0x20
                BZ      AC162DLINE1

                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Output data or instruct LCD
;
AC162DOUT       MOVWF   AC162DBUF
                XORLW   0x0D            ;IS CR?
                BZ      AC162DLINE1

                MOVF    AC162DBUF,W
                GOTO    AC162DDATA
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Move cursor to line
; 
AC162DLINE1     CLRF    AC162DIX
                MOVLW   0x80
                GOTO    AC162DINSTRUCT

AC162DLINE2     MOVLW   0xC0
                GOTO    AC162DINSTRUCT
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
