k8048 reference for the PIC16F648A
----------------------------------

Pinout
------
RA2 AN2   VREF 1----18 RA1 AN1
RA3 AN3   CMP1 2    17 RA0 AN0
RA4 T0CKI CMP2 3    16 RA7 OSC1  CLKIN
RA5 !MCLR VPP  4    15 RA6 OSC2  CLKOUT
VSS GND        5    14 VDD VCC
RB0 INT        6    13 RB7 T1OSI PGD
RB1 RX    DT   7    12 RB6 T1OSO T1CKI  PGC
RB2 TX    CK   8    11 RB5
RB3 CCP1       9----10 RB4 PGM

Function	Pin
--------	--------
LD1		RB0 (6)
LD2		RB1 (7)
LD3		RB2 (8)
LD4		RB3 (9)
LD5		RB4 (10)
LD6		RB5 (11)

PGC		RB6 (12)
PGD		RB7 (13)

SW1		RA0 (17)
SW2		RA1 (18)
SW3		RA2 (1)
SW4		RA3 (2)

		RA4 (3)
VPP		RA5 (4)
OSC2		RA6 (15)
OSC1		RA7 (16)

GND		VSS (5)
+5V		VDD (14)
