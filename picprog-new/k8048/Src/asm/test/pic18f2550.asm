;
; Velleman K8048 PIC18F2550 ICSPIO Demo Test (Receive commands, send data).
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
; !MCLR VPP RE3            1-----28 RB7 KBI3 PGD
; RA0 AN0                  2     27 RB6 KBI2 PGC
; RA1 AN1                  3     26 RB5 KBI1 PGM
; RA2 AN2 VREF- CVREF      4     25 RB4 KBI0 AN11
; RA3 AN3 VREF+            5     24 RB3 AN9  CCP2 VPO
; RA4 T0CKI C1OUT RCV      6     23 RB2 AN8  INT2 VMO
; RA5 AN4 !SS HLVDIN C2OUT 7     22 RB1 AN10 INT1 SCK SCL
; VSS GND                  8     21 RB0 AN12 INT0 FLT0 SDI SDA
; OSC1 CLKI                9     20 VDD VCC
; OSC2 CLKO RA6            10    19 VSS GND
; RC0 T1OSO T13CKI         11    18 RC7 RX DT SDO
; RC1 T1OSI CCP2 !UOE      12    17 RC6 TX CK
; RC2 CCP1                 13    16 RC5 D+ VP
; VUSB                     14----15 RC4 D- VM
;
; Function Pin Note
; -------- --- ----
; LD1      RA0
; LD2      RA1
; LD3      RA2
; LD4      RA3
; LD5      RA4
; LD6      RA5
; SW1      RB0
; SW2      RB1
; SW3      RB2
; SW4      RB4
;
; Program
; -------
; k16 p pic18f2550.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F2550
ERRORLEVEL      -302
#INCLUDE        "p18f2550.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; K8048 PIC18F2550 (DS39599G) ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets the six LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the four switches.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; PLL Prescaler Selection bits:
                CONFIG    PLLDIV=1
; CPU System Clock Postscaler:
                CONFIG    CPUDIV=OSC1_PLL2
; USB Clock Selection bit (used in Full Speed USB mode only; UCFG:FSEN = 1):
                CONFIG    USBDIV=1
; Oscillator Selection:
                CONFIG    FOSC=XT_XT
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Reset:
                CONFIG    BOR=OFF
; Brown-out Voltage:
                CONFIG    BORV=2
; Watchdog Timer:
                CONFIG    WDT=ON
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; MCLR Enable:
                CONFIG    MCLRE=ON
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; CCP2 Pin Function:
                CONFIG    CCP2MX=OFF
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=OFF
; Background Debugger Enable:
                CONFIG    DEBUG=OFF
; Code Protection Block 0:
                CONFIG    CP0=OFF
; Code Protection Block 1:
                CONFIG    CP1=OFF
; Code Protection Block 2:
                CONFIG    CP2=OFF
; Code Protection Block 3:
                CONFIG    CP3=OFF
; Boot Block Code Protection:
                CONFIG    CPB=OFF
; Data EEPROM Code Protection:
                CONFIG    CPD=OFF
; Write Protection Block 0:
                CONFIG    WRT0=OFF
; Write Protection Block 1:
                CONFIG    WRT1=OFF
; Write Protection Block 2:
                CONFIG    WRT2=OFF
; Write Protection Block 3:
                CONFIG    WRT3=OFF
; Boot Block Write Protection:
                CONFIG    WRTB=OFF
; Configuration Register Write Protection:
                CONFIG    WRTC=OFF
; Data EEPROM Write Protection:
                CONFIG    WRTD=OFF
; Table Read Protection Block 0:
                CONFIG    EBTR0=OFF
; Table Read Protection Block 1:
                CONFIG    EBTR1=OFF
; Table Read Protection Block 2:
                CONFIG    EBTR2=OFF
; Table Read Protection Block 3:
                CONFIG    EBTR3=OFF
; Boot Block Table Read Protection:
                CONFIG    EBTRB=OFF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
  __IDLOCS _IDLOC0,1
  __IDLOCS _IDLOC1,2
  __IDLOCS _IDLOC2,3
  __IDLOCS _IDLOC3,4
  __IDLOCS _IDLOC4,5
  __IDLOCS _IDLOC5,6
  __IDLOCS _IDLOC6,7
  __IDLOCS _IDLOC7,8
;
; XTAL = 4MHz
    CONSTANT CLOCK = 4000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F2550",0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK          0x00                        ;ACCESS RAM 0x00..0x7F
ENDC
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Begin
;
                ORG     0x0000
                GOTO    INIT
                ORG     0x0008
                RETFIE
                ORG     0x0018
                RETFIE
                ORG     0x0020
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; ICSP I/O
;
NPINS           SET     .28                 ;28-PIN PDIP
#INCLUDE        "delay.inc"                 ;DELAY COUNTERS
#INCLUDE        "icspio.inc"                ;ICSP I/O
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
INIT            BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   0xFF
                XORWF   LATA,F
                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATA                ;INIT PORTA
                CLRF    LATB                ;INIT PORTB

WATCHDOG        CLRWDT                      ;RESET WATCHDOG

                MOVLW   b'00000111'         ;DISABLE COMPARATORS
                MOVWF   CMCON

                MOVLW   b'00000000'         ;DISABLE A/D
                MOVWF   ADCON0
                MOVLW   b'00001111'         ;DIGITAL I/O
                MOVWF   ADCON1

                MOVLW   b'11000000'         ;LD1..LD6 O/P    
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PGD/PGC I/P SW1..SW4 I/P
                MOVWF   TRISB
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Main loop
;
                CLRF    LASTERROR
;
MAINLOOP        CLRF    CHECKSUM            ;START SESSION
                CALL    GETBYTE             ;GET COMMAND
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRWDT                      ;RESET WATCHDOG
;
; COMMAND VALIDATE
;
                MOVF    BUFFER,W            ;IS SLEEP?
                XORLW   CMD_SLEEP
                BZ      DOSLEEP

                MOVF    BUFFER,W            ;IS LED?
                XORLW   CMD_LED
                BZ      DOLED

                MOVF    BUFFER,W            ;IS SWITCH?
                XORLW   CMD_SWITCH
                BZ      DOSWITCH

                MOVF    BUFFER,W            ;IS ERROR?
                XORLW   CMD_ERROR
                BZ      DOERROR
;
; COMMAND UNSUPPORTED
;
                CALL    SENDNAK             ;COMMAND UNSUPPORTED
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
; Standby
;
DOSLEEP         CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CLRF    OSCCON              ;SET SLEEP MODE
                SLEEP                       ;SLEEP UNTIL WATCHDOG TIME-OUT
                GOTO    INIT                ;RESET ON WAKE-UP
;
; Set LD1..LD6
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                MOVF    BUFFER,W            ;SET LD1..LD6
                ANDLW   0x3F
                MOVWF   LATA

                GOTO    DOEND               ;COMMAND COMPLETED
;
; Get SW1..SW4
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVF    PORTB,W             ;GET SW1..SW3
                ANDLW   B'00000111'
                BTFSC   PORTB,4             ;GET SW4
                IORLW   B'00001000'

                CALL    SENDBYTE            ;SEND SW1..SW4
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
; Get last error
;
DOERROR         CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                MOVF    LASTERROR,W
                CLRF    LASTERROR
                CALL    SENDBYTE
                BC      IOERROR
;
; Command completed
;
DOEND           CALL    SENDSUM             ;CLOSE SESSION
                BC      IOERROR             ;TIME-OUT

                GOTO    MAINLOOP            ;CONTINUE
;
; Time-out, protocol or parity error
;
IOERROR         MOVWF   LASTERROR

                GOTO    MAINLOOP            ;CONTINUE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
