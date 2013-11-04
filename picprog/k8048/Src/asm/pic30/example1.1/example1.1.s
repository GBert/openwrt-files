            .TITLE " Sample dsPIC Assembler Source Code"
            .SBTTL " For illustration only."
;
;******************************************************************************
;
            ; dsPIC registers
            .EQU    CORCONL, CORCON
            .EQU    PSV, 2
;
;******************************************************************************
;
            .SECTION .const, psv

hello:      .ASCII "Hello world!\n\0"
;
;******************************************************************************
;
            .TEXT

            .GLOBAL __reset
__reset:    ; set PSVPAG to page that contains 'hello'
            MOV     #psvpage(hello), W0
            MOV     W0, PSVPAG

            ; enable Program Space Visibility
            BSET.B  CORCONL, #PSV

            ; make a pointer to 'hello'
            MOV     #psvoffset(hello), W0
;
;******************************************************************************
            .END
;******************************************************************************
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
