// Copyright (c) 2022 Technologiestiftung Berlin & Fabian Morón Zirfas
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "stadtpuls.h"
const char *PARAM_SSID = "ssid";
const char *PARAM_PASSWORD = "password";
const char *PARAM_SENSOR_ID = "sensorid";
const char *PARAM_AUTH_TOKEN = "authtoken";
AsyncWebServer ap_server(80);
WiFiClientSecure client;
String Stadtpuls::sensor_name = "STADTPULS_ESP32";
Stadtpuls::Stadtpuls()
{

  // Constructor
}
void Stadtpuls::begin(Stadtpuls_Options options)
{
#if defined(WIFI_LoRa_32_V2)

  Serial.println("LoRa 32 V2");
  //  defined( WIFI_Kit_32 ) || defined( WIFI_LoRa_32 )
#elif
  Serial.println("Not a LoRa 32 V2 board. Use the library at your own risk.");
  //  defined( WIFI_Kit_32 ) || defined( WIFI_LoRa_32 )
#warnings "This library is not tested with your board proceed at own risk"
#endif
  // options parsing
  if (options.sensor_name.length() > 0)
  {
    sensor_name = options.sensor_name;
  }
  if (options.ssid.length() > 0)
  {
    ssid = options.ssid;
  }
  if (options.password.length() > 0)
  {
    password = options.password;
  }
  if (options.sensor_id.length() > 0)
  {
    sensor_id = options.sensor_id;
  }
  if (options.auth_token.length() > 0)
  {
    auth_token = options.auth_token;
  }
  if (options.forget_pin != STADTPULS_FORGET_PIN)
  {
    forget_pin = options.forget_pin;
  }
  else
  {
    forget_pin = STADTPULS_FORGET_PIN;
  }
  if (options.server.length() > 0)
  {
    server = options.server;
  }
  else
  {
    server = STADTPULS_SERVER;
  }

  if (options.debug == true)
  {
    Serial.println("Debug mode enabled");
    PRINT = true;
  }
  else
  {
    PRINT = false;
  }

  pinMode(forget_pin, INPUT_PULLDOWN);
  client.setCACert(root_ca);

  if (PRINT)
  {
    WiFi.onEvent(stadtpuls_WiFiEvent);
  }

  Stadtpuls::preferences.begin(Stadtpuls::prefs_credentials_key, false);
  if (ssid == "" || password == "" || sensor_id == "" || auth_token == "")
  {
    ssid = Stadtpuls::preferences.getString(prefs_ssid_key, "");
    password = Stadtpuls::preferences.getString(prefs_password_key, "");
    sensor_id = Stadtpuls::preferences.getString(prefs_sensor_id_key, "");
    auth_token = Stadtpuls::preferences.getString(prefs_auth_token_key, "");
  }
  else
  {
    Stadtpuls::preferences.putString(prefs_ssid_key, ssid);
    Stadtpuls::preferences.putString(prefs_password_key, password);
    Stadtpuls::preferences.putString(prefs_sensor_id_key, sensor_id);
    Stadtpuls::preferences.putString(prefs_auth_token_key, auth_token);
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

    if (PRINT)
    {
      Serial.println("Access Point WiFi: " + sensor_name);
      Serial.println("Access Point IP: http://" + ap_ip);
    }

    ap_server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/style.css", "text/css"); });

    ap_server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
                 {
                  if(PRINT){
                    Serial.println("Client connected on path /");
                  }
                   request->send(SPIFFS, "/index.html", String(), false, processor); });
    // Send a POST request to <IP>/post
    // with a form field ssid and password set to <ssid> and <password>
    ap_server.on("/post", HTTP_POST, [&](AsyncWebServerRequest *request)
                 {
      String user_ssid;
      String user_password;
      String user_auth_token;
      String user_sensor_id;
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
      if (request->hasParam(PARAM_SENSOR_ID, true))
      {
        user_sensor_id= request->getParam(PARAM_SENSOR_ID, true)->value();
      }
      else
      {
        user_sensor_id = "";
      }
            if (request->hasParam(PARAM_AUTH_TOKEN, true))
      {
        user_auth_token= request->getParam(PARAM_AUTH_TOKEN, true)->value();
      }
      else
      {
        user_auth_token= "";
      }
      Stadtpuls::preferences.begin(prefs_credentials_key, false);
      Stadtpuls::preferences.putString(prefs_ssid_key, user_ssid);
      Stadtpuls::preferences.putString(prefs_password_key, user_password);
      Stadtpuls::preferences.putString(prefs_auth_token_key, user_auth_token);
      Stadtpuls::preferences.putString(prefs_sensor_id_key, user_sensor_id);
      Stadtpuls::preferences.end();
      if(PRINT){
      Serial.println("ssid: " + user_ssid);
      Serial.println("password: " + user_password);
      Serial.println("sensor_id: " + user_sensor_id);
      Serial.println("auth_token: " + user_auth_token);
      Serial.println("Please Restart the device");
      }
      // oled.clearDisplay(0);
      // oled.drawString("Saving credentials", 200);
      request->send(SPIFFS, "/post.html", String(), false, processor); });
    // set password in prefs
    // request->send(200, "text/plain", "SSID: " + user_ssid + " PASSWORD: " + user_password); });

    ap_server.onNotFound(not_found);

    ap_server.begin();
    if (PRINT)
    {
      Serial.println("Access Point Started");
    }
  }
}
void Stadtpuls::listen()
{ // listen for pin events
  if (digitalRead(forget_pin) == HIGH)
  {
    Stadtpuls::preferences.begin(prefs_credentials_key, false);
    Stadtpuls::preferences.clear();
    Stadtpuls::preferences.end();
    Serial.print("Credentials cleared, I will reboot in ");
    Serial.print(STADTOPULS_FORGET_DELAY_MS / 1000);
    Serial.println(" seconds");
    // wait for STADTOPULS_FORGET_DELAY_MS ms
    delay(STADTOPULS_FORGET_DELAY_MS);
    ESP.restart();
  }
}

void Stadtpuls::send(std::vector<double> measurements)
{
  if (wifi_active == true && WiFi.status() == WL_CONNECTED)
  {
    if (measurements.begin() == measurements.end())
    {
      return;
    }
    String m = "";
    for (auto it : measurements)
    {
      if (it != measurements.size())
      {
        m += String(measurements[it]) + ",";
      }
      else
      {
        m += String(measurements[it]);
      }
    }

    String payload = "{\"measurements\": [" + m + "], \"sensor_name\": \"" + sensor_name + "\"}";
    if (!client.connect(server.c_str(), 443))
    {
      if (PRINT)
      {
        Serial.println("Connection failed!");
      }
    }
    else
    {
      if (PRINT)
      {

        Serial.println("Connected to server!");
        Serial.println("\n- - - - - - - - - - - - - - - -");
      }
      // Make a HTTP request:
      client.println("POST " + String("/api/v3/sensors/" + String(sensor_id) + "/records") + " HTTP/1.0");
      client.println("Host: " + String(server));
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(payload.length()));
      client.println("Authorization: Bearer " + String(auth_token));
      client.println("Connection: close");
      client.println();
      client.println(payload);

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
        if (PRINT)
        {
          Serial.write(c);
        }
      }

      if (PRINT)
      {
        Serial.println("\n- - - - - - - - - - - - - - - -");
      }
      client.stop();
    }
  }
  else
  {
    if (PRINT)
    {
      Serial.println("WiFi not connected");
    }
  }
}

String Stadtpuls::processor(const String &var)
{

  // preferences.begin(Stadtpuls::prefs_credentials_key, false);

  String new_ssid = "";
  // preferences.getString(prefs_ssid_key, "");
  String new_password = "";
  // preferences.getString(prefs_password_key, "");
  String name = sensor_name;
  String auth_token = "";
  String sensor_id = "";
  // preferences.getString("name", "");
  Serial.println(var);
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
    return name;
  }
  if (var == "AUTH_TOKEN")
  {
    return auth_token;
  }
  if (var == "SENSOR_ID")
  {
    return sensor_id;
  }
  return String();
}