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
.title      "LED Demo"
.sbttl      "It blinks, quickly!"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; PIC24FJ64GA002 Pinout
; ---------------------
; !MCLR                             1-----28 VDD 3V3
; AN0 VREF+ CN2 RA0                 2     27 VSS GND
; AN1 VREF- CN3 RA1                 3     26 AN9 RP15 CN11 PMCS RB15
; PGD1 EMUD1 AN2 C2IN- RP0 CN4 RB0  4     25 AN10 CVREF RTCC RP14 CN12 PMWR RB14
; PGC1 EMUC1 AN3 C2IN+ RP1 CN5 RB1  5     24 AN11 RP13 CN13 PMRD RB13
; AN4 C1IN- SDA2 RP2 CN6 RB2        6     23 AN12 RP12 CN14 PMD0 RB12
; AN5 C1IN+ SCL2 RP3 CN7 RB3        7     22 PGC2 EMUC2 TMS RP11 CN15 PMD1 RB11
; VSS GND                           8     21 PGD2 EMUD2 TDI RP10 CN16 PMD2 RB10
; OSCI CLKI CN30 RA2                9     20 VCAP VDDCORE
; OSCO CLKO CN29 PMA0 RA3           10    19 DISVREG
; SOSCI RP4 PMBE CN1 RB4            11    18 TDO SDA1 RP9 CN21 PMD3 RB9
; SOSCO T1CK CN0 PMA1 RA4           12    17 TCK SCL1 RP8 CN22 PMD4 RB8
; VDD 3V3                           13    16 RP7 INT0 CN23 PMD5 RB7
; PGD3 EMUD3 SDA1 RP5 CN27 PMD7 RB5 14----15 PGC3 EMUC3 SCL1 RP6 CN24 PMD6 RB6
;
; POWER
; -----
;    3V3--------VDD                     13, 28
;
;     +---------VDDCORE                 20
;     |
; 10u =    +----DISVREG                 19
;     |    |
;     +----+----VSS                     8, 27
;     |
;    ///
;
; ICSP
; ----
; !MCLR         PROGRAMMING ENABLE      1
; PGC1          PRIMARY CLOCK           5
; PGD1          PRIMARY DATA            4
; PGC2          SECONDARY CLOCK         22
; PGD2          SECONDARY DATA          21
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.nolist
.include    "p24Fxxxx.inc"
.list
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
CONFIG      __CONFIG1,  JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1
CONFIG      __CONFIG2,  FNOSC_FRCPLL & OSCIOFNC_ON & POSCMOD_NONE & I2C1SEL_PRI
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.text
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.global     __reset
__reset:
.if 0
            MOV         #__SP_init, W15
            MOV         #__SPLIM_init, W0
            MOV         W0, _SPLIM
            NOP
            BCLR        _CORCON, #PSV
.endif
            MOV         #0xFFFF, W0
            MOV         W0, AD1PCFG

            MOV         #0xFFFE, W0
            MOV         W0, LATA
            MOV         W0, TRISA

loop:       BTG         LATA, #0
            
            CLR         W0
delay:      DEC         W0, W0
            BRA         NZ, delay
            
            GOTO        loop
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: ft=asm shiftwidth=12 tabstop=12 softtabstop=12 expandtab
;
