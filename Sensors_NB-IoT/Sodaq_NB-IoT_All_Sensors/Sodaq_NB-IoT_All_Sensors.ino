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
 "asset": "my_temperature",
 "value": {
 "byte": 0,
 "bytelength": 2,
 "type": "integer",
 "calculation": "val / 100"
 }
 },
 {
 "asset": "my_humidity",
 "value": {
 "byte": 2,
 "bytelength": 2,
 "type": "integer",
 "calculation": "val / 100"
 }
 },
 {
 "asset": "my_pressure",
 "value": {
 "byte": 4,
 "bytelength": 2,
 "type": "integer"
 }
 },
 {
 "asset": "my_gps",
 "value": {
 "latitude": {
 "byte": 6,
 "bytelength": 4,
 "type": "integer",
 "calculation": "val / 100000"
 },
 "longitude": {
 "byte": 10,
 "bytelength": 4,
 "type": "integer",
 "calculation": "val / 100000"
 }
 }
 }
 ]
 }
 */

#include <Arduino.h>
#include <Wire.h>
#include <Sodaq_nbIOT.h>
 // #include <SoftwareSerial.h> // Uno
#include "Sodaq_HTS221.h"
#include "Sodaq_LPS22HB.h"
#include "Sodaq_UBlox_GPS.h"

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

#elif defined(ARDUINO_SAM_ZERO)
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial1

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

 Sodaq_nbIOT nbiot;
 Sodaq_LPS22HB lps22hb;

 uint32_t lat = 0;
 uint32_t lon = 0;

 void setup();
 bool connectToNetwork();
 void initHumidityTemperature();
 void initPressureSensor();
 void initGPS();
 void loop();
 void do_flash_led(int pin);

 void setup()
 {
	 pinMode(13, OUTPUT);
	 digitalWrite(13, HIGH);

	 DEBUG_STREAM.begin(9600);
	 MODEM_STREAM.begin(nbiot.getDefaultBaudrate());

	 while ((!DEBUG_STREAM) || millis() < 10000) {
		 // Wait for serial monitor for 10 seconds
	 }

	 DEBUG_STREAM.println("\r\nSODAQ All Things Talk Arduino Example\r\n");

	 nbiot.init(MODEM_STREAM, 7);
	 nbiot.setDiag(DEBUG_STREAM);

	 delay(2000);

	 while(!connectToNetwork());

	 initHumidityTemperature();
	 initPressureSensor();
	 initGPS();

	 digitalWrite(13, LOW);
 }

 bool connectToNetwork() {
	 if (nbiot.connect("oceanconnect.t-mobile.nl", "172.16.14.20", "20416")) {
		 DEBUG_STREAM.println("Connected succesfully!");
		 return true;
	 }
	 else {
		 DEBUG_STREAM.println("Failed to connect!");
		 delay(2000);
		 return false;
	 }
 }

 void initHumidityTemperature() {
	 if (hts221.begin() == false)
	 {
		 DEBUG_STREAM.println("Error while retrieving WHO_AM_I byte...");
		 while (1);
	 }
 }

 void initPressureSensor() {
	 lps22hb.begin(0x5D);	// 

	 if (lps22hb.whoAmI() == false)
	 {
		 DEBUG_STREAM.println("Error while retrieving WHO_AM_I byte...");
	 }
 }

 void initGPS() {
	 sodaq_gps.init(6);
	 // sodaq_gps.setDiag(DEBUG_STREAM);
 }


 void loop()
 {
	 do_flash_led(13);
	 // Create the message
	 byte message[8];
	 uint16_t cursor = 0;
	 int16_t temperature;
	 int16_t humidity;
	 int16_t pressure;

	 temperature = hts221.readTemperature() * 100;
	 DEBUG_STREAM.println("Temperature x100 : " + (String)temperature);
	 message[cursor++] = temperature >> 8;
	 message[cursor++] = temperature;

	 delay(100);

	 humidity = hts221.readHumidity() * 100;
	 DEBUG_STREAM.println("Humidity x100 : " + (String)humidity);
	 message[cursor++] = humidity >> 8;
	 message[cursor++] = humidity;

	 delay(100);

	 pressure = lps22hb.readPressure() ;
	 DEBUG_STREAM.println("Pressure:" + (String)pressure);
	 message[cursor++] = pressure >> 8;
	 message[cursor++] = pressure;

	 uint32_t start = millis();
	 uint32_t timeout = 2UL * 60 * 1000; // 2 min timeout
	 
	 DEBUG_STREAM.println(String("waiting for fix ..., timeout=") + timeout + String("ms"));
	 if (sodaq_gps.scan(true, timeout)) {

		 lat = sodaq_gps.getLat() * 100000;
		 lon = sodaq_gps.getLon() * 100000;
	 }
	 else {
		 DEBUG_STREAM.println("No Fix");
	 } 

	 message[cursor++] = lat >> 24;
	 message[cursor++] = lat >> 16;
	 message[cursor++] = lat >> 8;
	 message[cursor++] = lat;

	
	 message[cursor++] = lon >> 24;
	 message[cursor++] = lon >> 16;
	 message[cursor++] = lon >> 8;
	 message[cursor++] = lon;

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

 void do_flash_led(int pin)
 {
	 for (size_t i = 0; i < 2; ++i) {
		 delay(100);
		 digitalWrite(pin, LOW);
		 delay(100);
		 digitalWrite(pin, HIGH);
	 }
 }
