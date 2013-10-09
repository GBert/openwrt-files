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
; PULL-UP (EXTERNAL PULL)
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
