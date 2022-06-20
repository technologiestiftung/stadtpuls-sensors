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
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images.h"

#define I2C_OLED_SDA_PIN 4
#define I2C_OLED_SCL_PIN 15
#define SSD1306_NO_SPLASH 1

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 16       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 aaaaaaarrrrrggggghhhhh this is the other way around
Adafruit_SSD1306 display;
TwoWire I2C_OLED = TwoWire(0);

void setup()
{
  Serial.begin(115200);
  I2C_OLED.begin(I2C_OLED_SDA_PIN, I2C_OLED_SCL_PIN);
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);
  bool status = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (status)
  {
    Serial.println("OLED initialized");
  }
  else
  {
    Serial.println("OLED init failed");
  }
  display.display();

  // delay(1000);
  display.clearDisplay();                                 // Clear display buffer
  display.drawBitmap(0, 0, citylab_logo, 128, 64, WHITE); // Draw Bitmap
  display.display();                                      // Display buffer
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.println(F("Hello, world!"));
  display.display();
  delay(2000);
  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // The txPin & rxPin can set to any output pin

  // Serial1.begin(115200, SERIAL_8N1, 2, 17);
  // Serial2.begin(115200, SERIAL_8N1, 22, 23);
}

void loop()
{

  if (Serial.available())
  {
    int ch = Serial.read();
    Serial.write(ch);
    // Serial.println("Hello");
  }

  // if (Serial2.available())
  // {
  //   int ch = Serial2.read();
  //   Serial2.write(ch);
  // }

  // if (Serial2.available())
  // {
  //   int ch = Serial2.read();
  //   Serial.write(ch);
  // }
}
