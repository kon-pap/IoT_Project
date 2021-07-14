/// @file espHandler.c
#include "usartHandler.h"

#include "espHandler.h"

/**
 * Add a sensor Name
 * @param[in] sensorName
*/
void addSensorESP
(char sensorName[])
{
	usart_transmit_string("ESP:addSensor: \"");
	usart_transmit_string(sensorName);
	usart_transmit_string("\"\n");
}

/**
 * Uses the provided SSID and Password to create an Access Point
*/
void apStartESP()
{
	usart_transmit_string("ESP:APStart\n");
}

/**
 * Resets the Serial to the required speed
 * @param[in] baudRate
*/
void baudRateESP
(char baudRate[])
{
	usart_transmit_string("ESP: baudrate: \"");
	usart_transmit_string(baudRate);
	usart_transmit_string("\"\n");
}

/**
 * Sends all SensorValues to the connected server
*/
void clientTransmitESP()
{
	usart_transmit_string("ESP:clientTransmit\n");
}

/**
 * Uses the provided SSID and Password to connect to the WiFi network
*/
void connectESP()
{
	usart_transmit_string("ESP:connect\n");
}

/**
 * Every command will send extra debug information if set to true
 * @param[in] debug "true" or "false"
*/
void debugESP
(char debug[])
{
	usart_transmit_string("ESP:debug: \"");
	usart_transmit_string(debug);
	usart_transmit_string("\"\n");
}

/**
 * Lists all commands
*/
void helpESP()
{
	usart_transmit_string("ESP:help\n");
}

/**
 * Requests the values of all Sensors
*/
void getAllValuesESP()
{
	usart_transmit_string("ESP:getAllValuesESP\n");
}

/**
 * Request the value of the specified Sensor
 * @param[in] sensorName
*/
void getValueESP
(char sensorName[])
{
	usart_transmit_string("ESP:getValue: \"");
	usart_transmit_string(sensorName);
	usart_transmit_string("\"\n");
}

/**
 * Your Host's IP address
 * @param[in] host IP address
*/
void hostESP
(char host[])
{
	usart_transmit_string("ESP:hostIP: \"");
	usart_transmit_string(host);
	usart_transmit_string("\"\n");
}

/**
 * Password to be used for the connection
 * @param[in] passw password
*/
void passwESP
(char passw[])
{
	usart_transmit_string("ESP:password: \"");
	usart_transmit_string(passw);
	usart_transmit_string("\"\n");
}

/**
 * Restarts ESP
 * Default parameters:
 * baud rate = 9600, debug = false, hostIP = 192.168.4.1
 * password = awesomePassword, ssid = awesomeAccessPoint
*/
void restartESP()
{
	usart_transmit_string("ESP:restart\n");
}

/**
 * Saves value to specified sensor. Values should not contain = and ,
 * @param[in] sensorName
 * @param[in] sensorValue
*/
void sensorValueESP
(char sensorName[], char sensorValue[])
{
	usart_transmit_string("ESP:sensorValue: \"");
	usart_transmit_string(sensorName);
	usart_transmit_string("\"[");
	usart_transmit_string(sensorValue);
	usart_transmit_string("]\n");
}

/**
 * SSID to be used for the connection
 * @param[in] ssid
*/
void ssidESP
(char ssid[])
{
	usart_transmit_string("ESP:ssid: \"");
	usart_transmit_string(ssid);
	usart_transmit_string("\"\n");
}

/**
* Saves value to specified sensor. Values should not contain = and ,
* @param[in] sensorName
* @param[in] sensorValue
*/
void sensorValueESPByte
(char sensorName[], char sensorValue)
{
	usart_transmit_string("ESP:sensorValue: \"");
	usart_transmit_string(sensorName);
	usart_transmit_string("\"[");
	usart_transmit(sensorValue);
	usart_transmit_string("]\n");
}