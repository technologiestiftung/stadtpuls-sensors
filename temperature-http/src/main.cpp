// Copyright (c) 2022 Technologiestiftung Berlin, Lucas Vogel, Ingo Hinterding & Fabian Morón Zirfas
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Based on the code from the following sources:
//
//
// - Rui Santos - esp32 tutorials - https://RandomNerdTutorials.com/
// - Adafruit_MCP9808_Library - examples (temp sensor) - https://github.com/adafruit/Adafruit_MCP9808_Library
// - Adafruit_SSD1306_Library - examples (oled display) - https://github.com/adafruit/Adafruit_SSD1306
// - Adafruit_GFX_Library - examples (oled display) - https://github.com/adafruit/Adafruit-GFX-Library
// - espressif arduino-esp32 examples - https://github.com/espressif/arduino-esp32/
// - Heltec_ESP32 - examples (board) - https://github.com/HelTecAutomation/Heltec_ESP32/
//   Permission is hereby granted, free of charge, to any person obtaining a copy
//   of this software and associated documentation files.

//   The above copyright notice and this permission notice shall be included in all
//   copies or substantial portions of the Software.

#include <Arduino.h>
#include <string> // std::string, std::to_string
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include "grfx.h"
#include "Adafruit_MCP9808.h"
#include <Preferences.h>
#include "env.h"

// #include "util/sensors/env_ares.h"
// #include "util/sensors/env_blink.h"
#include "util/sensors/env_cloak.h"
// #include "util/sensors/env_cypher.h"
// #include "util/sensors/env_dagger.h"
// #include "util/sensors/env_dazzler.h"
// #include "util/sensors/env_echo.h"
// #include "util/sensors/env_elektra.h"
// #include "util/sensors/env_elixir.h"
// #include "util/sensors/env_falcon.h"
// #include "util/sensors/env_havok.h"
// #include "util/sensors/env_mantis.h"
// #include "util/sensors/env_medusa.h"
// #include "util/sensors/env_moonstar.h"
// #include "util/sensors/env_nova.h"
// #include "util/sensors/env_pixie.h"
// #include "util/sensors/env_polaris.h"
// #include "util/sensors/env_storm.h"
// #include "util/sensors/env_tigra.h"
// #include "util/sensors/env_wasp.h"

// to make sure you are sending data to api.stadtpuls.com you need the following root certificate
// to get this via your browser do the folowing
// - open the api url of your sensor in Firefox e.g. https://api.stadtpuls.com/api/v1/sensors/<sensor_id>/records
// - click on the little lock icon left from the URL in the address bar
// - click on the "connection secure" button
// - click on the "More information" button
// - click on the "View certificate" button
// - go to the second tab where you can see infos about the root ca
// - scroll down and downlad the PEM (cert)
// - save the PEM to a file called root-ca.pem
// - open the file in your editor and copy the content to the clipboard
// - insert it into your editor and adjust it like the cert below.
// Hint!: the new line characters are important for the cert to work correctly

const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEmDCCA4CgAwIBAgIQAtiaFXPzs7jaFFtYOJgpBzANBgkqhkiG9w0BAQsFADBa\n"
    "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n"
    "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTIw\n"
    "MDEyNzEyNDYzOVoXDTI0MTIzMTIzNTk1OVowSjELMAkGA1UEBhMCVVMxGTAXBgNV\n"
    "BAoTEENsb3VkZmxhcmUsIEluYy4xIDAeBgNVBAMTF0Nsb3VkZmxhcmUgSW5jIFJT\n"
    "QSBDQS0yMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtV0myAmrsTNd\n"
    "DLKWBcIxA05+X4cCxm+AJO3arhy+552A98XPVAFmOCjo5mm7nfiBMWpW9ejBpehZ\n"
    "6LPgOgFhCCiwfs2g/c43AMPpqY0LyXjKpmBGgK90BS1P2ugJUsibWjLPISFHj1wa\n"
    "iqoXwBhwsQHx3t8thamV7FtXOTm+hcLv17YsSj/R1rMhopdPJSxCI33XsxnWEmmF\n"
    "AAKv1aoS2Ghkf+OtGJ1SL8Xl9JGv5QnZi0bnzzIVIvA3xP2L5uIdFsw6SNeBYYK/\n"
    "PXLZcF7sCmkk4jxI4/7T5VHUK8ASoU+TcJm/t9KDbmktyM2iYhhMQzdfFjp2k1r6\n"
    "bmldmsWIwQIDAQABo4IBaDCCAWQwHQYDVR0OBBYEFBipGvyyRUnBbzA0CCvZh5yw\n"
    "JVd6MB8GA1UdIwQYMBaAFOWdWTCCR1jMrPoIVDaGezq1BE3wMA4GA1UdDwEB/wQE\n"
    "AwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0TAQH/BAgw\n"
    "BgEB/wIBADA0BggrBgEFBQcBAQQoMCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3Nw\n"
    "LmRpZ2ljZXJ0LmNvbTA6BgNVHR8EMzAxMC+gLaArhilodHRwOi8vY3JsMy5kaWdp\n"
    "Y2VydC5jb20vT21uaXJvb3QyMDI1LmNybDBtBgNVHSAEZjBkMDcGCWCGSAGG/WwB\n"
    "ATAqMCgGCCsGAQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BTMAsG\n"
    "CWCGSAGG/WwBAjAIBgZngQwBAgEwCAYGZ4EMAQICMAgGBmeBDAECAzANBgkqhkiG\n"
    "9w0BAQsFAAOCAQEAfLCdjh6mZHIQ4WHd83Q9UKca58JOFwwVJisXj0HnkPCw8/LS\n"
    "51CA0gfGqX+6k+u+wcGG+E+EvAV85nIv6bbGdmnd8mpHa5NUGKQNgAM1J92Kn8iE\n"
    "l9O04NqmBfDnz+admJTSy9oid9hJC6hVjonU0s7Q6NrgQv0dzn6WhMqnw5Ennym8\n"
    "/+0uNP1GKu9OVnzo3CKX7VMJW7p74PJPpVmVQc2zci5cb3pKFUMrIsvUPyB88/p8\n"
    "2x4PSnNx6dXdRraeHRMegHGZ3/VQfjOB0jV1JF6PmFqokkX0tNiIprFkBun7+WVI\n"
    "9pbDH55GEgF+WjBTXTQcFeIS8vGLlfCU+K3jkw==\n"
    "-----END CERTIFICATE-----\n";
const char *server = "api.stadtpuls.com";

//  ██████╗ ██╗     ███████╗██████╗
// ██╔═══██╗██║     ██╔════╝██╔══██╗
// ██║   ██║██║     █████╗  ██║  ██║
// ██║   ██║██║     ██╔══╝  ██║  ██║
// ╚██████╔╝███████╗███████╗██████╔╝
//  ╚═════╝ ╚══════╝╚══════╝╚═════╝

// remove the adafruit splash screen from the display
#define SSD1306_NO_SPLASH 1
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 16       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 aaaaaaarrrrrggggghhhhh this is the other way around
Adafruit_SSD1306 display;
Grfx oled;

// ██╗██████╗  ██████╗
// ██║╚════██╗██╔════╝
// ██║ █████╔╝██║
// ██║██╔═══╝ ██║
// ██║███████╗╚██████╗
// ╚═╝╚══════╝ ╚═════╝

#define I2C_OLED_SDA_PIN 4
#define I2C_OLED_SCL_PIN 15
#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22
TwoWire I2C_MCP = TwoWire(0);
TwoWire I2C_OLED = TwoWire(1);
//
// ████████╗███████╗███╗   ███╗██████╗
// ╚══██╔══╝██╔════╝████╗ ████║██╔══██╗
//    ██║   █████╗  ██╔████╔██║██████╔╝
//    ██║   ██╔══╝  ██║╚██╔╝██║██╔═══╝
//    ██║   ███████╗██║ ╚═╝ ██║██║
//    ╚═╝   ╚══════╝╚═╝     ╚═╝╚═╝

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor;
float current_temperature = 0.0f;

// ██╗    ██╗██╗███████╗██╗
// ██║    ██║██║██╔════╝██║
// ██║ █╗ ██║██║█████╗  ██║
// ██║███╗██║██║██╔══╝  ██║
// ╚███╔███╔╝██║██║     ██║
//  ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝

// Replace with your network credentials
String ssid;
String password;
String ap_ip;

bool setup_access_point = false;
const char *ap_ssid = sensor_name;
const char *user_ssid = "esp32";
const char *user_password = "123456789";
const char *PARAM_SSID = "ssid";
const char *PARAM_PASSWORD = "password";
AsyncWebServer ap_server(80);
int wifi_setup_retries = 20;
Preferences preferences;
const char prefs_credentials_key[] = "credentials";
const char prefs_mode_key[] = "operation_mode";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
/**
 * @brief function to handle replacement in html templates
 *
 * @param var the string to replace
 * @return String the result of the replacement
 */
String processor(const String &var)
{

  preferences.begin(prefs_credentials_key, false);

  String new_ssid = preferences.getString("ssid", "");
  String new_password = preferences.getString("password", "");

  Serial.println(var);
  if (var == "STATE")
  {
    return "OK";
  }
  if (var == "SSID")
  {
    return new_ssid;
  }
  if (var == "PASSWORD")
  {
    return new_password;
  }
  if (var == "BOARD_NAME")
  {
    return sensor_name;
  }
  preferences.end();
  return String();
}

void WiFiEvent(WiFiEvent_t event)
{
  Serial.println();
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case SYSTEM_EVENT_WIFI_READY:
    Serial.println("WiFi interface ready");
    break;
  case SYSTEM_EVENT_SCAN_DONE:
    Serial.println("Completed scan for access points");
    break;
  case SYSTEM_EVENT_STA_START:
    Serial.println("WiFi client started");
    break;
  case SYSTEM_EVENT_STA_STOP:
    Serial.println("WiFi clients stopped");
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    Serial.println("Connected to access point");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("Disconnected from WiFi access point");
    break;
  case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    Serial.println("Authentication mode of access point has changed");
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
    Serial.print("Obtained IP address: ");
    Serial.println(WiFi.localIP());
    break;
  case SYSTEM_EVENT_STA_LOST_IP:
    Serial.println("Lost IP address and IP address is reset to 0");
    break;
  // case SYSTEM_EVENT_WPS_ER_SUCCESS:
  //   Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
  //   break;
  // case SYSTEM_EVENT_WPS_ER_FAILED:
  //   Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
  //   break;
  // case SYSTEM_EVENT_WPS_ER_TIMEOUT:
  //   Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
  //   break;
  // case SYSTEM_EVENT_WPS_ER_PIN:
  //   Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
  //   break;
  case SYSTEM_EVENT_AP_START:
    Serial.println("WiFi access point started");
    break;
  case SYSTEM_EVENT_AP_STOP:
    Serial.println("WiFi access point  stopped");
    break;
  case SYSTEM_EVENT_AP_STACONNECTED:
    Serial.println("Client connected");
    break;
  case SYSTEM_EVENT_AP_STADISCONNECTED:
    Serial.println("Client disconnected");
    break;
  case SYSTEM_EVENT_AP_STAIPASSIGNED:
    Serial.println("Assigned IP address to client");
    break;
  case SYSTEM_EVENT_AP_PROBEREQRECVED:
    Serial.println("Received probe request");
    break;
  case SYSTEM_EVENT_GOT_IP6:
    Serial.println("AP IPv6 is preferred");
    break;
  // case SYSTEM_EVENT_STA_GOT_IP6:
  //   Serial.println("STA IPv6 is preferred");
  //   break;
  // case SYSTEM_EVENT_ETH_GOT_IP6:
  //   Serial.println("Ethernet IPv6 is preferred");
  //   break;
  case SYSTEM_EVENT_ETH_START:
    Serial.println("Ethernet started");
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("Ethernet stopped");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("Ethernet connected");
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("Ethernet disconnected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.println("Obtained IP address");
    break;
  default:
    break;
  }
}
//--------------

// ████████╗██╗███╗   ███╗███████╗
// ╚══██╔══╝██║████╗ ████║██╔════╝
//    ██║   ██║██╔████╔██║█████╗
//    ██║   ██║██║╚██╔╝██║██╔══╝
//    ██║   ██║██║ ╚═╝ ██║███████╗
//    ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝
// measuring
double measurements_sum = 0;
long measurements_counter = 0;
int measuring_period = 2000; // in ms

long measuring_iteration = 0;
unsigned long time_now = 0;
// wifi probing
unsigned long previous_millis = 0;
unsigned long interval = 30000;
// http sending
unsigned long http_previous_millis = 0;
unsigned long http_interval = 60000;

// blink sonmething
#define BUILD_IN_LED 25
// for resetting/forgetting the wifi credentials
// connect pin 33 to VCC
#define FORGET_PIN 33

// useing the button for mode switches

#define PRO_BUTTON_PIN 0
// Variables will change:
int button_state;            // the current reading from the input pin
int last_button_state = LOW; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long last_debounce_time = 0; // the last time the output pin was toggled
unsigned long debounce_delay = 50;    // the debounce time; increase if the output flickers
#define SENSOR_MODE 0
#define HTTP_MODE 1
#define AP_MODE 2
int mode = 0; // 0  for sensor mode, 1 for http mode, 2 for AP mode

// some states we need to keep track of

bool oled_active = false;
bool wifi_active = false;
bool tempsensor_active = false;

// ███████╗███████╗████████╗██╗   ██╗██████╗
// ██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗
// ███████╗█████╗     ██║   ██║   ██║██████╔╝
// ╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝
// ███████║███████╗   ██║   ╚██████╔╝██║
// ╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝

void setup()
{

  pinMode(BUILD_IN_LED, OUTPUT);
  pinMode(FORGET_PIN, INPUT_PULLDOWN);
  pinMode(PRO_BUTTON_PIN, INPUT);

  preferences.begin(prefs_mode_key, false);
  mode = preferences.getInt("mode", 0);
  preferences.end();

  // ███████╗███████╗██████╗ ██╗ █████╗ ██╗
  // ██╔════╝██╔════╝██╔══██╗██║██╔══██╗██║
  // ███████╗█████╗  ██████╔╝██║███████║██║
  // ╚════██║██╔══╝  ██╔══██╗██║██╔══██║██║
  // ███████║███████╗██║  ██║██║██║  ██║███████╗
  // ╚══════╝╚══════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚══════╝

  Serial.begin(115200);
  // while (!Serial)
  //   ;
  // ██╗██████╗  ██████╗
  // ██║╚════██╗██╔════╝
  // ██║ █████╔╝██║
  // ██║██╔═══╝ ██║
  // ██║███████╗╚██████╗
  // ╚═╝╚══════╝ ╚═════╝

  // init the I2C connections
  I2C_MCP.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  I2C_OLED.begin(I2C_OLED_SDA_PIN, I2C_OLED_SCL_PIN);

  //  ██████╗ ██╗     ███████╗██████╗
  // ██╔═══██╗██║     ██╔════╝██╔══██╗
  // ██║   ██║██║     █████╗  ██║  ██║
  // ██║   ██║██║     ██╔══╝  ██║  ██║
  // ╚██████╔╝███████╗███████╗██████╔╝
  //  ╚═════╝ ╚══════╝╚══════╝╚═════╝

  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);

  bool display_status = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (!display_status)
  {
    Serial.println("Could not find OLED display");
  }
  else
  {
    oled_active = true;
  }
  oled.init(&display, &oled_active);
  Serial.println("Found OLED display");
  oled.splash(2000);
  oled.drawMultilineStringBig("Hi, I'm", sensor_name, 1500);

  // ████████╗███████╗███╗   ███╗██████╗
  // ╚══██╔══╝██╔════╝████╗ ████║██╔══██╗
  //    ██║   █████╗  ██╔████╔██║██████╔╝
  //    ██║   ██╔══╝  ██║╚██╔╝██║██╔═══╝
  //    ██║   ███████╗██║ ╚═╝ ██║██║
  //    ╚═╝   ╚══════╝╚═╝     ╚═╝╚═╝

  // ███████╗███████╗███╗   ██╗███████╗ ██████╗ ██████╗
  // ██╔════╝██╔════╝████╗  ██║██╔════╝██╔═══██╗██╔══██╗
  // ███████╗█████╗  ██╔██╗ ██║███████╗██║   ██║██████╔╝
  // ╚════██║██╔══╝  ██║╚██╗██║╚════██║██║   ██║██╔══██╗
  // ███████║███████╗██║ ╚████║███████║╚██████╔╝██║  ██║
  // ╚══════╝╚══════╝╚═╝  ╚═══╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝

  bool temp_status = tempsensor.begin(0x18, &I2C_MCP);

  if (!temp_status)
  {
    Serial.println("Couldn't find MCP9808!");
    oled.drawString("Cant find temp sensor", 1000);
  }
  else
  { //
    tempsensor_active = true;
    Serial.println("Found MCP9808!");
    oled.drawString("Found temp sensor!", 500);
    tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
    // Mode Resolution SampleTime
    //  0    0.5°C       30 ms
    //  1    0.25°C      65 ms
    //  2    0.125°C     130 ms
    //  3    0.0625°C    250 ms
    current_temperature = tempsensor.readTempC();
  }

  // ██╗    ██╗██╗███████╗██╗
  // ██║    ██║██║██╔════╝██║
  // ██║ █╗ ██║██║█████╗  ██║
  // ██║███╗██║██║██╔══╝  ██║
  // ╚███╔███╔╝██║██║     ██║
  //  ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝
  // oled.drawString("Connecting to WiFi", 0);
  WiFi.onEvent(WiFiEvent);
  preferences.begin(prefs_credentials_key, false);
  if (ssid == "" || password == "")
  {
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
  }
  else
  {
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
  }
  preferences.end();

  int no_wifi_count = 0;
  WiFi.mode(WIFI_STA);
  if (password == "")
  {
    WiFi.begin(ssid.c_str());
  }
  else
  {
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  Serial.print("Connecting to WiFi ..");

  // String dots = ".";
  while (WiFi.status() != WL_CONNECTED)
  {
    float mapped = map(no_wifi_count, 0, wifi_setup_retries, 0, DISPLAY_WIDTH);
    oled.drawWiFiProgress("Connecting to WiFi", (char *)ssid.c_str(), 0, DISPLAY_HEIGHT - 5, mapped, 5, 0),
        Serial.print('.');
    // oled.drawStringWithoutClear((char *)dots.c_str(), oled.margin, oled.margin + 10, 0);
    // dots = dots + ".";
    delay(1000);
    no_wifi_count++;
    if (no_wifi_count == wifi_setup_retries)
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        wifi_active = false;
        setup_access_point = true;
        mode = AP_MODE;
        break;
      }
    }
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    wifi_active = true;
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    oled.drawStringWithoutClear("Connected!", oled.margin, oled.margin + 20, 500);
    // oled.drawMultilineString("Connected to WiFi", (char *)ssid.c_str(), 500);
  }
  //  █████╗ ██████╗
  // ██╔══██╗██╔══██╗
  // ███████║██████╔╝
  // ██╔══██║██╔═══╝
  // ██║  ██║██║
  // ╚═╝  ╚═╝╚═╝

  if (wifi_active == false)
  {
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
    oled.drawString("Could not find or connect to a WiFi", 500);
    Serial.println("Creating access point");
    oled.drawMultilineString("Creating access point", (char *)ap_ssid, 1000);
    WiFi.softAP(ap_ssid);
    WiFi.mode(WIFI_AP);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    ap_ip = "http://" + IP.toString();
    char *cstr = new char[ap_ip.length() + 1];
    strcpy(cstr, ap_ip.c_str());
    oled.drawMultilineString("(open) Access point:", (char *)ap_ssid, 0);
    oled.drawStringWithoutClear("GoTo:", oled.margin, oled.margin + 20, 0);
    oled.drawStringWithoutClear(cstr, oled.margin, oled.margin + 30, 0);

    // Route to load style.css file
    ap_server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/style.css", "text/css"); });

    ap_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Send a POST request to <IP>/post
    // with a form field ssid and password set to <ssid> and <password>
    ap_server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
                 {
      String user_ssid;
      String user_password;
      if (request->hasParam(PARAM_SSID, true))
      {
        user_ssid = request->getParam(PARAM_SSID, true)->value();
      }
      else
      {
        user_ssid = "No ssid sent";
      }
      if (request->hasParam(PARAM_PASSWORD, true))
      {
        user_password = request->getParam(PARAM_PASSWORD, true)->value();
      }
      else
      {
        user_password = "";
      }

      preferences.begin(prefs_credentials_key, false);
      preferences.putString("ssid", user_ssid);
      preferences.putString("password", user_password);
      preferences.end();
      Serial.println("ssid: " + user_ssid);
      Serial.println("password: " + user_password);
      Serial.println("Please Restart the device");
      oled.clearDisplay(0);
      oled.drawString("Saving credentials", 200);
      request->send(SPIFFS, "/post.html", String(), false, processor); });
    // set password in prefs
    // request->send(200, "text/plain", "SSID: " + user_ssid + " PASSWORD: " + user_password); });

    ap_server.onNotFound(notFound);

    ap_server.begin();
  }
}

// ██╗      ██████╗  ██████╗ ██████╗
// ██║     ██╔═══██╗██╔═══██╗██╔══██╗
// ██║     ██║   ██║██║   ██║██████╔╝
// ██║     ██║   ██║██║   ██║██╔═══╝
// ███████╗╚██████╔╝╚██████╔╝██║
// ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝

void loop()
{
  int button_reading = digitalRead(PRO_BUTTON_PIN);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (button_reading != last_button_state)
  {
    // reset the debouncing timer
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (button_reading != button_state)
    {
      button_state = button_reading;

      // only toggle the LED if the new button state is HIGH
      if (button_state == LOW)
      {
        mode = (mode + 1) % 3;
        preferences.begin(prefs_mode_key, false);
        preferences.putInt("mode", mode);
        preferences.end();
        switch (mode)
        {
        case AP_MODE:
          Serial.println("AP/WIFI Mode");
          break;
        case SENSOR_MODE:
          Serial.println("SENSOR Mode");
          break;
        case HTTP_MODE:
          Serial.println("STADTPULS Mode");
          break;
        }
      }
    }
  }
  last_button_state = button_reading;
  // save the reading. Next time through the loop, it'll be the lastButtonState:

  switch (mode)
  {
  case AP_MODE:
    if (wifi_active == false)
    {
      String apmsg = "WiFi: " + (String)sensor_name;
      oled.drawAPMessage("ACCESS POINT MODE",
                         (char *)apmsg.c_str(),
                         (char *)ap_ip.c_str(), 0);
    }
    else
    {
      String ssidmsg = "WiFi: " + ssid;
      String ipmsg = "IP: " + WiFi.localIP().toString();
      oled.drawWiFiMessage("WIFI MODE",
                           (char *)ssidmsg.c_str(),
                           (char *)ipmsg.c_str(), 0);
    }
    break;
  case SENSOR_MODE:

    if (tempsensor_active)
    {
      // Serial.print("Temp: ");
      // Serial.print(current_temperature);
      // Serial.println(" C");
      // oled.drawValue("Temperature:", current_temperature, 0);
      oled.drawSensorMessage("SENSOR MODE", "Temperature", current_temperature, 0);
    }
    else
    { //
      oled.drawErrorMessage("SENSOR MODE", "Sensor not found :(", 0);
    }
    break;
  case HTTP_MODE:
    unsigned long http_current_millis = millis();
    // float step = http_interval / DISPLAY_WIDTH;
    unsigned long progress = http_current_millis - http_previous_millis;
    // map one value into another range
    float mapped = map(progress, 0, http_interval, 0, DISPLAY_WIDTH);
    if (tempsensor_active)
    {
      // Serial.print("Temp: ");
      // Serial.print(current_temperature);
      // Serial.println(" C");
      oled.drawSensorMessageHttp(
          "STADTPULS MODE",
          "Temperature",
          current_temperature,
          0,
          DISPLAY_HEIGHT - 5,
          mapped,
          5,
          0);
    }
    else
    {
      oled.drawErrorMessage("STADTPULS MODE", "Sensor not found :(", 0);
    }

    if (setup_access_point == false && WiFi.status() == WL_CONNECTED && (http_current_millis - http_previous_millis >= http_interval))
    {
      //   // ██╗  ██╗████████╗████████╗██████╗
      //   // ██║  ██║╚══██╔══╝╚══██╔══╝██╔══██╗
      //   // ███████║   ██║      ██║   ██████╔╝
      //   // ██╔══██║   ██║      ██║   ██╔═══╝
      //   // ██║  ██║   ██║      ██║   ██║
      //   // ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝

      String payload = "{\"measurements\": [" + String(measurements_sum / measurements_counter) + "], \"sensor_name\": \"" + sensor_name + "\"}";

      WiFiClientSecure client;

      client.setCACert(root_ca);
      // client.setInsecure(); // skip verification

      if (!client.connect(server, 443))
      {
        Serial.println("Connection failed!");
      }
      else
      {
        Serial.println("Connected to server!");
        Serial.println("\n- - - - - - - - - - - - - - - -");
        // Make a HTTP request:
        client.println("POST " + String(path) + " HTTP/1.0");
        client.println("Host: " + String(server));
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(payload.length()));
        client.println("Authorization: Bearer " + String(auth_token));
        client.println("Connection: close");
        client.println();
        client.println(payload);
        measurements_sum = 0;
        measurements_counter = 0;
        while (client.connected())
        {
          String line = client.readStringUntil('\n');
          if (line == "\r")
          {
            Serial.println("POST Success!");
            break;
          }
        }
        // if there are incoming bytes available
        // from the server, read them and print them:
        while (client.available())
        {
          char c = client.read();
          Serial.write(c);
        }

        Serial.println("\n- - - - - - - - - - - - - - - -");
        oled.drawStringWithoutClear("Sent data!", oled.margin, oled.margin + 47, 250);
        client.stop();
      }
      http_previous_millis = http_current_millis;
    }
    break;
  }

  if (digitalRead(FORGET_PIN) == HIGH)
  {
    preferences.begin(prefs_credentials_key, false);
    preferences.clear();
    preferences.end();
    Serial.println("Credentials cleared, I will reboot");
    oled.drawMultilineString("Credentials cleared", "I will reboot", 30000);
    ESP.restart();
  }
  if (setup_access_point == false)
  {

    // check every 30 seconds if we are still connected
    // if not try to reconnect

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
  }

  if (tempsensor_active)
  {

    //   // ████████╗███████╗███╗   ███╗██████╗
    //   // ╚══██╔══╝██╔════╝████╗ ████║██╔══██╗
    //   //    ██║   █████╗  ██╔████╔██║██████╔╝
    //   //    ██║   ██╔══╝  ██║╚██╔╝██║██╔═══╝
    //   //    ██║   ███████╗██║ ╚═╝ ██║██║
    //   //    ╚═╝   ╚══════╝╚═╝     ╚═╝╚═╝
    if (millis() >= measuring_iteration * measuring_period)
    {
      // Read and print out the temperature, then convert to *F
      // tempsensor.shutdown_wake(false); // wakey wakey!

      // Serial.println("Sampling data");
      current_temperature = tempsensor.readTempC();
      // float f = c * 9.0 / 5.0 + 32;
      // Serial.print(f);
      // Serial.println(" F");
      // tempsensor.shutdown_wake(true); // sleep the sensor
      // oled.drawValue("Temperature:", current_temperature, 0);
      measurements_sum += current_temperature;

      measuring_iteration += 1;
      measurements_counter += 1;
    }
  }
}