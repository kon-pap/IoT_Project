/// @file usartHandler.c
#include <avr/io.h>

#include "usartHandler.h"

/**
 * Initializes the USART
 * @param[in] ucsra state register
 * @param[in] ucsrb control register
 * @param[in] ucsrc control register
 * @param[in] ubrr baud rate register
*/
void usart_init
(unsigned char ucsra, unsigned char ucsrb, unsigned char ucsrc, unsigned int ubrr)
{
	UCSRA = ucsra;
	UCSRB = ucsrb;
	UCSRC = ucsrc;
	UBRRL = ((unsigned char *)&ubrr)[0];
	UBRRH = ((unsigned char *)&ubrr)[1];
}

/**
 * Sends a byte of data
 * @param[in] data byte to be transmitted
*/
void usart_transmit
(unsigned char data)
{
	while (((UCSRA >> UDRE) & 0x01) == 0x00);
	UDR = data;
}

/**
 * Receives a byte of data
 * @param[out] UDR data register
*/
unsigned char usart_receive()
{
	while (((UCSRA >> RXC) & 0x01) == 0x00);
	return UDR;
}

/**
 * Sends a string of data
 * @param[in] string data string to be transmitted
*/
void usart_transmit_string
(char string[])
{
	for (int i = 0; string[i] != '\0'; ++i)
	usart_transmit(string[i]);
}

/**
 * Clears USART buffer
*/
void usart_clear_buffer()
{
	while (((UCSRA >> RXC) & 0x01) != 0x00) usart_receive();
}