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
; 8-bit architectures
;
    CONSTANT BASELINE = 12
    CONSTANT MIDRANGE = 14
    CONSTANT ENHANCED = 15
    CONSTANT PIC18    = 18
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Detect architecture
;
ARCH            SET     BASELINE        ;12-BIT WORD

IFDEF OPTION_REG
ARCH            SET     MIDRANGE        ;14-BIT WORD
ENDIF

IFDEF WREG

IF WREG == 0x0009
ARCH            SET     ENHANCED        ;ENHANCED 14-BIT WORD 
ENDIF

IF WREG == 0x0FE8
ARCH            SET     PIC18           ;16-BIT WORD
ENDIF

ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data protection renamed
;
IFNDEF _CPD_ON

IFDEF _DATA_CP_ON
_CPD_ON         EQU     _DATA_CP_ON
ENDIF

ENDIF

IFNDEF _CPD_OFF

IFDEF _DATA_CP_OFF
_CPD_OFF        EQU     _DATA_CP_OFF
ENDIF

ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data sheet incorrect
; 
IFDEF __10F320
  __BADRAM 0x40-0x5F
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data low-byte renamed
;
IFNDEF EEDATA

IFDEF EEDATL
EEDATA          EQU     EEDATL
ENDIF

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
