  XDEF decToASCII      
       
 .data: SECTION
              
 .const: SECTION
       
       
 .init: SECTION
       
       
       
       ;by basel kelziye
      
            decToASCII:
            
            PSHX
            PSHD
            PSHY
            
            ;determine the sign of number
            CPD #0
            BGE plusSign
            
            PSHD
            LDAB #45
            STAB 0,X
            INX
            PULD
            
           
           
           ;take 2's complement of D register
           EORA #$FF
           EORB #$FF
           ADDD #1  
         
            BRA signExit
            
                     
            
            plusSign:
            PSHD
            LDAB #32
            STAB 0,X
            INX
            PULD
            
            
            signExit:
            
       
       
       
       
            PSHD
            PSHX    ;save values of registers
                    
            LDX #10000
            ;take the remainder of the coefficient
            IDIV
            TFR X,D
            
            PULX  ;retrieve register
            
            
            ADDB #48   ; convert to ascii char " + '0' "
            STAB 0,X  
            INX
            
            PULD
           
       
       
       
       
                
            PSHD
            PSHX         
            LDX #1000
            IDIV
            
            TFR X,D
            LDX #10
            IDIV
          
            PULX
            ADDB #48
            STAB 0,X 
            INX
            PULD
            
            
            PSHD
            PSHX
            LDX #100
            IDIV
            
            
        
            TFR X,D
            LDX #10
            IDIV
        
            PULX
            ADDB #48
            STAB 0,X
            INX
            PULD
            
            
            PSHD
            PSHX
            LDX #10
            IDIV
            
           
            TFR X,D
            LDX #10
            IDIV
           
            PULX
            ADDB #48
            STAB 0,X
            INX
            PULD
            
            
            PSHD
            PSHX
            LDX #10
            IDIV
            PULX 
            ADDB #48
            STAB 0,X
            INX
            PULD
            
            LDAA #0
            STAA 0,X
            
                     
            PULY
            PULD
            PULX
            
            
            RTS