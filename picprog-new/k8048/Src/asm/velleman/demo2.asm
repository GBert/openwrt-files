                LIST    P=PIC16F627
#INCLUDE        "p16f627.inc"
#INCLUDE        "device.inc"

;**************************************************************************
;*             VELLEMAN   High-Q KIT  K8048/VM111 SAMPLE SOFTWARE         *
;**************************************************************************
;*          Microchip PIC(tm) Programmer & experiment board               *
;*          DEMO2 program for testing K8048 with PIC16F627(A)!            *
;*   Generate 4 LED light effects, you can select these with  SW1..4      *
;**************************************************************************
;*          (C) VELLEMAN Components,2003  All rights reserved             *
;**************************************************************************
;*          Hardw. Rev: P8048'1          Softw. Rev:  1.21                *
;*          OSC.......: XT 4MHz Max.     POWER.....:  12V DC              *
;**************************************************************************

    __CONFIG    _BODEN_ON & _CP_OFF & _CPD_OFF & _PWRTE_ON & _WDT_OFF & _LVP_OFF & _MCLRE_ON & _XT_OSC

;==========================================================================
;       Variable Definition
;==========================================================================
;INPUTS
SW1             EQU     H'00'               ;SW1 is triggering RA0
SW2             EQU     H'01'               ;SW2 is triggering RA1
SW3             EQU     H'02'               ;SW3 is triggering RA2
SW4             EQU     H'03'               ;SW4 is triggering RA3
TIMER1          EQU     H'20'               ;Used in delay routine
TIMER2          EQU     H'21'               ; "        "        "
PATERN          EQU     H'22'               ;Pattern data for effect's


                ORG     0                   ;Reset vector address
                GOTO    RESET               ;goto RESET routine when boot.


;               *********************************************
;               *  Example of a delay routine               *
;               *********************************************

DELAY_ROUTINE   MOVLW   D'100'              ;54 Generate approx 10mS delay at 4Mhz CLK
                MOVWF   TIMER2
DEL_LOOP1       MOVLW   D'100'              ;60
                MOVWF   TIMER1
DEL_LOOP2       BTFSC   PORTA,SW1
                GOTO    MENU
                BTFSC   PORTA,SW2
                GOTO    MENU
                BTFSC   PORTA,SW3
                GOTO    MENU
                BTFSC   PORTA,SW4
                GOTO    MENU
                DECFSZ  TIMER1,F
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
                GOTO    EFFECT_4
;

MENU            CLRF    PORTB
;
                BTFSC   PORTA,SW1
                GOTO    EFFECT_1
                BTFSC   PORTA,SW2
                GOTO    EFFECT_2
                BTFSC   PORTA,SW3
                GOTO    EFFECT_3
                BTFSC   PORTA,SW4
                GOTO    EFFECT_4
                GOTO    MENU


EFFECT_1        BTFSC   PORTA,SW1
                GOTO    EFFECT_1
E1              MOVLW   B'00100000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00110000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111100'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111110'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111111'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111110'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111100'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00111000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00110000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                GOTO    E1

EFFECT_2        BTFSC   PORTA,SW2
                GOTO    EFFECT_2
E2              MOVLW   B'00000001'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000010'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000100'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00001000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00010000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00100000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00010000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00001000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000100'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                MOVLW   B'00000010'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                GOTO    E2

EFFECT_3        BTFSC   PORTA,SW3
                GOTO    EFFECT_3
E3              MOVLW   B'00000111'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                CALL    DELAY_ROUTINE
                MOVLW   B'00111000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                CALL    DELAY_ROUTINE
                GOTO    E3

EFFECT_4        BTFSC   PORTA,SW4
                GOTO    EFFECT_4
E4              MOVLW   B'00001000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                CALL    DELAY_ROUTINE
                MOVLW   B'00000000'         ;
                MOVWF   PORTB
                CALL    DELAY_ROUTINE
                CALL    DELAY_ROUTINE
                GOTO    E4

                END
