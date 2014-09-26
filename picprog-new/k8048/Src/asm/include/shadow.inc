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
; Port I/O shadow for 12/14/16-bit word architecture.
;
; NB. Not required for 14 or 16-bit word device with latch.
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

IFDEF __18F2431
; NO TRISE
; NO PORTE
ENDIF

IFDEF __18F25K22
; NO PORTE
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
