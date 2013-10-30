;******************************************************************************
; Test area for the currently unsupported PIC24 and dsPIC PICMicros.
;******************************************************************************
;
; PIC24FJ64GA002 Pinout
; ---------------------
; !MCLR                              1-----28 VDD 3V3
; AN0 VREF+ CN2 RA0                  2     27 VSS GND
; AN1 VREF- CN3 RA1                  3     26 AN9 RP15 CN11 PMCS1 RB15
; PGD1 EMUD1 AN2 C2IN- RP0 CN4 RB0   4     25 AN10 CVREF RTCC RP14 CN12 PMWR RB14
; PGC1 EMUC1 AN3 C2IN+ RP1 CN5 RB1   5     24 AN11 RP13 CN13 PMRD RB13
; AN4 C1IN- SDA2 RP2 CN6 RB2         6     23 AN12 RP12 CN14 PMDO RB12
; AN5 C1IN+ SCL2 RP3 CN7 RB3         7     22 PGC2 EMUC2 TMS RP11 CN15 PMD1 RB11
; VSS GND                            8     21 PGD2 EMUD2 TDI RP10 CN16 PMD2 RB10
; OSCI CLKI CN30 RA2                 9     20 VCAP VDDCORE
; OSCO CLKO CN29 PMA0 RA3            10    19 DIS/VREG
; SOSCI RP4 PMBE CN1 RB4             11    18 TDO SDA1 RP9 CN21 PMD3 RB9
; SOSCO T1CK CN0 PMA1 RA4            12    17 TCK SCL1 RP8 CN22 PMD4 RN8
; VDD 3V3                            13    16 RP7 INT0 CN23 PMD5 RB7
; PGD3 EMUD3 ASDA1 RP5 CN27 PMD7 RB5 14----15 PGC3 EMUC3 ASCL1 RP6 CN24 PMD6 RB6
;
; POWER
; -----
;    3V3--------VDD				13, 28
;
;     +---------VDDCORE				20
;     |
; 10u =    +----DIS/VREG			19
;     |    |
;     +----+----VSS				8, 27
;     |
;    ///
;
; ICSP
; ----
; !MCLR		PROGRAMMING DISABLE/ENABLE	1
; PGC1		PRIMARY CLOCK			5
; PGD1		PRIMARY DATA			4
; PGC2		SECONDARY CLOCK			22
; PGD2		SECONDARY DATA			21
;
;******************************************************************************
