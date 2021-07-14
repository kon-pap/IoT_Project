/// @file keypadHandler.c
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "keypadHandler.h"

/**
 * Checks 1 keypad line for pressed buttons
 * @param[in] rowNumber number of line for reading
 * @param[out] keypadRow status of keypad row in 4 LSBs
*/
unsigned char scan_row_sim(unsigned char rowNumber)
{
	PORTC = rowNumber;
	_delay_us(500);	//code segment for proper function of remote access

	;
	;
	return (PINC & 0x0F);
}

/**
 * Checks keypad for pressed buttons
 * @param[out] keypad button state
*/
unsigned int scan_keypad_sim()
{
	unsigned char stateMSB, stateLSB;

	stateMSB = scan_row_sim(0x10); //PC4: 1 2 3 A
	stateMSB = (((stateMSB & 0x0F) << 4) | ((stateMSB & 0xF0) >> 4));
	stateMSB = stateMSB + scan_row_sim(0x20); //PC5: 4 5 6 B
	stateLSB = scan_row_sim(0x40); //PC6: 7 8 9 C
	stateLSB = (((stateLSB & 0x0F) << 4) | ((stateLSB & 0xF0) >> 4));
	stateLSB = stateLSB + scan_row_sim(0x80); //PC7: * 0 # D
	
	return (stateMSB << 8) + stateLSB;
}

/**
 * Checks keypad for buttons that weren't pressed the last time we checked and now are pressed
 * @param[in] _tmp_ previous button state (new button state saved here too)
 * @param[out] keypad representative of number of buttons that have just been pressed
*/
unsigned int scan_keypad_rising_edge_sim(unsigned int _tmp_)
{
	unsigned int pressedPads, previousPressedPads;

	pressedPads = scan_keypad_sim();
	_delay_ms(15);
	pressedPads = pressedPads & scan_keypad_sim();
	previousPressedPads = _tmp_;
	_tmp_ = pressedPads;
	
	return pressedPads & ~previousPressedPads;
}

/**
 * Matches buttons with ascii code
 * @param[in] state representative of keypad state (use scan_keypad_sim or scan_keypad_rising_edge_sim)
 * @param[out] char ASCII code of the first pressed button
*/
unsigned char keypad_to_ascii_sim(unsigned int state)
{
	switch(state)
	{
		case 0x01:
		return '*';
		case 0x02:
		return '0';
		case 0x04:
		return '#';
		case 0x08:
		return 'D';
		case 0x10:
		return '7';
		case 0x20:
		return '8';
		case 0x40:
		return '9';
		case 0x80:
		return 'C';
		case 0x100:
		return '4';
		case 0x200:
		return '5';
		case 0x400:
		return '6';
		case 0x800:
		return 'B';
		case 0x1000:
		return '1';
		case 0x2000:
		return '2';
		case 0x4000:
		return '3';
		case 0x8000:
		return 'A';
		default:
		return 0;
	}
}