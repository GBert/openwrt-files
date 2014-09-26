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
; R-PI 3V3 PIC18LF27J53
;
; This demo allows control from `kio' to perform ICSPIO common operations.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Incompatible with the VELLEMAN K8048.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 65536 words Flash (16-bit)
; 3800 bytes RAM
; 0 bytes EEPROM
;
; Pinout
; ------
; !MCLR        1-----28 RB7 PGD
; RA0          2     27 RB6 PGC
; RA1          3     26 RB5
; RA2          4     25 RB4
; RA3          5     24 RB3
; VDDCORE/VCAP 6     23 RB2
; RA5          7     22 RB1
; VSS GND      8     21 RB0
; RA7          9     20 VDD 3V3
; RA6          10    19 VSS GND
; RC0          11    18 RC7
; RC1          12    17 RC6
; RC2          13    16 RC5
; VUSB         14----15 RC4
;
; VDDCORE 2.00V .. 2.70V (VDDCORE <= VDD)
; VDD     2.15V .. 3.60V (VDD >= VDDCORE)
;
; LP2951ACN Pinout 
; ----------------
; OUTPUT   1-----8 INPUT
; SENSE    2     7 FEEDBACK
; SHUTDOWN 3     6 VTAP
; GROUND   4-----5 !ERROR
;
; LP2951ACN VDDCORE
; -----------------
; OUTPUT = 1.23 * (1 + R1 / R2)
;
; FOR R1=100K & R2=100K OUTPUT=2.46V
;
; VDDCORE---+----+----+-----OUTPUT-+--------+-INPUT---VDD
;  2.46V    |    |    |            |        |         3V3
;          100n 10u  100K R1       | LP2951 |
;           |    |    |            |  ACN   |
;           |    |    +---FEEDBACK-+--------+
;           |    |    |                |
;           |    |   100K R2           |
;           |    |    |                |
;           +----+----+----------------+--------------GND
; Program
; -------
; k16 lvp program pic18lf27j53.hex
;
; Program timings
; ---------------
; $ k16 lvp blank
; Blank device: are you sure [y/N]? y
; Time: 0:00.98s
;
; $ k16 lvp dump
; :020000040000FA
; :020000040001F9
; :10FFF000FFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFF19
; :00000001FF
; Time: 0:02.22s
;
; $ k16 lvp program pic18_128kb.hex 
; Total: 131072
; Time: 0:04.60s
;
; $ k16 lvp verify pic18_128kb.hex
; pic16_verifyregion: error: read [F7] expected [FF] at [1FFF9]
; Total: 131072 Pass: 131071 Fail: 1
; Time: 0:03.16s
; 
; $ k16 lvp program pic18lf27j53.hex 
; Total: 1102
; Time: 0:00.59s
;
; $ k16 lvp verify pic18lf27j53.hex
; Total: 1102 Pass: 1102 Fail: 0
; Time: 0:00.05s
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18LF27J53
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18lf27j53.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; Watchdog Timer:
                CONFIG  WDTEN=OFF ; Disabled - Controlled by SWDTEN bit
; PLL Prescaler Selection:
                CONFIG  PLLDIV=1 ; No prescale (4 MHz oscillator input drives PLL directly)
; PLL Enable Configuration Bit:
                CONFIG  CFGPLLEN=ON ; PLL Enabled
; Stack Overflow/Underflow Reset:
                CONFIG  STVREN=ON
; Extended Instruction Set:
                CONFIG  XINST=OFF
; CPU System Clock Postscaler:
                CONFIG  CPUDIV=OSC1 ; No CPU system clock divide
; Code Protect:
                CONFIG  CP0=OFF
; Oscillator:
                CONFIG  OSC=INTOSCPLL
; T1OSC/SOSC Power Selection Bits:
                CONFIG  SOSCSEL=DIG
; EC Clock Out Enable Bit:
                CONFIG  CLKOEC=OFF
; Fail-Safe Clock Monitor:
                CONFIG  FCMEN=OFF
; Internal External Oscillator Switch Over Mode:
                CONFIG  IESO=OFF
; Watchdog Postscaler:
                CONFIG  WDTPS=1024
; DSWDT Clock Select:
                CONFIG  DSWDTOSC=INTOSCREF
; RTCC Clock Select:
                CONFIG  RTCOSC=INTOSCREF
; Deep Sleep BOR:
                CONFIG  DSBOREN=OFF
; Deep Sleep Watchdog Timer:
                CONFIG  DSWDTEN=OFF
; Deep Sleep Watchdog Postscaler:
                CONFIG  DSWDTPS=G2
; IOLOCK One-Way Set Enable bit:
                CONFIG  IOL1WAY=OFF
; ADC 10 or 12 Bit Select:
                CONFIG  ADCSEL=BIT12
; MSSP address masking:
                CONFIG  MSSP7B_EN=MSK7
; Write/Erase Protect Page Start/End Location:
                CONFIG  WPFP=PAGE_0
; Write/Erase Protect Configuration Region:
                CONFIG  WPCFG=OFF
; Write Protect Disable bit:
                CONFIG  WPDIS=OFF
; Write/Erase Protect Region Select bit (valid when WPDIS = 0):
                CONFIG  WPEND=PAGE_0
; Low Speed USB mode with 48 MHz system clock bit:
                CONFIG  LS48MHZ=SYS48X8
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; INTOSCPLL = 48MHz
    CONSTANT CLOCK = 48000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x00                        ;ACCESS RAM 0x00..0x5F
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0008
                GOTO    INTHIGH             ;When IPEN=0 or 1
                ORG     0x0018
                GOTO    INTLOW              ;When IPEN=1
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; ICSP I/O
;
NPINS           SET     .28                 ;28-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; High priority ISR
; 
INTHIGH
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Low priority ISR
;
INTLOW
                RETFIE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT?
                GOTO    POWERUP

                MOVLW   0xFF                ;WATCHDOG TIMED OUT
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATA                ;INIT PORT A
                CLRF    LATB                ;INIT PORT B

WATCHDOG
                MOVLW   b'00000000'         ;PORT A ALL O/P    
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PORT B ALL I/P
                MOVWF   TRISB

                CLRWDT                      ;INIT WATCHDOG TIMER
                BSF     WDTCON,SWDTEN       ;START WATCHDOG TIMER
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CALL    INITIO              ;INITIALISE ICSPIO
;
MAINLOOP        COMMON  MAINLOOP, INIT      ;DO COMMON COMMANDS

                GOTO    UNSUPPORTED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
