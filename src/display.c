#include "display.h"

sbit LCD_RS at GPIOD_ODR.B7;
sbit LCD_EN at GPIOD_ODR.B10;

sbit LCD_D4 at GPIOD_ODR.B11;
sbit LCD_D5 at GPIOD_ODR.B13;
sbit LCD_D6 at GPIOD_ODR.B14;
sbit LCD_D7 at GPIOD_ODR.B15;


void displayLoadingMessage(int row, char * message, unsigned long sleep_time_ms) {

     Lcd_Out(row, 1, message);
     VDelay_ms(sleep_time_ms);

}