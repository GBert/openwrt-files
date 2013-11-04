;
; Program IDLOC only, for row erase tests.
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC18F2550
ERRORLEVEL      -302
#INCLUDE        "p18f2550.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; View IDLOC
; ----------
;   k16 mchp id
;
; Erase IDLOC
; -----------
;   k16 mchp row 0x200000
;
; Program IDLOC
; -------------
;   k16 mchp program idloc.hex noblank
;
; Verify IDLOC
; ------------
;   k16 mchp verify idloc.hex 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS _IDLOC0,1
; __IDLOCS _IDLOC1,2
; __IDLOCS _IDLOC2,3
; __IDLOCS _IDLOC3,4
; __IDLOCS _IDLOC4,5
; __IDLOCS _IDLOC5,6
; __IDLOCS _IDLOC6,7
; __IDLOCS _IDLOC7,8
;
ERRORLEVEL      -220 
                ORG     0x200000
                DB      0x11,0x22
                DB      0x33,0x44
                DB      0x55,0x66
                DB      0x77,0x88
ERRORLEVEL      +220 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
