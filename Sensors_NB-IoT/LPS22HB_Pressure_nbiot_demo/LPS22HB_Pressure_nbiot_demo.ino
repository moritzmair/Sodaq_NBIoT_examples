/***************************************************************************
	This is a library for the LPS22HB Absolute Digital Barometer
		Originally written by Adrien Chapelet for Iotech
		Adjusted by Gregory Knauff of SODAQ for the NB-IoT shield

	Standard I2C-address is 0x5D (pullup/Vcc). If two LPS22HB 
	sensors are being used, please take note of changing 
	one of the addresses to 0x5C (pulldown/GND)

 ***************************************************************************/

#include <Wire.h>
#include <Sodaq_wdt.h>
#include "Sodaq_nbIOT.h"
#include "Sodaq_LPS22HB.h"

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

Sodaq_nbIOT nbiot;
Sodaq_LPS22HB lps22hb;

void setup()
{
	delay(500);
	DEBUG_STREAM.begin(9600);
  MODEM_STREAM.begin(nbiot.getDefaultBaudrate());
 
  while (!Serial);             // Leonardo: wait for serial monitor
	DEBUG_STREAM.println("\r\nSODAQ LPS22HB Arduino Example\r\n");

  nbiot.init(MODEM_STREAM, 7);
  nbiot.setDiag(DEBUG_STREAM);

  if (nbiot.connect("oceanconnect.t-mobile.nl", "172.16.14.20", "20416")) {
      DEBUG_STREAM.println("Connected succesfully!");
  }
  else {
      DEBUG_STREAM.println("Failed to connect!");
  }
	
	lps22hb.begin(0x5D);	// 

	if (lps22hb.whoAmI() == false)
	{
		DEBUG_STREAM.println("Error while retrieving WHO_AM_I byte...");
		while (1);
	}
}

void loop()
{
  // Create the message
  String message = String(lps22hb.readPressure()) + " mbar";

  // Print the message we want to send
  DEBUG_STREAM.println(message);

  // Send the message
  nbiot.sendMessage(message);

  // Wait some time between messages
  delay(10000); // 1000 = 1 sec
}

