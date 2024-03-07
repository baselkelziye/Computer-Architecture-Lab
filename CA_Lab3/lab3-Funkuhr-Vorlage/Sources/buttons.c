/*
  file: buttons.c
  written by Basel and Elias
*/

#include <mc9s12dp256.h>                       
#include "clock.h"

void initButtons() {
   DDRH = 0x00;         // Port H as inputs
}

void pollButtons() {
    #ifdef SIMULATOR
      if(PTH_PTH3) {
        isMiddleEuropeanTime = !isMiddleEuropeanTime;
      }
    #else
      if(!PTH_PTH3) {
        isMiddleEuropeanTime = !isMiddleEuropeanTime;
      }
    #endif  
}