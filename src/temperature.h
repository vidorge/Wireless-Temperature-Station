#ifndef _TEMPERATURE_
#define _TEMPERATURE_

extern char * high_str; // HI:
extern char * low_str;  // LO:

extern unsigned int high_val;
extern unsigned int low_val;
extern unsigned int current_val;

extern char * high_val_str;     // **.*
extern char * low_val_str;      // **.*
extern char * current_val_str;  // **.*

/*
  Read sensor using OneWire
*/
unsigned int fetchRawMeasurement();

/*
  Convert raw measurements to display-able values
*/
float convertRawToFloat(unsigned int raw);

/*
  Convert float values to strings
*/
void renderTemperatureStrings();

/*
  Display low, high and current temp on dislpay
*/
void displayTemperature();

#endif