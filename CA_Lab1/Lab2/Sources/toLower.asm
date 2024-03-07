; toLower.asm
; written by Elias Geiger

; export symbols
        XDEF toLower
        XDEF strCpy

; Defines

; RAM: Variable data section
.data: SECTION

; ROM: Constant data
.const: SECTION

; ROM: Code section
.init: SECTION

toLower:
    PSHX
    PSHY
    PSHD
    
    TFR D, X     ; copy string address to reg X
    
loop1:    
    LDAB X       ; load byte from mem into reg B
    
    ORAB #32     ; perform ascii offset conversion on byte
    STAB 1, X+   ; write back converted chars and post inc addr
    
    CMPB #0      ; look for end of string
    BNE loop1    ; stop conversion when end is reached
    
    PULD
    PULY
    PULX
    RTS
          
strCpy: 
    PSHX
    PSHY
    PSHD
     
loop2:    
    LDAB 1, X+    ; read byte from source addr and post inc
    STAB 1, Y+    ; store word to destination addr and post inc
    
    CMPB #0       ; look for end of string
    BNE loop2     ; stop copying
       
    PULD
    PULY
    PULX
    RTS