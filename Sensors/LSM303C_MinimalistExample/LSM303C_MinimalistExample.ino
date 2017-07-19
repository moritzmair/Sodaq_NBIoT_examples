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
#define DEBUG_STREAM Serial 

#elif defined(ARDUINO_SODAQ_EXPLORER)
#error "Sodaq ExpLoRer is not supporter yet."

#elif defined(ARDUINO_SAM_ZERO)
#error "Arduino Zero / M0 is not supporter yet."

#else
#error "Please select a Sodaq ExpLoRer, Arduino Leonardo or add your board."
#endif

void setup()
{
  DEBUG_STREAM.begin(115200);

  Wire.begin();

  while (!DEBUG_STREAM);
  
  if (myIMU.begin() != IMU_SUCCESS)
  {
    DEBUG_STREAM.println("Failed setup.");
    while(1);
  }
}

void loop()
{
  //Get all parameters
  DEBUG_STREAM.print("\nAccelerometer:\n");
  DEBUG_STREAM.print(" X = ");
  DEBUG_STREAM.println(myIMU.readAccelX(), 4);
  DEBUG_STREAM.print(" Y = ");
  DEBUG_STREAM.println(myIMU.readAccelY(), 4);
  DEBUG_STREAM.print(" Z = ");
  DEBUG_STREAM.println(myIMU.readAccelZ(), 4);

  DEBUG_STREAM.print("\nMagnetometer:\n");
  DEBUG_STREAM.print(" X = ");
  DEBUG_STREAM.println(myIMU.readMagX(), 4);
  DEBUG_STREAM.print(" Y = ");
  DEBUG_STREAM.println(myIMU.readMagY(), 4);
  DEBUG_STREAM.print(" Z = ");
  DEBUG_STREAM.println(myIMU.readMagZ(), 4);

  DEBUG_STREAM.print("\nThermometer:\n");
  DEBUG_STREAM.print(" Degrees C = ");
  DEBUG_STREAM.println(myIMU.readTempC(), 4);
  DEBUG_STREAM.print(" Degrees F = ");
  DEBUG_STREAM.println(myIMU.readTempF(), 4);
  
  delay(1000);
}
