// thermometer.c
// written by Elias Geiger

// Prototypes
void initADC(void);
void convertADC(void);
void temperatureValToASCII(char* text, int val);

// Variables
int temperature;
char temperatureText[3];

// public interface function: called once to setup thermometer
// Parameter: - 
// Return:    - 
// Registers  unchanged 
void initThermometer(void) {
    temperature = 0;
    initADC();
    temperatureValToASCII(temperatureText, temperature);
}

// public interface function: called periodically
// Parameter: - 
// Return:    - 
// Registers  unchanged 
void updateThermometer(void) {
    convertADC();
    temperatureValToASCII(temperatureText, temperature);
}

// public interface function: called periodically to get temperatur text
// Parameter: - 
// Return:    temperatureText  ... pointer on zero terminated ASCII string 
// Registers  unchanged 
char* getTemperatureText(void) {
    return temperatureText;
}

// helper method 
// private helper function: for converting temp. values into strings
// Parameter: String text, Integer temperatur in degrees celsius
// Return:    - 
// Registers  unchanged 
void temperatureValToASCII(char* text, int val) {
  int Q=0, R=0;
  if(val >= 0) {
    text[0] = ' ';
  } else {
    text[0] = '-';
    val = -val;
  }
  Q = val / 10;
  R = val % 10;
  text[1] = Q + '0';
  text[2] = R + '0';
}