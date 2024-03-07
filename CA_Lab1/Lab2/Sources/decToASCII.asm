; decToASCII.asm
; written by Elias Geiger

; export symbols
        XDEF decToASCII

; Defines

; RAM: Variable data section
.data: SECTION

; ROM: Constant data
.const: SECTION

D2A: DC.B "0123456789"

; ROM: Code section
.init: SECTION

; append dec digit to ascii string in ram 
storeDecInMem:
    PSHY
    LDY #D2A
    MOVB D, Y, 1, X+
    PULY
    RTS
    
; D = D / X (remainder in X and Y)
intDivision:
    IDIV
    TFR D, Y
    
    EXG D, X
    RTS    

; covert 16bit singed int in reg D to string and store at addr in reg X
decToASCII:
    PSHX
    PSHY
    PSHD
    
    CPD #$0
    BLT neg
pos:
    MOVB #$20, 1, X+    ; begin string with space
    BRA begin
neg:
    MOVB #$2D, 1, X+    ; begin string with neg sign
    COMA                ; build 2 complement
    COMB
    ADDD #$1
begin:
    PSHD                ; save the orig value
    PSHX                ; save the string addr ptr
    
    LDY #$2710          ; start with the first dec digit
    JSR intDivision
    PULX                ; restore string addr ptr
    JSR storeDecInMem
    TFR Y, D
    
    PSHX
    
    LDY #$3E8           ; start with the first dec digit
    JSR intDivision
    PULX                ; restore string addr ptr
    JSR storeDecInMem
    PULD                ; restore orig val in reg D
     
    MOVB #$0, 1, X+     ; Null-Terminate string
    
    PULD
    PULY
    PULX
    RTS
          