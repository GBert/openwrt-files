;
; Program USERID only, for row erase tests.
;
; Copyright (c) 2005-2013 Darron Broad
; All rights reserved.
;
; Licensed under the terms of the BSD license, see file LICENSE for details.
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
                LIST    P=PIC16F1825
ERRORLEVEL      -302
#INCLUDE        "p16f1825.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; View USERID
; ----------
;   k14 id
;
; Erase USERID
; -----------
;   k14 erase id
;
; Program USERID
; -------------
;   k14 program enhanced.hex noblank
;
; Verify USERID
; ------------
;   k14 verify enhanced.hex 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS 0x1234
;
ERRORLEVEL      -220
                ORG     0x8000
                DE      0x01    ;[8000] USERID0
                DE      0x02    ;[8001] USERID1
                DE      0x03    ;[8002] USERID2
                DE      0x04    ;[8003] USERID3
ERRORLEVEL      +220
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
