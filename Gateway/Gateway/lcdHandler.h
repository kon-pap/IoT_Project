/// @file lcdHandler.h

#ifndef LCDHANDLER_H_
#define LCDHANDLER_H_

void write_2_nibbles_sim
(unsigned char input);

void lcd_data_sim
(unsigned char data);

void lcd_command_sim
(unsigned char comm);

void lcd_init();

void lcd_string_print
(char stringAr[]);

#endif /* LCDHANDLER_H_ */