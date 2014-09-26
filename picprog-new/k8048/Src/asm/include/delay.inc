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
; Delays for 12/14/16-bit word architecture
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK
    COUNTER0    ;COUNTER
    COUNTER1    ;   "
    COUNTER2    ;   "
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Simple delay (W * 256 * 256)
;
; params  : W
; result  : NONE
; affects : COUNTER0, COUNTER1, COUNTER2
;
DELAY           MOVWF   COUNTER0
                CLRF    COUNTER1
                CLRF    COUNTER2
DELAYLOOP       DJNZ    COUNTER2,DELAYLOOP
                DJNZ    COUNTER1,DELAYLOOP
                DJNZ    COUNTER0,DELAYLOOP
                RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Reasonably accurate delay
;
IF ARCH != BASELINE
    CONSTANT DEL10U  = (CLOCK / 1600000) - 1
    CONSTANT DEL50U  = (CLOCK / 320000)  - 1
;
; ?us DELAY (time constant accounts for MOVLW & CALL in caller)
;
DELAYU          ADDLW   -1                      ;1 CYCLE
                BTFSS   STATUS,Z                ;1 CYCLE OR 2 CYCLES ON SKIP
                GOTO    DELAYU                  ;2 CYCLES
                RETLW   0                       ;1 CYCLE
;
; No less than N x 10us DELAY
;
DELAYT          MOVWF   COUNTER0
DELAYTL         MOVLW   DEL10U
                CALL    DELAYU
                DJNZ    COUNTER0,DELAYTL
                RETLW   0
;
; No less than N x 100us DELAY
;
DELAYH          MOVWF   COUNTER0
DELAYHL         MOVLW   DEL50U
                CALL    DELAYU
                MOVLW   DEL50U
                CALL    DELAYU
                DJNZ    COUNTER0,DELAYHL
                RETLW   0
;
; No less than N x 1ms DELAY
;
DELAYM          MOVWF   COUNTER1
DELAYML         MOVLW   10
                CALL    DELAYH
                DJNZ    COUNTER1,DELAYML
                RETLW   0
ENDIF
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
