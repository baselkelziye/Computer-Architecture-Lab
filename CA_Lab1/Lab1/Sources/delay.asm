; delay.asm

; export symbols
    XDEF delay_0_5sec

; ROM: Code section
.init: SECTION  

delay_0_5sec: PSHX
              PSHY
              
              LDX #$1E                  ; 30 down to 0
L3:           LDY #$FFFF                ; 65535 down to 0 
                                                           
L2:           DEY
              CPY #$0
              ; BGT L2
              BNE L2
        
              DEX 
              CPX #$0
              ; BGT L3
              BNE L3
              
              PULY
              PULX
              RTS        