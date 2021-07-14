/// @file usartHandler.h

#ifndef USARTHANDLER_H_
#define USARTHANDLER_H_

void usart_init
(unsigned char ucsra, unsigned char ucsrb, unsigned char ucsrc, unsigned int ubrr);

void usart_transmit
(unsigned char data);

unsigned char usart_receive();

void usart_transmit_string
(char string[]);

void usart_clear_buffer();

#endif /* USARTHANDLER_H_ */