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
; Velleman K8048 PIC16F648A Sky RC6 Remote Control Demo
;
; See:
;
; RC6 overview        http://www.sbprojects.com/knowledge/ir/rc6.php
; Sky RC6 (RC-6-20)   https://web.archive.org/web/20061102030113/http://john.fine.home.comcast.net/ir/DecodeIr.html
; Sky magic eye       http://groups.google.co.uk/group/uk.tech.tv.sky/browse_thread/thread/616289ac2ef88dc4/1ad625df42dc9de0
; Sky key codes       http://www.dusky-control.com/sky-control-codes.shtml
;
; Demo: SKY BLUE BUTTON (00-0-70)
;
;                4  2  1         8  4  2  1   8  4  2  1   8  4  2  1   8  4  2  1   8  4  2  1
; -- -- -- __ -_ -_ -_ _- __ --  _- _- _- _-  _- _- _- _-  _- _- _- _-  _- -_ -_ -_  _- _- _- _-
; YY YY YY XX YX YX YX XY XX YY  XY XY XY XY  XY XY XY XY  YX YX XY XY  XY YX YX YX  XY XY XY XY
;                                0  0  0  0   0  0  0  0   0  0  0  0   0  1  1  1   0  0  0  0  00-0-70
; ===LEADER== SB ==MODE== =TR==  ==CONTROL ADDRESS=======  ==INFORMATION COMMAND================
;
; X=OFF Y=ON XY=0 YX=1 SB=START-BIT TR=TRAILER-BIT
;
; BITSTREAM (MANCHESTER ENCODED BYTES)
;  
; PRE     [0] F C A 9 3              [0]=SPACER [FC]=LEADER [A9]=SB+MODE [3]=TRAILER = 0f ca 93
; ADDRESS     5 5 5 5
; DATA        5 5 6 A 5 5 (BLUE)
;
; TEST EMITTER SETUP ON K8048 BOARD
;
; 1 x VISHAY TSAL4400 connected to 14P DIL socket PIN7 (RB3) tied to ground via 390R
;
; RB3 MAY ALSO HAVE A PULL DOWN RESISTOR (EG. 1M) IF DRIVING LOGIC
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
                LIST    P=PIC16F648A
ERRORLEVEL      -302
#INCLUDE        "p16f648a.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
  __CONFIG _CP_OFF & _CPD_OFF & _LVP_OFF & _BODEN_OFF & _MCLRE_ON & _PWRTE_ON & _WDT_OFF & _XT_OSC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; XTAL = 4MHz
    CONSTANT CLOCK = 4000000
;   
; DETERMINES DELAY BETWEEN TRANSMISSION
    CONSTANT DELAYTIMH = 0x20

; PWM MARK TIME (LOW BITS) + PWM ENABLE (SPACE TIME/2)
    CONSTANT PWMMLEN = 0x3C

; PWM MARK TIME (HIGH BITS)
    CONSTANT PWMMH = 0x0D

; PWM SPACE TIME (HIGH BITS) (1MHz / 36000)
    CONSTANT PWMSH = 0x1B 

; RB3 CCP1 OUTPUT ENABLE : RB3=PWM         (TRISB) (RB4 IS MODULATOR TEST POINT)
    CONSTANT PBCCP1EN = B'11100111'

; RB3 CCP1 OUTPUT DISABLE: RB3=TRI-STATE   (TRISB) (PULLUPS DISABLED)
    CONSTANT PBCCP1DIS = B'11111111'

; TMR0 RELOAD 256 - (444us - RELOAD DELAY) / 2
    CONSTANT TMR0RELOAD = 0x29
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0xA0                        ;BANK1
    RCBUFPRE0   ;PRE  [0FCA93]  sky RC6 constant header
    RCBUFPRE1
    RCBUFPRE2
    RCBUFADDR0  ;ADDR [5555]    sky RC6 constant address
    RCBUFADDR1
    RCBUFDATA0  ;DATA [55]      sky RC6 device code 55=0=SKY A5=12=SKY+
    RCBUFDATA1  ;DATA [XX]      sky key code high nibble
    RCBUFDATA2  ;DATA [XX]      sky key code low  nibble
    RCBUFBYTIND ;BUFFER INDIRECT INDEX
    RCBUFBITCNT ;BIT COUNTER FOR CURRENTLY INDEXED BYTE
ENDC
CBLOCK          0x70                        ;GLOBAL
    W_SAVE
    S_SAVE
    F_SAVE
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
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BANKSEL BANK0

                MOVLW   0x10                ;INIT TEST POINT RB4
                MOVWF   PORTB

                MOVLW   B'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                CLRF    INTCON              ;DISABLE INTERRUPTS
                CLRF    PIR1                ;CLEAR INTERRRUPT FLAGS

                BANKSEL BANK1

                CLRF    PIE1                ;DISABLE PERIPHERAL INTERRUPTS

                MOVLW   B'10000000'         ;SET OPTIONS (DISABLE INPUT PULLUPS + TMR0 DEFAULTS)
                MOVWF   OPTION_REG

                MOVLW   B'11111111'
                MOVWF   TRISA               ;PORT A INP

                BANKSEL BANK1

; setup output buffer (empty)
                MOVLW   0x08                ;BUFFER EMPTY
                MOVWF   RCBUFBYTIND 

                MOVLW   0x08
                MOVWF   RCBUFBITCNT

; setup PWM @ approx 36 KHz (CCPR1L=HIGH TIME, PR2=LOW TIME, 1MHz/36000 = 27u = 0x1b)
                MOVLW   PBCCP1DIS
                MOVWF   TRISB

                BANKSEL BANK0

                MOVLW   PWMMLEN             ;PWM MARK (LOW BITS) + ENABLE
                MOVWF   CCP1CON
                MOVLW   PWMMH               ;PWM MARK (HIGH BITS)
                MOVWF   CCPR1L

                BANKSEL BANK1

                MOVLW   PWMSH               ;PWM SPACE (HIGH BITS)
                MOVWF   PR2

                BANKSEL BANK0

                MOVLW   0x04                ;TMR2 ON
                MOVWF   T2CON

; setup timer0 interrupt (444us)
                MOVLW   TMR0RELOAD          ;INIT TMR0
                MOVWF   TMR0

                MOVLW   0xA0                ;ENABLE INTERRUPTS TMR0 
                MOVWF   INTCON

                BANKSEL BANK1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop (send BLUE button repeatedly)
;
MAINLOOP        MOVLW   0x0f                ;PRE [0fca93]
                MOVWF   RCBUFPRE0
                MOVLW   0xca
                MOVWF   RCBUFPRE1
                MOVLW   0x93
                MOVWF   RCBUFPRE2

                MOVLW   0x55                ;ADDR [5555] = [00]
                MOVWF   RCBUFADDR0
                MOVLW   0x55
                MOVWF   RCBUFADDR1

                MOVLW   0x55                ;DATA [556a55] = [070]
                MOVWF   RCBUFDATA0
                MOVLW   0x6a
                MOVWF   RCBUFDATA1
                MOVLW   0x55
                MOVWF   RCBUFDATA2

                MOVLW   RCBUFPRE0           ;SEND RCBUFPRE0[0] .. RCBUFPRE0[7]
                MOVWF   RCBUFBYTIND

MAINSEND        BTFSS   RCBUFBYTIND,3       ;WAIT FOR SEND COMPLETION
                GOTO    MAINSEND

MAIN3           MOVLW   DELAYTIMH
                CALL    DELAY
                GOTO    MAINLOOP
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Interrupt service routine entered every 444us (approx)
;
INTSR           MOVWF   W_SAVE              ;SAVE WORKING REGISTER
                MOVF    STATUS,W
                MOVWF   S_SAVE              ;SAVE STATUS
                MOVF    FSR,W
                MOVWF   F_SAVE              ;SAVE FSR

                BANKSEL BANK0
                MOVLW   TMR0RELOAD          ;RELOAD TMR0
                MOVWF   TMR0                ;RELOAD DELAY = 8(HERE) + 2(TMR STALL) + 4(TCY)

                BANKSEL BANK1
                BTFSS   RCBUFBYTIND,3       ;BUFFER EMPTY?
                GOTO    SENDBIT             ;NO,  SEND ANOTHER BIT...

                MOVLW   PBCCP1DIS           ;YES, DISABLE (TRI-STATE) OUTPUT
                MOVWF   TRISB
                GOTO    INTSRRESTORE

SENDBIT         MOVF    RCBUFBYTIND,W       ;GET BYTE INDEX
                MOVWF   FSR
                MOVLW   PBCCP1DIS           ;DISABLE (TRI-STATE) OUTPUT
                RLF     INDF,F              ;GET BIT
                BTFSC   STATUS,C
                MOVLW   PBCCP1EN            ;ENABLE (PWM) OUTPUT
                MOVWF   TRISB               ;PASS THROUGH PWM 36KHz TONE ELSE TRI-STATE

                DECFSZ  RCBUFBITCNT,F       ;NEXT BIT
                GOTO    INTSRRESTORE

                MOVLW   0x08
                MOVWF   RCBUFBITCNT
                INCF    RCBUFBYTIND,F       ;NEXT BYTE

INTSRRESTORE    MOVF    F_SAVE,W            ;RESTORE FSR
                MOVWF   FSR
                MOVF    S_SAVE,W            ;RESTORE STATUS
                MOVWF   STATUS
                MOVF    W_SAVE,W            ;RESTORE WORKING REGISTER

                BCF     INTCON,2            ;RESET TMR0 INT FLAG
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
