//@file helper.c

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "espHandler.h"
#include "lcdHandler.h"
#include "usartHandler.h"

#include "helper.h"

/**
 * Parses message to value of sensor
 * @param[in] data message sent by getValueESP
 * @param[out] parsedValue single byte of data containing the value of sensor
*/
unsigned char parsedValue
(unsigned char data[])
{
	int j = 0;
	while (data[j] != '"') j++;
	
	return data[j+1];
}

/**
 * Consumes and prints answer of ESP
*/
void consume_response()
{
	char dataAr[200];
	int i;
	
	i = 0;
	while((dataAr[i++] = usart_receive()) != '\n');
	
	lcd_string_print((char *) dataAr);
	_delay_ms(200);
	lcd_init();
}

/**
 * Consumes answer of ESP
*/
void consume_response_no_print()
{
	char dataAr[200];
	int i;
	
	i = 0;
	while((dataAr[i++] = usart_receive()) != '\n');
}

/**
 * Adds sensor and prints message
 * @param[in] sensorName
*/
void addSensor
(char sensorName[])
{	
	lcd_string_print(sensorName);
	lcd_data_sim(':');
	
	usart_clear_buffer();
	addSensorESP(sensorName);
	consume_response();
}

/**
 * Adds sensor value to the specified sensor
 * @param[in] sensorName
 * @param[in] sensorValue one byte of data
*/
void sensorAddValue
(char sensorName[], char sensorValue)
{
	usart_clear_buffer();
	sensorValueESPByte(sensorName, sensorValue);
	consume_response_no_print();
}

/**
 * Reads sensor value of the specified sensor
 * @param[in] sensorName
 * @param[out] data one byte of data
*/
unsigned char readSensor
(char sensorName[])
{
	char dataAr[20];
	int i;
	
	usart_clear_buffer();
	getValueESP(sensorName);
	i = 0;
	while((dataAr[i++] = usart_receive()) != '\n');
	
	return parsedValue((unsigned char*) dataAr);
}