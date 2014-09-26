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
; Macros for 12/14/16-bit word architecture
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Macro definitions
;
; Decrement REG, goto LOC if result not ZERO
DJNZ            MACRO   REG,LOC
                DECFSZ  REG,F
                GOTO    LOC
                ENDM
;
; BIT TEST FILE GOTO IF SET
BTFGS           MACRO   REG,BIT,LOC
                BTFSC   REG,BIT
                GOTO    LOC
                ENDM
;
; BIT TEST FILE GOTO IF CLEAR
BTFGC           MACRO   REG,BIT,LOC
                BTFSS   REG,BIT
                GOTO    LOC
                ENDM
;
; BIT TEST FILE RETLW IF SET
BTFRS           MACRO   REG,BIT
                BTFSC   REG,BIT
                RETLW   0
                ENDM
;
; BIT TEST FILE RETLW IF CLEAR
BTFRC           MACRO   REG,BIT
                BTFSS   REG,BIT
                RETLW   0
                ENDM
;
; PULL-UP (EXTERNAL RESISTOR)
TRISPU          MACRO   REG,BIT
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL REG
ENDIF
                BSF     REG,BIT             ;TRIS=I/P PORT=X
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                ENDM
;
; PULL-DOWN (INTERNAL SINK)
TRISPD          MACRO   REG,BIT
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL REG
ENDIF
                BCF     REG,BIT             ;TRIS=O/P PORT=0
IF ARCH == MIDRANGE || ARCH == ENHANCED
                BANKSEL BANK0
ENDIF
                ENDM
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
