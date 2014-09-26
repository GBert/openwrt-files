                NOLIST
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
; Constant definitions
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
IF ARCH == MIDRANGE || ARCH == ENHANCED
    CONSTANT BANK0  = 0x000 ;BANK SELECTION
    CONSTANT BANK1  = 0x080
    CONSTANT BANK2  = 0x100
    CONSTANT BANK3  = 0x180
ENDIF
IF ARCH == ENHANCED
    CONSTANT BANK4  = 0x200
    CONSTANT BANK5  = 0x280
    CONSTANT BANK6  = 0x300
    CONSTANT BANK7  = 0x380
    CONSTANT BANK8  = 0x400
    CONSTANT BANK9  = 0x480
    CONSTANT BANK10 = 0x500
    CONSTANT BANK11 = 0x580
    CONSTANT BANK12 = 0x600
    CONSTANT BANK13 = 0x680
    CONSTANT BANK14 = 0x700
    CONSTANT BANK15 = 0x780
    CONSTANT BANK16 = 0x800
    CONSTANT BANK17 = 0x880
    CONSTANT BANK18 = 0x900
    CONSTANT BANK19 = 0x980
    CONSTANT BANK20 = 0xa00
    CONSTANT BANK21 = 0xa80
    CONSTANT BANK22 = 0xb00
    CONSTANT BANK23 = 0xb80
    CONSTANT BANK24 = 0xc00
    CONSTANT BANK25 = 0xc80
    CONSTANT BANK26 = 0xd00
    CONSTANT BANK27 = 0xd80
    CONSTANT BANK28 = 0xe00
    CONSTANT BANK29 = 0xe80
    CONSTANT BANK30 = 0xf00
    CONSTANT BANK31 = 0xf80
ENDIF
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
    CONSTANT  NUL = 0x00 ;'\0'
    CONSTANT  SOH = 0x01 ;(start of heading)
    CONSTANT  STX = 0x02 ;(start of text)
    CONSTANT  ETX = 0x03 ;(end of text)
    CONSTANT  EOT = 0x04 ;(end of transmission)
    CONSTANT  ENQ = 0x05 ;(enquiry)
    CONSTANT  ACK = 0x06 ;(acknowledge)
    CONSTANT  BEL = 0x07 ;'\a' (bell)
    CONSTANT  BS  = 0x08 ;'\b' (backspace)
    CONSTANT  HT  = 0x09 ;'\t' (horizontal tab)
    CONSTANT  LF  = 0x0A ;'\n' (new line)
    CONSTANT  VT  = 0x0B ;'\v' (vertical tab)
    CONSTANT  FF  = 0x0C ;'\f' (form feed)
    CONSTANT  CR  = 0x0D ;'\r' (carriage ret)
    CONSTANT  SO  = 0x0E ;(shift out)
    CONSTANT  SI  = 0x0F ;(shift in)
    CONSTANT  DLE = 0x10 ;(data link escape)
    CONSTANT _DC1 = 0x11 ;(device control 1)
    CONSTANT  DC2 = 0x12 ;(device control 2)
    CONSTANT  DC3 = 0x13 ;(device control 3)
    CONSTANT  DC4 = 0x14 ;(device control 4)
    CONSTANT  NAK = 0x15 ;(negative ack.)
    CONSTANT  SYN = 0x16 ;(synchronous idle)
    CONSTANT  ETB = 0x17 ;(end of trans. blk)
    CONSTANT  CAN = 0x18 ;(cancel)
    CONSTANT  EM  = 0x19 ;(end of medium)
    CONSTANT  SUB = 0x1A ;(substitute)
    CONSTANT  ESC = 0x1B ;(escape)
    CONSTANT  FS  = 0x1C ;(file separator)
    CONSTANT  GS  = 0x1D ;(group separator)
    CONSTANT  RS  = 0x1E ;(record separator)
    CONSTANT  US  = 0x1F ;(unit separator)
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
