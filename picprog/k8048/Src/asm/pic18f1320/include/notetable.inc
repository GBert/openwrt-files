NOTESTART   EQU .15
NOTETABLE
;OCTAVE -4
;MIDI= 15(0x0f) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^7 (f=19.445)
    DW  0xfbe0
;MIDI= 16(0x10) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^7 (f=20.602)
    DW  0xede0
;MIDI= 17(0x11) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^7 (f=21.827)
    DW  0xdfe0
;MIDI= 18(0x12) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^7 (f=23.125)
    DW  0xd3e0
;MIDI= 19(0x13) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^7 (f=24.500)
    DW  0xc7e0
;MIDI= 20(0x14) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^7 (f=25.957)
    DW  0xbce0
;MIDI= 21(0x15) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^7 (f=27.500)
    DW  0xb1e0
;MIDI= 22(0x16) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^7 (f=29.135)
    DW  0xa7e0
;MIDI= 23(0x17) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^7 (f=30.868)
    DW  0x9ee0
;OCTAVE -3
;MIDI= 24(0x18) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^7 (f=32.703)
    DW  0x95e0
;MIDI= 25(0x19) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^7 (f=34.648)
    DW  0x8ce0
;MIDI= 26(0x1a) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^7 (f=36.708)
    DW  0x85e0
;MIDI= 27(0x1b) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^6 (f=38.891)
    DW  0xfbc0
;MIDI= 28(0x1c) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^6 (f=41.203)
    DW  0xedc0
;MIDI= 29(0x1d) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^6 (f=43.654)
    DW  0xdfc0
;MIDI= 30(0x1e) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^6 (f=46.249)
    DW  0xd3c0
;MIDI= 31(0x1f) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^6 (f=48.999)
    DW  0xc7c0
;MIDI= 32(0x20) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^6 (f=51.913)
    DW  0xbcc0
;MIDI= 33(0x21) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^6 (f=55.000)
    DW  0xb1c0
;MIDI= 34(0x22) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^6 (f=58.270)
    DW  0xa7c0
;MIDI= 35(0x23) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^6 (f=61.735)
    DW  0x9ec0
;OCTAVE -2
;MIDI= 36(0x24) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^6 (f=65.406)
    DW  0x95c0
;MIDI= 37(0x25) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^6 (f=69.296)
    DW  0x8cc0
;MIDI= 38(0x26) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^6 (f=73.416)
    DW  0x85c0
;MIDI= 39(0x27) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^5 (f=77.782)
    DW  0xfba0
;MIDI= 40(0x28) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^5 (f=82.407)
    DW  0xeda0
;MIDI= 41(0x29) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^5 (f=87.307)
    DW  0xdfa0
;MIDI= 42(0x2a) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^5 (f=92.499)
    DW  0xd3a0
;MIDI= 43(0x2b) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^5 (f=97.999)
    DW  0xc7a0
;MIDI= 44(0x2c) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^5 (f=103.826)
    DW  0xbca0
;MIDI= 45(0x2d) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^5 (f=110.000)
    DW  0xb1a0
;MIDI= 46(0x2e) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^5 (f=116.541)
    DW  0xa7a0
;MIDI= 47(0x2f) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^5 (f=123.471)
    DW  0x9ea0
;OCTAVE -1
;MIDI= 48(0x30) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^5 (f=130.813)
    DW  0x95a0
;MIDI= 49(0x31) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^5 (f=138.591)
    DW  0x8ca0
;MIDI= 50(0x32) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^5 (f=146.832)
    DW  0x85a0
;MIDI= 51(0x33) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^4 (f=155.563)
    DW  0xfb80
;MIDI= 52(0x34) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^4 (f=164.814)
    DW  0xed80
;MIDI= 53(0x35) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^4 (f=174.614)
    DW  0xdf80
;MIDI= 54(0x36) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^4 (f=184.997)
    DW  0xd380
;MIDI= 55(0x37) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^4 (f=195.998)
    DW  0xc780
;MIDI= 56(0x38) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^4 (f=207.652)
    DW  0xbc80
;MIDI= 57(0x39) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^4 (f=220.000)
    DW  0xb180
;MIDI= 58(0x3a) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^4 (f=233.082)
    DW  0xa780
;MIDI= 59(0x3b) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^4 (f=246.942)
    DW  0x9e80
;OCTAVE 0
;MIDI= 60(0x3c) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^4 (f=261.626)
    DW  0x9580
;MIDI= 61(0x3d) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^4 (f=277.183)
    DW  0x8c80
;MIDI= 62(0x3e) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^4 (f=293.665)
    DW  0x8580
;MIDI= 63(0x3f) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^3 (f=311.127)
    DW  0xfb60
;MIDI= 64(0x40) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^3 (f=329.628)
    DW  0xed60
;MIDI= 65(0x41) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^3 (f=349.228)
    DW  0xdf60
;MIDI= 66(0x42) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^3 (f=369.994)
    DW  0xd360
;MIDI= 67(0x43) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^3 (f=391.995)
    DW  0xc760
;MIDI= 68(0x44) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^3 (f=415.305)
    DW  0xbc60
;MIDI= 69(0x45) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^3 (f=440.000)
    DW  0xb160
;MIDI= 70(0x46) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^3 (f=466.164)
    DW  0xa760
;MIDI= 71(0x47) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^3 (f=493.883)
    DW  0x9e60
;OCTAVE 1
;MIDI= 72(0x48) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^3 (f=523.251)
    DW  0x9560
;MIDI= 73(0x49) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^3 (f=554.365)
    DW  0x8c60
;MIDI= 74(0x4a) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^3 (f=587.330)
    DW  0x8560
;MIDI= 75(0x4b) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^2 (f=622.254)
    DW  0xfb40
;MIDI= 76(0x4c) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^2 (f=659.255)
    DW  0xed40
;MIDI= 77(0x4d) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^2 (f=698.456)
    DW  0xdf40
;MIDI= 78(0x4e) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^2 (f=739.989)
    DW  0xd340
;MIDI= 79(0x4f) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^2 (f=783.991)
    DW  0xc740
;MIDI= 80(0x50) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^2 (f=830.609)
    DW  0xbc40
;MIDI= 81(0x51) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^2 (f=880.000)
    DW  0xb140
;MIDI= 82(0x52) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^2 (f=932.328)
    DW  0xa740
;MIDI= 83(0x53) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^2 (f=987.767)
    DW  0x9e40
;OCTAVE 2
;MIDI= 84(0x54) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^2 (f=1046.502)
    DW  0x9540
;MIDI= 85(0x55) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^2 (f=1108.730)
    DW  0x8c40
;MIDI= 86(0x56) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^2 (f=1174.659)
    DW  0x8540
;MIDI= 87(0x57) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^1 (f=1244.508)
    DW  0xfb20
;MIDI= 88(0x58) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^1 (f=1318.510)
    DW  0xed20
;MIDI= 89(0x59) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^1 (f=1396.913)
    DW  0xdf20
;MIDI= 90(0x5a) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^1 (f=1479.978)
    DW  0xd320
;MIDI= 91(0x5b) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^1 (f=1567.982)
    DW  0xc720
;MIDI= 92(0x5c) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^1 (f=1661.219)
    DW  0xbc20
;MIDI= 93(0x5d) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^1 (f=1760.000)
    DW  0xb120
;MIDI= 94(0x5e) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^1 (f=1864.655)
    DW  0xa720
;MIDI= 95(0x5f) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^1 (f=1975.533)
    DW  0x9e20
;OCTAVE 3
;MIDI= 96(0x60) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^1 (f=2093.005)
    DW  0x9520
;MIDI= 97(0x61) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^1 (f=2217.461)
    DW  0x8c20
;MIDI= 98(0x62) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^1 (f=2349.318)
    DW  0x8520
;MIDI= 99(0x63) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0xfb CCPR1L=0x7d POSTSCALER=2^0 (f=2489.016)
    DW  0xfb00
;MIDI=100(0x64) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0xed CCPR1L=0x76 POSTSCALER=2^0 (f=2637.021)
    DW  0xed00
;MIDI=101(0x65) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0xdf CCPR1L=0x6f POSTSCALER=2^0 (f=2793.826)
    DW  0xdf00
;MIDI=102(0x66) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0xd3 CCPR1L=0x69 POSTSCALER=2^0 (f=2959.955)
    DW  0xd300
;MIDI=103(0x67) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0xc7 CCPR1L=0x63 POSTSCALER=2^0 (f=3135.963)
    DW  0xc700
;MIDI=104(0x68) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0xbc CCPR1L=0x5e POSTSCALER=2^0 (f=3322.438)
    DW  0xbc00
;MIDI=105(0x69) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0xb1 CCPR1L=0x58 POSTSCALER=2^0 (f=3520.000)
    DW  0xb100
;MIDI=106(0x6a) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0xa7 CCPR1L=0x53 POSTSCALER=2^0 (f=3729.310)
    DW  0xa700
;MIDI=107(0x6b) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x9e CCPR1L=0x4f POSTSCALER=2^0 (f=3951.066)
    DW  0x9e00
;OCTAVE 4
;MIDI=108(0x6c) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x95 CCPR1L=0x4a POSTSCALER=2^0 (f=4186.009)
    DW  0x9500
;MIDI=109(0x6d) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x8c CCPR1L=0x46 POSTSCALER=2^0 (f=4434.922)
    DW  0x8c00
;MIDI=110(0x6e) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x85 CCPR1L=0x42 POSTSCALER=2^0 (f=4698.636)
    DW  0x8500
;MIDI=111(0x6f) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0x7d CCPR1L=0x3e POSTSCALER=2^0 (f=4978.032)
    DW  0x7d00
;MIDI=112(0x70) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0x76 CCPR1L=0x3b POSTSCALER=2^0 (f=5274.041)
    DW  0x7600
;MIDI=113(0x71) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0x6f CCPR1L=0x37 POSTSCALER=2^0 (f=5587.652)
    DW  0x6f00
;MIDI=114(0x72) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0x69 CCPR1L=0x34 POSTSCALER=2^0 (f=5919.911)
    DW  0x6900
;MIDI=115(0x73) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0x63 CCPR1L=0x31 POSTSCALER=2^0 (f=6271.927)
    DW  0x6300
;MIDI=116(0x74) NOTE=G#,Ab HSPLL=10000000 PRESCALER=16 PR2=0x5e CCPR1L=0x2f POSTSCALER=2^0 (f=6644.875)
    DW  0x5e00
;MIDI=117(0x75) NOTE=A     HSPLL=10000000 PRESCALER=16 PR2=0x58 CCPR1L=0x2c POSTSCALER=2^0 (f=7040.000)
    DW  0x5800
;MIDI=118(0x76) NOTE=A#/Bb HSPLL=10000000 PRESCALER=16 PR2=0x53 CCPR1L=0x29 POSTSCALER=2^0 (f=7458.620)
    DW  0x5300
;MIDI=119(0x77) NOTE=B     HSPLL=10000000 PRESCALER=16 PR2=0x4f CCPR1L=0x27 POSTSCALER=2^0 (f=7902.133)
    DW  0x4f00
;OCTAVE 5
;MIDI=120(0x78) NOTE=C     HSPLL=10000000 PRESCALER=16 PR2=0x4a CCPR1L=0x25 POSTSCALER=2^0 (f=8372.019)
    DW  0x4a00
;MIDI=121(0x79) NOTE=C#/Db HSPLL=10000000 PRESCALER=16 PR2=0x46 CCPR1L=0x23 POSTSCALER=2^0 (f=8869.844)
    DW  0x4600
;MIDI=122(0x7a) NOTE=D     HSPLL=10000000 PRESCALER=16 PR2=0x42 CCPR1L=0x21 POSTSCALER=2^0 (f=9397.272)
    DW  0x4200
;MIDI=123(0x7b) NOTE=D#/Eb HSPLL=10000000 PRESCALER=16 PR2=0x3e CCPR1L=0x1f POSTSCALER=2^0 (f=9956.063)
    DW  0x3e00
;MIDI=124(0x7c) NOTE=E     HSPLL=10000000 PRESCALER=16 PR2=0x3b CCPR1L=0x1d POSTSCALER=2^0 (f=10548.082)
    DW  0x3b00
;MIDI=125(0x7d) NOTE=F     HSPLL=10000000 PRESCALER=16 PR2=0x37 CCPR1L=0x1b POSTSCALER=2^0 (f=11175.304)
    DW  0x3700
;MIDI=126(0x7e) NOTE=F#/Gb HSPLL=10000000 PRESCALER=16 PR2=0x34 CCPR1L=0x1a POSTSCALER=2^0 (f=11839.821)
    DW  0x3400
;MIDI=127(0x7f) NOTE=G     HSPLL=10000000 PRESCALER=16 PR2=0x31 CCPR1L=0x18 POSTSCALER=2^0 (f=12543.854)
    DW  0x3100