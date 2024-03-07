/*  Radio signal clock - Free running clock

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann Hochschule Esslingen

    Author:   W.Zimmermann, Jun  10, 2016
    Modified: Basel and Elias
*/

#include <stdio.h>

#include "clock.h"
#include "lcd.h"
#include "led.h"
#include "dcf77.h"

// Defines
#define ONESEC  (1000/10)                       // 10ms ticks per second
#define MSEC200 (200/10)

// Event flag initializations
CLOCKEVENT clockEvent = NOCLOCKEVENT;
POLLINGEVENT pollingEvent = NOPOLLINGEVENT;

// clock related time variables
static char hrs = 0, mins = 0, secs = 0;
static int ticks = 0;
static int uptime = 0;
char isMiddleEuropeanTime = 1;

// ****************************************************************************
//  Initialize clock module
//  Called once before using the module
void initClock(void)
{   
    displayTimeClock();
}

// ****************************************************************************
// This function is called periodically every 10ms by the ticker interrupt (callback)
// Keep processing short in this function, run time must not exceed 10ms!
void tick10ms(void)
{   
    if(++ticks >= ONESEC)
    {   
        // every second, trigger the clockEvent
        ticks = 0;
        clockEvent = SECONDTICK;                
        setLED(0x01);                           // turn on LED at B.0
    } 
    else if(ticks == MSEC200) 
    {   
        clrLED(0x01);                           // after 200ms turn B.0 off
    }
    
    // every 200ms, trigger the polling event 
    if(ticks % MSEC200 == 0) {                  
      pollingEvent = YESPOLLINGEVENT;           
    }
      
    uptime += 10;   // increase uptime by 10ms
    dcf77Event = sampleSignalDCF77(uptime);    // Sample the DCF77 signal
}

// ****************************************************************************
// Process the clock events
// This function must be called every second to update the clock correctly
// Parameter:   -
// Returns:     -
void processEventsClock(void)
{   
    secs++;
    if(secs >= 60)
    {   
        secs = 0;
        mins++;
        if(mins >= 60)
        {
            mins = 0;
            hrs++;
            if(hrs >= 24) { hrs = 0; }
        }
    }
}

// ****************************************************************************
// Allow other modules to set the hours
// Parameters:  hours as integers
// Returns:     -
void setHour(char hours)
{
    hrs = hours;
}

// ****************************************************************************
// Allow other modules, e.g. DCF77, so set the time
// Parameters:  hours, minutes, seconds as integers
// Returns:     -
void setClock(char hours, char minutes, char seconds)
{  
    ticks = 0; 
    hrs  = hours;
    mins = minutes;
    secs = seconds;
}

// ****************************************************************************
// Display the time derived from the clock module on the LCD display, line 0
// Parameter:   -
// Returns:     -
void displayTimeClock(void)
{   
    char uhrzeit[32] = "00:00:00";
    sprintf(uhrzeit, "%s %02d:%02d:%02d", isMiddleEuropeanTime ? "DE" : "US", hrs, mins, secs);
    writeLine(uhrzeit, 0);    // write time in the first line
}