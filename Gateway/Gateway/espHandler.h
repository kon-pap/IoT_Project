/// @file espHandler.h

#ifndef ESPHANDLER_H_
#define ESPHANDLER_H_

void addSensorESP
(char sensorName[]);

void apStartESP();

void baudRateESP
(char baudRate[]);

void clientTransmitESP();

void connectESP();

void debugESP
(char debug[]);

void helpESP();

void getAllValuesESP();

void getValueESP
(char sensorName[]);

void hostESP
(char host[]);

void passwESP
(char passw[]);

void restartESP();

void sensorValueESP
(char sensorName[], char sensorValue[]);

void ssidESP
(char ssid[]);

void sensorValueESPByte
(char sensorName[], char sensorValue);

#endif /* ESPHANDLER_H_ */