; hexToASCII.asm
; written by Elias Geiger

; export symbols
        XDEF hexToASCII

; Defines

; RAM: Variable data section
.data: SECTION

; ROM: Constant data
.const: SECTION

H2A: DC.B "0123456789ABCDEF"

; ROM: Code section
.init: SECTION

; convert lowest hex nibble in D to ASCII val
hexNibbleToAscii:
    PSHY
    ANDB #$0F    ; delete higher nibble in reg B
    ANDA #$00    ; mask reg A
    LDY #H2A     ; load start addr of lookup string
    MOVB D, Y, 1, X+      ; 
    
    PULY
    RTS
    

; right shift D reg Y times
shiftRight:
    LSRD
    DEY
    CPY #0
    BNE shiftRight
    RTS     

; covert 16bit hex val in reg D to string and store at addr in reg X
hexToASCII:
    PSHX
    PSHY
    
    MOVB #$30, 1, X+    ; first character '0'
    MOVB #$78, 1, X+    ; second character 'x'
    
    PSHD
    LDY #12
    JSR shiftRight
    JSR hexNibbleToAscii
    PULD
    
    PSHD
    LDY #8
    JSR shiftRight
    JSR hexNibbleToAscii
    PULD
    
    PSHD
    LDY #4
    JSR shiftRight
    JSR hexNibbleToAscii
    PULD
    
    PSHD
    JSR hexNibbleToAscii
    PULD
        
    MOVB #$0, 1, X+     ; Null-Terminate string
    
    PULY
    PULX
    RTS
          