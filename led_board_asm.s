.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
#pragma config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
#pragma config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

    .bss        ; put the following labels in RAM
counter:
    .space 2    ; a variable that takes two bytes (we won?t use
                ; it for now, but put here to make this a generic
                ; template to be used later).
stack:
    .space  32  ; this will be our stack area, needed for func calls

.text           ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _write_0, _write_1, _reset_50
    
_write_0: ;2 cycles to call
    inc LATA ;sets RA0 pin to high, 1 cycle
    ;need 6 cycles for high
    repeat #3 ;1 cycle + 4 cycles of nop
    nop
    clr LATA ;sets RA0 pin to low, 1 cycle
    ;need 13 cycles of low
    repeat #5 ;1 cycle + 6 cycles of nop
    nop
    return ;3 cycles
    
_write_1: ;2 cycles to call
    inc LATA ;sets RA0 pin to high, 1 cycle
    ;need 11 cycles for high
    repeat #8 ;1 cycle + 9 cycles of nop
    nop
    clr LATA ;sets RA0 pin to low, 1 cycle
    ;need 10 cycles for low
    repeat #2 ;1 cycle + 3 cycles of nop
    nop
    return ;3 cycles

_reset_50: ;2 cycles to call
    repeat #953 ;1 cycle + 954 cycles of nop
    nop
    return ;3 cycles
    
