; led.asm
; written by Elias Geiger

; export symbols
    XDEF initLED, setLED, getLED, toggleLED
    
; include 
    INCLUDE 'mc9s12dp256.inc'
    

; ROM: Code section
.init: SECTION  

; Public interface function: initLED ... initialize LEDs 
; Parameter: -
; Return:    -
initLED:  PSHX
          PSHY
          
          BSET DDRJ, #2         ; Bit Set:   Port J.1 as output
          BCLR PTJ,  #2         ; Bit Clear: J.1=0 --> Activate LED

              
          MOVB #$FF, DDRB      ; $FF -> DDRB:  Port B.7...0 as outputs (LEDs)
          MOVB #$FF, PORTB    ; $FF -> PORTB: Turn off all LEDs
           
          PULY
          PULX
          RTS 

; Public interface function: setLED ... loads B in PORTB to control LEDs
; Parameter: B
; Return:    -          
setLED:   PSHX
          PSHY
                   
          STAB  PORTB       ; write RegB content to LEDs
              
          PULY
          PULX
          RTS

; Public interface function: getLED ... returns State of LEDs into B
; Parameter: -
; Return:    B
getLED:   PSHX
          PSHY
                 
          LDAB PORTB     ; read LEDs (PORTB) int regB
             
          PULY
          PULX
          RTS

; Public interface function: toggleLED ... toggles all LEDs
; Parameter: -
; Return:    -
toggleLED:  PSHX
            PSHY
              
            LDX PORTB   ; load LED state in X reg
            EORB X      ; toggle the LEDs by doing B = B XOR X bitwise  
            STAB PORTB  ; store back to PORTB
              
            PULY
            PULX
            RTS               