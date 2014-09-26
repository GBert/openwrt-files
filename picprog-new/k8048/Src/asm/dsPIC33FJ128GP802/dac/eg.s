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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.title      "eg.s"
.sbttl      "ENVELOPE GENERATOR"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.nolist
.include    "p33Fxxxx.inc"
.list
.include    "symbol.inc"
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.text
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; _eg
;           global      _voice = VOICE[VOICE_SIZE]
;
;           local       W5 = VOICE[]
;                       W6 = VOICE_SIZE
;                       W0 = *VOICE
;                       W1 = ENVELOPE_STATE / ENVELOPE_VOLUME LSB
;                       W2 = ENVELOPE_VOLUME MSB
;                       W3 = ENVELOPE_[A|D|R] LSB / SUM LSB
;                       W4 = ENVELOPE_[A|D|R] MSB / SUM MSB
;           
;           return      nothing
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.global     _eg
_eg:
            MOV         #_voice, W5
            MOV         #VOICE_SIZE, W6
eg_loop:
            MOV         [W5++], W0
            RCALL       eg_voice
            DEC         W6, W6
            BRA         NZ, eg_loop
            RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
eg_voice:
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_STATE], W1
            CP          W1, #STATE_ATTACK
            BRA         Z, eg_attack
            CP          W1, #STATE_DECAY
            BRA         Z, eg_decay
            CP          W1, #STATE_RELEASE
            BRA         Z, eg_release
            RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
eg_attack:
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME], W1
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2], W2

            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_A], W3
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_A + 2], W4

            ADD         W1, W3, W3
            ADDC        W2, W4, W4
            BRA         N, eg_attackover

            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME]
            MOV         W4, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2]
            RETURN

eg_attackover:
            MOV         #STATE_DECAY, W3
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_STATE]
            BRA         eg_decaystart
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
eg_decay:
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME], W1
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2], W2
eg_decaystart:
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_D], W3
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_D + 2], W4

            ADD         W1, W3, W3
            ADDC        W2, W4, W4
            BRA         N, eg_releaseover

            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_S], W1
            CP          W4, W1
            BRA         N, eg_decayover

            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME]
            MOV         W4, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2]
            RETURN

eg_decayover:
            MOV         #STATE_SUSTAIN, W3
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_STATE]

            CLR         W3
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME]
            MOV         W1, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2]
            RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
eg_release:
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME], W1
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2], W2

            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_R], W3
            MOV         [W0 + VOICE_ENVELOPE + ENVELOPE_R + 2], W4

            ADD         W1, W3, W3
            ADDC        W2, W4, W4
            BRA         N, eg_releaseover

            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME]
            MOV         W4, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2]
            RETURN

eg_releaseover:
            MOV         #STATE_NONE, W3
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_STATE]

            CLR         W3
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME]
            MOV         W3, [W0 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2]
            RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: ft=asm shiftwidth=12 tabstop=12 softtabstop=12 expandtab
;
