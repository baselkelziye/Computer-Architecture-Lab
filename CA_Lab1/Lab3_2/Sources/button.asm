; button.asm
; written by Basel and Elias Geiger

; export symbols
    XDEF updateCnt, initButton
    
; include 
    INCLUDE 'mc9s12dp256.inc'
    

; ROM: Code section
.init: SECTION  

; Public interface function: initButton ... initialize Buttons
; Paramaters: -
; Return:    -    
initButton:  
    MOVB #$00, DDRH     ; Buttons connected at Port H
    RTS


; Public interface function: updateCnt ... changes counter value based on button states
; Paramaters: Y
; Return:    Y   
updateCnt:
    EXG D, Y   ; first swap counter over to reg D
    ; check all button states and react  
    ; for the simulator
    ;BRSET PTH, #$01, btn0pressed
    ;BRSET PTH, #$02, btn1pressed
    ;BRSET PTH, #$04, btn2pressed
    ;BRSET PTH, #$08, btn3pressed
    ; for real hardware
    BRCLR PTH, #$01, btn0pressed
    BRCLR PTH, #$02, btn1pressed
    BRCLR PTH, #$04, btn2pressed
    BRCLR PTH, #$08, btn3pressed
    
    ADDD #$0001     ; when no  button is pressed increment by 1
    
done:
    EXG Y, D   ; at last swap counter back to reg Y   
    RTS

; event specific handler routines    
btn0pressed:
    ADDD #$0010   ; increment by 16
    BRA done

btn1pressed:
    ADDD #$000A   ; increment by 10
    BRA done

btn2pressed:
    SUBD #$0010   ; decrement by 16
    BRA done        

btn3pressed:
    SUBD #$000A   ; decrement by 10 
    BRA done
         
              