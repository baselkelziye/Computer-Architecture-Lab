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


; converts the lowest nibble 
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


; Public interface function: hexNibbleToAscii
; converts 16bit hex val in reg D to ASCII string and stores 
; the result in RAM at address given in register X
; Paramaters: X, D
; Return: -    
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
    
    ; insert padding to achieve string length 16
    MOVB #$20, 1, X+    
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
    MOVB #$20, 1, X+
        
    MOVB #$0, 1, X+     ; Null-Terminate string
    
    PULY
    PULX
    RTS
          