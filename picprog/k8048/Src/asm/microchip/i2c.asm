;
; Software License Agreement
;
; The software supplied herewith by Microchip Technology Incorporated 
; (the “Company”) for its PICmicro® Microcontroller is intended and 
; supplied to you, the Company’s customer, for use solely and 
; exclusively on Microchip PICmicro Microcontroller products. The 
; software is owned by the Company and/or its supplier, and is 
; protected under applicable copyright laws. All rights are reserved. 
;  Any use in violation of the foregoing restrictions may subject the 
; user to criminal sanctions under applicable laws, as well as to 
; civil liability for the breach of the terms and conditions of this 
; license.  
;
; THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES, 
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
;************************************************************
;	PIC18CXX2 EXAMPLE CODE FOR PICDEM-2
;
;	TITLE:	I2C hardware interface PICmicro to serial EEPROM
;	FILENAME:  i2c.asm
;	REVISION HISTORY:  A 5/13/00 jb   format change
;	HARDWARE: PICDEM-2 board
;	FREQUENCY: 4MHz
;
;************************************************************
; This program uses the advanced features of the PIC18C452, specifically
; full hardware support for master mode I2C
;
; This program loads EEPROM address 0x00 to 0xFF with 0x00 to 0xFF
; (each location contains its own address). 
;
; Each location is then read out, and compared to what is expected.
; The data is displayed on the LEDS. If the data is wrong, 
; the TX LED will flash briefly before proceeding to the next address.
; 
; Revised Version	(05-05-99).
; Note:  1) All timing is based on a reference crystal frequency of 4MHz
;	which is equivalent to an instruction cycle time of 1 usec.
;        2) Address and literal values are read in hexidecimal unless 
;	otherwise specified.
;        3) The PIC18C452 MSSP module offers full hardware support for
;	master mode I2C.
;************************************************************

	LIST P=18C452
	#include "p18c452.inc"
	list	n=0	; suppress list file page breaks
	list	ST=off	; suppress list file symbol table

;************************************************************
; Register File Assignment

EEADDR	equ	0x000	; Address register
EEDATA	equ	0x001	; data to read/write
EESLAVE	equ	0x002	; Device address (1010xxxy)
DelayCtr1	equ	0x003	; delay routine counter
DelayCtr2	equ	0x004	; delay routine counter

SlaveAddr	equ	0xA0	; slave address literal

;************************************************************
; Vector Assignment
	ORG	0x00000
	goto	Start	; Reset Vector

;************************************************************
; Main Program

	ORG	0x00020		; Start of Program space
Start
	; initialize PORTB
	clrf	LATB		; Clear PORTB output latch
	clrf	TRISB		; Set PORTB as all outputs
	
	; configure SSP for hardware master mode I2C
	bsf	SSPSTAT,SMP	; I2C slew rate control disabled
	bsf	SSPCON1,SSPM3	; I2C master mode in hardware
	bsf	SSPCON1,SSPEN	; enable SSP module

	movlw	0x09		; set I2C clock rate to 100kHz
	movwf	SSPADD
	
	bsf	TRISC,3		; I2C SCL pin is input
	bsf	PORTC,3		; (will be controlled by SSP)
	bsf	TRISC,4		; I2C SDA pin is input
	bsf	PORTC,4		; (will be controlled by SSP)
	
	movlw	SlaveAddr	; EEPROM I2C address
	movwf	EESLAVE

Main
	clrf	PORTB		; initialize variables
 	clrf	EEDATA
	clrf	EEADDR

WrEEPROM
	bsf	PORTB,7		; indicate write, light TX LED
	rcall	Delay
	
	bcf	EESLAVE,0	; write mode
	rcall	WakeSlave	; gets slave attention
	rcall	WrADDR		; sends EEPROM address
	rcall	WrDATA		; sends data to slave
	rcall	Stop		; send stop bit
	
	incf	PORTB,F		; increment count
	incf	EEDATA,F	; increment data
	incf	EEADDR,F	; Point to next address
	
	btfss	EEADDR,7	; at end of EEPROM?
	goto	WrEEPROM	; no, write more data

RdLoop
	clrf	PORTB		; initialize variables
	clrf	EEDATA
	clrf	EEADDR

RdEEPROM
	rcall	Delay
	
	bcf	EESLAVE,0	; write mode
	rcall	WakeSlave	; gets slave attention
	rcall	WrADDR		; sends EEPROM address
	rcall	Stop		; send stop bit
	
	bsf	EESLAVE,0	; read mode
	rcall	WakeSlave	; gets slave attention
	rcall	RdDATA		; receive one data byte, leaves idle

	movf	EEDATA,W	; get data
	movwf	PORTB		; move received data to PORTB
	xorwf	EEADDR,W	; compare data with address
	bz	GoodData	; branch if DATA = ADDR
	rcall	Errorloop	; DATA is wrong, indicate error
	
GoodData	incf	EEADDR,F		; Point to next address
	btfss	EEADDR,7	; at end of EEPROM?
	goto	RdEEPROM	; no, read more data
	
	goto	Main		; do it all over again
	
;************************************************************
; TX LED flashes to indicate error while displaying received data.

Errorloop
	rcall	Delay
	btg	PORTB,7		; Toggle TX LED
	rcall	Delay
	btg	PORTB,7		; Toggle TX LED
	rcall	Delay
	btg	PORTB,7		; Toggle TX LED
	rcall	Delay
	btg	PORTB,7		; Toggle TX LED
	rcall	Delay
	btg	PORTB,7		; Toggle TX LED
	rcall	Delay
	btg	PORTB,7		; Toggle sTX LED
	return

;************************************************************
; sends start bit, slave address
; if ACK not recieved, sends restart, tries again
; execution can get stuck in this loop if slave not present
; can be used to poll slave status (retries until slave responds)

WakeSlave
	bsf	SSPCON2,SEN	; Send start bit
	btfsc	SSPCON2,SEN	; Has SEN cleared yet?
	goto	$-2		; No, loop back to test.

rWakeSlave
	bcf	PIR1,SSPIF	; clear interrupt flag
	nop
	movf	EESLAVE,W
	movwf	SSPBUF		; move slave address to SSPBUF
	btfss	PIR1,SSPIF		; has SSP completed sending SLAVE Address?
	goto	$-2		; no, loop back to test

	btfss	SSPCON2,ACKSTAT	; was ACK received from slave?
	return			; yes, return to calling routine

	bsf	SSPCON2,RSEN	; send repeated start bit
	btfsc	SSPCON2,RSEN	; has repeated start been sent yet?
	goto	$-2		; no, loop back to test

	bra	rWakeSlave	; send slave address again

;************************************************************
; writes EEPROM memory address, hangs if no ACK

WrADDR
	bcf	PIR1,SSPIF	; clear interrupt flag

	movff	EEADDR,SSPBUF	; move EEPROM address to SSPBUF
	btfss	PIR1,SSPIF	; has SSP completed sending EEPROM Address?
	goto	$-2		; no, loop back to test

	btfsc	SSPCON2,ACKSTAT	; has slave sent ACK?
	goto	$-2		; no, try again
	
	return

;************************************************************
; Sends one byte of data to slave, hangs if no ACK

WrDATA
	bcf	PIR1,SSPIF	; clear interrupt flag

	movff	EEDATA,SSPBUF	; move data to SSPBUF
	btfss	PIR1,SSPIF	; has SSP completed sending data to EEPROM?
	goto	$-2		; no, loop back to test

	btfsc	SSPCON2,ACKSTAT	; has slave sent ACK?
	goto	$-2		; no, try again
	
	return

;************************************************************
; receive one byte from slave
; do not send ACK, send stop bit instead

RdDATA
	bcf	PIR1,SSPIF	; clear interrupt flag
	
	bsf	SSPCON2,RCEN	; enable receive mode
	btfss	PIR1,SSPIF	; has SSP received a data byte?
	goto	$-2		; no, loop back to test

	bsf	SSPCON2,ACKDT	; no ACK
	bsf	SSPCON2,ACKEN	; send ACKDT bit
	
	btfsc	SSPCON2,ACKEN	; has ACKDT bit been sent yet?
	goto	$-2		; no, loop back to test
	
	bsf	SSPCON2,PEN	; send stop bit
	btfsc	SSPCON2,PEN	; has stop bit been sent?
	goto	$-2		; no, loop back to test

	movff	SSPBUF,EEDATA	; save data to RAM
	bcf	SSPCON2,RCEN	; disable receive mode

	return

	
;************************************************************
; Sends stop bit, waits until sent

Stop
	bsf	SSPCON2,PEN	; send stop bit
	btfsc	SSPCON2,PEN	; has stop bit been sent?
	goto	$-2		; no, loop back to test

	return
	
;************************************************************
; a delay of 98.57mS

Delay
	movlw	0x80
	movwf	DelayCtr2	; preset
	clrf	DelayCtr1	; clear counter

Delay1
	decfsz	DelayCtr1	; decrement counter
	bra	Delay1		; back to top of loop

	decfsz	DelayCtr2	; decrement counter
	bra	Delay1		; back to top of loop

	return


	END
