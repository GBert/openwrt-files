;
; R-PI 3V3 PIC18LF27J53 TODO
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 65536 words Flash (16-bit)
; 3800 bytes RAM
; 0 bytes EEPROM
;
; Pinout
; ------
; !MCLR        1-----28 RB7
; RA0          2     27 RB6
; RA1          3     26 RB5
; RA2          4     25 RB4
; RA3          5     24 RB3
; VDDCORE/VCAP 6     23 RB2
; RA5          7     22 RB1
; VSS GND      8     21 RB0
; RA7          9     20 VDD VCC
; RA6          10    19 VSS GND
; RC0          11    18 RC7
; RC1          12    17 RC6
; RC2          13    16 RC5
; VUSB         14----15 RC4
;
; Program
; -------
; k16 mchp program pic18lf27j53.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18LF27J53
ERRORLEVEL      -302
#INCLUDE        "p18lf27j53.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;******************************************************************************
;
; R-PI 3V3 PIC18LF27J53 TODO
;
; This demo allows control from `kio' to perform ICSPIO common operations.
;
;******************************************************************************
;
; Config
;
; Watchdog Timer:
                CONFIG  WDTEN=OFF 
; PLL Prescaler Selection:
                CONFIG  PLLDIV=3
; PLL Enable Configuration Bit:
                CONFIG  CFGPLLEN=OFF
; Stack Overflow/Underflow Reset:
                CONFIG  STVREN=ON
; Extended Instruction Set:
                CONFIG  XINST=OFF
; CPU System Clock Postscaler:
                CONFIG  CPUDIV=OSC1
; Code Protect:
                CONFIG  CP0=OFF
; Oscillator:
                CONFIG  OSC=INTOSC
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
; HFINTOSC = TODO
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
INIT            ;TODO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CLRF    LASTERROR
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
