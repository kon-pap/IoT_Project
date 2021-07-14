/// @file helper.h

#ifndef HELPER_H_
#define HELPER_H_

unsigned char parsedValue
(unsigned char data[]);

void consume_response();

void consume_response_no_print();

void addSensor
(char sensorName[]);

void sensorAddValue
(char sensorName[], char sensorValue);

unsigned char readSensor
(char sensorName[]);

#endif /* HELPER_H_ */