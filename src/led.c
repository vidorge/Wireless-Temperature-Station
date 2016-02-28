#include "led.h"

sbit LED1 at GPIOE_ODR.B12;
sbit LED2 at GPIOE_ODR.B15;

void initLed() {

     GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_12 | _GPIO_PINMASK_15);
     
     LED1 = 0;
     LED2 = 0;

}

void setLed(int ledId, int value) {

     switch(ledId) {
         case 1: LED1 = value; break;
         case 2: LED2 = value; break;
         default: return;
     }
     
}