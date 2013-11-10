            .TITLE " ICSP Hex Generator"
            .SBTTL " Enter ICSP code here and assemble for hex"
;
;******************************************************************************
;
            .NOLIST
            .INCLUDE "p24Fxxxx.inc"
            .LIST

CONFIG __CONFIG2, POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & SOSCSEL_SOSC & WUTSEL_FST & IESO_OFF
CONFIG __CONFIG1, WDTPS_PS1024 & FWPSA_PR128 & WINDIS_ON & FWDTEN_OFF & ICS_PGx3 & COE_OFF & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF
;
;******************************************************************************
;
VISI        =       0x784
;
;******************************************************************************
            .BSS                        ;VAR

a:          .SPACE  1
b:          .SPACE  1
c:          .SPACE  1

            .TEXT                       ;CODE

            .GLOBAL __reset

            .SECTION *, CODE, ADDRESS(0x200)
__reset:
            MOV     #__SP_init, W15
            MOV     #__SPLIM_init, W0
            MOV     W0, _SPLIM
            NOP    
            BCLR    _CORCON, #PSV

            ;SET UP US THE BOMB
            MOV     #0x404F, W10
            MOV     W10, NVMCON
 
            MOV     #0x0000, W0     ;CODE
            MOV     W0, TBLPAG
            MOV     #0x0080, W0     ;CHIP?
            MOV     W0, TBLPAG

            MOV     #0x0000, W0
            TBLWTL  W0, [W0]        ;?
            NOP
            NOP

            ;HA HA HA!
            MOV     _PCL, W0
            MOV     _PCH, W0
            MOV     #0xAAAA,W0
            MOV     W0, [W7]
            MOV     W0, VISI

            ;ALL YOUR BASE...

            ;0xFF0000 23:16
            MOV     #0xFF, W0
            MOV     W0, TBLPAG
        
            ;0xFF0000 15:0
            MOV     #0x0000, W6

            MOV     #VISI, W7

            CLR     [W7]

            TBLRDH [W6], [W7]
            TBLRDH [W6++], [W7]

            TBLRDL  [W6], [W7]
            TBLRDL  [W6++], [W7]

            TBLRDH.B [W6], [W7]
            TBLRDH.B [W6++], [W7++]
            TBLRDH.B [++W6], [W7--]

            ;ARE BELONG TO US!
            RESET
;
;******************************************************************************
            .END
;******************************************************************************
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
