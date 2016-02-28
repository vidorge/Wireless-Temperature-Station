#include "led.h"
#include "display.h"
#include "temperature.h"
#include "network.h"

//#define RESET_MODULE
#define DEBUG

#define AP_NAME "name"
#define AP_PASSWORD "password"

char * response_body_0 = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n\
<html><head>\
<title>Temperature Station</title><style>\
tr { text-align: center; }\
h2 { padding: 0; margin: 0; }\
</style></head><body>";

char * response_body_1 = "<table border=\"1\" style=\"margin: auto;\" cellpadding=\"0\" cellspacing=\"0\">\
<tr><td colspan=\"2\"><h2>Current</h2>\
<span style=\"font-size:15em\">";

char * response_body_2 = "xx   &deg;c</span>\
</td></tr><tr><td><h2>Minimum</h2>\
<span style=\"font-size:10em\">yy   </span>\
</td><td><h2>Maximum</h2>\
<span style=\"font-size:10em\">zz   </span>\
</td></tr></table></body></html>";

char response_cmd[18];
char * close_cmd = "AT+CIPCLOSE=X";

int cnt;

void main(){

     char * insert_current_start = strstr(response_body_2, "xx");
     char * insert_low_start = strstr(response_body_2, "yy");
     char * insert_high_start = strstr(response_body_2, "zz");
     
     int response_length = strlen(response_body_0) + strlen(response_body_1) + strlen(response_body_2);

     initLed();

     initNetwork();
     Delay_ms(2000);
     
     initNetworkTransferInterrupt();
     Delay_ms(1000);

     Lcd_Init();                        // Initialize LCD
     Lcd_Cmd(_LCD_CLEAR);               // Clear display
     Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off

     displayLoadingMessage(1, "Starting App...", 300);

#ifdef RESET_MODULE
     Lcd_Out(1, 1, "Reseting module");
     resetModule();
#endif

     Lcd_Out(1, 1, "Setup as client");
     sendCmd("AT+CWMODE_CUR=1", 1000);
     
     Lcd_Out(1, 1, "Connecting to AP");
     sendCmd("AT+CWJAP_CUR=\""AP_NAME"\",\""AP_PASSWORD"\"", 10000);

     sendCmd("AT+CIFSR", 2000);
     
     if ( extractIp() == 0 ) {
        Lcd_Out(1, 1, "Couldn't join AP");
        return;
     }
     
     sendCmd("AT+CIPMUX=1", 500);
     sendCmd("AT+CIPSERVER=1,80", 500);
     
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "Connected");
     Lcd_Out(2, 1, dynamic_ip);
     
     while( 1 ) {

            char * temp_ptr;
     
            LED1 = 0;
            asm wfi; // wait for interrupt

            Delay_ms(200);
            
            if ( (temp_ptr = strstr(buffer, "+IPD")) != 0 ) {

               char connection_id = temp_ptr[5];

               current_val = fetchRawMeasurement();

               if ( current_val > high_val ) high_val = current_val;
               if ( current_val < low_val ) low_val = current_val;
               
               renderTemperatureStrings();
               //displayTemperature();
               
               // encode temperature in response message body
               for ( cnt = 0; cnt < 4; cnt++ ) {
                   insert_current_start[cnt] = current_val_str[cnt];
                   insert_low_start[cnt] = low_val_str[cnt];
                   insert_high_start[cnt] = high_val_str[cnt];
               }
                
               sprintf(response_cmd, "AT+CIPSEND=%c,%d",  connection_id, response_length);
               sendCmd(response_cmd, 200);
               //sendRaw(response_body);
               UART3_Write_Text(response_body_0);
               UART3_Write_Text(response_body_1);
               UART3_Write_Text(response_body_2);
               UART3_Write(CR);
               UART3_Write(LF);
               
               Delay_ms(500);
               
               for ( cnt = 0; cnt < 5; cnt++ ) {
                   close_cmd[12] = '0' + cnt;
                   sendCmd(close_cmd, 300);
               }
               
               resetBuffer();

            }

            resetBuffer();
            Delay_ms(200);
            
     }
}