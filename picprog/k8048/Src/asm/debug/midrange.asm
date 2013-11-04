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
                LIST    P=PIC16F819
ERRORLEVEL      -302
#INCLUDE        "p16f819.inc"
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
;   k14 program midrange.hex noblank
;
; Verify USERID
; ------------
;   k14 verify midrange.hex 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Constants
;
; __IDLOCS 0x1234
;
ERRORLEVEL      -220
                ORG     0x2000
                DE      0x01    ;[2000] USERID0
                DE      0x02    ;[2001] USERID1
                DE      0x03    ;[2002] USERID2
                DE      0x04    ;[2003] USERID3
ERRORLEVEL      +220
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                END
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
