/*  Header for DCF77 module

    Computerarchitektur 3
    (C) 2018 J. Friedrich, W. Zimmermann Hochschule Esslingen

    Author:   W.Zimmermann, Jun  10, 2016
    Modified: Basel and Elias
*/


// Enumerated type for DCF77 events
typedef enum { NODCF77EVENT, VALIDZERO, VALIDONE, VALIDSECOND, VALIDMINUTE, INVALID } DCF77EVENT;

// global dcf77 event variable
extern DCF77EVENT dcf77Event;

// Function prototypes for dcf77 module
void initDCF77(void);
void displayDateDcf77(void);
DCF77EVENT sampleSignalDCF77(unsigned int);
void processEventsDCF77(DCF77EVENT);
void convertTime(void);

// display functions
void displaySuccessfulSync(void);
void displayLossOfSync(void);
void displaySuccessfulDecoding(void);
void displayDecodingError(void);

// helper functions
unsigned char isLeapYear(unsigned int);
unsigned char daysInMonth(unsigned char, unsigned int);
void decrementDay(void);
void decrementMonth(void);
void decrementHour(void);
