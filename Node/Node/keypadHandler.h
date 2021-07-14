/// @file keypadHadler.h

#ifndef KEYPADHANDLER_H_
#define KEYPADHANDLER_H_

unsigned char scan_row_sim
(unsigned char rowNumber);

unsigned int scan_keypad_sim();

unsigned int scan_keypad_rising_edge_sim
(unsigned int _tmp_);

unsigned char keypad_to_ascii_sim
(unsigned int state);

#endif /* KEYPADHANDLER_H_ */