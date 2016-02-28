#ifndef _NETWORK_
#define _NETWORK_

#define CR 0x0D
#define LF 0x0A

extern char buffer[4096];
extern volatile unsigned int position;

// DHCP assigned IP address
extern char dynamic_ip[16];

// Init serial interface to ESP8266
void initNetwork();

// Enable interrupt for serial data receive
void initNetworkTransferInterrupt();

void sendCmd(char * cmd, unsigned long sleep_time_ms);
void sendRaw(char * raw);

int extractIp();

void resetBuffer();
void resetModule();

void enableNetworkTransferInterrupt();
void disableNetworkTransferInterrupt();

#endif