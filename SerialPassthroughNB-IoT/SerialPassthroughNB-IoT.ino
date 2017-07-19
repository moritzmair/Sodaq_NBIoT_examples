#include "Arduino.h"

#if defined(ARDUINO_AVR_LEONARDO)
#define SerialOut Serial 
#define UBLOX Serial1

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define SerialOut SerialUSB
#define UBLOX Serial

#elif defined(ARDUINO_SAM_ZERO)
#define SerialOut SerialUSB
#define UBLOX Serial1

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

// Pin to turn on/off the nb-iot module
#define powerPin 7 

void setup() 
{
  // Turn the nb-iot module on
  pinMode(powerPin, OUTPUT); 
  digitalWrite(powerPin, HIGH);

  // Start communication
  SerialOut.begin(9600);
  UBLOX.begin(9600);
}

// Forward every message to the other serial
void loop() 
{
  while (SerialOut.available())
  {
	uint8_t c = SerialOut.read();
	SerialOut.write(c);
    UBLOX.write(c);
  }

  while (UBLOX.available())
  {     
    SerialOut.write(UBLOX.read());
  }
}
