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
; Port I/O shadow.
;
; Not required for 14 or 16-bit word device with latch.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
IF ARCH == BASELINE

IFDEF GPIO
    CBLOCK
        TRISIO  ;GPIO TRISIO EMULATION/TRISIO SHADOW
    ENDC
ENDIF

IFDEF PORTA
    CBLOCK
        TRISA   ;PORTA TRIS EMULATION/TRISA SHADOW
    ENDC
ENDIF

IFDEF PORTB
    CBLOCK
        TRISB   ;PORTB TRIS EMULATION/TRISB SHADOW
    ENDC
ENDIF

IFDEF PORTC
    CBLOCK
        TRISC   ;PORTC TRIS EMULATION/TRISC SHADOW
    ENDC
ENDIF

IFDEF PORTD
    CBLOCK
        TRISD   ;PORTD TRIS EMULATION/TRISD SHADOW
    ENDC
ENDIF

IFDEF PORTE
    CBLOCK
        TRISE   ;PORTE TRIS EMULATION/TRISE SHADOW
    ENDC
ENDIF

ENDIF

IF ARCH == BASELINE || ARCH == MIDRANGE

SHADOWLATCH     SET     1 ; NB. SOME MIDRANGE DEVICES HAVE LATCHES

IFDEF GPIO
    CBLOCK
        LATIO   ;GPIO LATCH EMULATION/GPIO SHADOW
    ENDC
ENDIF

IFDEF PORTA
    CBLOCK
        LATA    ;PORTA LATCH EMULATION/PORTA SHADOW
    ENDC
ENDIF

IFDEF PORTB
    CBLOCK
        LATB    ;PORTB LATCH EMULATION/PORTB SHADOW
    ENDC
ENDIF

IFDEF PORTC
    CBLOCK
        LATC    ;PORTC LATCH EMULATION/PORTC SHADOW
    ENDC
ENDIF

IFDEF PORTD
    CBLOCK
        LATD    ;PORTD LATCH EMULATION/PORTD SHADOW
    ENDC
ENDIF

IFDEF PORTE
    CBLOCK
        LATE    ;PORTE LATCH EMULATION/PORTE SHADOW
    ENDC
ENDIF

ENDIF

IF ARCH == PIC18

IFDEF __18F25K22
    CBLOCK
        LATE    ;PORTE LATCH EMULATION/PORTE SHADOW
    ENDC
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
