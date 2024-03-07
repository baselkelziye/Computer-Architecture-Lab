// clock.c
// written by Basel 

// includes
#include <mc9s12dp256.h>  

#define SELECT12HOURS (1)

void decToASCII_Wrapper(char *txt, int val);
void WriteLine_Wrapper(char *text, char line);
char* getTemperatureText(void);

// prototypes
void adjust_clock_string(void);
void add_second(void);
void add_minute(void);
void add_hour(void);
void display_LED_C(void);
void print_names_periodically(void);
void display_LED_C(void);
void displayClock(void);

// variables
unsigned int hours = 11;
unsigned int minutes = 59;
unsigned int seconds = 30;
unsigned int seconds_passed = 0;

unsigned char isPM = 0xFF;

char hour_char[16];
char minutes_char[16];
char seconds_char[16];
char result_char[16];

unsigned int  my_semaphore = 0;//to switch the printed names


//add_second() calls -> add_minute() (if needed) and add_minute() calls -> add_hour() (if needed)
//so we can handle the overflows properly

// Public interface function: add_second ->  add 1 second
//if seconds = 60 calls add_minute()
// Parameter: -
// Return: -
// Registers: Unchanged 
void add_second(void){
  seconds_passed++; 
  seconds = (seconds + 1) % 60;
  if(seconds == 0){
    add_minute();
  }
}

// Public interface function: add_minute -> add 1 minutes
//called only from add_second, calls add_hour
// Parameter: -
// Return: -
// Registers: Unchanged 
void add_minute(void){
  minutes = (minutes + 1) % 60;
  if(minutes == 0){
    add_hour();
  }
}


// Public interface function:clock_pulse -> each second gets called
//adds second, display the current time to the LCD
// Parameter: -
// Return: -
// Registers: Unchanged 

void clock_pulse(void) {
  add_second();
  if(seconds_passed % 10 == 0) {
    print_names_periodically();
    seconds_passed = 0;        // prevent overflow	
  }      
  displayClock();
}

void displayClock(void) {
  decToASCII_Wrapper(hour_char,hours);
  decToASCII_Wrapper(minutes_char,minutes);
  decToASCII_Wrapper(seconds_char, seconds);
  adjust_clock_string();
  display_LED_C();
  WriteLine_Wrapper(result_char, 1);    
}

// Public interface function:print_names_periodically ->  called every 10 seconds
//global variable my_semaphore changes state for the names
// Parameter: -
// Return: -
// Registers: Unchanged 
void print_names_periodically(void) {   
  if(my_semaphore)
    {
    my_semaphore = 0;   
    WriteLine_Wrapper("Basel ITS2022/23",0);
    }
   else
   {
    my_semaphore = 1;
    WriteLine_Wrapper("Elias ITS2022/23",0);
   } 
}

//Macro SELECT12HOURS if its set to 1
//add_hour function modulus on 13
//if hours = 12 isPM switches(from AM to PM vice-versa)
//if hours = 0 increment hours
#if  SELECT12HOURS == 1  
void add_hour(void){
hours = (hours + 1) % 13;
if(hours == 12){
  isPM = !isPM;
}

if(hours == 0){
  hours++;
 }        
}

//adjust_clock_string
//creates string from hour,minute,seconds and if SELECT12HOURS =1 it attach AM or PM
void adjust_clock_string(void) { 
  char* tmpText;       
  result_char[0] = hour_char[4];
  result_char[1] = hour_char[5];
  result_char[2] = ':';
  result_char[3] = minutes_char[4];
  result_char[4] = minutes_char[5];
  result_char[5] = ':';
  result_char[6] = seconds_char[4];
  result_char[7] = seconds_char[5];
  result_char[9] = 'M';
  if(isPM) {
    result_char[8] = 'P';  
  } else {
   result_char[8] = 'A';
  }
  result_char[10] = ' ';

  tmpText = getTemperatureText();
  result_char[11] = tmpText[0];
  if(tmpText[1] == '0') {
    result_char[12] = ' '; 
  } else {
    result_char[12] = tmpText[1]; 
  }
  result_char[13] = tmpText[2];
  #ifndef _HCS12_SERIALMON
     result_char[14] = 176;   // for simulation
    
  #else
    result_char[14] = 223;   // for hardware  
  #endif
  result_char[15] = 'C';           
}



#else
//its important to have the same name for the functions, because they called only once
//they function for 24H system (SELECT12HOURS = 0)
void add_hour(void) {
  hours = (hours + 1)%24;
}

void adjust_clock_string(void) { 
  char* tmpText;       
  result_char[0] = hour_char[4];
  result_char[1] = hour_char[5];
  result_char[2] = ':';
  result_char[3] = minutes_char[4];
  result_char[4] = minutes_char[5];
  result_char[5] = ':';
  result_char[6] = seconds_char[4];
  result_char[7] = seconds_char[5];
  
  result_char[8] = ' ';
  tmpText = getTemperatureText();
  result_char[9] = tmpText[0];
  if(tmpText[1] == '0') {
    result_char[10] = ' '; 
  } else {
    result_char[10] = tmpText[1]; 
  }
  result_char[11] = tmpText[2];
  #ifndef _HCS12_SERIALMON
    result_char[12] = 176;   // for simulation
  #else
    result_char[12] = 223;   // for hardware  
  #endif
  result_char[13] = 'C';
  result_char[14] = ' ';
  result_char[15] = ' ';                
}

#endif

  


