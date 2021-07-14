//@file adcHandler.c

#include <avr/io.h>

/**
 * Initializes ADC converter
*/
unsigned char adc_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}