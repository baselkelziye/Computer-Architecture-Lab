; led.asm
; written by Elias Geiger

; export symbols
    XDEF initLED, setLED, getLED, toggleLED
    
; include 
    INCLUDE 'mc9s12dp256.inc'
    

; ROM: Code section
.init: SECTION  


initLED:  PSHX
          PSHY
          
          BSET DDRJ, #2         ; Bit Set:   Port J.1 as output
          BCLR PTJ,  #2         ; Bit Clear: J.1=0 --> Activate LED

              
          MOVB #$FF, DDRB      ; $FF -> DDRB:  Port B.7...0 as outputs (LEDs)
          MOVB #$FF, PORTB    ; $FF -> PORTB: Turn off all LEDs
           
          PULY
          PULX
          RTS 
          
setLED:   PSHX
          PSHY
              
          STD PORTB       ; write RegB content to LEDs
          ;STAB  PORTB
              
          PULY
          PULX
          RTS

getLED:   PSHX
          PSHY
              
          LDD PORTB       ; read LEDs (PORTB) int regB
          ;LDAB PORTB
             
          PULY
          PULX
          RTS

toggleLED:  PSHX
            PSHY
              
            LDX PORTB   ; load LED state in X reg
            EORB X      ; toggle the LEDs by doing B = B XOR X bitwise
            STD PORTB   ; store back to PORTB
            ;STAB PORTB
              
            PULY
            PULX
            RTS               