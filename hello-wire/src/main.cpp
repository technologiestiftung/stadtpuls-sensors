#include <Arduino.h>
#include <Wire.h>
#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22
void setup()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Serial.begin(115200);
  while (!Serial)
    ; // Wait for serial monitor
  Serial.println("---I2C Scanner---");
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    Wire.beginTransmission(address + 1);

    if (error == 0 && Wire.endTransmission() != 0) // Special flag for SAMD Series
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("!");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // wait 5 seconds for next scan
}