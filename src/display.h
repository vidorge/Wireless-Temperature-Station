#ifndef _DISPLAY_
#define _DISPLAY_

extern sbit LCD_RS;
extern sbit LCD_EN;

extern sbit LCD_D4;
extern sbit LCD_D5;
extern sbit LCD_D6;
extern sbit LCD_D7;

void displayLoadingMessage(int row, char * message, unsigned long sleep_time_ms);

#endif