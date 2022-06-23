#include "stadtpuls.h"
const char *PARAM_SSID = "ssid";
const char *PARAM_PASSWORD = "password";
Stadtpuls::Stadtpuls()
{

  // Constructor
}
void Stadtpuls::begin(String name, bool debug)
{
  // TwoWire I2C_oled = TwoWire(1);
  PRINT = debug;
#if defined(WIFI_LoRa_32_V2)
  if (PRINT)
    Serial.println("LoRa 32 V2");
    //  defined( WIFI_Kit_32 ) || defined( WIFI_LoRa_32 )
#elif
#warnings "This library is not tested with your board proceed at own risk"
#endif
  if (PRINT)
    WiFi.onEvent(stadtpuls_WiFiEvent);
  sensor_name = name;
  Stadtpuls::preferences.begin(Stadtpuls::prefs_credentials_key, false);
  if (ssid == "" || password == "")
  {
    ssid = Stadtpuls::preferences.getString("ssid", "");
    password = Stadtpuls::preferences.getString("password", "");
  }
  else
  {
    Stadtpuls::preferences.putString("ssid", ssid);
    Stadtpuls::preferences.putString("password", password);
  }
  Stadtpuls::preferences.end();
  int no_wifi_count = 0;
  WiFi.mode(WIFI_STA);
  if (PRINT)
    Serial.println("Connecting to WiFi");
  if (password == "")
  {
    WiFi.begin(ssid.c_str());
  }
  else
  {
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    if (PRINT)
      Serial.print('.');
    delay(1000);
    no_wifi_count++;
    if (no_wifi_count == STADTPULS_WIFI_SETUP_RETRIES)
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        wifi_active = false;
        setup_access_point = true;
        if (PRINT)
          Serial.println("WiFi connection failed");
        break;
      }
    }
  }
  if (PRINT)
    Serial.println("");

  if (WiFi.status() == WL_CONNECTED)
  {
    wifi_active = true;
    if (PRINT)
    {
      Serial.println("Connected!");
      Serial.println(WiFi.localIP());
    }
  }
  if (wifi_active == false)
  {
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
      if (PRINT)
        Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
    WiFi.softAP(sensor_name.c_str());
    WiFi.mode(WIFI_AP);
    IPAddress ip = WiFi.softAPIP();
    ap_ip = ip.toString();
    AsyncWebServer ap_server(80);
    if (PRINT)
    {
      Serial.println("Access Point WiFi: " + sensor_name);
      Serial.println("Access Point IP: http://" + ap_ip);
    }

    ap_server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/style.css", "text/css"); });

    ap_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Send a POST request to <IP>/post
    // with a form field ssid and password set to <ssid> and <password>
    ap_server.on("/post", HTTP_POST, [this](AsyncWebServerRequest *request)
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
      if(PRINT){
      Serial.println("ssid: " + user_ssid);
      Serial.println("password: " + user_password);
      Serial.println("Please Restart the device");
      }
      // oled.clearDisplay(0);
      // oled.drawString("Saving credentials", 200);
      request->send(SPIFFS, "/post.html", String(), false, processor); });
    // set password in prefs
    // request->send(200, "text/plain", "SSID: " + user_ssid + " PASSWORD: " + user_password); });

    ap_server.onNotFound(not_found);

    ap_server.begin();
  }
}

String processor(const String &var)
{

  // preferences.begin(Stadtpuls::prefs_credentials_key, false);

  String new_ssid = "";
  // preferences.getString("ssid", "");
  String new_password = "";
  // preferences.getString("password", "");
  String sensor_name = "";
  // preferences.getString("sensor_name", "");
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
  return String();
}