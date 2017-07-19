/***************************************************************************
	This is a library for the HTS221 Humidity Temperature Sensor
		Originally written by speirano for SmartEverything
		Adjusted by Gregory Knauff of SODAQ for the NB-IoT shield
    Adjusted by Jan van Loenen to work on Sodaq Explorer and Arduino Leonardo
    
	Standard I2C-address is 0x5F.

***************************************************************************/

#include <Wire.h>
#include <Arduino.h>
#include "Sodaq_HTS221.h"

#if defined(ARDUINO_AVR_LEONARDO)
#define DEBUG_STREAM Serial 
#define MODEM_STREAM Serial1

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial

#elif defined(ARDUINO_SAM_ZERO)
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial1

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

void setup() 
{
	delay(500);
	DEBUG_STREAM.begin(9600);
  while (!DEBUG_STREAM);             // Wait for serial monitor
	DEBUG_STREAM.println("\r\nSODAQ HTS221 Arduino Example\r\n");

  delay(2000);

	if (hts221.begin() == false) 
	{
		DEBUG_STREAM.println("Error while retrieving WHO_AM_I byte...");
			while (1);
	}
}


void loop() 
{
	DEBUG_STREAM.print("Humidity   : ");
	DEBUG_STREAM.print(hts221.readHumidity());
	DEBUG_STREAM.println(" %");

	DEBUG_STREAM.print("Temperature: ");
	DEBUG_STREAM.print(hts221.readTemperature());
	DEBUG_STREAM.println(" celsius");

	delay(1000);
}
