; adc.asm
; written by Elias Geiger

; Inludes
    INCLUDE 'mc9s12dp256.inc'

; export symbols
    XDEF initADC, convertADC
    
; import symbols
    XREF temperature       

; RAM section
.data: SECTION
value       DS.W 1

; Interrupt vectors
.vect: SECTION
        ORG $FFD2
int22:  DC.W adcISR    

; ROM Code section
.init: SECTION

; ADC initialization routine called once at the beginning
; Parameter: - 
; Return:    - 
; Registers  unchanged 
initADC:
    MOVB #%11000010, ATD0CTL2     ; Enable ADC with interrupt
    MOVB #%00100000, ATD0CTL3     ; 4 measurements (floating average)
    MOVB #%00000101, ATD0CTL4     ; Resolution 10bit, Freq. 2MHz, ATD0
    MOVB #%10000111, ATD0CTL5     ; Begin to read 
    RTS
    
; ADC value conversion routine called periodically
; Parameter: - 
; Return:    
; Registers: unchanged     
convertADC:
    LDD   value
    
    LDY #100
    EMUL            ; multiply with 100
    LDX #1023
    EDIV            ; divide by 1023
    TFR Y, D
    SUBD  #30       ; convert to celsius scale
    
    STD temperature
    RTS 
    
; Interrupt service routine invoked after every ADC reading
; Parameter: - 
; Return:    - 
; Registers  unchanged 
adcISR:
    LDD   ATD0DR0       ; read in register 
    ADDD  ATD0DR1       ; add up the 4 measurements
    ADDD  ATD0DR2   
    ADDD  ATD0DR3
    
    LSRD  ; divide by 4 --> 2x logical shift right
    LSRD  ; " "
    
    ; LDD #200     ; fixed value for testing in simulator
    
    STD   value
    BSET  ATD0CTL2, #$01              ; reset interrupt triggered flag
    MOVB  #%10000111, ATD0CTL5        ; begin next reading
    
    RTI
  