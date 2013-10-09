;
; Velleman K8048 PIC16F627 VREF Demo
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Pinout
; ------
; RA2 AN2   VREF 1----18 RA1 AN1
; RA3 AN3   CMP1 2    17 RA0 AN0
; RA4 T0CKI CMP2 3    16 RA7 OSC1  CLKIN
; RA5 !MCLR VPP  4    15 RA6 OSC2  CLKOUT
; VSS GND        5    14 VDD VCC
; RB0 INT        6    13 RB7 T1OSI PGD
; RB1 RX    DT   7    12 RB6 T1OSO T1CKI PGC
; RB2 TX    CK   8    11 RB5
; RB3 CCP1       9----10 RB4 PGM
;
; K8048 Pin
; ----- ---
; LD1   RB0 (6)
; LD2   RB1 (7)
; LD3   RB2 (8)
; LD4   RB3 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; PGC   RB6 (12)
; PGD   RB7 (13)
; SW1   RA0 (17)
; SW2   RA1 (18)
; SW3   RA2 (1)
; SW4   RA3 (2)
; VPP   RA5 (4)
; OSC2  RA6 (15)
; OSC1  RA7 (16)
; GND   VSS (5)
; +5V   VDD (14)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F627
ERRORLEVEL      -302
#INCLUDE        "p16f627.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; VREF Demo
;
; Output voltage on RA2/AN2/VREF pin-1 from timer interrupt.
;
; SW1 raises frequency
; SW2 lowers frequency
; SW3 do not touch (connected to vref output)
; SW4 changes waveform
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _BODEN_OFF & _CP_OFF & _CPD_OFF & _PWRTE_ON & _WDT_OFF & _LVP_OFF & _MCLRE_ON & _INTRC_OSC_CLKOUT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS 0x1234
;
; INTOSC = 4MHz
    CONSTANT CLOCK = 4000000
;
    CONSTANT VOLTAGECTRL = 0xE0             ;VREN + VROE + VRR
    CONSTANT TIMERRELOAD = 0x80
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; EEPROM
;
                ORG     0x2100
SINE            DE      0,1,2,5,    8,10,13,14,  15,14,13,10, 8,5,2,1
TRIANGLE        DE      1,3,5,7,    9,11,13,15,  13,11,9,7,   5,3,1,0
SAWTOOTH        DE      1,3,5,7,    9,11,13,15,  1,3,5,7,     9,11,13,15
SQUARE          DE      0,0,0,0,    0,0,0,0,     15,15,15,15, 15,15,15,15
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x20                        ;RAM 0x20..0x6F
ENDC
CBLOCK          0x70                        ;RAM 0x70..0x7F
    W_SAVE
    S_SAVE 
    VOLTAGE
    BASEADDR ; 0=SINE, 16=TRI, 32=SAW, 48=SQUARE
    RELOAD
    DELAY1
    DELAY2
    DELAY3
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0004
                GOTO    INTSR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set output voltage level
;
SETVRCON        BANKSEL BANK1

                INCF    VOLTAGE,F
                MOVF    VOLTAGE,W
                ANDLW   0x0F
                MOVWF   EEADR
                MOVF    BASEADDR,W
                ADDWF   EEADR,F

                BSF     EECON1,RD           ;READ EEPROM DATA
                MOVF    EEDATA,W

                IORLW   VOLTAGECTRL
                MOVWF   VRCON

                BANKSEL BANK0
                RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                CLRF    INTCON              ;DISABLE INTERRUPTS

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                MOVLW   B'11111111'         ;INIT PORT A
                MOVWF   PORTA
                CLRF    PORTB               ;INIT PORT B

                BANKSEL BANK1

                BSF     PCON,OSCF           ;INTRC OPERATION 4 MHz

                MOVLW   B'10000000'         ;SET OPTIONS (DISABLE PULLUPS)
                MOVWF   OPTION_REG

                MOVLW   B'11111111'         ;PORT A ALL I/P
                MOVWF   TRISA

                MOVLW   B'11111110'         ;PORT B LD1 O/P
                MOVWF   TRISB
; VREF
                CLRF    VOLTAGE
                MOVLW   VOLTAGECTRL
                MOVWF   VRCON

                BANKSEL BANK0
; timer0
                MOVLW   TIMERRELOAD
                MOVWF   RELOAD
                MOVWF   TMR0

                MOVLW   0xA0
                MOVWF   INTCON              ;INT TMR0 ON
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                MOVLW   LOW SINE            ;SINE WAVE
                MOVWF   BASEADDR
MAINLOOP
                BTFSC   PORTA,0             ;SW1?
                GOTO    DOSW1

                BTFSC   PORTA,1             ;SW2?
                GOTO    DOSW2

                BTFSC   PORTA,3             ;SW4?
                GOTO    DOSW4

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW1           MOVF    RELOAD,W
                XORLW   0xF8
                BZ      MAINLOOP
                INCF    RELOAD,F

SW1DELAY        MOVLW   0x20
                MOVWF   DELAY1
                CLRF    DELAY2
SW1DELAYLOOP    DJNZ    DELAY2,SW1DELAYLOOP
                DJNZ    DELAY1,SW1DELAYLOOP

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW2           MOVF    RELOAD,W
                XORLW   0x00
                BZ      MAINLOOP
                DECF    RELOAD,F

                GOTO    SW1DELAY
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
DOSW4           MOVLW   0x10                ;0x00 -> 0x10 -> 0x20 -> 0x30
                ADDWF   BASEADDR,F
                MOVLW   0x30
                ANDWF   BASEADDR,F
                
                MOVLW   0x02
                MOVWF   DELAY1
                CLRF    DELAY2
                CLRF    DELAY3
SW4DELAYLOOP    DJNZ    DELAY3,SW4DELAYLOOP
                DJNZ    DELAY2,SW4DELAYLOOP
                DJNZ    DELAY1,SW4DELAYLOOP

                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine
;
INTSR           MOVWF   W_SAVE
                MOVF    STATUS,W
                MOVWF   S_SAVE

                MOVLW   1
                XORWF   PORTB,F

                CALL    SETVRCON

                MOVF    RELOAD,W
                MOVWF   TMR0

                MOVF    S_SAVE,W
                MOVWF   STATUS
                MOVF    W_SAVE,W

                BCF     INTCON,2
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;