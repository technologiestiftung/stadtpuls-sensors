#include "grfx.h"

Grfx::Grfx()
{
  margin = 2;
  // Nothing to do here
}

void Grfx::init(Adafruit_SSD1306 *_oled)
{
  oled = _oled;
}
void Grfx::splash(int timeout)
{
  oled->clearDisplay();                                 // Clear display buffer
  oled->drawBitmap(0, 0, citylab_logo, 128, 64, WHITE); // Draw Bitmap
  oled->display();                                      // Display buffer
  delay(timeout);
}

void Grfx::drawString(char message[], int timeout)
{
  oled->clearDisplay();              // Clear display buffer
  oled->setTextSize(1);              // Set text size to 1
  oled->setTextColor(SSD1306_WHITE); // Set text color to white
  oled->setCursor(margin, margin);   // Set cursor to top left
  oled->println(F(message));         // Print message
  oled->display();                   // Display buffer
  delay(timeout);
}
void Grfx::drawMultilineString(char message1[], char message2[], int timeout)
{
  oled->clearDisplay();                 // Clear display buffer
  oled->setTextSize(1);                 // Set text size to 1
  oled->setTextColor(SSD1306_WHITE);    // Set text color to white
  oled->setCursor(margin, margin);      // Set cursor to top left
  oled->println(F(message1));           // Print message
  oled->setCursor(margin, margin + 10); // Set cursor to top left

  oled->println(F(message2)); // Print value

  oled->display(); // Display buffer
  delay(timeout);
}

void Grfx::clearDisplay(int timeout)
{
  oled->clearDisplay(); // Clear display buffer
  oled->display();      // Display buffer
  delay(timeout);
}

void Grfx::drawStringWithoutClear(char message1[], int16_t x, int16_t y, int timeout)
{
  oled->setTextSize(1);              // Set text size to 1
  oled->setTextColor(SSD1306_WHITE); // Set text color to white
  oled->setCursor(x, y);             // Set   // Set cursor to top left
  oled->println(F(message1));
  oled->display(); // Display buffer
  delay(timeout);
}
void Grfx::drawValue(char message[], double value, int timeout)
{
  oled->clearDisplay();                 // Clear display buffer
  oled->setTextSize(1);                 // Set text size to 1
  oled->setTextColor(SSD1306_WHITE);    // Set text color to white
  oled->setCursor(margin, margin);      // Set cursor to top left
  oled->println(F(message));            // Print message
  oled->setCursor(margin, margin + 10); // Set cursor to top left

  oled->print(value, 2);   // Print value
  oled->print(" Celsius"); // Print value
  oled->display();         // Display buffer
  delay(timeout);
}