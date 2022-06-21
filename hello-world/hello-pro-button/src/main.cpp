/*
 * Based on https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
 */

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_OLED_SDA_PIN 4
#define I2C_OLED_SCL_PIN 15
#define SSD1306_NO_SPLASH 1

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 16       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 aaaaaaarrrrrggggghhhhh this is the other way around
Adafruit_SSD1306 display;
TwoWire I2C_OLED = TwoWire(0);

#define PRO_BUTTON_PIN 0
// Variables will change:
int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
int mode = 0;
void setup()
{
  pinMode(PRO_BUTTON_PIN, INPUT);

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

  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.println(F("OLED is working"));
  display.display();
  delay(2000);
}

void loop()
{

  // read the state of the switch into a local variable:
  int reading = digitalRead(PRO_BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW)
      {
        mode = (mode + 1) % 2;
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  if (mode == 0)
  { //
    Serial.println("Mode is 0");
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("Mode is 0"));
    display.display();
  }
  else if (mode == 1)
  { //
    Serial.println("Mode is 1");
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("mode is 1"));
    display.display();
  }
}
