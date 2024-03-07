/*  Lab 2 - Main C file for Clock program

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann
    Hochschule Esslingen

    Author:  W.Zimmermann, July 19, 2017
*/

/* Edited By Basel and Elias */

#include <hidef.h>                              // Common defines
#include <mc9s12dp256.h>                        // CPU specific defines

#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"     // Mask for the push buttons

#define SEVEN_SEGS_OFF


 
// PLEASE NOTE !!!!!!!!!!!!!!!!!!!!!!!!!d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:
// Files lcd.asm and ticker.asm do contain SOFTWARE BUGS. Please overwrite them
// with the lcd.asm file, which you bug fixed in lab 1, and with file ticker.asm
// which you bug fixed in prep task 2.1 of this lab 2.
//
// To use decToASCII you must insert file decToASCII from the first lab into
// this project
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// ****************************************************************************
// Function prototype(s)
// Note: Only void Fcn(void) assembler functions can be called from C directly.
//       For non-void functions a C wrapper function is required.
void initTicker(void);
void initButton(void);
void print_names_periodically(void);
void display_LED_C(void);
void decToASCII(void);
void clock_pulse(void);
void displayClock(void);

void initThermometer(void);
void updateThermometer(void);


// ****************************************************************************
// Global variables
unsigned char clockEvent = 0;
unsigned int clockMode = 0; // 0 for normal mode, FF for set

unsigned char pollingEvent = 0;


void decToASCII_Wrapper(char *txt, int val)
{   asm
    {  	LDX txt
        LDD val
        JSR decToASCII
    }
}

// Prototypes and wrapper functions for LCD driver (from lab 1)
void initLCD(void);
void writeLine(void);
void add_minute(void);
void add_second(void);
void add_hour(void);
void check_button(void);
void initButton(void);

//clock.c


void WriteLine_Wrapper(char *text, char line)
{   asm
    {
    	  LDX  text
        LDAB line
        JSR  writeLine
    }
}

// ****************************************************************************


void initLED_C(void)
{ 
    DDRJ_DDRJ1  = 1;	  	// Port J.1 as output
    PTIJ_PTIJ1  = 0;		
    DDRB        = 0xFF;		// Port B as output
    PORTB = 0;
        
    // disable 7 segments
    DDRP = 0x0F;
    PTP = 0x0F;
}


// Public interface function: display_LED_C ... Displays
// The value from PORTB to the LED
// Parameter: -
// Return: -
// Registers: Unchanged 
void display_LED_C(void) {  
     
     if(clockMode) {
      // dont't blink, signal set mode
      PORTB = 0x80;
     } else {
      // let led blink
      PORTB = !PORTB;
     }
}



// ****************************************************************************
void main(void) 
{   
    EnableInterrupts;                   // Global interrupt enable

    initLED_C();                    		// Initialize the LEDs
    initButton();
    initLCD();                    		  // Initialize the LCD
    initTicker(); 
    initThermometer();                  // Initialize the Thermometer   
    WriteLine_Wrapper("Basel ITS2022/23", 0);
    for(;;)                                     // Endless loop
    {
      if(pollingEvent) {
        check_button();
        displayClock();
        pollingEvent = 0;
      }
      if (clockEvent) {
        updateThermometer();
        // only pulse the clock when not in set mode
        if(!clockMode) {
    	    clock_pulse();
        } 
   	    clockEvent = 0;                
    	}
    	
    }
}
