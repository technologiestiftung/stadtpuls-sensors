#ifndef STADTPULS_UTILS_H
#define STADTPULS_UTILS_H
#include <WiFi.h>
#include <Arduino.h>
#include "stadtpuls.h"

void stadtpuls_WiFiEvent(WiFiEvent_t event);
void not_found(AsyncWebServerRequest *request);

#endif