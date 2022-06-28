#include "grfx.h"

Grfx::Grfx()
{
  margin = 2;
  // Nothing to do here
}

void Grfx::init(Adafruit_SSD1306 *_display, bool *_oled_active)
{
  display = _display;
  oled_active = _oled_active;
}
void Grfx::splash(int timeout)
{
  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay();                                 // Clear display buffer
  display->drawBitmap(0, 0, citylab_logo, 128, 64, WHITE); // Draw Bitmap
  display->display();                                      // Display buffer
  delay(timeout);
}

void Grfx::drawString(char message[], int timeout)
{
  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set cursor to top left
  display->println(F(message));         // Print message
  display->display();                   // Display buffer
  delay(timeout);
}
void Grfx::drawMultilineString(char message1[], char message2[], int timeout)
{

  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay();                 // Clear display buffer
  display->setTextSize(1);                 // Set text size to 1
  display->setTextColor(SSD1306_WHITE);    // Set text color to white
  display->setCursor(margin, margin);      // Set cursor to top left
  display->println(F(message1));           // Print message
  display->setCursor(margin, margin + 10); // Set cursor to top left

  display->println(F(message2)); // Print value

  display->display(); // Display buffer
  delay(timeout);
}
void Grfx::drawMultilineStringBig(char message1[], char message2[], int timeout)
{

  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay();                 // Clear display buffer
  display->setTextSize(1);                 // Set text size to 1
  display->setTextColor(SSD1306_WHITE);    // Set text color to white
  display->setCursor(margin, margin);      // Set cursor to top left
  display->println(F(message1));           // Print message
  display->setCursor(margin, margin + 10); // Set cursor to top left
  display->setTextSize(2);                 // Set text size to 2

  display->println(F(message2)); // Print value

  display->display(); // Display buffer
  delay(timeout);
}
void Grfx::clearDisplay(int timeout)
{
  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay(); // Clear display buffer
  display->display();      // Display buffer
  delay(timeout);
}

void Grfx::drawWiFiProgress(char line1[], char line2[], int16_t x, int16_t y, int16_t w, int16_t h, int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(line1));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(line2));
  display->fillRect(x, y, w, h, SSD1306_WHITE);

  display->display(); // Display buffer
  delay(timeout);
}

void Grfx::drawStringWithoutClear(char message1[], int16_t x, int16_t y, int timeout)
{
  if (*oled_active == false)
  {
    return;
  }
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(x, y);             // Set   // Set cursor to top left
  display->println(F(message1));
  display->display(); // Display buffer
  delay(timeout);
}

void Grfx::drawValue(char message[], double value, int timeout)
{
  if (*oled_active == false)
  {
    return;
  }
  display->clearDisplay();                 // Clear display buffer
  display->setTextSize(1);                 // Set text size to 1
  display->setTextColor(SSD1306_WHITE);    // Set text color to white
  display->setCursor(margin, margin);      // Set cursor to top left
  display->println(F(message));            // Print message
  display->setCursor(margin, margin + 10); // Set cursor to top left

  display->print(value, 2);   // Print value
  display->print(" Celsius"); // Print value
  display->display();         // Display buffer
  delay(timeout);
}

void Grfx::drawWiFiMessage(char mode[], char ap[], char url[], int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(mode));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(ap));
  display->setCursor(margin, margin + 20); // Set   // Set cursor to top left
  display->println(F(url));
  display->display(); // Display buffer
  delay(timeout);
}
void Grfx::drawAPMessage(char mode[], char ap[], char url[], int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(mode));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(ap));
  display->setCursor(margin, margin + 20); // Set   // Set cursor to top left
  display->println(F("URL:"));
  display->setCursor(margin, margin + 30); // Set   // Set cursor to top left
  display->println(F(url));
  display->display(); // Display buffer
  delay(timeout);
}
void Grfx::drawSensorMessage(char mode[], char msg[], double value, int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(mode));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(msg));
  display->setCursor(margin, margin + 20); // Set   // Set cursor to top left
  display->setTextSize(2);                 // Set text size to 1
  display->println(value);
  display->setTextSize(1);                 // Set text size to 1
  display->setCursor(margin, margin + 37); // Set   // Set cursor to top left
  display->print("Degrees Celsius");       // Print value
  display->display();                      // Display buffer
  delay(timeout);
}

void Grfx::drawSensorMessageHttp(char mode[],
                                 char msg[],
                                 double value,
                                 int16_t x,
                                 int16_t y,
                                 int16_t w,
                                 int16_t h,
                                 int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(mode));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(msg));
  display->setCursor(margin, margin + 20); // Set   // Set cursor to top left
  display->setTextSize(2);                 // Set text size to 1
  display->println(value);
  display->setTextSize(1);                 // Set text size to 1
  display->setCursor(margin, margin + 37); // Set   // Set cursor to top left
  display->print("Degrees Celsius");       // Print value

  display->fillRect(x, y, w, h, SSD1306_WHITE);

  display->display(); // Display buffer
  delay(timeout);
}

void Grfx::drawErrorMessage(char mode[], char msg[], int timeout)
{
  display->clearDisplay();              // Clear display buffer
  display->setTextSize(1);              // Set text size to 1
  display->setTextColor(SSD1306_WHITE); // Set text color to white
  display->setCursor(margin, margin);   // Set   // Set cursor to top left
  display->println(F(mode));
  display->setCursor(margin, margin + 10); // Set   // Set cursor to top left
  display->println(F(msg));
  display->setCursor(margin, margin + 20); // Set   // Set cursor to top left
  display->display();                      // Display buffer
  delay(timeout);
}