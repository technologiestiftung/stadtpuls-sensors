/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/solved-reconnect-esp32-to-wifi/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include "heltec.h"
#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_MCP9808.h"
#include "grfx.h"
#include "env.h"

#define I2C_OLED_SDA_PIN 4
#define I2C_OLED_SCL_PIN 15
// remove the adafruit splash screen from the display
#define SSD1306_NO_SPLASH 1
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 16       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 aaaaaaarrrrrggggghhhhh this is the other way around
Adafruit_SSD1306 display;
Grfx oled;

#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22
TwoWire I2C_MCP = TwoWire(0);
TwoWire I2C_OLED = TwoWire(1);

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor;

// wifi things
unsigned long previous_millis = 0;
unsigned long interval = 30000;

// Replace with your network credentials
const char *ssid = secret_ssid; // imported from env.h
const char *password = secret_password;

//--------------

double measurements_sum = 0;
int measurements_counter = 0;
int measuring_period = 2000; // in ms
int measuring_iteration = 0;
unsigned long time_now = 0;

#define BUILD_IN_LED 25
#define FORGET_PIN 39

void setup()
{

  pinMode(BUILD_IN_LED, OUTPUT);
  pinMode(FORGET_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  while (!Serial)
    ;
  // init the I2C connections
  I2C_MCP.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  I2C_OLED.begin(I2C_OLED_SDA_PIN, I2C_OLED_SCL_PIN);
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);

  bool display_status = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (!display_status)
  {
    Serial.println("Could not find OLED display");
    while (1)
      ;
  }
  oled.init(&display);
  Serial.println("Found OLED display");
  oled.splash(2000);

  bool status = tempsensor.begin(0x18, &I2C_MCP);

  if (!status)
  {
    Serial.println("Couldn't find MCP9808!");
    while (1)
      ;
  }
  Serial.println("Found MCP9808!");
  oled.drawString("Found temp sensor!", 500);
  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println(WiFi.localIP());
  digitalWrite(BUILD_IN_LED, HIGH);

  oled.drawMultilineString("Connected to WiFi", strdup(ssid), 500);
}

void loop()
{

  unsigned long current_millis = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (current_millis - previous_millis >= interval))
  {
    oled.drawString("Reconnecting WiFi", 0);
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");

    WiFi.disconnect();
    WiFi.reconnect();
    previous_millis = current_millis;
  }

  // Read and print out the temperature, then convert to *F

  if (millis() >= measuring_iteration * measuring_period)
  {
    // tempsensor.shutdown_wake(false); // wakey wakey!

    // Serial.println("Sampling data");
    float c = tempsensor.readTempC();
    // float f = c * 9.0 / 5.0 + 32;
    Serial.print("Temp: ");
    Serial.print(c);
    Serial.println(" C");
    // Serial.print(f);
    // Serial.println(" F");
    // tempsensor.shutdown_wake(true); // sleep the sensor
    oled.drawValue("Temperatur:", c, 0);

    measuring_iteration += 1;
  }
}