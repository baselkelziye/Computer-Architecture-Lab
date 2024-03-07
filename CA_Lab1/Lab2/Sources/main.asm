;   Labor 1 - Vorbereitungsaufgabe 2.4
;   Convert a zero-terminated ASCIIZ string to lower characters
;   Main program
;
;   Computerarchitektur
;   (C) 2019-2022 J. Friedrich, W. Zimmermann, R. Keller
;   Hochschule Esslingen
;
;   Author:   R. Keller, Jul 4, 2019
;            (based on code provided by J. Friedrich, W. Zimmermann)
;   Modified: -
;

; export symbols
        XDEF Entry, main

; import symbols
        XREF __SEG_END_SSTACK           ; End of stack
        XREF toLower                    ; Referenced from other object file
        XREF strCpy                     ; Referenced from other object file
        XREF hexToASCII                 ; Referenced from other object file
        XREF decToASCII

; include derivative specific macros
        INCLUDE 'mc9s12dp256.inc'

; Defines

; RAM: Variable data section
.data:  SECTION
Vtext:  DS.B    7                      ; Please store String here

; ROM: Constant data
.const: SECTION
Ctext:  DC.B  "Test 12345 *!? ABCDE abcde zZ", 0

; ROM: Code section
.init:  SECTION

main:                                   ; Begin of the program
Entry:
        LDS  #__SEG_END_SSTACK          ; Initialize stack pointer
        CLI                             ; Enable interrupts, needed for debugger
        
        ; Task 2.1
        ;LDX #Ctext
        ;LDY #Vtext
        ;JSR strCpy
        ;LDD #Vtext
        ;JSR toLower
        
        LDX #Vtext         ; Load string pointer in reg X
        
        ; Task 2.2
        LDD #$F018        ; Copy a 16 bit hex value in reg D
        JSR hexToASCII    ; call subroutine for conversion to ASCII
        
        ; Task 2.3
        ;LDD #$0761         ; load a decimal value in reg D
        ;JSR decToASCII     ; call subroutine for conversion to ASCII

endLoop:    
        BRA endLoop