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
.title      "fifo.s"
.sbttl      "DAC FIFO UPDATE"
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
; _fifo 
;           global      _voice = VOICE[VOICE_SIZE]
;                       _dac   = DAC
;
;           local       A   = VOLUME
;                       B   = MIX
;                       W8  = DAC_FIFO[FIFO_SIZE]
;                       W9  = FIFO_SIZE
;                       W10 = VOICE[]
;                       W11 = VOICE_SIZE
;                       W12 = *VOICE
;                       W0  = WAVEFORM_ACC LSB / *WAVEFORM / function()
;                       W1  = WAVEFORM_ACC MSB
;                       W2  = WAVEFORM_CONTROL LSB / SUM LSB
;                       W3  = WAVEFORM_CONTROL MSB / SUM MSB
;                       W4  = VOLUME
;                       W5  = VOLUME
;
;           return      nothing
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
.global     _fifo
_fifo:
.if 0
            PUSH        _ACCAL
            PUSH        _ACCAH
            PUSH        _ACCAU
            PUSH        _ACCBL
            PUSH        _ACCBH
            PUSH        _ACCBU
.endif
            MOV.D       W8, [W15++] 
            MOV.D       W10, [W15++] 
            MOV.W       W12, [W15++] 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
            MOV         #_voice, W10
            MOV         #VOICE_SIZE, W11
fifo_nextvolume:
            MOV         [W10++], W12

            MOV         _dac + DAC_VOLUME, W4
            MOV         [W12 + VOICE_WAVEFORM + WAVEFORM_VOLUME], W5
            MPY         W4 * W5, A
            SAC.R       A, #0, W4

            MOV         [W12 + VOICE_ENVELOPE + ENVELOPE_VOLUME + 2], W5
            MPY         W4 * W5, A
            SAC.R       A, #0, W4

            MOV         W4, [W12 + VOICE_VOLUME]

            DEC         W11, W11
            BRA         NZ, fifo_nextvolume
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
            MOV         #_dac + DAC_FIFO, W8
            MOV         #FIFO_SIZE, W9
fifo_nextindex:
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
            MOV         #_voice, W10
            MOV         #VOICE_SIZE, W11
            CLR         B
fifo_nextvoice:
            MOV         [W10++], W12

            MOV         [W12 + VOICE_WAVEFORM + WAVEFORM_ACC], W0
            MOV         [W12 + VOICE_WAVEFORM + WAVEFORM_ACC + 2], W1
            MOV         [W12 + VOICE_WAVEFORM + WAVEFORM_CONTROL], W2
            MOV         [W12 + VOICE_WAVEFORM + WAVEFORM_CONTROL + 2], W3
            ADD         W0, W2, W2
            ADDC        W1, W3, W3
            MOV         W2, [W12 + VOICE_WAVEFORM + WAVEFORM_ACC]
            MOV         W3, [W12 + VOICE_WAVEFORM + WAVEFORM_ACC + 2]

            ADD         W12, #VOICE_WAVEFORM, W0
            RCALL       _function

            MOV         W0, W4
            MOV         [W12 + VOICE_VOLUME], W5
            MPY         W4 * W5, A

            ADD         B

            DEC         W11, W11
            BRA         NZ, fifo_nextvoice
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
            SAC.R       B, #0, [W8++]

            DEC         W9, W9
            BRA         NZ, fifo_nextindex
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
            MOV.W       [--W15], W12
            MOV.D       [--W15], W10
            MOV.D       [--W15], W8
.if 0
            POP         _ACCBU
            POP         _ACCBH
            POP         _ACCBL
            POP         _ACCAU
            POP         _ACCAH
            POP         _ACCAL
.endif
            RETURN
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; vim: ft=asm shiftwidth=12 tabstop=12 softtabstop=12 expandtab
;
