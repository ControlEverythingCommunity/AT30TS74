// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// AT30TS74
// This code is designed to work with the AT30TS74_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// AT30TS74 I2C address is 0x48(72)
#define Addr 0x48

double cTemp = 0.0, fTemp = 0.0;
int temp = 0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "AT30TS74");
  Particle.variable("cTemp", cTemp);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select configuration register 0x01(01)
  // config msb, config lsb
  Wire.write(0x01);
  // Set resolution = 12-bits, normal operations, comparator mode
  Wire.write(0x60);
  // Reserved, default to 0x00
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[2];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to 12 bits
  temp = ((data[0] * 256) + (data[1] & 0xF0)) / 16;
  if (temp > 2047)
  {
    temp -= 4096;
  }
  
  cTemp = temp * 0.0625;
  fTemp = cTemp * 1.8 + 32;
  
  // Output data to dashboard
  Particle.publish("Temperature in Celsius : ", String(cTemp));
  Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
  delay(500);
}

