#include "USART.c"


void sendPowerUsage(float newAverage);

void USART_init( unsigned int baud );

unsigned char USART_rx(void);

void USART_tx(unsigned char data);

void HandleWirelessData(unsigned char *incomingPacket, unsigned char *priceCharArray, unsigned char OnOff,float newAverage);
