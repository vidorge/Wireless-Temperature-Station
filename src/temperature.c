#include "temperature.h"
#include "network.h"

char * high_str = "HI:";
char * low_str = "LO:";

unsigned int high_val = 0;
unsigned int low_val = 200;
unsigned int current_val = 0;

char * high_val_str = "**.*";
char * low_val_str = "**.*";
char * current_val_str = "**.*";


void displayTemperature() {

    // Print temperature on LCD
    Lcd_Cmd(_LCD_CLEAR);

    Lcd_Out(1, 1, high_str);
    Lcd_Out(1, 4, high_val_str);
    Lcd_Out(1, 10, low_str);
    Lcd_Out(1, 13, low_val_str);

    Lcd_Out(2, 5, current_val_str);

}

void renderTemperatureStrings() {

    sprintf(current_val_str, "%.1f", convertRawToFloat(current_val));
    sprintf(high_val_str, "%.1f", convertRawToFloat(high_val));
    sprintf(low_val_str, "%.1f", convertRawToFloat(low_val));
  
}

float convertRawToFloat(unsigned int raw) {

    // TODO
    return raw / 2.0f;

}

unsigned int fetchRawMeasurement() {

    unsigned int temp;

    disableNetworkTransferInterrupt();

    Delay_ms(200);

    // Initialize temperature calculation
    Ow_Reset(&GPIOE_IDR, 5);          // RESET
    Ow_Write(&GPIOB_IDR, 5, 0xCC);    // SKIP ROM
    Ow_Write(&GPIOB_IDR, 5, 0x44);    // CONVERT_T
    Delay_ms(600);

    // Request temperature calculation
    Ow_Reset(&GPIOB_IDR, 5);          // RESET
    Ow_Write(&GPIOB_IDR, 5, 0xCC);    // SKIP ROM
    Ow_Write(&GPIOB_IDR, 5, 0xBE);    // READ_SCRATCHPAD
    Delay_ms(500);

    // Read temperature calculation
    temp =  Ow_Read(&GPIOB_IDR, 5);
    temp = (Ow_Read(&GPIOB_IDR, 5) << 8) + temp;

    Ow_Reset(&GPIOB_IDR, 5);

    enableNetworkTransferInterrupt();

    return temp;

}

/*
void Display_Temperature(unsigned int temp2write) {

  char temp_whole;
  unsigned int temp_fraction;

  sprintf(raw, "%u", temp2write);
  sprintf(high_val_str, "%.1f",  high_val / 2.0f);
  sprintf(low_val_str, "%.1f",  low_val / 2.0f);

  // Check if temperature is negative
  if (temp2write & 0x8000) {
    text[0] = '-';
    temp2write = ~temp2write + 1;
  }
  // Extract temp_whole
  temp_whole = temp2write >> 1 ;

  // Convert temp_whole to characters
  if (temp_whole/100)
    text[0] = temp_whole/100  + 48;
  else
    text[0] = ' ';

  text[1] = (temp_whole/10)%10 + 48;             // Extract tens digit
  text[2] =  temp_whole%10     + 48;             // Extract ones digit

  // Extract temp_fraction and convert it to unsigned int
  temp_fraction  = temp2write << (4-1);
  temp_fraction &= 0x000F;
  temp_fraction *= 625;

  // Convert temp_fraction to characters
  text[4] =  temp_fraction/1000    + 48;         // Extract thousands digit

  text[5] = 176; // step


  // Print temperature on LCD
  Lcd_Cmd(_LCD_CLEAR);

  Lcd_Out(1, 1, high_str);
  Lcd_Out(1, 4, high_val_str);
  Lcd_Out(1, 10, low_str);
  Lcd_Out(1, 13, low_val_str);

  Lcd_Out(2,5,text);

}
*/