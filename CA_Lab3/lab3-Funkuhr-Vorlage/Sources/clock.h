/*  Header for Clock module

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann Hochschule Esslingen

    Author:   W.Zimmermann, Jun  10, 2016
    Modified: Basel and Elias
*/

#include <mc9s12dp256.h> // CPU specific defines

// For automatic compile time detection of platform (sim or real)
#ifndef _HCS12_SERIALMON
  #define SIMULATOR 1
#endif

// enumerated type for clock events
typedef enum { NOCLOCKEVENT, SECONDTICK } CLOCKEVENT;
typedef enum { NOPOLLINGEVENT, YESPOLLINGEVENT } POLLINGEVENT;

// event flag variables
extern CLOCKEVENT clockEvent;
extern POLLINGEVENT pollingEvent;
extern char isMiddleEuropeanTime;

// function prototyped for ther clock module
void initClock(void);
void processEventsClock(void);
void setClock(char, char, char);
void displayTimeClock(void);
void setHour(char);
