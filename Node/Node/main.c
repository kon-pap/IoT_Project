/*
 * Node
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
volatile unsigned char parking0, parking1, parking2, parking3;
unsigned char hit;
int nodeIndex, _tmp_;
volatile unsigned int adc_out, updatedParking;

void node_init
(char ssid[], char passw[], char hostIP[])
{
	/* Welcome message */
	lcd_init();
	lcd_string_print("Node:");
	
	usart_init(0x00, (1<<RXEN) | (1<<TXEN), (1 << URSEL) | (3 << UCSZ0), 0x33);
	
	/* Set ESP server */
	usart_transmit('\n');
	restartESP();
	_delay_ms(500);
	usart_clear_buffer();
	debugESP("true");
	consume_response();
	ssidESP(ssid);
	consume_response();
	passwESP(passw);
	consume_response();
	hostESP(hostIP);
	consume_response();
	
 	/* Connect */
	connectESP();
	consume_response();
	lcd_init();
	debugESP("false");
	consume_response();
}

void changeBleacher
(unsigned char hit)
{
	if (hit == 0x0A) nodeIndex = 0;
	if (hit == 0x0B) nodeIndex = 1;
	if (hit == 0x0C) nodeIndex = 2;
	if (hit == 0x0D) nodeIndex = 3;
}

void updateBleacherLCD()
{
	lcd_init();
	if (nodeIndex == 0) { lcd_string_print("Bleacher A"); }
	if (nodeIndex == 1) { lcd_string_print("Bleacher B"); }
	if (nodeIndex == 2) { lcd_string_print("Bleacher C"); }
	if (nodeIndex == 3) { lcd_string_print("Bleacher D"); }
}

unsigned char transformHit(unsigned char hit)
{
	switch(hit)
	{
		case '1':
		return 0x00;
		case '2':
		return 0x01;
		case '3':
		return 0x02;
		case '4':
		return 0x03;
		case '5':
		return 0x04;
		case '6':
		return 0x05;
		case '7':
		return 0x06;
		case '8':
		return 0x07;
		case 'A':
		lcd_init();
		lcd_string_print("Bleacher A");
		return 0x0A;
		case 'B':
		lcd_init();
		lcd_string_print("Bleacher B");
		return 0x0B;
		case 'C':
		lcd_init();
		lcd_string_print("Bleacher C");
		return 0x0C;
		case 'D':
		lcd_init();
		lcd_string_print("Bleacher D");
		return 0x0D;
		break;
		default:
		return 0x08;
	}
}

void handleTickets
(unsigned char hit)
{
	if (hit < 0x08)
	{
		switch(nodeIndex)
		{
			case 0:
			if (((ticket0 >> hit) & 0x01) != 0x01)
			{
				ticket0 = ticket0 | (1 << hit);
				sensorAddValue("Ticket0", ticket0);
				clientTransmit();
			}
			else
			{
				lcd_init();
				lcd_string_print("Stop");
				_delay_ms(300);
				updateBleacherLCD();
			}
			break;
			
			case 1:
			if (((ticket1 >> hit) & 0x01) != 0x01)
			{
				ticket1 = ticket1 | (1 << hit);
				sensorAddValue("Ticket1", ticket1);
				clientTransmit();
			}
			else
			{
				lcd_init();
				lcd_string_print("Stop");
				_delay_ms(300);
				updateBleacherLCD();
			}
			break;
			
			case 2:
			if (((ticket2 >> hit) & 0x01) != 0x01)
			{
				ticket2 = ticket2 | (1 << hit);
				sensorAddValue("Ticket2", ticket2);
				clientTransmit();
			}
			else
			{
				lcd_init();
				lcd_string_print("Stop");
				_delay_ms(300);
				updateBleacherLCD();
			}
			break;
			
			case 3:
			if (((ticket3 >> hit) & 0x01) != 0x01)
			{
				ticket3 = ticket3 | (1 << hit);
				sensorAddValue("Ticket3", ticket3);
				clientTransmit();
			}
			else
			{
				lcd_init();
				lcd_string_print("Stop");
				_delay_ms(300);
				updateBleacherLCD();
			}
			break;
		}
	}
	
}

ISR(TIMER1_OVF_vect)
{
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	TCNT1 = 0x6769;
}

ISR(ADC_vect)
{
	adc_out = ADC;
	adc_out = (adc_out / 3) + 0x10;
	if (nodeIndex == 0 && parking0 != ((unsigned char *)&adc_out)[0]) 
	{ 
		parking0 = ((unsigned char *)&adc_out)[0]; updatedParking = 1;
		if (parking0 == 0x00) sensorAddValue("Parking0", 0x01);
		else sensorAddValue("Parking0", parking0);
	}
	if (nodeIndex == 1 && parking1 != ((unsigned char *)&adc_out)[0]) 
	{ 
		parking1 = ((unsigned char *)&adc_out)[0]; updatedParking = 1;
		if (parking1 == 0x00) sensorAddValue("Parking1", 0x01);
		else sensorAddValue("Parking1", parking1);
	}
	if (nodeIndex == 2 && parking2 != ((unsigned char *)&adc_out)[0]) 
	{ 
		parking2 = ((unsigned char *)&adc_out)[0]; updatedParking = 1;
		if (parking2 == 0x00) sensorAddValue("Parking2", 0x01);
		else sensorAddValue("Parking2", parking2);
	}
	if (nodeIndex == 3 && parking3 != ((unsigned char *)&adc_out)[0]) 
	{ 
		parking3 = ((unsigned char *)&adc_out)[0]; updatedParking = 1;
		if (parking3 == 0x00) sensorAddValue("Parking3", 0x01);
		else sensorAddValue("Parking3", parking3);
	}
}

int main(void)
{
	/* Initialize variables */
	nodeIndex = _tmp_ = 0;
	
	ticket0 = ticket1 = ticket2 = ticket3 = 0;
	
	parking0 = parking1 = parking2 = parking3 = 0x01;
	adc_out = updatedParking = 0;
	
	/* Initialize ports */
	DDRB = 0xEE;
	DDRC = 0xF0;
	DDRD = 0xFF;
	
	/*Initialize timer1 */
	TIMSK = (1<<TOIE1);
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10);
	
	TCNT1 = 0x6769;
	
	/* Initialize ADC */
	ADMUX = (1<<REFS0);
	
	/* Initialize gateway */
	node_init("kpSSID123456", "kpTest123456", "192.168.4.1");
	addSensor("Ticket0"); sensorAddValue("Ticket0", 0xFF);
	addSensor("Ticket1"); sensorAddValue("Ticket1", 0xFF);
	addSensor("Ticket2"); sensorAddValue("Ticket2", 0xFF);
	addSensor("Ticket3"); sensorAddValue("Ticket3", 0xFF);
	addSensor("Parking0"); sensorAddValue("Parking0", parking0);
	addSensor("Parking1"); sensorAddValue("Parking1", parking1);
	addSensor("Parking2"); sensorAddValue("Parking2", parking2);
	addSensor("Parking3"); sensorAddValue("Parking3", parking3);
	
	lcd_init();
	lcd_string_print("Bleacher A");
	
	sei();
	
	while (1)
	{	
				
		/* Read keypad & buttons*/
		hit = 0;
		while (hit == 0 && updatedParking == 0)
		{
			hit = keypad_to_ascii_sim(scan_keypad_rising_edge_sim(_tmp_));
		}
		
		/* Parking Handling */
		if (updatedParking == 1)
		{
			updatedParking = 0;
			clientTransmit();
		}
		
		/* Tickets & Bleacher handling */
		if (hit != 0)
		{
			hit = transformHit(hit);
			changeBleacher(hit);
			handleTickets(hit);
		}
	}
}