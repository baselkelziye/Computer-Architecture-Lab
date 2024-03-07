// buttons.c 
// written by Elias Geiger

// includes
#include <mc9s12dp256.h>       // CPU specific defines

// Variables
extern int clockMode;   // read clockMode from main.c

// prototypes
void add_hour(void);
void add_minute(void);
void add_second(void);

// PTH_PTH0  --> im sim = 1 und Hardware = 0
#ifndef _HCS12_SERIALMON
  #define PRESSED 1  // --> Software
#else
  #define PRESSED 0  // --> Hardware
#endif

// public interface function: called once to setup buttons
// Parameter: - 
// Return:    - 
// Registers  unchanged 
void initButton() {
   DDRH = 0x00;               // Port H as inputs
}

// public interface function: called periodically to handle button input
// Parameter: - 
// Return:    - 
// Registers  unchanged 
void check_button() {
  if(PTH_PTH0 == PRESSED) {
     // toggle clock mode
     clockMode = !clockMode;
  } else if(clockMode) {
    if(PTH_PTH1 == PRESSED) {
      // add seconds
      add_second();
    } else if(PTH_PTH2 == PRESSED) {
      // add minutes    
      add_minute();
    } else if(PTH_PTH3 == PRESSED) {
      // add hours 
      add_hour();   
    }
  }
}
