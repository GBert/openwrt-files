                LIST
;
; Velleman K8048 12/14/16-bit word architecture library include
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                NOLIST
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
IFNDEF PS2CLK
    CONSTANT PS2CLK  = 0        ;CLOCK PORT BIT
    CONSTANT PS2CLKT = TRISA    ;CLOCK TRIS REGISTER
    CONSTANT PS2CLKP = PORTA    ;CLOCK PORT REGISTER
    CONSTANT PS2DAT  = 1        ;DATA PORT BIT
    CONSTANT PS2DATT = TRISA    ;DATA TRIS REGISTER
    CONSTANT PS2DATP = PORTA    ;DATA PORT REGISTER
ENDIF
;
IFNDEF PS2LOADL
    CONSTANT PS2LOADL = CLOCK / 1000000
ENDIF
IFNDEF PS2LOADH
    CONSTANT PS2LOADH = CLOCK / 125000
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Commands 
;
	CONSTANT PS2_CMD_SETSCALE11 = 0xE6 ;0xFA 1:1 scaling
	CONSTANT PS2_CMD_SETSCALE21 = 0xE7 ;0xFA 2:1 scaling
	CONSTANT PS2_CMD_SETRES     = 0xE8 ;0xFA (0=1, 1=2, 2=4, 3=8 counts/mm)
	CONSTANT PS2_CMD_GETINFO    = 0xE9 ;0xFA + 3 BYTES
	CONSTANT PS2_CMD_SETSTREAM  = 0xEA ;0xFA
	CONSTANT PS2_CMD_POLL       = 0xEB ;0xFA
	CONSTANT PS2_CMD_RESET_WRAP = 0xEC ;0xFA
	CONSTANT PS2_CMD_SETPOLL    = 0xF0 ;0xFA
	CONSTANT PS2_CMD_GETID      = 0xF2 ;0xFA + 2 BYTES
	CONSTANT PS2_CMD_SETRATE    = 0xF3 ;0xFA (10,20,40,60,80,100 or 200 sam/s)
	CONSTANT PS2_CMD_ENABLE     = 0xF4 ;0xFA
	CONSTANT PS2_CMD_DISABLE    = 0xF5 ;0xFA
	CONSTANT PS2_CMD_RESET_DIS  = 0xF6 ;0xFA
	CONSTANT PS2_CMD_RESET_BAT  = 0xFF ;0xFA,0xAA,0x00 (ACK,BAT,ID)
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Return codes
;
	CONSTANT PS2_RET_ID  = 0x00 ;MOUSE IDENT.
	CONSTANT PS2_RET_BAT = 0xAA
	CONSTANT PS2_RET_ACK = 0xFA
	CONSTANT PS2_RET_ERR = 0xFC
	CONSTANT PS2_RET_NAK = 0xFE
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Variables
;
CBLOCK
    COUNTER3    ;COUNTER
ENDC
IFNDEF BUFFER
CBLOCK
    BUFFER      ;DATA
    PARITY      ;PARITY BIT
ENDC
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for PS/S clock low (RESET may take > 500ms)
;
PS2CLKLOW 
                BCF     STATUS,C
                MOVLW   PS2LOADL
                MOVWF   COUNTER1
                CLRF    COUNTER2
                CLRF    COUNTER3
PS2CLKLOWW      BTFSS   PS2CLKP,PS2CLK
                RETLW   0               ;CLOCK LOW
                DJNZ    COUNTER3,PS2CLKLOWW
                DJNZ    COUNTER2,PS2CLKLOWW
                DJNZ    COUNTER1,PS2CLKLOWW
                BSF     STATUS,C
                RETLW   0               ;TIME-OUT WITH CARRY SET
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Wait for PS/S clock high
;
PS2CLKHIGH
                BCF     STATUS,C
                MOVLW   PS2LOADH
                MOVWF   COUNTER1
                CLRF    COUNTER2
PS2CLKHIGHW     BTFSC   PS2CLKP,PS2CLK
                RETLW   0               ;CLOCK HIGH
                DJNZ    COUNTER2,PS2CLKHIGHW
                DJNZ    COUNTER1,PS2CLKHIGHW
                BSF     STATUS,C
                RETLW   0               ;TIME-OUT WITH CARRY SET
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Request to send, send byte, receive ack
;
PS2SEND         MOVWF   BUFFER
                MOVLW   12
                CALL    DELAYT
;REQUEST TO SEND
                TRISPU  PS2DATT,PS2DAT  ;DATA HIGH
                TRISPD  PS2CLKT,PS2CLK  ;CLOCK LOW
                MOVLW   12
                CALL    DELAYT
;START BIT
                TRISPD  PS2DATT,PS2DAT  ;DATA LOW
                MOVLW   4
                CALL    DELAYT
                TRISPU  PS2CLKT,PS2CLK  ;CLOCK HIGH
;DATA BITS
                CLRF    PARITY
                MOVLW   0x08
                MOVWF   COUNTER0
PS2SENDBIT
                CALL    PS2CLKLOW       ;DATA BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                RRF     BUFFER,F
                BTFGS   STATUS,C,PS2SENDHIGH
                TRISPD  PS2DATT,PS2DAT  ;DATA LOW
                GOTO    PS2SENDNEXT
PS2SENDHIGH     TRISPU  PS2DATT,PS2DAT  ;DATA HIGH
                INCF    PARITY,F
PS2SENDNEXT     CALL    PS2CLKHIGH      ;DATA BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
                DJNZ    COUNTER0,PS2SENDBIT
;PARITY BIT
                CALL    PS2CLKLOW       ;PARITY BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                RRF     PARITY,F
                BTFGC   STATUS,C,PS2SENDHIGHP
                TRISPD  PS2DATT,PS2DAT  ;DATA LOW
                GOTO    PS2SENDNEXTP
PS2SENDHIGHP    TRISPU  PS2DATT,PS2DAT  ;DATA HIGH
PS2SENDNEXTP    CALL    PS2CLKHIGH      ;PARITY BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
;STOP BIT
                CALL    PS2CLKLOW       ;STOP BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                TRISPU  PS2DATT,PS2DAT  ;DATA HIGH
                CALL    PS2CLKHIGH      ;STOP BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
;ACKNOWLEDGE
                CALL    PS2CLKLOW       ;ACK BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                CALL    PS2CLKHIGH      ;ACK BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Receive byte
;
PS2RECEIVE      CLRF    BUFFER
;START BIT
                CALL    PS2CLKLOW       ;START BIT CLOCK LOW
                BTFGS   STATUS,C,PS2RECVERROR
                CALL    PS2CLKHIGH      ;START BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2RECVERROR
;DATA BITS
                MOVLW   0x08
                MOVWF   COUNTER0
PS2RECVBIT      CALL    PS2CLKLOW       ;DATA BIT CLOCK LOW
                BTFGS   STATUS,C,PS2RECVERROR
                RRF     BUFFER,F
                MOVLW   0x00
                BTFSC   PS2DATP,PS2DAT  ;TEST BIT
                MOVLW   0x80
                IORWF   BUFFER,F
                CALL    PS2CLKHIGH      ;DATA BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2RECVERROR
                DJNZ    COUNTER0,PS2RECVBIT
;PARITY BIT
                CALL    PS2CLKLOW       ;PARITY BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                CALL    PS2CLKHIGH      ;PARITY BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
;STOP BIT
                CALL    PS2CLKLOW       ;STOP BIT CLOCK LOW
                BTFGS   STATUS,C,PS2SENDERROR
                CALL    PS2CLKHIGH      ;STOP BIT CLOCK HIGH
                BTFGS   STATUS,C,PS2SENDERROR
PS2SENDERROR
PS2RECVERROR    RETLW   0
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; .vimrc:
;  " GPASM Include
;  augroup module
;   autocmd BufRead *.inc set filetype=asm
;  augroup END
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
                LIST
