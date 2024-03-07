/*  Radio signal clock - C Main Program

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann Hochschule Esslingen

    Author:   W.Zimmermann, Jun  10, 2016
    Modified: Basel and Elias

*/

#include <hidef.h>                              // Common defines
#include <mc9s12dp256.h>                        // CPU specific defines

#include "led.h"
#include "lcd.h"
#include "clock.h"
#include "dcf77.h"
#include "ticker.h"
#include "buttons.h"

#pragma LINK_INFO DERIVATIVE "mc9s12dp256b"

// ****************************************************************************
void main(void)
{   
    EnableInterrupts;                           

    initLED();                                  // Init LEDs at port B
    initLCD();                                  // Init LCD display
    initClock();                                // Init clock module
    initDCF77();                                // Init DCF77 module
    initTicker();                               // Init ticker module
    initButtons();                              // Init buttons at port H

    // Main loop
    for(;;)
    {   
        // First handle clock events
        if (clockEvent != NOCLOCKEVENT) {
            processEventsClock();
            displayTimeClock();
            clockEvent = NOCLOCKEVENT;            // Reset clock event flag             
        }
        
        // Then synchronize to latest time info
        if (dcf77Event != NODCF77EVENT) {   
            processEventsDCF77(dcf77Event);
            displayDateDcf77();
            dcf77Event = NODCF77EVENT;          // Reset dcf77 sync event flag
        }
        
        // handle user inputs over buttons
        if(pollingEvent != NOPOLLINGEVENT) {
            pollButtons();
            convertTime();
            pollingEvent = NOPOLLINGEVENT;       // Reset polling event flag
        }
    }
}



