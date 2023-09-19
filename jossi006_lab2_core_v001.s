.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

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
.global _main               ;BP
;your functions go here

_main:

    bclr    CLKDIV,#8                 ;BP
    nop
    ;; --- Begin your program below here ---
    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0001,w0  
    mov     w0,LATA            ; set pin RA0 high
    call bitCycle

bitCycle:
    inc LATA ;sets RA0 to high, 1 cycle
    repeat #22 ;1 cycle + 22 cycles for nop
    nop
    clr LATA ; sets RA0 to low, 1 cycle
    repeat #28 ;1 cycle to call, 28 cycles of nop
    nop
    bra bitCycle ; 2 cycles
    
;100us is 1600 cycles
delay100us: ;call takes 2 cycles
    repeat #1593 ;1 cycle + 1594 cycles of nop
    nop
    return ;3 cycles to execute
    
;1ms is 16000 cycles
delay1ms: ;call takes 2 cycles
    repeat #15993 ;1 cycle + 15994 cycles of nop
    nop
    return ;3 cycles to execute
    