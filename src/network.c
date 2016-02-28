#include "network.h"
#include "led.h"

char buffer[4096];
volatile unsigned int position = 0;

char tmp;

char dynamic_ip[16];

void initNetwork() {

    UART3_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART3_PD89);

    position = 0;
    memset(buffer, 0, 4096);

}

void initNetworkTransferInterrupt() {

    UE_USART3_CR1_bit = 1;
    RE_USART3_CR1_bit = 1;
    RXNEIE_USART3_CR1_bit = 1;
    TXEIE_USART3_CR1_bit = 0;
    NVIC_IntEnable(IVT_INT_USART3);

    EnableInterrupts();

}

void enableNetworkTransferInterrupt() {

    NVIC_IntEnable(IVT_INT_USART3);
    EnableInterrupts();
    
}

void disableNetworkTransferInterrupt() {

    NVIC_IntDisable(IVT_INT_USART3);
    DisableInterrupts();
     
}

void UART3_Interrupt() iv IVT_INT_USART3 ics ICS_AUTO {

    tmp = UART3_Read();

    buffer[position++] = tmp;

    if ( position > 4094 ) { position = 0; LED2 = 1; }

    LED1 = 1;

}

void sendCmd(char * cmd, unsigned long sleep_time_ms) {

    resetBuffer();

    UART3_Write_Text(cmd);
    UART3_Write(CR);
    UART3_Write(LF);

    position = 0;

    VDelay_ms(sleep_time_ms);

    buffer[position] = 0;
    position = 0;

}

 void sendRaw(char * raw) {

    resetBuffer();

    UART3_Write_Text(raw);
    UART3_Write(CR);
    UART3_Write(LF);
     
}

int extractIp() {

    char * ptr = strstr(buffer, "STAIP");
    int i;
    
    if ( ptr == 0 ) return 0;

     ptr += 7;
     i = 0;
     
     while ( *ptr != '"' )
           dynamic_ip[i++] = *ptr++;

     dynamic_ip[i] = 0;
     
     return 1;

}

void resetBuffer() {

    position = 0;
    
    memset(buffer, 0, 4096);

    LED2 = 0;

}

void resetModule() {

    UART3_Write_Text("AT+RESTORE");
    UART3_Write(CR);
    UART3_Write(LF);

    Delay_ms(300);

    buffer[position] = 0;

    resetBuffer();

}