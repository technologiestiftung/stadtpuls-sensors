//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//
// tutorials
// https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/#example2
//  restart
// https://github.com/espressif/arduino-esp32/blob/a38ffe58fe1d73a56295edb0c6051a46932e0175/cores/esp32/Esp.h
// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/#2
#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char *ssid = "esp32";
const char *password = "123456789";

const char *PARAM_SSID = "ssid";
const char *PARAM_PASSWORD = "password";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setup()
{

    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    WiFi.mode(WIFI_AP);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", "<style>*{font-family: monospace; font-size:12;}</style>   <form action=\"/post\" method=\"post\">      <ul>        <li>          <label for=\"ssid\">SSID:</label>          <input type=\"text\" id=\"ssid\" name=\"ssid\" />        </li>        <li>          <label for=\"password\">password:</label>          <input type=\"password\" id=\"password\" name=\"password\" />        </li>        <li class=\"button\">          <button type=\"submit\">Send</button>        </li>      </ul>    </form>"); });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message); });

    // Send a POST request to <IP>/post
    // with a form field ssid and password set to <ssid> and <password>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String ssid;
                  String password;
                  if (request->hasParam(PARAM_SSID, true))
                  {
                      ssid = request->getParam(PARAM_SSID, true)->value();
                  }
                  else
                  {
                      ssid = "No ssid sent";
                  }
                  if (request->hasParam(PARAM_PASSWORD, true))
                  {
                      password = request->getParam(PARAM_PASSWORD, true)->value();
                  }
                  else
                  {
                      password = "No password sent";
                  }
                  request->send(200, "text/plain", "SSID: " + ssid + " PASSWORD: " + password); });

    server.onNotFound(notFound);

    server.begin();
}

void loop()
{
}