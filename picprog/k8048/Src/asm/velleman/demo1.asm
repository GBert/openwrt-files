                LIST    P=PIC16F627
#INCLUDE        "p16f627.inc"
#INCLUDE        "device.inc"

;**************************************************************************
;*           VELLEMAN   High-Q KIT  K8048/VM111 SAMPLE SOFTWARE           *
;**************************************************************************
;*           DEMO1: Simple Running light for testing LD1...LD5            *
;*           DEMO1: Program for testing K8048/VM111 with PIC16F627!       *
;**************************************************************************
;*          (C) VELLEMAN Components,2003  All rights reserved             *
;**************************************************************************
;*          Hardw. Rev: P8048'1          Softw. Rev:  1.20                *
;*          OSC.......: XT 4MHz Max.     POWER.....:  12V DC              *
;**************************************************************************

    __CONFIG    _BODEN_ON & _CP_OFF & _CPD_OFF & _PWRTE_ON & _WDT_OFF & _LVP_OFF & _MCLRE_ON & _XT_OSC

;==========================================================================
;       Variable Definition
;==========================================================================
TIMER1          EQU         H'20'           ;Used in delay routine
TIMER2          EQU         H'21'           ; "        "        "
PATERN          EQU         H'22'           ;Pattern data for effect's

                ORG         0               ;Reset vector address
                GOTO        RESET           ;goto RESET routine when boot.


;                *********************************************
;                *  Example of a delay routine               *
;                *********************************************

DELAY_ROUTINE   MOVLW   D'255'              ;54 Generate approx 10mS delay at 4Mhz CLK
                MOVWF   TIMER2
DEL_LOOP1       MOVLW   D'255'              ;60
                MOVWF   TIMER1
DEL_LOOP2       DECFSZ  TIMER1,F
                GOTO    DEL_LOOP2
                DECFSZ  TIMER2,F
                GOTO    DEL_LOOP1
                RETLW   0


;               **********************************
;               **  RESET :  main boot routine  **
;               **********************************

RESET           MOVLW   B'00000111'         ;Disable Comparator module's
                MOVWF   CMCON
;
                BSF     STATUS,RP0          ;Switch to register bank 1
                                            ;Disable pull-ups
                                            ;INT on rising edge
                                            ;TMR0 to CLKOUT
                                            ;TMR0 Incr low2high trans.
                                            ;Prescaler assign to Timer0
                                            ;Prescaler rate is 1:256
                MOVLW   B'11010111'         ;Set PIC options (See datasheet).
                MOVWF   OPTION_REG          ;Write the OPTION register.
;
                CLRF    INTCON              ;Disable interrupts
                MOVLW   B'11000000'
                MOVWF   TRISB               ;RB7 & RB6 are inputs.
                                            ;RB5...RB0 are outputs.
                MOVLW   B'11111111'         ;all RA ports are inputs
                MOVWF   TRISA
                BCF     STATUS,RP0          ;Switch Back to reg. Bank 0
                CLRF    PORTB
;

EFFECT_1        MOVLW   B'00000001'         ;Activate LD1 (RB0)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000010'         ;Activate LD2 (RB1)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000100'         ;Activate LD3 (RB2)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00001000'         ;Activate LD4 (RB3)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00010000'         ;Activate LD5 (RB4)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00100000'         ;Activate LD6 (RB5)
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                GOTO    EFFECT_1
                END
