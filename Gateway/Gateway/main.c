/*
 * Gateway
 *
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "espHandler.h"
#include "helper.h"
#include "keypadHandler.h"
#include "lcdHandler.h"
#include "usartHandler.h"

unsigned char ticket0, ticket1, ticket2, ticket3;
unsigned char parking0, parking1, parking2, parking3;
int nodeIndex, refreshLCD, changedBleacher, clientTransmitted;

ISR(USART_RXC_vect)
{
	if (UDR == '\n') clientTransmitted = 1;
	
}

void gateway_init
(char ssid[], char passw[], char hostIP[])
{
	/* Welcome message */
	lcd_init();
	lcd_string_print("Gateway:");
	
	usart_init(0x00, (1<<RXEN) | (1<<TXEN) | (1<<RXCIE), (1 << URSEL) | (3 << UCSZ0), 0x33);

	/* Set ESP server */
	usart_transmit('\n');
	restartESP();
	_delay_ms(500);
	usart_clear_buffer();
	ssidESP(ssid);
	consume_response_no_print();
	passwESP(passw);
	consume_response_no_print();
	hostESP(hostIP);
	consume_response_no_print();
	
	/* Start AP */
	apStartESP();
	consume_response();
}

void checkButtons()
{
	unsigned char input = PINC;
	
	if (((input & 0x01) == 0x01) && nodeIndex != 0)	{ nodeIndex = 0; changedBleacher = 1; }
	if (((input & 0x02) == 0x02) && nodeIndex != 1) { nodeIndex = 1; changedBleacher = 1; }
	if (((input & 0x04) == 0x04) && nodeIndex != 2) { nodeIndex = 2; changedBleacher = 1; }
	if (((input & 0x08) == 0x08) && nodeIndex != 3) { nodeIndex = 3; changedBleacher = 1; }
}

void updateParking
(char sensorName[], int index)
{
	unsigned char parkingTmp;
	
	parkingTmp = readSensor(sensorName);
	
	switch(index)
	{
		case 0:
		if (parkingTmp != parking0) {
			parking0 = parkingTmp;
			refreshLCD = 1;
		}
		break;
		
		case 1:
		if (parkingTmp != parking1) {
			parking1 = parkingTmp;
			refreshLCD = 1;
		}
		break;
		
		case 2:
		if (parkingTmp != parking2) {
			parking2 = parkingTmp;
			refreshLCD = 1;
		}
		break;
		
		case 3:
		if (parkingTmp != parking3) {
			parking3 = parkingTmp;
			refreshLCD = 1;
		}
		break;
	}
}

void printParkingStatus
(unsigned char parking)
{	
	if (parking > 0x40) lcd_data_sim('#');
	else {lcd_string_print("        "); return;}
	if (parking > 0x80) lcd_data_sim('#');
	else {lcd_string_print("      "); return;}
	if (parking > 0xC0) lcd_data_sim('#');
	else {lcd_string_print("    "); return;}
	if (parking >= 0xF9) lcd_data_sim('#');
	else {lcd_string_print("  ");return;}
}

void updateParkingLCD() 
{
	lcd_init();
	lcd_string_print("P1:");
	printParkingStatus(parking0);
	lcd_string_print("|P2:");
	printParkingStatus(parking1);
	lcd_data_sim('\n');
	lcd_string_print("P3:");
	printParkingStatus(parking2);
	lcd_string_print("|P4:");
	printParkingStatus(parking3);
}

int main(void)
{
	/* Initialize variables */
	nodeIndex = clientTransmitted = 0;
	refreshLCD = changedBleacher = 1;
	
	/* Initialize ports */
	DDRC = 0XC0;
	DDRB = 0xFF;
	DDRD = 0xFF;
	
	/* Initialize gateway */
	gateway_init("kpSSID123456", "kpTest123456", "192.168.4.1");
	addSensor("Ticket0"); sensorAddValue("Ticket0", 0xFF);
	addSensor("Ticket1"); sensorAddValue("Ticket1", 0xFF);
	addSensor("Ticket2"); sensorAddValue("Ticket2", 0xFF);
	addSensor("Ticket3"); sensorAddValue("Ticket3", 0xFF);
	addSensor("Parking0"); sensorAddValue("Parking0", 0x01);
	addSensor("Parking1"); sensorAddValue("Parking1", 0x01);
	addSensor("Parking2"); sensorAddValue("Parking2", 0x01);
	addSensor("Parking3"); sensorAddValue("Parking3", 0x01);

	while (1) 
    {	
		sei();
		
		while (changedBleacher == 0 && clientTransmitted == 0)
		{
			checkButtons();
		}
		
		if (changedBleacher == 1) changedBleacher = 0;
		if (clientTransmitted == 1) clientTransmitted = 0;
		
		cli();
		
 		/* Read values */
		ticket0 = readSensor("Ticket0");
		ticket1 = readSensor("Ticket1");
		ticket2 = readSensor("Ticket2");
		ticket3 = readSensor("Ticket3");
		updateParking("Parking0", 0);
		updateParking("Parking1", 1);
		updateParking("Parking2", 2);
		updateParking("Parking3", 3);
		
		/* Update board status */
		if (refreshLCD == 1)
		{
			refreshLCD = 0;
			updateParkingLCD();
		}
		
		/* Update Tickets LEDs */
		if (nodeIndex == 0) PORTB = ticket0;
		if (nodeIndex == 1) PORTB = ticket1;
		if (nodeIndex == 2) PORTB = ticket2;
		if (nodeIndex == 3) PORTB = ticket3;
    }
}