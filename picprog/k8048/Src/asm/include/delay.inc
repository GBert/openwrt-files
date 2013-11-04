                LIST
;
; Velleman K8048 12/14/16-bit word architecture library include
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
                NOLIST
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
