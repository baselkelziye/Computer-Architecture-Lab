/*  Radio signal clock - DCF77 Module

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann Hochschule Esslingen

    Author:   W.Zimmermann, Jun  10, 2016
    Modified: Basel and Elias
*/

#include <hidef.h>       // Common defines
#include <mc9s12dp256.h> // CPU specific defines
#include <stdio.h>

#include "dcf77.h"
#include "led.h"
#include "clock.h"
#include "lcd.h"

// init event flag
DCF77EVENT dcf77Event = NODCF77EVENT;

// module internal variables
static int dcf77Year = 2000, dcf77Month = 0, dcf77Day = 0, dcf77Hour = 0, dcf77Minute = 0, dcf77DayOfWeek = 1; // dcf77 Date and time as integer values
static int convertedYear = 2000, convertedMonth = 0, convertedDay = 0, convertedHour = 0, convertedDayOfWeek = 1;

static char signal;
static char prevSignal;

static unsigned int fallTime = 0;
static unsigned int riseTime = 0;
static unsigned int timeLow;
static unsigned int timePulse;

static long dataLSB = 0x00;
static long dataMSB = 0x00;
static char validSecondsCtr;

static char parity1, parity2, parity3;

static unsigned char i;

// Prototypes of functions simulation DCF77 signals, when testing without
// a DCF77 radio signal receiver
void initializePortSim(void); // Use instead of initializePort() for testing
char readPortSim(void);       // Use instead of readPort() for testing

void translateData(void);
char dataInvalid(void);
char parityInvalid(void);

// ****************************************************************************
// Initalize the hardware port on which the DCF77 signal is connected as input
// Parameter:   -
// Returns:     -
void initializePort(void)
{
}

// ****************************************************************************
// Read the hardware port on which the DCF77 signal is connected as input
// Parameter:   -
// Returns:     0 if signal is Low, >0 if signal is High
char readPort(void) 
{
    if(PTH & 0x01) {
      return 1;
    } else {
      return 0; 
    }
}

// ****************************************************************************
//  Initialize DCF77 module
//  Called once before using the module
void initDCF77(void)
{
    setClock((char)dcf77Hour, (char)dcf77Minute, 0);
    displayDateDcf77();

#ifdef SIMULATOR
    initializePortSim();
#else
    initializePort();
#endif
}

// ****************************************************************************
// Display the date derived from the DCF77 signal on the LCD display, line 1
// Parameter:   -
// Returns:     -
void displayDateDcf77(void)
{
    char dateStr[32];
    char weekDayStr[3];

    switch (convertedDayOfWeek)
    {
    case 1:
        sprintf(weekDayStr, "Mon");
        break;
    case 2:
        sprintf(weekDayStr, "Tue");
        break;
    case 3:
        sprintf(weekDayStr, "Wed");
        break;
    case 4:
        sprintf(weekDayStr, "Thu");
        break;
    case 5:
        sprintf(weekDayStr, "Fri");
        break;
    case 6:
        sprintf(weekDayStr, "Sat");
        break;
    case 7:
        sprintf(weekDayStr, "Sun");
        break;
    default:
        sprintf(weekDayStr, "!!!");
    }

    sprintf(dateStr, "%s %02d.%02d.%04d", weekDayStr, convertedDay, convertedMonth, convertedYear);
    writeLine(dateStr, 1);    // write the date in the second line
}

// ****************************************************************************
//  Read and evaluate DCF77 signal and detect events
//  Must be called by user every 10ms
//  Parameter:  Current CPU time base in milliseconds
//  Returns:    DCF77 event, i.e. second pulse, 0 or 1 data bit or minute marker
DCF77EVENT sampleSignalDCF77(unsigned int currentTime)
{
    DCF77EVENT event = NODCF77EVENT;

#ifdef SIMULATOR
    signal = readPortSim() | readPort(); // to test with intentional signal scrambling over PTH0
#else
    signal = readPort();
#endif

    if (!signal)
        setLED(0x02);
    else
        clrLED(0x02);

    // if change from 1 to 0
    if (prevSignal && !signal)
    {
        timePulse = currentTime - fallTime;

        // range for valid sec
        if (timePulse >= 900 && timePulse <= 1100)
        {
            event = VALIDSECOND;
        }
        // range for valid min
        else if (timePulse >= 1900 && timePulse <= 2100)
        {
            event = VALIDMINUTE;
        } else {
          event = INVALID;
        }
        fallTime = currentTime;
    }
    // if change from 0 to 1
    else if (!prevSignal && signal)
    {
        riseTime = currentTime;
        timeLow = riseTime - fallTime;
        // range for valid zero bit
        if (timeLow >= 70 && timeLow <= 130)
        {
            event = VALIDZERO;

        } // range for valid one bit
        else if (timeLow >= 170 && timeLow <= 230)
        {
            event = VALIDONE;
        }
        else
        {
            event = INVALID;
        }
    }

    prevSignal = signal;
    return event;
}

// ****************************************************************************
// Process the DCF77 events
// Contains the DCF77 state machine
// Parameter:   Result of sampleSignalDCF77 as parameter
// Returns:     -
void processEventsDCF77(DCF77EVENT e)
{
    if(e == VALIDSECOND)
    {
        validSecondsCtr++;
    }
    else if(e == VALIDMINUTE)
    {
        displaySuccessfulSync();
        displaySuccessfulDecoding();

        translateData();

        if (dataInvalid() || parityInvalid())
        {
            e = INVALID;
        }
        else
        {
            convertTime();
            setClock((char)convertedHour, (char)dcf77Minute, validSecondsCtr);
        }
        validSecondsCtr = dataLSB = dataMSB = 0;
    }
    else if(e == VALIDONE)
    {
        if(validSecondsCtr < 32)
        {
            dataLSB = dataLSB | ((long)1 << validSecondsCtr);
        }
        else
        {
            dataMSB = dataMSB | ((long)1 << (validSecondsCtr - 32));
        }
    }
    
    if(e == INVALID) {
      displayLossOfSync();
      displayDecodingError();
    }
}

// ****************************************************************************
// checks if the data recieved has invalid values
// Parameter:   -
// Returns:     boolean of check result
char dataInvalid(void)
{
    if(dcf77Minute > 59 || dcf77Hour > 23 || dcf77Day > 31 || dcf77Day < 1 || dcf77Month > 12 || dcf77Month < 1 || dcf77DayOfWeek > 7 || dcf77DayOfWeek < 1)
    {
        return 1;
    }
    return 0;
}

// ****************************************************************************
// checks if the data recieved has invalid parity bits
// Parameter:   -
// Returns:     boolean of check result
char parityInvalid()
{
    return (parity1 % 2 || parity2 % 2 || parity3 % 2);
}

// ****************************************************************************
// translates the data dat has been recieved into all the variables
// like dcf77Minute, dcf77Hour ...
// Parameter:   -
// Returns:     -
void translateData()
{
    char numArray[] = { 1, 2, 4, 8, 10, 20, 40, 80 };

    parity1 = parity2 = parity3 = 0;
    dcf77Minute = dcf77Hour = dcf77Day = dcf77DayOfWeek = dcf77Month = 0;
    dcf77Year = 2000;
    for (i = 0; i < 8; i++)
    {
        // translating year
        dcf77Year += ((dataMSB >> (i + 18)) & 1) * numArray[i];
        // translating parity bit 1
        parity1 += ((dataLSB >> (i + 21)) & 1);

        if (i < 7)
        {
            // translating minute
            dcf77Minute += ((dataLSB >> (i + 21)) & 1) * numArray[i];
            // translating parity bit 2
            if (i < 3)
            {
                parity2 += ((dataLSB >> (i + 29)) & 1);
            }
            else
            {
                parity2 += ((dataMSB >> (i - 3)) & 1);
            }
        }
        if (i < 6)
        {
            // translating day
            dcf77Day += ((dataMSB >> (i + 4)) & 1) * numArray[i];
            // translating hour
            if (i < 3)
            {
                dcf77Hour += ((dataLSB >> (i + 29)) & 1) * numArray[i];
            }
            else
            {
                dcf77Hour += ((dataMSB >> (i - 3)) & 1) * numArray[i];
            }
        }
        if (i < 5)
        {
            // translating month
            dcf77Month += ((dataMSB >> (i + 13)) & 1) * numArray[i];
        }
        if (i < 3)
        {
            // translating weekday
            dcf77DayOfWeek += ((dataMSB >> (i + 10)) & 1) * numArray[i];
        }
    }

    // translating parity bit 3
    for (i = 4; i < 26; i++)
    {
        parity3 += (dataMSB >> i) & 1;
    }
}

// ****************************************************************************
// converts the current time/date from recieved raw data into the respective format
// middle european time (DE) or eastern time (US)
// Parameter:   -
// Returns:     -
void convertTime()
{
    convertedYear = dcf77Year;
    convertedMonth = dcf77Month;
    convertedDay = dcf77Day;
    convertedHour = dcf77Hour;
    convertedDayOfWeek = dcf77DayOfWeek;
    if (!isMiddleEuropeanTime)
    {
          decrementHour();           
    }
    setHour((char)convertedHour);
}

//******************
// successfull synchronization of DCF77 signal
void displaySuccessfulSync(void)
{
    setLED(0x08);
}

//******************
// failed synchronization of DCF77 signal
void displayLossOfSync(void)
{
    clrLED(0x08);
}

//******************
// successfull decoding of DCF77 signal
void displaySuccessfulDecoding(void)
{
    clrLED(0x04);
}

//******************
// failure decoding of DCF77 signal
void displayDecodingError(void)
{
    setLED(0x04);
}

unsigned char isLeapYear(unsigned int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

unsigned char daysInMonth(unsigned char month, unsigned int year)
{
    if (month == 1 || month == 3 || month == 5 || month == 7 ||
        month == 8 || month == 10 || month == 12)
    {
        return 31;
    }
    else if (month == 2)
    {
        if (isLeapYear(year))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    return 30;
}


// Decrement functions //
void decrementMonth(void)
{
    convertedMonth--;
    if(convertedMonth < 1) {
        convertedMonth = 12;
        convertedYear--;  
    }
}

void decrementDay(void)
{
    convertedDay--;
    if(convertedDay < 1) {
      decrementMonth();
      convertedDay = daysInMonth((unsigned char)convertedMonth, convertedYear);
    }   
}

void decrementHour(void) {
  convertedHour -= 6;
  if(convertedHour < 0) {
    convertedHour += 24;
    decrementDay();
  }
}