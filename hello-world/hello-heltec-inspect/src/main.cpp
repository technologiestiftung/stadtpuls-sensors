/*
 * Heltec Automation ESP32 Serial 1 & 2 example.
 * shows the usage of all 3 hardware uarts
 * work with ESP32's IO MUX
 *
 * to test the software and hardware
 * wire Rx1 to Tx1 and Rx2 to Tx2
 * type text in serial monitor
 * text will walk trough all 2 serials
 * result is echoed to serial (usb)
 */

#include "Arduino.h"
#include "heltec.h"
#include <Adafruit_I2CDevice.h>

Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(0x10);

void setup()
{
  while (!Serial)
  {
    delay(10);
  }
  Serial.begin(115200);
  Serial.println("I2C address detection test");

  if (!i2c_dev.begin())
  {
    Serial.print("Did not find device at 0x");
    Serial.println(i2c_dev.address(), HEX);
    while (1)
      ;
  }
  Serial.print("Device found on address 0x");
  Serial.println(i2c_dev.address(), HEX);
}

void loop()
{
}
