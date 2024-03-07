;
;   Labor 1 - Test program for LCD driver
;
;   Computerarchitektur
;   (C) 2019-2022 J. Friedrich, W. Zimmermann, R. Keller
;   Hochschule Esslingen
;
;   Author:   	   J.Friedrich, W. Zimmermann
;   Last Modified: R. Keller, August 2022

; Edited By Basel and Elias


; Export symbols
        XDEF Entry, main

; Import symbols
        XREF __SEG_END_SSTACK                   ; End of stack
        XREF initLCD, writeLine, delay_10ms     ; LCD functions
        XREF initLED, setLED, getLED, toggleLED ; LED functions
        XREF delay_0_5sec                       ; delay function
        XREF hexToASCII, decToASCII             ; conversion functions
        XREF initButton, updateCnt              ; button functions

; Include derivative specific macros
        INCLUDE 'mc9s12dp256.inc'

; Defines

; RAM: Variable data section
.data:  SECTION
strDecVal: ds.b 17   ; LCD display strings
strHexVal: ds.b 17

; ROM: Constant data
.const: SECTION

; ROM: Code section
.init:  SECTION

main:
Entry:
        LDS  #__SEG_END_SSTACK          ; Initialize stack pointer
        CLI                             ; Enable interrupts, needed for debugger

        JSR  delay_10ms                 ; Delay 20ms during power up
        JSR  delay_10ms                 ; by Jump-Subroutine (use step-over)

        JSR initLCD                     ; Initialize the LCD
        JSR initLED                     ; Initialize the LEDs
        JSR initButton                  ; Initialize the Buttons
        
        LDY #$0                         ; init counter register with zero
loop1:
        ; output lower byte from counter to LEDs
        TFR Y, D
        JSR setLED
         
        ; dec to ASCII on LCD  
        LDX  #strDecVal                                       
        JSR decToASCII                  ; perform conversion, store in string
        LDAB #0                         ; display decimal value in line 0
        JSR  writeLine
        
        ; hex to ASCII on LCD
        LDX #strHexVal
        TFR Y, D
        JSR hexToASCII            ; perform conversion, store in string
        LDAB #1                   ; display hexadecimal value in line 1      
        LDX #strHexVal                         
        JSR writeLine
   
        JSR delay_0_5sec          ; wait for 500ms
        JSR updateCnt             ; in - or decrement counter based on pressed buttons
        BRA loop1                 ; next loop iteration
        

back:   BRA back

