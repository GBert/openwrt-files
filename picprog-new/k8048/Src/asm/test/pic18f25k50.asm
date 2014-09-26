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
; Velleman K8048 PIC18F25K50 ICSPIO Demo Test (Receive commands, send data).
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
; 16384 words Flash (16-bit)
; 2048 bytes RAM
; 256 bytes EEPROM
;
; Pinout
; ------
; !MCLR VPP RE3 1-----28 RB7 PGD
; RA0           2     27 RB6 PGC
; RA1           3     26 RB5
; RA2           4     25 RB4
; RA3           5     24 RB3
; RA4           6     23 RB2
; RA5           7     22 RB1
; VSS GND       8     21 RB0
; RA7           9     20 VDD VCC
; RA6           10    19 VSS GND
; RC0           11    18 RC7
; RC1           12    17 RC6
; RC2           13    16 D+
; VUSB3V3       14----15 D-
;
; K8048 Pin
; ----- ---
; LD1   RA0
; LD2   RA1
; LD3   RA2
; LD4   RA3
; LD5   RA4
; LD6   RA5
; SW1   RB0
; SW2   RB1
; SW3   RB2
; SW4   RB4
;
; Program
; -------
; k16 p pic18f25k50.hex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F25K50
ERRORLEVEL      -302
ERRORLEVEL      -305
#INCLUDE        "p18f25k50.inc"
#INCLUDE        "device.inc"                ;DEVICE CONFIG
#INCLUDE        "const.inc"                 ;CONSTANTS
#INCLUDE        "macro.inc"                 ;MACROS
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Config
;
; PLL Selection:
                CONFIG    PLLSEL=PLL4X
; PLL Enable Configuration bit:
                CONFIG    CFGPLLEN=OFF 
; CPU System Clock Postscaler:
                CONFIG    CPUDIV=NOCLKDIV
; Low Speed USB mode with 48 MHz system clock:
                CONFIG    LS48MHZ=SYS48X8
; Oscillator Selection:
                CONFIG    FOSC=INTOSCIO
; Primary Oscillator Shutdown:
                CONFIG    PCLKEN=OFF
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover:
                CONFIG    IESO=OFF
; Power-up Timer Enable:
                CONFIG    nPWRTEN=OFF
; Brown-out Reset Enable:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage:
                CONFIG    BORV=285
; Watchdog Timer Enable bits:
                CONFIG    WDTEN=SWON
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; CCP2 MUX bit:
                CONFIG    CCP2MX=RC1
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; Timer3 Clock input mux bit:
                CONFIG    T3CMX=RB5
; SDO Output MUX bit:
                CONFIG    SDOMX=RB3 
; Master Clear Reset Pin Enable:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Single-Supply ICSP Enable bit:
                CONFIG    LVP=ON
; Dedicated In-Circuit Debug/Programming Port Enable:
                CONFIG    ICPRT = OFF 
; Extended Instruction Set Enable bit:
                CONFIG    XINST=OFF
; Background Debugger Enable bit:
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
; HFINTOSC = 16MHz PLL X4 (16 MIPS)
    CONSTANT CLOCK = 64000000
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data EEPROM
;
                ORG     0xF00000
                DE      "PIC18F25K50",0
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
INIT            MOVLB   0x0F                ;SFR ACCESS IN BANKED MODE

                CLRF    OSCCON2             ;INIT CLOCK 16MHz HFINTOSC PLL X4
                BCF     OSCTUNE,SPLLMULT    ;PLL MULIPLIER X4
                BSF     OSCCON2,PLLEN       ;PLL ENABLED
                MOVLW   b'01110000'         ;16 MHz
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCCON,HFIOFS       ;WAIT FOR HFINTOSC FREQUENCY STABLE
                GOTO    INITHFIOFS

                BTFSC   RCON,NOT_TO         ;WATCHDOG TIME-OUT?
                GOTO    POWERUP

                MOVLW   0xFF                ;WATCHDOG TIMED OUT
                XORWF   LATA,F

                GOTO    WATCHDOG            ;CONTINUE

POWERUP         SETF    LATA                ;INIT PORT A
                CLRF    LATB                ;INIT PORT B

WATCHDOG
                CLRF    ADCON0              ;DISABLE A/D
                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC

                MOVLW   b'11000000'         ;LD1..LD6 O/P    
                MOVWF   TRISA

                MOVLW   b'11111111'         ;PGD/PGC I/P SW1..SW4 I/P
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

                MOVF    BUFFER,W            ;SET LD1..LD6
                ANDLW   0x3F
                MOVWF   LATA

                GOTO    DOEND               ;COMMAND COMPLETED
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
