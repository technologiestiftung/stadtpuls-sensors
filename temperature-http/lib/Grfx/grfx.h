#include <Arduino.h>
#include "images.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"

#ifndef GRFX_H
#define GRFX_H
class Grfx
{
  Adafruit_SSD1306 *oled;
  bool *oled_active;

public:
  Grfx();
  int margin;
  void init(Adafruit_SSD1306 *_oled, bool *_oled_active);
  void splash(int timeout);
  void clearDisplay(int timeout);
  void drawString(char message[], int timeout);
  void drawValue(char message[], double value, int timeout);
  void drawMultilineString(char message1[], char message2[], int timeout);
  void drawStringWithoutClear(char message1[], int16_t x, int16_t y, int timeout);
  void drawAPMessage(char mode[], char ap[], char url[], int timeout);
  void drawSensorMessage(char mode[], char msq[], char value[], int timeout);
};
#endif