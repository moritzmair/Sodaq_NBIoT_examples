#include <Arduino.h>
#include <Wire.h>
// #include <SoftwareSerial.h> // Uno
#include <Sodaq_nbIOT.h>

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

void setup() 
{
	delay(500);
	DEBUG_STREAM.begin(9600);
  MODEM_STREAM.begin(nbiot.getDefaultBaudrate());
  
  //while ((!DEBUG_STREAM));// Wait for serial monitor
	
	DEBUG_STREAM.println("\r\nSODAQ NB-IoT Hello World Arduino Example\r\n");

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
}


void loop() 
{
  // Create the message
  String message = "Hello world!!";

  // Print the message we want to send
  DEBUG_STREAM.println(message);

  // Send the message
  nbiot.sendMessage(message);

  // Wait some time between messages
	delay(10000); // 1000 = 1 sec
}
