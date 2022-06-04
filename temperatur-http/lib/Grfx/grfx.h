#include <Arduino.h>
#include "images.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"

#ifndef GRFX_H
#define GRFX_H
class Grfx
{
  Adafruit_SSD1306 *oled;

private:
  int margin;

public:
  Grfx();
  void init(Adafruit_SSD1306 *_oled);
  void splash(int timeout);
  void drawString(char message[], int timeout);
  void drawValue(char message[], double value, int timeout);
  void drawMultilineString(char message1[], char message2[], int timeout);
};
#endif