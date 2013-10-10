;
; R-PI 3V3 PIC18F26K80 TODO
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
; RC3          14----15 RC4
;
; Program
; -------
; k16 mchp program pic18f26k80.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F26K80
ERRORLEVEL      -302
#INCLUDE        "p18f26k80.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; R-PI 3V3 PIC18F26K80 TODO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; VREG Sleep Enable bit:
                CONFIG    RETEN=ON
; LF-INTOSC Low-power Enable bit:
;               CONFIG    INTOSCSEL=LOW
; SOSC Power Selection and mode Configuration bits:
                CONFIG    SOSCSEL=DIG
; Extended Instruction Set:
                CONFIG    XINST=OFF
; Oscillator:
                CONFIG    FOSC=INTIO2
; PLL x4 Enable bit:
                CONFIG    PLLCFG=OFF
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Oscillator Switch Over Mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRTEN=OFF
; Brown-out Detect:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=1
; BORMV Power level:
;               CONFIG    BORPWR=LOW
; Watchdog Timer:
                CONFIG    WDTEN=ON
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; ECAN Mux bit:
                CONFIG    CANMX=PORTC
; MSSP address masking:
                CONFIG    MSSPMSK=MSK5
; Master Clear Enable:
                CONFIG    MCLRE=ON
; Stack Overflow Reset:
                CONFIG    STVREN=ON
; Boot Block Size:
                CONFIG    BBSIZ=BB1K
; Code Protect 00800-03FFF:
                CONFIG    CP0=OFF
; Code Protect 04000-07FFF:
                CONFIG    CP1=OFF
; Code Protect 08000-0BFFF:
                CONFIG    CP2=OFF
; Code Protect 0C000-0FFFF:
                CONFIG    CP3=OFF
; Code Protect Boot:
                CONFIG    CPB=OFF
; Data EE Read Protect:
                CONFIG    CPD=OFF
; Table Write Protect 00800-03FFF:
                CONFIG    WRT0=OFF
; Table Write Protect 04000-07FFF:
                CONFIG    WRT1=OFF
; Table Write Protect 08000-0BFFF:
                CONFIG    WRT2=OFF
; Table Write Protect 0C000-0FFFF:
                CONFIG    WRT3=OFF
; Config. Write Protect:
                CONFIG    WRTC=OFF
; Table Write Protect Boot:
                CONFIG    WRTB=OFF
; Data EE Write Protect:
                CONFIG    WRTD=OFF
; Table Read Protect 00800-03FFF:
                CONFIG    EBTR0=OFF
; Table Read Protect 04000-07FFF:
                CONFIG    EBTR1=OFF
; Table Read Protect 08000-0BFFF:
                CONFIG    EBTR2=OFF
; Table Read Protect 0C000-0FFFF:
                CONFIG    EBTR3=OFF
; Table Read Protect Boot:
                CONFIG    EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS _IDLOC0,8
; __IDLOCS _IDLOC1,7
; __IDLOCS _IDLOC2,6
; __IDLOCS _IDLOC3,5
; __IDLOCS _IDLOC4,4
; __IDLOCS _IDLOC5,3
; __IDLOCS _IDLOC6,2
; __IDLOCS _IDLOC7,1
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F26K80",0
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
; High priority ISR
; 
INTHIGH
                RETFIE  FAST
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
INIT
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
MAIN            GOTO    MAIN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
