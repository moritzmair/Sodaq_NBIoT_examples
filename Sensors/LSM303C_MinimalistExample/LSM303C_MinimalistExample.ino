// I2C interface by default

// Install the libraries in the library manager
// Search for 303C
// Install the Sparkfun IMU 303 library
#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

// #define DEBUG 1 in SparkFunLSM303C.h turns on debugging statements.
// Redefine to 0 to turn them off.

LSM303C myIMU;

#if defined(ARDUINO_AVR_LEONARDO)
//#define SerialOut Serial 
//#define UBLOX Serial1

#elif defined(ARDUINO_SODAQ_EXPLORER)
#error "Sodaq ExpLoRer is not supporter yet."
//#define SerialOut SerialUSB
//#define UBLOX Serial

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

void setup()
{
  Serial.begin(115200);

  Wire.begin();

  while (!Serial);
  
  if (myIMU.begin() != IMU_SUCCESS)
  {
    Serial.println("Failed setup.");
    while(1);
  }
}

void loop()
{
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readAccelZ(), 4);

  // Not supported by hardware, so will return NAN
  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readGyroZ(), 4);

  Serial.print("\nMagnetometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readMagX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readMagY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readMagZ(), 4);

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees C = ");
  Serial.println(myIMU.readTempC(), 4);
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 4);
  
  delay(1000);
}
