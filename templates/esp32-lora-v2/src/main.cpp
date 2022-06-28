// Copyright (c) 2022 Technologiestiftung Berlin & Fabian Morón Zirfas
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <Arduino.h>
#include "stadtpuls.h"

Stadtpuls stadtpuls;
Stadtpuls_Options options;
unsigned long previous_millis = 0;
unsigned long interval = 60000; // 60 seconds

bool done_once = false;
void setup()
{
  options.debug = true;
  options.sensor_name = "Stadtpuls";
  // options.use_display = true;
  // options.ssid = "Stadtpuls";
  // options.password = "Stadtpuls";
  // options.sensor_id = "0";
  // options.auth_token = "Stadtpuls";
  options.server = "stadtpuls-esp32.eu.ngrok.io";
  // options.forget_pin = STADTPULS_FORGET_PIN; //currently 33

  Serial.begin(115200);
  Serial.println("Hello Setup!");
  stadtpuls.begin(options);
}

void loop()
{

  if (!done_once)
  {
    Serial.println("Hello Loop!");
    done_once = true;
  }
  stadtpuls.listen();
  unsigned long current_millis = millis();
  if (current_millis - previous_millis >= interval)
  {

    stadtpuls.send(std::vector<double>{1, 2, 3});
    previous_millis = current_millis;
  }
}