;
; Velleman K8048 PIC18F1330 ICSPIO Demo Test (Receive commands, send data).
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
;; Pinout
; ------
; RA0       1----18 RB3
; RA1       2    17 RB2
; RA4       3    16 RA7
; !MCLR VPP 4    15 RA6
; VSS GND   5    14 VDD VCC
; RA2       6    13 RB7 PGD
; RA3       7    12 RB6 PGC
; RB0       8    11 RB5
; RB1       9----10 RB4
;
; K8048 Pin
; ----- ---
; LD1   RA2 (6)
; LD2   RA3 (7)
; LD3   RB0 (8)
; LD4   RB1 (9)
; LD5   RB4 (10)
; LD6   RB5 (11)
; SW1   RB2 (17)
; SW2   RB3 (18)
; SW3   RA0 (1)
; SW4   RA1 (2)
;       RA4 (3)
; VPP   RA5 (4)
; PGC   RB6 (12)
; PGD   RB7 (13)
; OSC2  RA6 (15)
; OSC1  RA7 (16)
; GND   VSS (5)
; +5V   VDD (14)
;
; Program
; -------
; k16 p pic18f1330.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F1330
ERRORLEVEL      -302
#INCLUDE        "p18f1330.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;******************************************************************************
;
; K8048 PIC18F1330 (DS39599G) ICSPIO Demo Test (Receive commands, send data).
;
; This demonstrates how we may receive commands from the host computer
; via the ISCP port and execute them. Two commands are implemented.
; The first command takes one argument which sets the six LEDs to that
; value and the second command takes no argument yet demonstrates how
; we may send a value back to the host which, in this case, is the
; current status of the four switches.
;
;******************************************************************************
;
; Config
;
; Oscillator Selection bits:
                CONFIG    OSC=INTIO2
; Fail-Safe Clock Monitor Enable bit:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover bit:
                CONFIG    IESO=OFF
; Power-up Timer Enable bit:
                CONFIG    PWRT=OFF
; Brown-out Reset Enable bits:
                CONFIG    BOR=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=1
; Watchdog Timer Enable bits:
                CONFIG    WDT=OFF
; Watchdog Postscale Select bits:
                CONFIG    WDTPS=1024
; PWM Output Pins Reset State Control bit:
                CONFIG    PWMPIN=OFF
; Low-Side Transistors Polarity bit (Even PWM Output Polarity Control bit):
;               CONFIG    LPOL=HIGH
; High Side Transistors Polarity bit (Odd PWM Output Polarity Control bit):
;               CONFIG    HPOL=HIGH
; FLTA Mux bit:
                CONFIG    FLTAMX=RA7
; T1OSO/T1CKI MUX bit:
;               CONFIG    T1OSCMX=HIGH
; MCLR Pin Enable bit:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset Enable bit:
                CONFIG    STVREN=ON
; Boot Block Size Select bits:
                CONFIG    BBSIZ=BB256
; Extended Instruction Set Enable bit:
                CONFIG    XINST=OFF
; Background Debugger Enable bit:
                CONFIG    DEBUG=OFF
; Code Protection Block 0 (000800-000FFF):
                CONFIG    CP0=OFF
; Code Protection Block 1 (001000-001FFF):
                CONFIG    CP1=OFF
; Code Protection bit (Boot Block Memory Area):
                CONFIG    CPB=OFF
; Code Protection bit (Data EEPROM):
                CONFIG    CPD=OFF
; Write Protection bit Block 0 (000800-000FFF):
                CONFIG    WRT0=OFF
; Write Protection bit Block 1 (001000-001FFF):
                CONFIG    WRT1=OFF
; Write Protection bit (Configuration Registers):
                CONFIG    WRTC=OFF
; Write Protection bit (Boot Block Memory Area):
                CONFIG    WRTB=OFF
; Write Protection bit (Data EEPROM):
                CONFIG    WRTD=OFF
; Table Read Protection bit Block 0 (000800-000FFF):
                CONFIG    EBTR0=OFF
; Table Read Protection bit Block 1 (001000-001FFF):
                CONFIG    EBTR1=OFF
; Table Read Protection bit (Boot Block Memory Area):
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
; 
    CONSTANT CLOCK = 8000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F1330",0
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
#INCLUDE        "common.inc"                ;COMMON COMMANDS MACRO
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Initialise
;
; XXX With LVP mode enabled RB5 is not available and this pin will not operate
;     as this demo expects.
;
INIT            MOVLW   b'01110010'         ;INIT CLOCK 8MHZ INTRC
                MOVWF   OSCCON
INITIOFS        BTFSS   OSCCON,IOFS         ;WAIT FOR INTRC FREQUENCY STABLE
                GOTO    INITIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT
                GOTO    POWERUP

                MOVLW   b'00001100'
                XORWF   LATA,F              ;TOGGLE RA2 RA3
                MOVLW   b'00110011'
                XORWF   LATB,F              ;TOGGLE RB0 RB1 RB4 RB5

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         MOVLW   b'00001100'         ;INIT PORTA
                MOVWF   LATA                ;SET RA2 RA3
                MOVLW   b'00110011'         ;INIT PORTB
                MOVWF   LATB                ;SET RB0 RB1 RB4 RB5

WATCHDOG        MOVLW   b'00000000'         ;DISABLE A/D
                MOVWF   ADCON0
                MOVLW   b'00001111'         ;DIGITAL I/O
                MOVWF   ADCON1

                MOVLW   b'11110011'         ;PORTA O/P RA2 RA3
                MOVWF   TRISA               ;LD1+LD2  O/P SW3+SW4 I/P

                MOVLW   b'11001100'         ;PORTB O/P RB0 RB1 RB4 RB5
                MOVWF   TRISB               ;LD3..LD6 O/P SW1+SW2 I/P

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

                MOVF    BUFFER,W            ;IS LED?
                XORLW   CMD_LED
                BZ      DOLED

                MOVF    BUFFER,W            ;IS SWITCH?
                XORLW   CMD_SWITCH
                BZ      DOSWITCH

                GOTO    UNSUPPORTED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Set LD1..LD6
;
DOLED           CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CALL    GETBYTE             ;GET LD ARG
                BC      IOERROR             ;TIME-OUT, PROTOCOL OR PARITY ERROR

                CLRW                        ;SET LD1..LD2
                BTFSC   BUFFER,0
                IORLW   B'00000100'         ;LD1 = RA2
                BTFSC   BUFFER,1
                IORLW   B'00001000'         ;LD2 = RA3
                MOVWF   LATA
                CLRW                        ;SET LD3..LD6
                BTFSC   BUFFER,2
                IORLW   B'00000001'         ;LD3 = RB0
                BTFSC   BUFFER,3
                IORLW   B'00000010'         ;LD4 = RB1
                BTFSC   BUFFER,4
                IORLW   B'00010000'         ;LD5 = RB4
                BTFSC   BUFFER,5
                IORLW   B'00100000'         ;LD6 = RB5
                MOVWF   LATB

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Get SW1..SW4
;
DOSWITCH        CALL    SENDACK             ;COMMAND SUPPORTED
                BC      IOERROR             ;TIME-OUT

                CLRW                        ;GET SW1..SW4
                BTFSC   PORTB,2
                IORLW   B'00000001'         ;SW1 = RB2
                BTFSC   PORTB,3
                IORLW   B'00000010'         ;SW2 = RB3
                BTFSC   PORTA,0
                IORLW   B'00000100'         ;SW2 = RA0
                BTFSC   PORTA,1
                IORLW   B'00001000'         ;SW2 = RA1

                CALL    SENDBYTE            ;SEND SW1..SW4
                BC      IOERROR             ;TIME-OUT

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
