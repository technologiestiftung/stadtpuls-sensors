// Copyright (c) 2022 Technologiestiftung Berlin & Fabian Morón Zirfas
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef STADTPULS_H
#define STADTPULS_H

#include <Arduino.h>
#include <Wifi.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "stadtpuls_util.h"

#define SSD1306_NO_SPLASH 1           // remove the adafruit splash screen from the display
#define STADTPULS_SCREEN_WIDTH 128    // OLED display width, in pixels
#define STADTPULS_SCREEN_HEIGHT 64    // OLED display height, in pixels
#define STADTPULS_OLED_RESET 16       // Reset pin # (or -1 if sharing Arduino reset pin)
#define STADTPULS_SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 aaaaaaarrrrrggggghhhhh this is the other way around
#define STADTPULS_I2C_OLED_SDA_PIN 4
#define STADTPULS_I2C_OLED_SCL_PIN 15
#define STADTPULS_FORGET_PIN 33
#define STADTOPULS_FORGET_DELAY_MS 5000
#define STADTPULS_PRO_BUTTON_PIN 0 // useing the button for mode switches
#define STADTPULS_WIFI_SETUP_RETRIES 5

#define STADTPULS_SERVER "api.stadtpuls.com"

struct Stadtpuls_Options
{
  bool debug;
  String sensor_name;
  String ssid;
  String password;
  String sensor_id;
  String auth_token;
  String server;
  int forget_pin;
  bool use_display;
};
class Stadtpuls
{
public:
  Stadtpuls();

  void begin(Stadtpuls_Options options);
  void listen();
  void send(std::vector<double> measurements);

private:
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
  String server = "api.stadtpuls.com";
  Adafruit_SSD1306 display;
  String ssid = "";
  String password = "";
  String auth_token = "";
  String sensor_id = "";
  String ap_ip;
  int forget_pin;
  static String sensor_name;
  Preferences preferences;
  const char *prefs_credentials_key = "puls_wifi_creds";
  const char *prefs_mode_key = "puls_curr_mode";
  const char *prefs_sensor_id_key = "sensor_id";
  const char *prefs_sensor_name_key = "sensor_name";
  const char *prefs_auth_token_key = "auth_token";
  const char *prefs_password_key = "password";
  const char *prefs_ssid_key = "ssid";
  bool wifi_active = false;
  bool setup_access_point = true;
  bool PRINT;
  static String processor(const String &var);
};

// String processor(const String &var);
#endif