#ifndef _LED_
#define _LED_

extern sfr sbit LED1;
extern sfr sbit LED2;

/*
  Initialize pins for digital output
*/
void initLed();

/*
  ledId = [1, 2]
  value = [0, 1]
*/
void setLed(int ledId, int value);

#endif