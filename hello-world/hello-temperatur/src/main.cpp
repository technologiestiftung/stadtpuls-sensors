#include <Arduino.h>
#include "heltec.h"
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22
TwoWire I2MCP = TwoWire(0);
// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor;

void setup()
{

  Serial.begin(115200);
  I2MCP.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  bool status = tempsensor.begin(0x18, &I2MCP);

  if (!status)
  {
    Serial.println("Couldn't find MCP9808!");
    while (1)
      ;
  }
}

void loop()
{
  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temp: ");
  Serial.print(c);
  Serial.print(" C\t");
  Serial.print(f);
  Serial.println(" F");
  delay(250);

  tempsensor.shutdown_wake(1);
  delay(2000);
  tempsensor.shutdown_wake(0);
}