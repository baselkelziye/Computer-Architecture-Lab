; Project: Lab1
; main.asm
; written by Elias Geiger

; export symbols
        XDEF Entry, main

; import symbols
        XREF __SEG_END_SSTACK           ; End of stack
        XREF delay_0_5sec
        XREF initLED, setLED, getLED, toggleLED

; include derivative specific macros
        INCLUDE 'mc9s12dp256.inc'

IMAX: EQU 2048                          ; Symbolic constant

; RAM: Variable data section
.data: SECTION                          ; (Not used here, count variable i is put in register)
ctr:    ds.w 1

; ROM: Constant data
.const:SECTION                          ; (Not used here)

; ROM: Code section
.init: SECTION  

main:                                   ; Begin of the program
Entry:                  
        LDS  #__SEG_END_SSTACK          ; Initialize stack pointer
        CLI                             ; Enable interrupts, needed for debugger
        
        JSR initLED

loop:   LDD #0                          ; init D Reg with zeros
        JSR setLED
        
L1:     ADDD #2                         ; increment D by 2
        JSR setLED
        JSR delay_0_5sec
        
        CPD #63                         ; compare D with 63
        BLO L1                          ; continue incrementing when smaller 
        
        BRA loop                        ; start over at value 0
        


