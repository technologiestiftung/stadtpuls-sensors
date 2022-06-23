#include <Arduino.h>
#include "images.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"

#ifndef GRFX_H
#define GRFX_H
class Grfx
{

public:
  Adafruit_SSD1306 *display;
  bool *oled_active;
  Grfx();
  int margin;
  void init(Adafruit_SSD1306 *_display, bool *_oled_active);
  void splash(int timeout);
  void clearDisplay(int timeout);
  void drawString(char message[], int timeout);
  void drawValue(char message[], double value, int timeout);
  void drawMultilineString(char message1[], char message2[], int timeout);
  void drawMultilineStringBig(char message1[], char message2[], int timeout);
  void drawStringWithoutClear(char message1[], int16_t x, int16_t y, int timeout);
  void drawAPMessage(char mode[], char ap[], char url[], int timeout);
  void drawWiFiMessage(char mode[], char ap[], char url[], int timeout);

  void drawSensorMessage(char mode[], char msq[], double value, int timeout);
  void drawSensorMessageHttp(char mode[], char msg[], double value, int16_t x, int16_t y, int16_t w, int16_t h, int timeout);
  void drawWiFiProgress(char line1[], char line2[], int16_t x, int16_t y, int16_t w, int16_t h, int timeout);
};
#endif