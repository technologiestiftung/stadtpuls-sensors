#include <Arduino.h>
#include "stadtpuls.h"

Stadtpuls stadtpuls;

bool done_once = false;
void setup()
{ //
  Serial.begin(115200);
  Serial.println("Hello Setup!");
  stadtpuls.begin("Stadtpuls", true);
}

void loop()
{ //

  if (!done_once)
  {
    Serial.println("Hello Loop!");
    done_once = true;
  }
}