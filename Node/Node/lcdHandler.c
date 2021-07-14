/// @file lcdHandler.c
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "lcdHandler.h"

/**
 * Sends 1 byte, 4bit at a time to LCD controller
 * The logical level of pin of R/S does not change
 * @param[in] input byte to be transmitted
*/
void write_2_nibbles_sim
(unsigned char input)
{
	unsigned char lsb, msb;
	_delay_us(6000);
	
	lsb = PIND & 0x0f;
	msb = input & 0xf0;
	PORTD = msb | lsb;
	PORTD |= _BV(PD3);
	PORTD &= ~(_BV(PD3));
	
	_delay_us(6000);
	
	msb = (((input & 0x0F) << 4) | ((input & 0xF0) >> 4));
	msb = msb & 0xf0;
	PORTD = msb | lsb;
	PORTD |= _BV(PD3);
	PORTD &= ~(_BV(PD3));
}

/**
 * Sends byte of data to LCD controller
 * The controller must be in 4 bit mode
 * @param[in] data data byte to be transmitted
*/
void lcd_data_sim
(unsigned char data)
{
	PORTD |= _BV(PD2);
	write_2_nibbles_sim(data);
	_delay_us(43);
}

/**
 * Sends instruction to LCD controller
 * The controller must be in 4 bit mode
 * @param[in] comm command byte to be transmitted
*/
void lcd_command_sim
(unsigned char comm)
{
	PORTD &= ~(_BV(PD2));
	write_2_nibbles_sim(comm);
	_delay_us(39);
}

/**
 * Initializes and sets the LCD display
 * DL=0 4-bit mode, N=1 2 lines, F=0 5x8 dots,
 * D=1 display on, C=0 cursor off, B=0 blinking off
 * I/D=1 DDRAM address auto increment, SH=0 shift of entire display off
 * @param[in] comm command byte to be transmitted
*/
void lcd_init()
{
	_delay_ms(40);
	PORTD = 0x30;
	PORTD |= _BV(PD3);
	PORTD &= ~(_BV(PD3));
	_delay_us(39);
	
	_delay_us(1000);
	
	PORTD = 0x30;
	PORTD |= _BV(PD3);
	PORTD &= ~(_BV(PD3));
	_delay_us(39);
	
	_delay_us(1000);
	
	PORTD = 0x20;
	PORTD |= _BV(PD3);
	PORTD &= ~(_BV(PD3));
	_delay_us(39);
	
	_delay_us(1000);
	
	lcd_command_sim(0x28);
	lcd_command_sim(0x0c);
	lcd_command_sim(0x01);
	_delay_us(1530);
	lcd_command_sim(0x06);
}

/**
 * Prints string in LCD display
 * @param[in] string char buffer to be printed
 * @param[in] limit number of chars to be printed
*/
void lcd_string_print(char stringAr[])
{
	int j = 0;
	while (stringAr[j] != '\0' && stringAr[j] != '\n')
	{
		lcd_data_sim((unsigned char) stringAr[j]);
		j++;
	}
}