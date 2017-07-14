/***************************************************************************
	This is a library for the HTS221 Humidity Temperature Sensor
		Originally written by speirano for SmartEverything
		Adjusted by Gregory Knauff of SODAQ for the NB-IoT shield
	Adjusted by Jan van Loenen to work on Sodaq Explorer and Arduino Leonardo

	Standard I2C-address is 0x5F.

***************************************************************************/

/*****
 * ATT Settings
 *
 * create a new asset as Number
 *
 * device decoding:
 
 {
 "sense": [
 {
	 "asset": "temperature",
		 "value" : {
		 "byte": 0,
			 "bytelength" : 2,
			 "type" : "integer",
			 "calculation" : "val / 100"
	 }
 },
 {
	 "asset": "humidity",
		 "value" : {
		 "byte": 2,
			 "bytelength" : 2,
			 "type" : "integer",
			 "calculation" : "val / 100"
	 }
 }
 ]
 }
 */

#include <Arduino.h>
#include <Wire.h>
 // #include <SoftwareSerial.h> // Uno
#include <Sodaq_wdt.h>
#include "Sodaq_nbIOT.h"
#include "Sodaq_HTS221.h"

#if defined(ARDUINO_AVR_LEONARDO)
#define DEBUG_STREAM Serial 
#define MODEM_STREAM Serial1

#elif defined(ARDUINO_AVR_UNO)
 SoftwareSerial softSerial(10, 11); // RX, TX
 // You can connect an uartsbee or other board (e.g. 2nd Uno) to connect the softserial.
#define DEBUG_STREAM softSerial 
#define MODEM_STREAM Serial

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

 Sodaq_nbIOT nbiot;

 void setup()
 {
	 delay(500);
	 DEBUG_STREAM.begin(9600);
	 MODEM_STREAM.begin(nbiot.getDefaultBaudrate());

	 //while ((!DEBUG_STREAM));// Wait for serial monitor

	 DEBUG_STREAM.println("\r\nSODAQ HTS221 Arduino Example\r\n");

	 nbiot.init(MODEM_STREAM, 7);
	 nbiot.setDiag(DEBUG_STREAM);

	 delay(2000);

	 if (nbiot.connect("oceanconnect.t-mobile.nl", "172.16.14.20", "20416")) {
		 DEBUG_STREAM.println("Connected succesfully!");
	 }
	 else {
		 DEBUG_STREAM.println("Failed to connect!");
		 return;
	 }

	 if (hts221.begin() == false)
	 {
		 DEBUG_STREAM.println("Error while retrieving WHO_AM_I byte...");
		 while (1);
	 }
 }


 void loop()
 {
	 // Create the message
	 byte message[8];
	 uint16_t cursor = 0;
	 int16_t temperature;
	 int16_t humidity;

	 temperature = hts221.readTemperature() * 100;
	 DEBUG_STREAM.println(temperature);
	 message[cursor++] = temperature >> 8;
	 message[cursor++] = temperature;

	 humidity = hts221.readHumidity() * 100;
	 DEBUG_STREAM.println(humidity);
	 message[cursor++] = humidity >> 8;
	 message[cursor++] = humidity;

	 // Print the message we want to send
	 // DEBUG_STREAM.println(message);
	 for (int i = 0; i < cursor; i++) {
		 if (message[i] < 10) {
			 DEBUG_STREAM.print("0");
		 }
		 DEBUG_STREAM.print(message[i], HEX);
		 DEBUG_STREAM.print(":");
	 }
	 DEBUG_STREAM.println();

	 // Send the message
	 nbiot.sendMessage(message, cursor);

	 // Wait some time between messages
	 delay(10000); // 1000 = 1 sec
 }
